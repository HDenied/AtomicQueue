#include "mqueue.hpp"
#include <gtest/gtest.h>
#include <string>

namespace mt=mtqueue;
/*
TEST(MtQueueTest,testAtomicUint8)
{
  ASSERT_EQ(cu::MtQueue<uint8_t>::isSizeLockFree<uint8_t>(),true);
}

TEST(MtQueueTest,testAtomicUint16)
{
  ASSERT_EQ(cu::MtQueue<uint8_t>::isSizeLockFree<uint16_t>(),true);
}

TEST(MtQueueTest,testAtomicUint32)
{
  EXPECT_EQ(cu::MtQueue<uint8_t>::isSizeLockFree<uint32_t>(),true);
}

TEST(MtQueueTest, testEmpty) {
  cu::MtQueue<int, 10> q{};
  ASSERT_EQ(q.empty(), true);
}

TEST(MtQueueTest, testCountEmpty) {
  cu::MtQueue<int, 10> q{};
  ASSERT_EQ(q.size(), 0);
}

TEST(MtQueueTest, testPush) {
  cu::MtQueue<int, 10> q{};
  q.push(1);
  ASSERT_EQ(q.empty(), false);
}

TEST(MtQueueTest, testFront) {
  cu::MtQueue<int, 10> q{};
  q.push(5);
  ASSERT_EQ(q.front(), 5);
}

TEST(MtQueueTest, testMultiplePop) {
  cu::MtQueue<int, 10> q{};
  q.push(5);
  q.pop();
  ASSERT_EQ(q.empty(), true);
  q.pop();
  ASSERT_EQ(q.empty(), true);
}

TEST(MtQueueTest, testCountMax) {
  cu::MtQueue<int> q{};
  for (int val = 0; val < UCHAR_MAX; val++)
    q.push(val);

  ASSERT_EQ(q.empty(), false);
  ASSERT_EQ(q.size(), UCHAR_MAX);
  ASSERT_EQ(q.front(), 0);
  ASSERT_EQ(q.back(), UCHAR_MAX - 1);
}

TEST(MtQueueTest, beyondCountMax) {
  cu::MtQueue<int> q{};
  for (int val = 0; val < (UCHAR_MAX + 10); val++)
    q.push(val);

  ASSERT_EQ(q.empty(), false);
  ASSERT_EQ(q.size(), UCHAR_MAX);
  ASSERT_EQ(q.back(), UCHAR_MAX - 1);
}

TEST(MtQueueTest, pushReadNelementsTwice) {
  cu::MtQueue<int> q{};
  int t_arr1[]{4, 53, -321, 43, 12, 43, -1212, 43};
  int t_arr2[]{123, -3654, 321, 567, -16, 8, 32, 5, 0, -7};
  int t_arr1_size = sizeof(t_arr1) / sizeof(int);
  int t_arr2_size = sizeof(t_arr2) / sizeof(int);
  for (int n = 0; n < 2; n++) {
    int *t_arr = n == 0 ? t_arr1 : t_arr2;
    int n_el = n == 0 ? t_arr1_size : t_arr2_size;
    for (int val = t_arr[0]; val < n_el; val++) {
      q.push(t_arr[val]);
    }

    for (int val = t_arr[0]; val < n_el; val++) {
      ASSERT_EQ(q.front(), t_arr[val]);
      q.pop();
    }

    ASSERT_EQ(q.empty(), true);
  }
}

TEST(MtQueueTest, pushReadInterleaved)
{
  cu::MtQueue<int> q{};

  for(int idx=1; idx <= UCHAR_MAX; idx++)
  {
    q.push(idx);
    ASSERT_EQ(q.front(), idx);
    ASSERT_EQ(q.back(), idx);
    q.pop();
  }

}

TEST(MtQueueTest, pushReadWrapping) {
  cu::MtQueue<int, 10> q{};
  for (int val = 10; val >= 1; val--) {
    q.push(val);
  }

  ASSERT_EQ(q.size(), 10);

  for (int val = 0; val < 9; val++) {
    q.pop();
  }

  ASSERT_EQ(q.size(), 1);

  q.push(91);
  q.push(-1);
  q.push(-180);

  ASSERT_EQ(q.back(), -180);
  ASSERT_EQ(q.front(), 1);

  q.push(7);
  q.push(-6);
  q.push(82);
  q.push(83);
  q.push(-84);
  q.push(85);

  // Here we check that the values are still the last remaining 1 wasn't popped
  // before and the last one pushed
  ASSERT_EQ(q.back(), 85);
  ASSERT_EQ(q.front(), 1);
  ASSERT_EQ(q.size(), 10);

  // If we push more values the queue shouldn't get more values because is full
  q.push(97);
  q.push(392);
  ASSERT_EQ(q.back(), 85);
  ASSERT_EQ(q.front(), 1);
  ASSERT_EQ(q.size(), 10);

  // We now try to remove  a value and see what happens
  q.pop();
  ASSERT_EQ(q.size(), 9);
  q.push(128);
  ASSERT_EQ(q.back(), 128);
  ASSERT_EQ(q.front(), 91);
}

TEST(MtQueueTest, pushReadCustomClass) {
  cu::MtQueue<std::string, 10> q{};
  q.push("Italia");
  ASSERT_EQ(q.front(), "Italia");
  q.pop();
  ASSERT_EQ(q.size(), 0);
}
*/