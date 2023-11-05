#include "../include/s21_matrix_oop.h"

#include <algorithm>
#include <stdexcept>

namespace s21 {

S21Matrix::S21Matrix() : S21Matrix(3, 3) {}

S21Matrix::S21Matrix(const int &rows, const int &cols,
                     std::initializer_list<float> list)
    : S21Matrix(rows, cols) {
  if (rows * cols != int(list.size())) {
    throw std::invalid_argument(
        "Incorrect input, matrix rows and cols not eq list size");
  }
  int i = 0;
  for (const float &elm : list) {
    matrix_[i] = elm;
    ++i;
  }
}

/* : <attribute_name>(value) syntax helps to automatically fill attributes with
values */
S21Matrix::S21Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  if (rows > 0 && cols > 0) {
    matrix_ = new float[rows_ * cols_]();
  } else {
    throw std::out_of_range(
        "Incorrect input, matrix rows and cols can't be zero");
  }
}

// copy constructor
S21Matrix::S21Matrix(const S21Matrix &other)
    : S21Matrix(other.rows_, other.cols_) {
  std::copy_n(other.matrix_, rows_ * cols_, matrix_);
}

// move constructor
S21Matrix::S21Matrix(S21Matrix &&other) noexcept
    : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_) {
  other.matrix_ = nullptr;
}

// destructor
S21Matrix::~S21Matrix() { delete[] matrix_; }

S21Matrix S21Matrix::operator*(const S21Matrix &other) {
  // creating result matrix
  S21Matrix res = *this;
  res.MulMatrix(other);
  return res;
}

S21Matrix &S21Matrix::operator=(const S21Matrix &other) & {
  if (this != &other) {
    S21Matrix tmp = other;
    *this = std::move(tmp);
  }
  return *this;
}

S21Matrix &S21Matrix::operator=(S21Matrix &&other) &noexcept {
  cols_ = other.cols_;
  rows_ = other.rows_;
  std::swap(matrix_, other.matrix_);
  return *this;
}

S21Matrix &S21Matrix::operator*=(const S21Matrix &other) & {
  // the result goes to the left matrix
  MulMatrix(other);
  return *this;
}

// index operator overload
float &S21Matrix::operator()(int row, int col) & {
  if (row >= rows_ || col >= cols_) {
    throw std::out_of_range("Incorrect input, index is out of range");
  }
  if (row < 0 || col < 0) {
    throw std::out_of_range("Incorrect input, index is out of range");
  }
  return matrix_[row * cols_ + col];
}

float &S21Matrix::operator()(int row, int col) const & {
  if (row >= rows_ || col >= cols_) {
    throw std::out_of_range("Incorrect input, index is out of range");
  }
  if (row < 0 || col < 0) {
    throw std::out_of_range("Incorrect input, index is out of range");
  }
  return matrix_[row * cols_ + col];
}

// simple arithmetics

void S21Matrix::MulMatrix(const S21Matrix &other) {
  if (cols_ != other.rows_)
    throw std::invalid_argument(
        "The number of columns of the first matrix is not equal to the number "
        "of rows of the second matrix");
  S21Matrix tmp(rows_, other.cols_);
  for (int i = 0, ie = tmp.rows_; i < ie; ++i) {
    for (int j = 0, je = tmp.cols_; j < je; ++j) {
      tmp(i, j) = CalcRowColMul(other, i, j);
    }
  }
  *this = std::move(tmp);
}

float S21Matrix::CalcRowColMul(const S21Matrix &other, int i,
                               int j) const noexcept {
  float sum = 0;

  for (int k = 0, ke = cols_; k < ke; ++k) {
    sum += (*this)(i, k) * other(k, j);
  }
  return sum;
}

S21Matrix S21Matrix::Transpose() const {
  S21Matrix res(cols_, rows_);
  for (auto i = 0; i < res.rows_; ++i) {
    for (auto j = 0; j < res.cols_; ++j) {
      res(i, j) = (*this)(j, i);
    }
  }
  return res;
}

S21Matrix S21Matrix::CreateIdentity(const int dimension) {
  S21Matrix temp(dimension, dimension);
  for (auto i = 0; i < dimension; ++i) {
    temp(i, i) = 1.0f;
  }
  return temp;
}

const float *S21Matrix::GetPointer() const { return matrix_; }
float *S21Matrix::GetPointer() { return matrix_; }
S21Matrix S21Matrix::Init4x4fv(float *matrix) {
  S21Matrix tmp(4, 4);
  std::copy_n(matrix, 16, tmp.matrix_);
  return tmp;
}

}  // namespace s21