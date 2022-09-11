#pragma once
#include "atlock.hpp"
#include <atomic>
#include <iostream>
#include <limits.h>
#include <memory>
#include <mutex>
#include <stdint.h>
#include <thread>
#include <unistd.h>
#include <utility> 

template <class T, int32_t MAX_SIZE = UCHAR_MAX> class MtQueue {
protected:
  T _q[MAX_SIZE]{};

  int32_t _front{-1};
  int32_t _b{0};

public:
  MtQueue() = default;

  bool push(T &&obj) {
    int32_t pos = 0;
    if ((pos = _front) < MAX_SIZE) {

      if (pos != -1) {
        _q[pos] = obj;
        pos++;
      } else {
        _q[0] = obj;
        pos = 1;
      }

      _front=pos;
      // still room to write
      return true;
    }
    // Writer gets at the end of the queue
    else {
      return false;
    }
  }

  bool pop(T &elem) {
    int32_t end = 0;
    int32_t start = _b.load();
    if ((end = _front) >= 0) {
      if (((end < MAX_SIZE || start < end) && start != end) || end == 0) {
        elem = _q[start];
        _b=start + 1;

        return true;
      } else if (end == MAX_SIZE && start == end) {
        _b=0;
        _front=-1;
        return false;
      } else {
        return false;
      }
    }
    // Not yet started any write
    else {
      return false;
    }
  }
}; // namespace cpputils

