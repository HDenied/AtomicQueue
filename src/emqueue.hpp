#pragma once
#include <atomic>
#include <iostream>
#include <limits.h>
#include <memory>
#include <mutex>
#include <stdint.h>
#include <thread>
#include <unistd.h>
#include <utility>

namespace emtqueue {

template <class T, int32_t MAX_SIZE = UCHAR_MAX> class EMtQueue {
protected:
  T _q[MAX_SIZE]{};

  std::atomic<int32_t> _front{-1};
  std::atomic<int32_t> _b{0};

public:
  EMtQueue() = default;

  bool push(T &&obj) {
    int32_t pos = 0;
    if ((pos = _front.load(std::memory_order_acquire)) < MAX_SIZE) {

      if (pos != -1) {
        _q[pos] = std::move(obj);
        pos++;
      } else {
        _q[0] = std::move(obj);
        pos = 1;
      }

      _front.store(pos, std::memory_order_release);
      // still some slots are available for writing
      return true;
    }
    // writer is at the end of the queue
    else {
      return false;
    }
  }

  bool push(T &obj) {
    int32_t pos = 0;
    if ((pos = _front.load(std::memory_order_acquire)) < MAX_SIZE) {

      if (pos != -1) {
        _q[pos] = obj;
        pos++;
      } else {
        _q[0] = obj;
        pos = 1;
      }

      _front.store(pos, std::memory_order_release);
      // still some slots are available for writing
      return true;
    }
    // writer is at the end of the queue
    else {
      return false;
    }
  }

  bool pop(T &elem) {
    int32_t end = 0;
    int32_t start = _b.load();
    bool res{false};

    if ((end = _front.load(std::memory_order_acquire)) >= 0) {
      if (((end < MAX_SIZE || start < end) && start != end) || end == 0) {
        elem = _q[start];
        _b.store(start + 1, std::memory_order_release);

        res = true;
      }

      if (end == MAX_SIZE && start+1 == end) {
        _b.store(0, std::memory_order_release);
        _front.store(-1, std::memory_order_acquire);
        return false | res;
      }
    }

    return res;
  }

  bool wait_pop(T &elem, uint32_t timeout_us = 1000) {
    if (pop(elem)) {
      return true;
    } else {
      usleep(timeout_us);
      return false;
    }
  }
};
}; // namespace emtqueue