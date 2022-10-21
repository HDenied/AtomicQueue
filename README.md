# Eventually Monotonic Queue

## Requirements

This project was born to address following requirements:

- Faster handling of data in a queue in case of multithreading environment
- Avoiding dynamic memory allocation in the queue
- Portability on different kind of architectures (i.e. tested on x86 with glibc and armv7 with musl)

## Philosophy

The project uses atomic variable only, as the reader and writers can't be syncronised by any sort of mutex, I assumed that the only invariants that could hold was that _both reader and writers couldn't know the exact position in the queue of their respective counterpart_ and that _the writer could just end up filling the queue_. With these assumptions, I created a queue where the writer could just add elements until the end of the allocated space and once at the end of it would wrap up at the starting position. The name of the queue derives from the mentioned behaviour, therefore, if the queue object had infinite space available would just grow in terms of index even if we pop out the elements from it.

## Performance

Some preliminary results show that if the emqueue has a size greater than one performs better than a traditional queue which uses mutex.

### Environment

- Linux mint kernel 5.15.0-48-generic x86_64
- g++ version 9.4.0

### Results

```
[----------] 2 tests from testInterleavedAtomicOneDepth
[ RUN      ] testInterleavedAtomicOneDepth.size1samples100000
[       OK ] testInterleavedAtomicOneDepth.size1samples100000 (46 ms)
[ RUN      ] testInterleavedAtomicOneDepth.size1samples1000000
[       OK ] testInterleavedAtomicOneDepth.size1samples1000000 (156 ms)
[----------] 2 tests from testInterleavedAtomicOneDepth (203 ms total)

[----------] 2 tests from testInterleavedAtomicSmallDepth
[ RUN      ] testInterleavedAtomicSmallDepth.size10samples100000
[       OK ] testInterleavedAtomicSmallDepth.size10samples100000 (3 ms)
[ RUN      ] testInterleavedAtomicSmallDepth.size10samples1000000
[       OK ] testInterleavedAtomicSmallDepth.size10samples1000000 (37 ms)
[----------] 2 tests from testInterleavedAtomicSmallDepth (40 ms total)

[----------] 2 tests from testInterleavedAtomicMediumDepth
[ RUN      ] testInterleavedAtomicMediumDepth.size1000samples100000
[       OK ] testInterleavedAtomicMediumDepth.size1000samples100000 (2 ms)
[ RUN      ] testInterleavedAtomicMediumDepth.size1000samples1000000
[       OK ] testInterleavedAtomicMediumDepth.size1000samples1000000 (36 ms)
[----------] 2 tests from testInterleavedAtomicMediumDepth (39 ms total)

[----------] 1 test from testInterleavedAtomicFullDepth
[ RUN      ] testInterleavedAtomicFullDepth.size100000samples100000
[       OK ] testInterleavedAtomicFullDepth.size100000samples100000 (1 ms)
[----------] 1 test from testInterleavedAtomicFullDepth (1 ms total)

[----------] 2 tests from testInterleavedLock
[ RUN      ] testInterleavedLock.samples100000
[       OK ] testInterleavedLock.samples100000 (8 ms)
[ RUN      ] testInterleavedLock.samples1000000
[       OK ] testInterleavedLock.samples1000000 (99 ms)
[----------] 2 tests from testInterleavedLock (107 ms total)
```

More systematic tests to come...

## Limitations

For now the queue works correctly just in the case of one reader and one writer