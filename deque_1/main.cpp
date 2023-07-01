#include <vector>
#include <stdexcept>
template <typename T>
class Deque {
 public:
  template <bool IsConst>

  class common_iterator;
  using iterator = common_iterator<false>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_iterator = common_iterator<true>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  Deque();
  Deque(const Deque& other);
  Deque(size_t count);
  Deque(size_t count, const T& k_value);
  ~Deque();

  Deque& operator=(const Deque& other);
  size_t size() const;
  bool empty();
  T& operator[](size_t index);
  const T& operator[](size_t index) const;
  T& at(size_t index);
  const T& at(size_t index) const;
  void push_back(const T& k_value);
  void pop_back();
  void push_front(const T& k_value);
  void pop_front();
  void insert(iterator deque_it, const T& val);
  void erase(iterator deque_it);
  void clear();
  iterator begin();
  const_iterator cbegin() const;
  iterator end();
  const_iterator cend() const;
  reverse_iterator rend();
  reverse_iterator rbegin();
  const_reverse_iterator crend() const;
  const_reverse_iterator crbegin() const;
  const size_t kCountBlock = 256;
 private:
  size_t count_block_ = kCountBlock;
  static const size_t kSizeBlock = 100000;
  std::vector<T*> buff_{count_block_};
  iterator begin_;
  iterator end_;
};

template <typename T>
template <bool IsConst>
class Deque<T>::common_iterator {
 public:
  using value_type = typename std::conditional<IsConst,const T, T>::type;
  using pointer = typename std::conditional<IsConst,const T*, T*>::type;
  using iterator_category = typename std::random_access_iterator_tag;
  using reference = typename std::conditional<IsConst,const T&, T&>::type;
  using difference_type = typename std::ptrdiff_t;
  size_t block = 0;
  size_t position = 0;
  Deque<T>* container;
  common_iterator(size_t block, size_t position) : block(block), position(position) {}
  common_iterator(Deque<T>* container, size_t block, size_t position) : container(container), block(block), position(position) {}
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
    common_iterator new_it = *this;
    new_it += kValue;
    return new_it;
  }
  common_iterator operator-(const size_t kValue) const {
    common_iterator new_it = *this;
    new_it -= kValue;
    return new_it;
  }
  bool operator<(const common_iterator& other) const {
    return (block < other.block || (block == other.block && position < other.position));
  }
  bool operator>(const common_iterator& other) const {
    return other < *this;
  }
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
  pointer operator->() {
    return (container->buff_)[block] + position;
  }
  pointer operator->() const {
    return (container->buff_)[block] + position;
  }
  reference operator*() {
    return *((container->buff_)[block] + position);
  }

  difference_type operator-(const common_iterator& other) const {
    if(block != other.block) {
      return kSizeBlock + (block - other.block - 1) * kSizeBlock + position - other.position;
    }
    return position - other.position;
  }
  operator common_iterator<true>() const {
    return common_iterator<true>(container, block, position);
  }
};
template <typename T>
Deque<T>::Deque() : begin_(this, (count_block_ - 1) / 2, 0),
                    end_(this, (count_block_ - 1) / 2, 0) {

  for (size_t i = 0; i < count_block_; ++i) {
    // выделяем память не создавая объекты
    buff_[i] = reinterpret_cast<T*>(::operator new(sizeof(T) * kSizeBlock));
  }
}

template <typename T>
Deque<T>::Deque(const Deque& other) : count_block_(other.size()),
                                      buff_(count_block_),
                                      begin_(this, (count_block_ - 1) / 2, 0),
                                      end_(this, (count_block_ - 1) / 2, 0) {
  try {
    for (size_t i = 0; i < count_block_; ++i) {
      // выделяем память не создавая объекты
      buff_[i] = reinterpret_cast<T*>(::operator new(sizeof(T) * kSizeBlock));
    }
    for (size_t i = 0; i < count_block_; ++i) {
      new(buff_[end_.block] + end_.position) T(other.operator[](i));
      ++end_;
    }
  } catch(...) {
    clear();
    throw;
  }

}

template <typename T>
Deque<T>::Deque(size_t count) : count_block_((count / kSizeBlock + 1) * 2),
                                buff_(count_block_),
                                begin_(this, (count_block_ - 1) / 2, 0),
                                end_(this, (count_block_ - 1) / 2, 0){
  try {
    for (size_t i = 0; i < count_block_; ++i) {
      // выделяем память не создавая объекты
      buff_[i] = reinterpret_cast<T*>(::operator new(sizeof(T) * kSizeBlock));
    }
    for (size_t i = 0; i < count; ++i) {
      new(buff_[end_.block] + end_.position) T(0);
      ++end_;
    }
  } catch(...) {
    clear();
    throw;
  }
}

template <typename T>
Deque<T>::Deque(size_t count, const T& k_value) :  count_block_((count / kSizeBlock + 1) * 2),
                                                   begin_(this, (count_block_ - 1) / 2, 0),
                                                   end_(this, (count_block_ - 1) / 2, 0),
                                                   buff_(count_block_) {
  try {
    for (size_t i = 0; i < count_block_; ++i) {
      // выделяем память не создавая объекты
      buff_[i] = reinterpret_cast<T*>(::operator new(sizeof(T) * kSizeBlock));
    }
    for (size_t i = 0; i < count; ++i) {
      new(buff_[end_.block] + end_.position) T(k_value);
      ++end_;
    }
  } catch(...) {
    clear();
    throw;
  }
}

template <typename T>
void Deque<T>::clear() {
  for (size_t i = 0; i < count_block_; ++i) {
    for (size_t j = 0; j < kSizeBlock; ++j) {
      buff_[i][j].~T();
    }
    ::operator delete(buff_[i]);
  }
}

template<typename T>
Deque<T>::~Deque() {
  clear();
}
template<typename T>
Deque<T>& Deque<T>::operator=(const Deque<T>& other) {
  if (this == &other) {
    return *this;
  }
  if (size() != other.size()) {
    try {
      clear();
      buff_.clear();
      for (size_t i = 0; i < other.count_block_; ++i) {
        // выделяем память не создавая объекты
        buff_.push_back(reinterpret_cast<T*>(::operator new(sizeof(T) * kSizeBlock)));
      }
      count_block_ = other.count_block_;
      begin_ = other.begin_;
      end_ = other.begin_;
      for (size_t i = 0; i < other.size(); ++i) {
        new(buff_[end_.block] + end_.position) T(other[i]);
        ++end_;
      }
    } catch (...) {
      throw;
    }
  }
  return *this;
}

template <typename T>
size_t Deque<T>::size() const {
  if (begin_.block != end_.block) {
    return (end_.block - begin_.block - 1) * kSizeBlock + kSizeBlock + end_.position - begin_.position;
  }
  return end_.position - begin_.position;
}

template <typename T>
bool Deque<T>::empty() {
  if (begin_.block == end_.block) {
    if (end_.position == begin_.position) {
      return true;
    }
  }
  return false;
}

template <typename T>
T& Deque<T>::operator[](size_t index){
  iterator deque_it = begin_;
  deque_it += index;
  return *(buff_[deque_it.block]+deque_it.position);
}

template <typename T>
const T& Deque<T>::operator[](size_t index) const{
  iterator deque_it = begin_;
  deque_it += index;
  return *(buff_[deque_it.block]+deque_it.position);
}

template <typename T>
T& Deque<T>::at(size_t index) {
  if (index >= size()) {
    throw std::out_of_range("Out of range");
  }
  return Deque<T>::operator[](index);
}

template <typename T>
const T& Deque<T>::at(size_t index) const{
  if (index >= size()) {
    throw std::out_of_range("Out of range");
  }
  return Deque<T>::operator[](index);
}

template <typename T>
void Deque<T>::push_back(const T& k_value) {
  if (end_.block == count_block_ && end_.position == 0) {
    size_t size_new = end_.block - begin_.block + 1;
    // copy all elem
    size_t index = 0;
    while (index < size_new) {
      buff_.insert(buff_.end(), 1, reinterpret_cast<T*>(::operator new(sizeof(T) * kSizeBlock)));
      ++index;
    }
    count_block_ += size_new;
  }
  new (buff_[end_.block] + end_.position) T(k_value);
  ++end_;
}

template <typename T>
void Deque<T>::pop_back() {
  --end_;
  buff_[end_.block][end_.position].~T();
}
template <typename T>
void Deque<T>::push_front(const T& k_value) {
  if (begin_.block == 0 && begin_.position == 0) {
    size_t size_new = end_.block - begin_.block + 1;
    // copy all elem
    size_t index = 0;
    while (index < size_new) {
      buff_.insert(buff_.begin(), 1, reinterpret_cast<T*>(::operator new(sizeof(T) * kSizeBlock)));
      ++index;
    }
    end_.block += size_new;
    begin_.block+= size_new;
    count_block_ += size_new;
  }
  --begin_;
  new (buff_[begin_.block] + begin_.position) T(k_value);
}
template <typename T>
void Deque<T>::pop_front() {
  buff_[begin_.block][begin_.position].~T();
  ++begin_;
}

template <typename T>
void Deque<T>::insert(Deque<T>::iterator deque_it, const T& val) {
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

template <typename T>
void Deque<T>::erase(Deque<T>::iterator deque_it) {
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

template <typename T>
typename Deque<T>::iterator Deque<T>::begin() {
  return begin_;
}

template <typename T>
typename Deque<T>::const_iterator Deque<T>::cbegin() const {
  return begin_;
}

template <typename T>
typename Deque<T>::iterator Deque<T>::end() {
  return end_;
}

template <typename T>
typename Deque<T>::const_iterator Deque<T>::cend() const {
  return end_;
}

template <typename T>
typename Deque<T>::reverse_iterator Deque<T>::rend() {
  return std::make_reverse_iterator(begin());
}

template <typename T>
typename Deque<T>::reverse_iterator Deque<T>::rbegin() {
  return std::make_reverse_iterator(end());
}

template <typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::crend() const {
  return std::make_reverse_iterator(cbegin());
}

template <typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::crbegin() const {
  return std::make_reverse_iterator(cend());
}
