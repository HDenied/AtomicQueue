#include "emqueue.hpp"
#include <gtest/gtest.h>
#include <string>

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
