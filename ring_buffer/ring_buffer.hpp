#pragma once

#include <cstddef>
#include <vector>

class RingBuffer {
 public:
  explicit RingBuffer(size_t capacity) {
    queue_.resize(capacity, 0);
    max_size_ = capacity;
  }

  size_t Size() const { return size_; }

  bool Empty() const { return size_ == 0; }

  bool TryPush(int element) {
    if (size_ == max_size_) {
      return false;
    }
    queue_[last_] = element;
    ++size_;
    ++last_;
    if (last_ >= max_size_) {
      last_ %= max_size_;
    }
    return true;
  }

  bool TryPop(int* element) {
    if (size_ == 0) {
      return false;
    }
    *element = queue_[first_];
    --size_;
    ++first_;
    if (first_ >= max_size_) {
      first_ %= max_size_;
    }
    return true;
  }

 private:
  std::vector<int> queue_;
  size_t max_size_ = 0;
  size_t size_ = 0;
  size_t first_ = 0;
  size_t last_ = 0;
};
