#pragma once

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <system_error>
#include <vector>
template <size_t N, size_t M, typename T = int64_t>
class Matrix {
 public:
  Matrix();
  Matrix(std::vector<std::vector<T>> arr);
  Matrix(T elem);
  Matrix operator+=(Matrix src);
  Matrix operator-=(Matrix src);
  size_t GetN();
  size_t GetM();
  bool operator==(const Matrix& src) const;
  T operator()(size_t rows, size_t cols) const;
  T& operator()(size_t rows, size_t cols);
  template <size_t K>
  Matrix<N, K, T> operator*(const Matrix<M, K, T>& src) const;
  Matrix<M, N, T> Transposed();
  T Trace();

 private:
  std::vector<std::vector<T>> matrix_;
  size_t n_{0};
  size_t m_{0};
};

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix() : n_(N), m_(M) {
  matrix_.assign(N, std::vector<T>(M, T()));
}
template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix(T elem) {
  matrix_.resize(N, std::vector<T>(M, elem));
  n_ = N;
  m_ = M;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix(std::vector<std::vector<T>> arr) {
  matrix_ = arr;
  n_ = N;
  m_ = M;
}

template <size_t N, size_t M, typename T>
T Matrix<N, M, T>::operator()(size_t rows, size_t cols) const {
  if (rows < n_ && cols < m_) {
    return matrix_[rows][cols];
  }
  throw std::errc::invalid_argument;
}

template <size_t N, size_t M, typename T>
T& Matrix<N, M, T>::operator()(size_t rows, size_t cols) {
  if (rows < n_ && cols < m_) {
    return matrix_[rows][cols];
  }
  throw std::errc::invalid_argument;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> Matrix<N, M, T>::operator+=(Matrix<N, M, T> src) {
  for (size_t i = 0; i < n_; ++i) {
    for (size_t j = 0; j < m_; ++j) {
      (*this)(i, j) += src(i, j);
    }
  }
  return *this;
}
template <size_t N, size_t M, typename T>
Matrix<N, M, T> Matrix<N, M, T>::operator-=(Matrix<N, M, T> src) {
  for (size_t i = 0; i < n_; ++i) {
    for (size_t j = 0; j < m_; ++j) {
      (*this)(i, j) -= src(i, j);
    }
  }
  return *this;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator-(const Matrix<N, M, T>& arr1,
                          const Matrix<N, M, T>& arr2) {
  Matrix<N, M, T> tmp = arr1;
  tmp -= arr2;
  return tmp;
}
template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator+(const Matrix<N, M, T>& arr1,
                          const Matrix<N, M, T>& arr2) {
  Matrix<N, M, T> tmp = arr1;
  tmp += arr2;
  return tmp;
}
template <size_t N, size_t M, typename T>
size_t Matrix<N, M, T>::GetN() {
  return n_;
}

template <size_t N, size_t M, typename T>
template <size_t K>
Matrix<N, K, T> Matrix<N, M, T>::operator*(const Matrix<M, K, T>& src) const {
  Matrix<N, K, T> tmp;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < K; ++j) {
      for (size_t k = 0; k < M; ++k) {
        tmp(i, j) += (*this)(i, k) * src(k, j);
      }
    }
  }
  return tmp;
}
template <size_t N, size_t M, typename T>
size_t Matrix<N, M, T>::GetM() {
  return m_;
}
template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator*(const Matrix<N, M, T>& arr1, const T& arr2) {
  Matrix<N, M, T> tmp = arr1;
  for (size_t i = 0; i < tmp.GetN(); ++i) {
    for (size_t j = 0; j < tmp.GetM(); ++j) {
      tmp(i, j) *= arr2;
    }
  }
  return tmp;
}
template <size_t N, size_t M, typename T>
Matrix<M, N, T> Matrix<N, M, T>::Transposed() {
  Matrix<M, N, T> res;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      res(j, i) += (*this)(i, j);
    }
  }
  return res;
}

template <size_t N, size_t M, typename T>
T Matrix<N, M, T>::Trace() {
  T res = 0;
  for (size_t i = 0; i < N; ++i) {
    res += (*this)(i, i);
  }
  return res;
}

template <size_t N, size_t M, typename T>
bool Matrix<N, M, T>::operator==(const Matrix<N, M, T>& src) const {
  bool tmp = true;
  if (src.n_ != n_ || m_ != src.m_) {
    return false;
  }
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      tmp = ((*this)(i, j) == src(i, j));
      if (!tmp) {
        return tmp;
      }
    }
  }
  return tmp;
}
