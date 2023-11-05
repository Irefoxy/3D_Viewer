#ifndef INC_3DVIEWER_SRC_INCLUDE_S21MATRIX_H__
#define INC_3DVIEWER_SRC_INCLUDE_S21MATRIX_H__

#include <initializer_list>

/**
 * @file s21_matrix_oop.cc - contains matrix class definitions
 */

namespace s21 {

/**
 * @class matrix class implementation
 */
class S21Matrix {
 public:
  S21Matrix(const int &rows, const int &cols,
            std::initializer_list<float> list);
  S21Matrix();                            // default constructor
  S21Matrix(int rows, int cols);          // parameterized constructor
  S21Matrix(const S21Matrix &other);      // copy constructor
  S21Matrix(S21Matrix &&other) noexcept;  // move constructor
  ~S21Matrix();                           // destructor

  // overloading operators
  S21Matrix operator*(const S21Matrix &other);
  S21Matrix &operator=(
      const S21Matrix &other) &;  // assignment operator overload
  S21Matrix &operator=(
      S21Matrix &&other) &noexcept;  // assignment operator overload
  S21Matrix &operator*=(const S21Matrix &other) &;

  // indexation by matrix elements
  float &operator()(int row, int col) &;  // index operator overload
  float &operator()(
      int row,
      int col) const &;  // this overloading patern allows to inspect
                         // matrix without changin its' values

  void MulMatrix(const S21Matrix &other);
  [[nodiscard]] S21Matrix Transpose() const;

  [[nodiscard]] const float *GetPointer() const;
  float *GetPointer();

  static S21Matrix CreateIdentity(int dimension);
  static S21Matrix Init4x4fv(float *matrix);

 private:
  [[nodiscard]] float CalcRowColMul(const S21Matrix &other, int i,
                                    int j) const noexcept;

 private:
  // private attributes
  int rows_, cols_;  // rows and columns attributes
  float *matrix_;    // pointer to the memory where the matrix will be allocated
};
}  // namespace s21
#endif  // INC_3DVIEWER_SRC_INCLUDE_S21MATRIX_H__
