#include "emqueue.hpp"
#include <algorithm>
#include <array>
#include <gtest/gtest.h>
#include <iterator>
#include <limits.h>
#include <memory>
#include <queue>
#include <string>
#include <thread>
#include <vector>

namespace mt = emtqueue;

template <class T> class LockQueStd {
  std::queue<T> _q{};
  mutable std::mutex mtx;

public:
  bool push(T &&obj) {
    std::lock_guard<std::mutex> lck(mtx);
    _q.push(std::forward<T>(obj));
    return true;
  }

  bool pop(T &obj) {
    std::lock_guard<std::mutex> lck(mtx);
    if (!_q.empty()) {
      obj = _q.front();
      _q.pop();
      return true;
    } else {
      return false;
    }
  }

  ~LockQueStd() {
    std::lock_guard<std::mutex> lck(mtx);
    while (!_q.empty()) {
      _q.pop();
    }
  }
};

class DummyComm {

public:
  int l_val1{};
  int l_val2{};
  DummyComm(DummyComm &&rhs) {

    this->l_val1 = rhs.l_val1;
    this->l_val2 = rhs.l_val2;
    rhs.l_val1 = -1;
    rhs.l_val2 = -1;
  }
  DummyComm &operator=(DummyComm &&rhs) {
    if (this != &rhs) {
      this->l_val1 = rhs.l_val1;
      this->l_val2 = rhs.l_val2;
      rhs.l_val1 = -1;
      rhs.l_val2 = -1;
    }
    return *this;
  }
  DummyComm(const DummyComm &rhs) // = delete;
  {
    this->l_val1 = rhs.l_val1;
    this->l_val2 = rhs.l_val2;
  }
  DummyComm &operator=(const DummyComm &rhs) {
    this->l_val1 = rhs.l_val1;
    this->l_val2 = rhs.l_val2;
    return *this;
  }
  DummyComm(int val1, int val2) : l_val1{val1}, l_val2{val2} {}
  DummyComm() = default;
};

template <class T> void producer_m(T &q, uint32_t samples) {

  for (uint32_t idx = 0; idx < samples; idx++) {

    while (!q.push(DummyComm{int(idx), int(idx)}))
      ;
  }
}

template <class T>
void consumer_m(T &q, std::vector<DummyComm> &vec, uint32_t samples) {
  DummyComm res{};

  while (true) {
    if (q.pop(res)) {
      vec.push_back(res);
      if (vec.size() > 0 && vec.back().l_val1 == samples - 1) {
        break;
      }
    }
  }
}

template <class T> void producer(T &q, uint32_t samples) {

  for (uint32_t idx = 0; idx < samples; idx++) {

    while (!q.push(DummyComm{int(idx), int(idx)}))
      ;
  }
}

template <class T>
void consumer(T &q, std::vector<DummyComm> &vec, uint32_t samples) {
  DummyComm res{};

  while (true) {
    if (q.pop(res)) {
      vec.push_back(DummyComm(res));
      if (vec.size() > 0 && vec.back().l_val1 == samples - 1) {
        break;
      }
    }
  }
}

TEST(testInterleavedAtomicOneDepth, size1samples100000) {
  std::vector<DummyComm> vec{};
  const uint32_t SIZE_Q = 1;
  uint32_t samples = 100000;
  mt::EMtQueue<DummyComm, SIZE_Q> q{};
  std::thread prod{(producer_m<mt::EMtQueue<DummyComm, SIZE_Q>>), std::ref(q),
                   samples};
  std::thread cons{(consumer_m<mt::EMtQueue<DummyComm, SIZE_Q>>), std::ref(q),
                   std::ref(vec), samples};
  prod.join(), cons.join();
  uint32_t idx{};
  for (auto &var : vec) {
    ASSERT_EQ(idx, var.l_val1);
    idx++;
  }
}

TEST(testInterleavedAtomicSmallDepth, size10samples100000) {
  std::vector<DummyComm> vec{};
  const uint32_t SIZE_Q = 10;
  uint32_t samples = 100000;
  mt::EMtQueue<DummyComm, SIZE_Q> q{};
  std::thread prod{(producer_m<mt::EMtQueue<DummyComm, SIZE_Q>>), std::ref(q),
                   samples};
  std::thread cons{(consumer_m<mt::EMtQueue<DummyComm, SIZE_Q>>), std::ref(q),
                   std::ref(vec), samples};
  prod.join(), cons.join();
  uint32_t idx{};
  for (auto &var : vec) {
    ASSERT_EQ(idx, var.l_val1);
    idx++;
  }
}

TEST(testInterleavedAtomicMediumDepth, size1000samples100000) {
  std::vector<DummyComm> vec{};
  const uint32_t SIZE_Q = 1000;
  uint32_t samples = 100000;
  mt::EMtQueue<DummyComm, SIZE_Q> q{};
  std::thread prod{(producer_m<mt::EMtQueue<DummyComm, SIZE_Q>>), std::ref(q),
                   samples};
  std::thread cons{(consumer_m<mt::EMtQueue<DummyComm, SIZE_Q>>), std::ref(q),
                   std::ref(vec), samples};
  prod.join(), cons.join();
  uint32_t idx{};
  for (auto &var : vec) {
    ASSERT_EQ(idx, var.l_val1);
    idx++;
  }
}

TEST(testInterleavedAtomicFullDepth, size100000samples100000) {
  std::vector<DummyComm> vec{};
  const uint32_t SIZE_Q = 100000;
  uint32_t samples = 100000;
  mt::EMtQueue<DummyComm, SIZE_Q> q{};
  std::thread prod{(producer_m<mt::EMtQueue<DummyComm, SIZE_Q>>), std::ref(q),
                   samples};
  std::thread cons{(consumer_m<mt::EMtQueue<DummyComm, SIZE_Q>>), std::ref(q),
                   std::ref(vec), samples};
  prod.join(), cons.join();
  uint32_t idx{};
  for (auto &var : vec) {
    ASSERT_EQ(idx, var.l_val1);
    idx++;
  }
}

TEST(testInterleavedAtomicOneDepth, size1samples1000000) {
  std::vector<DummyComm> vec{};
  const uint32_t SIZE_Q = 1;
  uint32_t samples = 1000000;
  mt::EMtQueue<DummyComm, SIZE_Q> q{};
  std::thread prod{(producer_m<mt::EMtQueue<DummyComm, SIZE_Q>>), std::ref(q),
                   samples};
  std::thread cons{(consumer_m<mt::EMtQueue<DummyComm, SIZE_Q>>), std::ref(q),
                   std::ref(vec), samples};
  prod.join(), cons.join();
  uint32_t idx{};
  for (auto &var : vec) {
    ASSERT_EQ(idx, var.l_val1);
    idx++;
  }
}

TEST(testInterleavedAtomicSmallDepth, size10samples1000000) {
  std::vector<DummyComm> vec{};
  const uint32_t SIZE_Q = 10;
  uint32_t samples = 1000000;
  mt::EMtQueue<DummyComm, SIZE_Q> q{};
  std::thread prod{(producer_m<mt::EMtQueue<DummyComm, SIZE_Q>>), std::ref(q),
                   samples};
  std::thread cons{(consumer_m<mt::EMtQueue<DummyComm, SIZE_Q>>), std::ref(q),
                   std::ref(vec), samples};
  prod.join(), cons.join();
  uint32_t idx{};
  for (auto &var : vec) {
    ASSERT_EQ(idx, var.l_val1);
    idx++;
  }
}

TEST(testInterleavedAtomicMediumDepth, size1000samples1000000) {
  std::vector<DummyComm> vec{};
  const uint32_t SIZE_Q = 1000;
  uint32_t samples = 1000000;
  mt::EMtQueue<DummyComm, SIZE_Q> q{};
  std::thread prod{(producer_m<mt::EMtQueue<DummyComm, SIZE_Q>>), std::ref(q),
                   samples};
  std::thread cons{(consumer_m<mt::EMtQueue<DummyComm, SIZE_Q>>), std::ref(q),
                   std::ref(vec), samples};
  prod.join(), cons.join();
  uint32_t idx{};
  for (auto &var : vec) {
    ASSERT_EQ(idx, var.l_val1);
    idx++;
  }
}

TEST(testInterleavedLock, samples100000) {
  std::vector<DummyComm> vec{};
  uint32_t samples = 100000;
  LockQueStd<DummyComm> q{};
  std::thread prod{(producer<LockQueStd<DummyComm>>), std::ref(q), samples};
  std::thread cons{(consumer<LockQueStd<DummyComm>>), std::ref(q),
                   std::ref(vec), samples};
  prod.join(), cons.join();

  uint32_t idx{};
  for (auto &var : vec) {
    ASSERT_EQ(idx, var.l_val1);
    idx++;
  }
}

TEST(testInterleavedLock, samples1000000) {
  std::vector<DummyComm> vec{};
  uint32_t samples = 1000000;
  LockQueStd<DummyComm> q{};
  std::thread prod{(producer<LockQueStd<DummyComm>>), std::ref(q), samples};
  std::thread cons{(consumer<LockQueStd<DummyComm>>), std::ref(q),
                   std::ref(vec), samples};
  prod.join(), cons.join();

  uint32_t idx{};
  for (auto &var : vec) {
    ASSERT_EQ(idx, var.l_val1);
    idx++;
  }
}
