#include "emqueue.hpp"
#include <ctime>
#include <gtest/gtest.h>
#include <string>

using namespace std::chrono;
namespace mt = emtqueue;

struct DummyVal {
  int32_t val1{};
  int32_t val2{};
};

TEST(EMtQueueTest, testEmpty) {
  DummyVal dm{};
  mt::EMtQueue<DummyVal, 10> q{};
  ASSERT_EQ(q.pop(dm), false);
}

TEST(EMtQueueTest, testFull) {
  const int32_t Q_SIZE{10};
  int32_t idx{};
  int32_t limit{10};
  mt::EMtQueue<DummyVal, Q_SIZE> q{};
  for (; idx < limit; idx++) {
    q.push(DummyVal{idx, idx});
  }
  ASSERT_EQ(q.push(DummyVal{idx + 1, idx + 1}), false);
}

TEST(EMtQueueTest, testDepth1Sample100000) {
  const int32_t Q_SIZE{1};
  int32_t idx{};
  int32_t limit{100000};
  DummyVal dm{};
  mt::EMtQueue<DummyVal, Q_SIZE> q{};
  bool res1{false}, res2{false};
  for (; idx < limit; idx++) {
    do {
      res1 = q.push(DummyVal{idx, idx});
      res2 = q.pop(dm);
    } while (res1 == false || res2 == false);
    ASSERT_EQ(dm.val1, idx);
    ASSERT_EQ(dm.val2, idx);
  }
}

TEST(EMtQueueTest, testDepth10Sample100000) {
  const int32_t Q_SIZE{10};
  int32_t idx{};
  int32_t limit{100000};
  DummyVal dm{};
  mt::EMtQueue<DummyVal, Q_SIZE> q{};
  bool res1{false}, res2{false};
  for (; idx < limit; idx++) {
    do {
      res1 = q.push(DummyVal{idx, idx});
      res2 = q.pop(dm);
    } while (res1 == false || res2 == false);
    ASSERT_EQ(dm.val1, idx);
    ASSERT_EQ(dm.val2, idx);
  }
}

TEST(EMtQueueTest, testDepth100000Sample100000) {
  const int32_t Q_SIZE{100000};
  int32_t idx{};
  int32_t limit{100000};
  DummyVal dm{};
  mt::EMtQueue<DummyVal, Q_SIZE> q{};
  bool res1{false}, res2{false};
  for (; idx < limit; idx++) {
    do {
      res1 = q.push(DummyVal{idx, idx});
      res2 = q.pop(dm);
    } while (res1 == false || res2 == false);
    ASSERT_EQ(dm.val1, idx);
    ASSERT_EQ(dm.val2, idx);
  }
}

TEST(EMtQueueTest, testWaitPopSamplesNoWait) {
  mt::EMtQueue<DummyVal> q{};
  DummyVal dm{};
  for (int32_t idx = 0; idx < 5; idx++) {
    q.push(DummyVal{idx, idx});
  }
  for (int32_t idx = 0; idx < 5; idx++) {
    q.wait_pop(dm, 100000000);
    ASSERT_EQ(dm.val1, idx);
    ASSERT_EQ(dm.val2, idx);
  }
}

TEST(EMtQueueTest, testWaitPopSamplesWait) {
  mt::EMtQueue<DummyVal> q{};
  DummyVal dm{};
  std::time_t t1 = std::time(0);
  ASSERT_EQ(q.wait_pop(dm, 1000000), false);
  std::time_t t2 = std::time(0);
  ASSERT_GE(t2 - t1, 1);
}

TEST(EMtQueueTest, testWaitPassByReference) {
  mt::EMtQueue<DummyVal> q{};
  DummyVal out{};
  {

    for (int32_t idx = 0; idx < 5; idx++) {
      DummyVal in{idx, idx};
      q.push(in);
    }
  }
  for (int32_t idx = 0; idx < 5; idx++) {
    q.wait_pop(out, 100000000);
    ASSERT_EQ(out.val1, idx);
    ASSERT_EQ(out.val2, idx);
  }
}

TEST(EMtQueueTest, lastElementPopUnblocksPushes) {
  mt::EMtQueue<DummyVal, 2> q{};
  DummyVal val{};

  q.push(DummyVal{0, 0});
  q.push(DummyVal{1, 1});
  ASSERT_EQ(q.push(DummyVal{3, 3}), false);

  ASSERT_EQ(q.pop(val), true);
  ASSERT_EQ(val.val1 == 0 & val.val2 == 0, true);

  ASSERT_EQ(q.pop(val), true);
  ASSERT_EQ(val.val1 == 1 & val.val2 == 1, true);

  ASSERT_EQ(q.push(DummyVal{2, 2}), true);
  ASSERT_EQ(q.push(DummyVal{7, 7}), true);
}