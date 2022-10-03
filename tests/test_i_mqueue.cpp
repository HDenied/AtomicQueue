#include "mqueue.hpp"
#include <algorithm>
#include <array>
#include <gtest/gtest.h>
#include <iterator>
#include <limits.h>
#include <memory>
#include <string>
#include <thread>
#include <vector>


namespace mt=mtqueue;


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

/*
template <class T> void producer(T &q, int32_t samples, const uint8_t q_size) {
  for (int idx = 0; idx < samples; idx++) {
    while (idx < samples && q.size() == q_size)
      ;

    // sleep(0.0001);
    // std::cout << "Tx: " << idx << "\n";
    q.push(DummyComm{idx, idx});
  }
}

template <class T>
void consumer(T &q, std::vector<DummyComm> &res, int32_t samples) {
  DummyComm val{};
  while (true) {

    if (!q.empty()) {
      val = q.front();

      if (val.l_val1 < samples - 1) {
        // std::cout << "Rx: " << val.l_val1 << "\n";
        res.push_back(std::move(val));
        q.pop();
      } else if (val.l_val1 == samples - 1) {
        // std::cout << "Rx final: " << val.l_val1 << "\n";
        res.push_back(std::move(val));
        q.pop();
        break;
      }
    }
  }
}

TEST(LockqueueTest, testMultiThInterleavedQ1) {
  const uint8_t Q_SIZE = 1;
  cu::Lockqueue<DummyComm, Q_SIZE> q{};
  std::vector<DummyComm> res{};
  const int _SIZE{100000};
  int samples{_SIZE};
  std::array<bool, _SIZE> test_arr{false};
  std::thread prod{(producer<cu::Lockqueue<DummyComm, Q_SIZE>>), std::ref(q),
                   samples, Q_SIZE};
  std::thread cons{(consumer<cu::Lockqueue<DummyComm, Q_SIZE>>), std::ref(q),
                   std::ref(res), samples};
  prod.join(), cons.join();
  for (auto &val : res) {
    test_arr[val.l_val2] = true;
  }

  ASSERT_EQ(std::all_of(test_arr.begin(), test_arr.end(),
                        [](bool i) { return i == true; }),
            true);
}

TEST(LockqueueTest, testMultiThInterleavedQMax) {
  const uint8_t Q_SIZE = UCHAR_MAX;
  cu::Lockqueue<DummyComm, Q_SIZE> q{};
  std::vector<DummyComm> res{};
  const int _SIZE{100000};
  int samples{_SIZE};
  std::array<bool, _SIZE> test_arr{false};
  std::thread prod{(producer<cu::Lockqueue<DummyComm, Q_SIZE>>), std::ref(q),
                   samples, Q_SIZE};
  std::thread cons{(consumer<cu::Lockqueue<DummyComm, Q_SIZE>>), std::ref(q),
                   std::ref(res), samples};
  prod.join(), cons.join();
  for (auto &val : res) {
    test_arr[val.l_val2] = true;
  }

  ASSERT_EQ(std::all_of(test_arr.begin(), test_arr.end(),
                        [](bool i) { return i == true; }),
            true);
}
*/



template<class T>
void producer_m(T &q, uint32_t samples) {

  for (uint32_t idx = 0; idx < samples; idx++) {

    while (!q.push(DummyComm{int(idx), int(idx)}))
      ;
  }
}

template <class T>
void consumer_m(T &q, std::vector<DummyComm>&vec, uint32_t samples) {
  DummyComm res{};

  while (true) {
    if (q.pop(res)) {
      vec.push_back(res);
      //std::cout<<"el is "<<res.l_val1<<"\n";
      if (vec.size() > 0 && vec.back().l_val1 == samples-1) {
        break;
      }
    }
  }

 
}

TEST(testInterleavedAtomic, size1samples100000) {
  std::vector<DummyComm> vec{};
  const uint32_t SIZE_Q =1;
  uint32_t samples = 100000;
  mt::MtQueue<DummyComm, SIZE_Q> q{};
  std::thread prod{(producer_m<mt::MtQueue<DummyComm, SIZE_Q>>),std::ref(q), samples};
  std::thread cons{(consumer_m<mt::MtQueue<DummyComm, SIZE_Q>>),std::ref(q), std::ref(vec), samples};
  prod.join(), cons.join();
   uint32_t idx{};
  for (auto &var : vec) {
    ASSERT_EQ(idx, var.l_val1);
    idx++;
  }
}

TEST(testInterleavedAtomic, size1samples1000000) {
  std::vector<DummyComm> vec{};
  const uint32_t SIZE_Q =1;
  uint32_t samples = 1000000;
  mt::MtQueue<DummyComm, SIZE_Q> q{};
  std::thread prod{(producer_m<mt::MtQueue<DummyComm, SIZE_Q>>),std::ref(q), samples};
  std::thread cons{(consumer_m<mt::MtQueue<DummyComm, SIZE_Q>>),std::ref(q), std::ref(vec), samples};
  prod.join(), cons.join();
   uint32_t idx{};
  for (auto &var : vec) {
    ASSERT_EQ(idx, var.l_val1);
    idx++;
  }
}

TEST(testInterleavedAtomic, size1000samples100000) {
  std::vector<DummyComm> vec{};
  const uint32_t SIZE_Q =1000;
  uint32_t samples = 100000;
  mt::MtQueue<DummyComm, SIZE_Q> q{};
  std::thread prod{(producer_m<mt::MtQueue<DummyComm, SIZE_Q>>),std::ref(q), samples};
  std::thread cons{(consumer_m<mt::MtQueue<DummyComm, SIZE_Q>>),std::ref(q), std::ref(vec), samples};
  prod.join(), cons.join();
   uint32_t idx{};
  for (auto &var : vec) {
    ASSERT_EQ(idx, var.l_val1);
    idx++;
  }
}

TEST(testInterleavedAtomic, size1000samples1000000) {
  std::vector<DummyComm> vec{};
  const uint32_t SIZE_Q =1000;
  uint32_t samples = 1000000;
  mt::MtQueue<DummyComm, SIZE_Q> q{};
  std::thread prod{(producer_m<mt::MtQueue<DummyComm, SIZE_Q>>),std::ref(q), samples};
  std::thread cons{(consumer_m<mt::MtQueue<DummyComm, SIZE_Q>>),std::ref(q), std::ref(vec), samples};
  prod.join(), cons.join();
   uint32_t idx{};
  for (auto &var : vec) {
    ASSERT_EQ(idx, var.l_val1);
    idx++;
  }
}