#include <iostream>
template <typename T, typename Allocator = std::allocator<T>>
class List {
 public:
  template <bool IsConst>
  class ListIterator;
  using iterator = ListIterator<false>;
  using const_iterator = ListIterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  struct Node;
  using value_type = T;
  using allocator_type = Allocator;
  using alloc_traits = std::allocator_traits<Allocator>;
  using node_alloc = typename alloc_traits::template rebind_alloc<Node>;
  using node_alloc_traits = typename std::allocator_traits<node_alloc>;
  /* iterators method: begin, end, etc */
  iterator begin() const;
  const_iterator cbegin() const;
  reverse_iterator rbegin() const;
  const_reverse_iterator crbegin() const;
  iterator end() const;
  const_iterator cend() const;
  reverse_iterator rend() const;
  const_reverse_iterator crend() const;
  /* -----------------constructors-------------------- */
  List() = default;
  explicit List(size_t count, const Allocator& alloc = Allocator());
  List(size_t count, const T& value,
                const Allocator& alloc = Allocator());
  List(const List<T, Allocator>& other);
  List(std::initializer_list<T> init, const Allocator& alloc = Allocator());
  Allocator get_allocator() { return alloc_node_; }
  /* ------------------capacity---------------------- */
  size_t size() const;
  bool empty() const;
  /* -----------------Modifiers---------------------- */
  void push_back(const T& value);
  void push_front(const T& value);
  void push_back(T&& value);
  void push_front(T&& value);
  void pop_back();
  void pop_front();
  /* ------------element access methods--------------- */
  T& front();
  const T& front() const;
  T& back();
  const T& back() const;
  /* ------------operator=--------------------------- */
  List& operator=(const List& other);
  /* clear list */
  void clear_list();
  /* destructor */
  ~List();

 private:
  template <typename Type>
  void copy_from_list(Type& other);
  void add_node(Node* new_node);
  Node* head_ = nullptr;
  Node* tail_ = nullptr;
  size_t size_ = 0;
  Allocator alloc_;
  node_alloc alloc_node_;
};
template <typename T, typename Allocator>
using list_alloc = List<T, Allocator>;
template <typename T, typename Allocator>
struct List<T, Allocator>::Node {
  T value;
  Node* prev = nullptr;
  Node* next = nullptr;
  Node() = default;
  Node(const T& val, Node* prev = nullptr, Node* next = nullptr)
      : value(val), prev(prev), next(next) {}
};

template <typename T, typename Allocator>
template <bool IsConst>
class List<T, Allocator>::ListIterator {
 public:
  using value_type = typename std::conditional<IsConst, const T, T>::type;
  using pointer = typename std::conditional<IsConst, const T*, T*>::type;
  using reference = typename std::conditional<IsConst, const T&, T&>::type;
  using iterator_category = typename std::bidirectional_iterator_tag;
  using difference_type = typename std::ptrdiff_t;
  ListIterator() = default;
  ListIterator(const List<T, Allocator>* list, Node* list_node)
      : list_(list), list_node_(list_node) {}
  ListIterator(const Node* list_node) : list_node_(list_node){};
  ListIterator(const ListIterator& other)
      : list_node_(other.list_node_), list_(other.list_) {}

  /* operators */
  ListIterator& operator=(const ListIterator& other) {
    list_node_ = other.list_node_;
    return *this;
  }
  bool operator==(const ListIterator& other) const {
    return list_node_ == other.list_node_;
  }
  bool operator!=(const ListIterator& other) const {
    return list_node_ != other.list_node_;
  }
  /* increment, decrement */
  ListIterator& operator++() {
    if (list_node_ == nullptr) {
      list_node_ = list_->head_;
    } else {
      list_node_ = list_node_->next;
    }
    return *this;
  }
  ListIterator operator++(int) {
    ListIterator tmp = *this;
    ++(*this);
    return tmp;
  }
  ListIterator& operator--() {
    if (list_node_ == nullptr) {
      list_node_ = list_->tail_;
    } else {
      list_node_ = list_node_->prev;
    }
    return *this;
  }
  ListIterator operator--(int) {
    ListIterator tmp = *this;
    --(*this);
    return tmp;
  }
  /* operators * ->*/
  pointer operator->() const { return &(list_node_->value); }
  reference operator*() const { return list_node_->value; }

 private:
  Node* list_node_ = nullptr;
  const List<T, Allocator>* list_;
};

/*realization iterators method begin, end, etc*/
template <typename T, typename Allocator>
typename List<T, Allocator>::iterator List<T, Allocator>::begin() const {
  return iterator(this, head_);
}

template <typename T, typename Allocator>
typename List<T, Allocator>::const_iterator List<T, Allocator>::cbegin() const {
  return const_iterator(this, head_);
}

template <typename T, typename Allocator>
typename List<T, Allocator>::reverse_iterator List<T, Allocator>::rbegin()
    const {
  return std::make_reverse_iterator(begin());
}

template <typename T, typename Allocator>
typename List<T, Allocator>::const_reverse_iterator
List<T, Allocator>::crbegin() const {
  return std::make_reverse_iterator(cbegin());
}

template <typename T, typename Allocator>
typename List<T, Allocator>::iterator List<T, Allocator>::end() const {
  return iterator(this, tail_->next);
}

template <typename T, typename Allocator>
typename List<T, Allocator>::const_iterator List<T, Allocator>::cend() const {
  return const_iterator(this, tail_->next);
}

template <typename T, typename Allocator>
typename List<T, Allocator>::reverse_iterator List<T, Allocator>::rend() const {
  return std::make_reverse_iterator(end());
}

template <typename T, typename Allocator>
typename List<T, Allocator>::const_reverse_iterator List<T, Allocator>::crend()
    const {
  return std::make_reverse_iterator(cend());
}

/* -----------------constructors-------------------- */
/* Constructs the container with count copies of elements with value value. */
template <typename T, typename Allocator>
List<T, Allocator>::List(size_t count, const T& value, const Allocator& alloc)
    : size_(0), alloc_(alloc) {
  try {
    for (size_t i = 0; i < count; ++i) {
      push_back(value);
    }
  } catch (...) {
    clear_list();
    throw;
  }
}

/* Constructs the container with count default-inserted instances of T. No
 * copies are made. */
template <typename T, typename Allocator>
List<T, Allocator>::List(size_t count, const Allocator& alloc)
    : size_(0), alloc_(alloc) {
  Node* new_node = nullptr;
  try {
    for (size_t i = 0; i < count; ++i) {
      new_node = alloc_node_.allocate(1);
      node_alloc_traits::construct(alloc_node_, new_node);
      add_node(new_node);
      ++size_;
    }
  } catch (...) {
    if (new_node) {
      node_alloc_traits::deallocate(alloc_node_, new_node, 1);
    }
    clear_list();
    throw;
  }
}

template <typename T, typename Allocator>
List<T, Allocator>::List(const List<T, Allocator>& other)
    : alloc_(alloc_traits::select_on_container_copy_construction(other.alloc_)),
      alloc_node_(node_alloc_traits::select_on_container_copy_construction(
          (other.alloc_node_))) {
  try {
    copy_from_list(other);
  } catch (...) {
    throw;
  }
}
template <typename T, typename Allocator>
List<T, Allocator>::List(std::initializer_list<T> init, const Allocator& alloc)
    : alloc_(alloc) {
  try {
    copy_from_list(init);
  } catch (...) {
    throw;
  }
}

/* ------------------capacity---------------------- */
template <typename T, typename Allocator>
size_t List<T, Allocator>::size() const {
  return size_;
}

template <typename T, typename Allocator>
bool List<T, Allocator>::empty() const {
  return size_ == 0;
}

/* -----------------Modifiers---------------------- */
template <typename T, typename Allocator>
template <typename Type>
void List<T, Allocator>::copy_from_list(Type& other) {
  try {
    for (const auto& value : other) {
      push_back(value);
    }
  } catch (...) {
    clear_list();
    throw;
  }
}
template <typename T, typename Allocator>
void List<T, Allocator>::add_node(List<T, Allocator>::Node* new_node) {
  if (size_ == 0) {
    head_ = tail_ = new_node;
  } else {
    new_node->prev = tail_;
    tail_->next = new_node;
    tail_ = tail_->next;
  }
}

template <typename T, typename Allocator>
void List<T, Allocator>::push_back(const T& value) {
  Node* new_node = nullptr;
  try {
    new_node = alloc_node_.allocate(1);
    node_alloc_traits::construct(alloc_node_, new_node, value);
    add_node(new_node);
    ++size_;
  } catch (...) {
    // если словили исклбчение очищаем память и пробрасываем исколючение наверх
    if (new_node) {
      node_alloc_traits::deallocate(alloc_node_, new_node, 1);
    }
    throw;
  }
}

template <typename T, typename Allocator>
void List<T, Allocator>::push_front(const T& value) {
  Node* new_node = node_alloc_traits::allocate(alloc_node_, 1);
  try {
    node_alloc_traits::construct(alloc_node_, new_node, value, nullptr, head_);
  } catch (...) {
    node_alloc_traits::deallocate(alloc_node_, new_node, 1);
    throw;
  }
  if (empty()) {
    head_ = tail_ = new_node;
  } else {
    new_node->next = head_;
  }
  head_ = new_node;
  ++size_;
}

template <typename T, typename Allocator>
void List<T, Allocator>::push_back(T&& value) {
  push_back(value);
}

template <typename T, typename Allocator>
void List<T, Allocator>::push_front(T&& value) {
  Node* new_node = node_alloc_traits::allocate(alloc_node_, 1);
  try {
    node_alloc_traits::construct(alloc_node_, new_node, std::forward<T>(value),
                                 nullptr, head_);
  } catch (...) {
    node_alloc_traits::deallocate(alloc_node_, new_node, 1);
    throw;
  }
  if (empty()) {
    head_ = tail_ = new_node;
  } else {
    head_->prev = new_node;
  }
  head_ = new_node;
  ++size_;
}

template <typename T, typename Allocator>
void List<T, Allocator>::pop_back() {
  if (empty()) {
    throw std::out_of_range("List is empty");
  }
  Node* temp = tail_;
  Node* prev = (tail_ && tail_->prev) ? tail_->prev : nullptr;
  try {
    tail_ = prev;
    if (!tail_) {
      head_ = nullptr;
    } else {
      tail_->next = nullptr;
    }
    --size_;
    node_alloc_traits::destroy(alloc_node_, temp);
    node_alloc_traits::deallocate(alloc_node_, temp, 1);
  } catch (...) {
    // восстановим то что было
    if (prev) {
      prev->next = tail_;
    } else {
      head_ = tail_;
    }
    if (tail_) {
      tail_->prev = prev;
    }
    throw;
  }
}

template <typename T, typename Allocator>
void List<T, Allocator>::pop_front() {
  if (empty()) {
    throw std::out_of_range("List is empty");
  }
  Node* temp = head_;
  Node* next = (head_ && head_->next) ? head_->next : nullptr;
  try {
    head_ = next;
    if (!head_) {
      tail_ = nullptr;
    } else {
      head_->prev = nullptr;
    }
    --size_;
    node_alloc_traits::destroy(alloc_node_, temp);
    node_alloc_traits::deallocate(alloc_node_, temp, 1);
  } catch (...) {
    // восстановим то что было
    if (next) {
      next->prev = head_;
    } else {
      tail_ = head_;
    }
    if (head_) {
      head_->next = next;
    }
    throw;
  }
}
/* ------------element access methods--------------- */
template <typename T, typename Allocator>
T& List<T, Allocator>::front() {
  if (size_ == 0) {
    throw std::out_of_range("List is empty");
  }
  return head_->value;
}

template <typename T, typename Allocator>
const T& List<T, Allocator>::front() const {
  if (size_ == 0) {
    throw std::out_of_range("List is empty");
  }
  return head_->value;
}

template <typename T, typename Allocator>
T& List<T, Allocator>::back() {
  if (empty()) {
    throw std::out_of_range("List is empty");
  }
  return tail_->value;
}

template <typename T, typename Allocator>
const T& List<T, Allocator>::back() const {
  if (empty()) {
    throw std::out_of_range("List is empty");
  }
  return tail_->value;
}

/* ------------operator=--------------------------- */

template <typename T, typename Allocator>
List<T, Allocator>& List<T, Allocator>::operator=(
    const List<T, Allocator>& other) {
  if (this == &other) {
    return *this;
  }
  clear_list();
  if (alloc_traits::propagate_on_container_copy_assignment::value) {
    alloc_ = other.alloc_;
  }
  if (node_alloc_traits::propagate_on_container_copy_assignment::value) {
    alloc_node_ = other.alloc_node_;
  }
  copy_from_list(other);
  return *this;
}
/* clear list */
template <typename T, typename Allocator>
void List<T, Allocator>::clear_list() {
  while (!empty()) {
    pop_back();
  }
}
/* destructor */
template <typename T, typename Allocator>
List<T, Allocator>::~List() {
  clear_list();
}
