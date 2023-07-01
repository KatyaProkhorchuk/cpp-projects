#include <array>
#include <stdexcept>
#include <vector>
template <typename T, typename Allocator = std::allocator<T>>
class Deque {
 public:
  /* iterator */
  template <bool IsConst>
  class common_iterator;
  /* using */
  using iterator = common_iterator<false>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_iterator = common_iterator<true>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  /* allocator */
  using allocator_type = Allocator;
  using alloc_traits = std::allocator_traits<Allocator>;
  Allocator get_allocator() { return alloc_; }
  /* constructor */
  Deque();
  explicit Deque(const Allocator& allocator);
  Deque(const Deque& other);
  Deque(Deque&& other);
  Deque(size_t count, const Allocator& alloc = Allocator());
  Deque(size_t count, const T& k_value, const Allocator& alloc = Allocator());
  Deque(std::initializer_list<T> init, const Allocator& alloc = Allocator());
  /* destructor */
  ~Deque();
  void clear();
  void reserve();
  void resize_back();
  void resize_front();
  /* operators =, [] */
  Deque& operator=(const Deque& other);
  Deque& operator=(Deque&& other) noexcept;
  T& operator[](size_t index);
  const T& operator[](size_t index) const;
  T& at(size_t index);
  const T& at(size_t index) const;
  /* check state */
  size_t size() const;
  bool empty();
  /* emplace */
  template <typename... Args>
  void emplace_back(Args&&... args);
  template <typename... Args>
  void emplace_front(Args&&... args);
  /* push, pop*/
  void push_back(const T& k_value);
  void pop_back();
  void push_front(const T& k_value);
  void push_front(T&& k_value);
  void push_back(T&& value);
  void pop_front();
  /* insert, erase */
  void insert(iterator deque_it, const T& val);
  void erase(iterator deque_it);
  /* swap */
  void swap(const Deque& other);
  void copy_from_other(const Deque& other);
  /* iterator */
  iterator begin();
  const_iterator cbegin() const;
  iterator end();
  const_iterator cend() const;
  reverse_iterator rend();
  reverse_iterator rbegin();
  const_reverse_iterator crend() const;
  const_reverse_iterator crbegin() const;
  const size_t kCountBlock = 64;

 private:
  size_t count_block_ = kCountBlock;
  static const size_t kSizeBlock = 100000;
  std::vector<T*> buff_{count_block_};
  common_iterator<false> begin_;
  common_iterator<false> end_;
  Allocator alloc_;
};
/* -----------class iterator--------------*/
template <typename T, typename Allocator>
template <bool IsConst>
class Deque<T, Allocator>::common_iterator {
 public:
  using value_type = typename std::conditional<IsConst, const T, T>::type;
  using pointer = typename std::conditional<IsConst, const T*, T*>::type;
  using iterator_category = typename std::random_access_iterator_tag;
  using reference = typename std::conditional<IsConst, const T&, T&>::type;
  using difference_type = typename std::ptrdiff_t;
  size_t block = 0;
  size_t position = 0;
  Deque<T, Allocator>* container;
  common_iterator(size_t block, size_t position)
      : block(block), position(position) {}
  common_iterator(Deque<T, Allocator>* container, size_t block, size_t position)
      : container(container), block(block), position(position) {}
  common_iterator& operator++() {
    if (position == kSizeBlock - 1) {
      ++block;
      position = 0;
      return *this;
    }
    ++position;
    return *this;
  }
  common_iterator operator++(int) {
    common_iterator tmp = *this;
    if (position == kSizeBlock - 1) {
      position = 0;
      ++block;
      return tmp;
    }
    ++position;
    return tmp;
  }
  common_iterator& operator--() {
    if (position == 0) {
      --block;
      position = kSizeBlock - 1;
      return *this;
    }
    --position;
    return *this;
  }
  common_iterator operator--(int) {
    common_iterator tmp = *this;
    if (position == 0) {
      --block;
      position = kSizeBlock - 1;
      return tmp;
    }
    --position;
    return tmp;
  }
  common_iterator& operator+=(const size_t kValue) {
    block = (block * kSizeBlock + kValue + position);
    block /= kSizeBlock;
    position = (kValue + position);
    position %= kSizeBlock;
    return *this;
  }
  common_iterator& operator-=(const size_t kValue) {
    block = (block * kSizeBlock - kValue + position);
    block /= kSizeBlock;
    position = (position - kValue);
    position %= kSizeBlock;
    return *this;
  }
  common_iterator operator+(const size_t kValue) const {
    common_iterator iterator = *this;
    iterator += kValue;
    return iterator;
  }
  common_iterator operator-(const size_t kValue) const {
    common_iterator iterator = *this;
    iterator -= kValue;
    return iterator;
  }
  bool operator<(const common_iterator& other) const {
    return (block < other.block ||
            (block == other.block && position < other.position));
  }
  bool operator>(const common_iterator& other) const { return other < *this; }
  bool operator>=(const common_iterator& other) const {
    return !(*this < other);
  }
  bool operator<=(const common_iterator& other) const {
    return !(other < *this);
  }
  bool operator==(const common_iterator& other) const {
    return *this >= other && *this <= other && container == other.container;
  }
  bool operator!=(const common_iterator& other) const {
    return !(other == *this);
  }
  pointer operator->() { return (container->buff_)[block] + position; }
  pointer operator->() const { return (container->buff_)[block] + position; }
  reference operator*() { return *((container->buff_)[block] + position); }

  difference_type operator-(const common_iterator& other) const {
    if (block != other.block) {
      return kSizeBlock + (block - other.block - 1) * kSizeBlock + position -
             other.position;
    }
    return position - other.position;
  }
  operator common_iterator<true>() const {
    return common_iterator<true>(container, block, position);
  }
};

template <typename T, typename Allocator>
void Deque<T, Allocator>::reserve() {
  for (size_t i = 0; i < count_block_; ++i) {
    buff_[i] = alloc_traits::allocate(alloc_, kSizeBlock * sizeof(T));
  }
}
template <typename T, typename Allocator>
Deque<T, Allocator>::Deque() : Deque(Allocator()) {}
template <typename T, typename Allocator>
Deque<T, Allocator>::Deque(const Allocator& allocator)
    : begin_(this, (count_block_ - 1) / 2, 0),
      end_(this, (count_block_ - 1) / 2, 0),
      alloc_(allocator) {
  try {
    reserve();
  } catch (...) {
    clear();
    throw;
  }
}

template <typename T, typename Allocator>
Deque<T, Allocator>::Deque(const Deque& other)
    : count_block_((other.size() / kSizeBlock + 1) * 2),
      buff_(count_block_),
      begin_(this, (count_block_ - 1) / 2, 0),
      end_(this, (count_block_ - 1) / 2, 0),
      alloc_(
          alloc_traits::select_on_container_copy_construction(other.alloc_)) {
  try {
    reserve();
    for (size_t i = 0; i < other.size(); ++i) {
      alloc_traits::construct(alloc_, buff_[end_.block] + end_.position,
                              other.operator[](i));
      ++end_;
    }
  } catch (...) {
    clear();
    throw;
  }
}
template <typename T, typename Allocator>
Deque<T, Allocator>::Deque(Deque&& other)
    : count_block_((other.size() / kSizeBlock + 1) * 2),
      buff_(count_block_),
      begin_(this, (count_block_ - 1) / 2, 0),
      end_(this, (count_block_ - 1) / 2, 0),
      alloc_(
          alloc_traits::select_on_container_copy_construction(other.alloc_)) {
  try {
    reserve();
    for (size_t i = 0; i < other.size(); ++i) {
      alloc_traits::construct(alloc_, buff_[end_.block] + end_.position,
                              std::move(other.operator[](i)));
      ++end_;
    }
    while (other.begin_ != other.end_) {
      other.pop_back();
    }
  } catch (...) {
    clear();
    throw;
  }
}
template <typename T, typename Allocator>
Deque<T, Allocator>::Deque(size_t count, const Allocator& alloc)
    : count_block_((count / kSizeBlock + 1) * 2),
      buff_(count_block_),
      begin_(this, (count_block_ - 1) / 2, 0),
      end_(this, (count_block_ - 1) / 2, 0),
      alloc_(alloc) {
  try {
    reserve();
    for (size_t i = 0; i < count; ++i) {
      alloc_traits::construct(alloc_, buff_[end_.block] + end_.position);
      ++end_;
    }
  } catch (...) {
    clear();
    throw;
  }
}

template <typename T, typename Allocator>
Deque<T, Allocator>::Deque(size_t count, const T& k_value,
                           const Allocator& alloc)
    : count_block_((count / kSizeBlock + 1) * 2),
      begin_(this, (count_block_ - 1) / 2, 0),
      end_(this, (count_block_ - 1) / 2, 0),
      buff_(count_block_),
      alloc_(alloc) {
  try {
    reserve();
    for (size_t i = 0; i < count; ++i) {
      alloc_traits::construct(alloc_, buff_[end_.block] + end_.position,
                              k_value);
      ++end_;
    }
  } catch (...) {
    clear();
    throw;
  }
}
template <typename T, typename Allocator>
Deque<T, Allocator>::Deque(std::initializer_list<T> init,
                           const Allocator& alloc)
    : count_block_((init.size() / kSizeBlock + 1) * 2),
      begin_(this, (count_block_ - 1) / 2, 0),
      end_(this, (count_block_ - 1) / 2, 0),
      buff_(count_block_),
      alloc_(alloc) {
  try {
    reserve();
    for (auto& value : init) {
      alloc_traits::construct(alloc_, buff_[end_.block] + end_.position, value);
      ++end_;
    }
  } catch (...) {
    clear();
    throw;
  }
}

/*------------------clear--------------*/
template <typename T, typename Allocator>
void Deque<T, Allocator>::clear() {
  size_t size_deque = size();
  for (size_t i = 0; i < count_block_; ++i) {
    for (size_t j = 0; j < kSizeBlock && i * kSizeBlock + j < size_deque; ++j) {
      pop_back();
    }
    alloc_traits::deallocate(alloc_, buff_[i], kSizeBlock * sizeof(T));
  }
}
/* destructor */
template <typename T, typename Allocator>
Deque<T, Allocator>::~Deque() {
  clear();
}

template <typename T, typename Allocator>
void Deque<T, Allocator>::swap(const Deque<T, Allocator>& other) {
  count_block_ = other.count_block_;
  begin_ = other.begin_;
  end_ = other.begin_;
  alloc_ = std::move(other.alloc_);
}

template <typename T, typename Allocator>
void Deque<T, Allocator>::copy_from_other(const Deque<T, Allocator>& other) {
  for (size_t i = 0; i < other.size(); ++i) {
    alloc_traits::construct(alloc_, buff_[end_.block] + end_.position,
                            other[i]);
    ++end_;
  }
}

template <typename T, typename Allocator>
Deque<T, Allocator>& Deque<T, Allocator>::operator=(
    const Deque<T, Allocator>& other) {
  if (this != &other) {
    if (size() != other.size()) {
      try {
        if (size() > other.size()) {
          clear();
          buff_.clear();
          reserve();
        } else {
          for (size_t i = 0; i < other.count_block_ - count_block_; ++i) {
            buff_.push_back(alloc_.allocate(kSizeBlock * sizeof(T)));
          }
        }
        swap(other);
        copy_from_other(other);
      } catch (...) {
        for (; begin_ != end_;) {
          pop_back();
        }
        throw;
      }
    } else {
      if constexpr (alloc_traits::propagate_on_container_copy_assignment::
                        value) {
        alloc_ = std::move(other.alloc_);
      }
      end_ = begin_;
      copy_from_other(other);
    }
  }
  return *this;
}

template <typename T, typename Allocator>
Deque<T, Allocator>& Deque<T, Allocator>::operator=(
    Deque<T, Allocator>&& other) noexcept {
  if (this != &other) {
    if (size() != other.size()) {
      try {
        if (size() > other.size()) {
          clear();
          buff_.clear();
          reserve();
        } else {
          for (size_t i = 0; i < other.count_block_ - count_block_; ++i) {
            buff_.push_back(alloc_.allocate(kSizeBlock * sizeof(T)));
          }
        }
        swap(other);
        copy_from_other(std::move(other));
      } catch (...) {
        for (; begin_ != end_;) {
          alloc_traits::destroy(alloc_, buff_[end_.block] + end_.position);
          --end_;
        }
        throw;
      }
    } else {
      if constexpr (alloc_traits::propagate_on_container_copy_assignment::
                        value) {
        alloc_ = std::move(other.alloc_);
      }
      end_ = begin_;
      copy_from_other(other);
    }
  }
  return *this;
}

template <typename T, typename Allocator>
size_t Deque<T, Allocator>::size() const {
  if (begin_.block != end_.block) {
    return (end_.block - begin_.block - 1) * kSizeBlock + kSizeBlock +
           end_.position - begin_.position;
  }
  return end_.position - begin_.position;
}

template <typename T, typename Allocator>
bool Deque<T, Allocator>::empty() {
  if (begin_.block == end_.block) {
    if (end_.position == begin_.position) {
      return true;
    }
  }
  return false;
}

template <typename T, typename Allocator>
T& Deque<T, Allocator>::operator[](size_t index) {
  iterator deque_it = begin_;
  deque_it += index;
  return *(buff_[deque_it.block] + deque_it.position);
}

template <typename T, typename Allocator>
const T& Deque<T, Allocator>::operator[](size_t index) const {
  iterator deque_it = begin_;
  deque_it += index;
  return *(buff_[deque_it.block] + deque_it.position);
}

template <typename T, typename Allocator>
T& Deque<T, Allocator>::at(size_t index) {
  if (index >= size()) {
    throw std::out_of_range("Out of range");
  }
  return Deque<T>::operator[](index);
}

template <typename T, typename Allocator>
const T& Deque<T, Allocator>::at(size_t index) const {
  if (index >= size()) {
    throw std::out_of_range("Out of range");
  }
  return Deque<T, Allocator>::operator[](index);
}

/* resize */
template <typename T, typename Allocator>
void Deque<T, Allocator>::resize_back() {
  if (end_.block == count_block_ && end_.position == 0) {
    size_t size_tmp = end_.block - begin_.block + 1;
    // copy all elem
    size_t index = 0;
    while (index < size_tmp) {
      buff_.push_back(alloc_traits::allocate(alloc_, kSizeBlock * sizeof(T)));
      ++index;
    }
    count_block_ += size_tmp;
  }
}

template <typename T, typename Allocator>
void Deque<T, Allocator>::resize_front() {
  if (begin_.block == 0 && begin_.position == 0) {
    size_t size_tmp = end_.block - begin_.block + 1;
    // copy all elem
    size_t index = 0;
    while (index < size_tmp) {
      buff_.insert(buff_.begin(), 1,
                   alloc_traits::allocate(alloc_, kSizeBlock * sizeof(T)));
      ++index;
    }
    end_.block += size_tmp;
    begin_.block += size_tmp;
    count_block_ += size_tmp;
  }
}
/* push_back(T&&) */
template <typename T, typename Allocator>
void Deque<T, Allocator>::push_back(T&& value) {
  emplace_back(std::forward<T>(value));
}
/* push_back(T&) */
template <typename T, typename Allocator>
void Deque<T, Allocator>::push_back(const T& k_value) {
  resize_back();
  alloc_traits::construct(alloc_, buff_[end_.block] + end_.position, k_value);
  ++end_;
}
template <typename T, typename Allocator>
template <typename... Args>
void Deque<T, Allocator>::emplace_back(Args&&... args) {
  resize_back();
  alloc_traits::construct(alloc_, buff_[end_.block] + end_.position,
                          std::forward<Args>(args)...);
  ++end_;
}
template <typename T, typename Allocator>
template <typename... Args>
void Deque<T, Allocator>::emplace_front(Args&&... args) {
  resize_front();
  --begin_;
  alloc_traits::construct(alloc_, buff_[begin_.block] + begin_.position,
                          std::forward<Args>(args)...);
}
template <typename T, typename Allocator>
void Deque<T, Allocator>::pop_back() {
  --end_;
  alloc_traits::destroy(alloc_, buff_[end_.block] + end_.position);
}
template <typename T, typename Allocator>
void Deque<T, Allocator>::push_front(const T& k_value) {
  resize_front();
  --begin_;
  alloc_traits::construct(alloc_, buff_[begin_.block] + begin_.position,
                          k_value);
}
template <typename T, typename Allocator>
void Deque<T, Allocator>::push_front(T&& value) {
  emplace_front(std::forward<T>(value));
}
template <typename T, typename Allocator>
void Deque<T, Allocator>::pop_front() {
  alloc_traits::destroy(alloc_, buff_[begin_.block] + begin_.position);
  ++begin_;
}

template <typename T, typename Allocator>
void Deque<T, Allocator>::insert(Deque<T, Allocator>::iterator deque_it,
                                 const T& val) {
  if (begin_ == deque_it) {
    push_front(val);
    return;
  }
  push_back(*(end_ - 1));
  iterator sdvig = end_ - 2;
  while (sdvig > deque_it) {
    *(sdvig) = *(sdvig - 1);
    --sdvig;
  }
  *deque_it = val;
}

template <typename T, typename Allocator>
void Deque<T, Allocator>::erase(Deque<T, Allocator>::iterator deque_it) {
  if (begin_ == deque_it) {
    pop_front();
    return;
  }
  iterator sdvig = deque_it + 1;
  while (sdvig < end_) {
    *(sdvig - 1) = *sdvig;
    ++sdvig;
  }
  pop_back();
}

template <typename T, typename Allocator>
typename Deque<T, Allocator>::iterator Deque<T, Allocator>::begin() {
  return begin_;
}

template <typename T, typename Allocator>
typename Deque<T, Allocator>::const_iterator Deque<T, Allocator>::cbegin()
    const {
  return begin_;
}

template <typename T, typename Allocator>
typename Deque<T, Allocator>::iterator Deque<T, Allocator>::end() {
  return end_;
}

template <typename T, typename Allocator>
typename Deque<T, Allocator>::const_iterator Deque<T, Allocator>::cend() const {
  return end_;
}

template <typename T, typename Allocator>
typename Deque<T, Allocator>::reverse_iterator Deque<T, Allocator>::rend() {
  return std::make_reverse_iterator(begin());
}

template <typename T, typename Allocator>
typename Deque<T, Allocator>::reverse_iterator Deque<T, Allocator>::rbegin() {
  return std::make_reverse_iterator(end());
}

template <typename T, typename Allocator>
typename Deque<T, Allocator>::const_reverse_iterator
Deque<T, Allocator>::crend() const {
  return std::make_reverse_iterator(cbegin());
}

template <typename T, typename Allocator>
typename Deque<T, Allocator>::const_reverse_iterator
Deque<T, Allocator>::crbegin() const {
  return std::make_reverse_iterator(cend());
}
