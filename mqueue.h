#pragma once
#include <atomic>
#include <mutex>
#include <stdint.h>

template <class T, uint32_t N = 100> class Mqueue {
  T el[N];
  std::mutex id_guard;
  std::uint8_t id_shift = 0;

  std::atomic<uint8_t> thq_id{};

  uint32_t wr_idx = 0;
  uint32_t rd_idx = 0;

  std::atomic<uint8_t> q_guard{};

public:
  enum S_TYPE : uint8_t { FREE = 0, B_READER = 1, B_WRITER = 2 };
  bool is_supported() const { return std::atomic_is_lock_free(&thq_id); }

  bool get_id(uint8_t &id) {
    std::unique_lock<std::mutex> lck(id_guard);

    if (id_shift < 8) {
      id = 1 << id_shift;
      return false;
    } else {
      return true;
    }
  }

  S_TYPE front(const T **res) {
    if (q_guard.load(std::memory_order_acquire) != S_TYPE::B_WRITER) {
      *res = &(el[rd_idx]);
      return S_TYPE::FREE;
    } else {
      return S_TYPE::B_WRITER;
    }
  }
};