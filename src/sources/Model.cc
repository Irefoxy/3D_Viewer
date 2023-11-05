//
// Created by ruslan on 02.06.23.
//

#include "Model.h"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace s21 {

void OpenFileCommand::Open() {
  in_file_.open(filename_);
  if (!in_file_.is_open()) {
    throw std::runtime_error("Failed to open the file.");
  }
}

void OpenFileCommand::ReadObj() {
  std::string line;
  result_.vertexes = new vertex;
  result_.facetes = new facet;

  while (std::getline(in_file_, line)) {
    auto begin = line.substr(0, 2);
    if (begin == "v ") {
      std::istringstream stream(line.substr(2));
      float x, y, z;

      if (stream >> x >> y >> z) {
        result_.vertexes->push_back(FindMaxMin(x));
        result_.vertexes->push_back(FindMaxMin(y));
        result_.vertexes->push_back(FindMaxMin(z));
      }
    }
  }
  in_file_.clear();
  in_file_.seekg(0, std::ios::beg);
  while (std::getline(in_file_, line)) {
    auto begin = line.substr(0, 2);
    if (begin == "f ") {
      std::istringstream stream(line.substr(2));
      std::string f1, f2, f3;

      if (stream >> f1 >> f2 >> f3) {
        unsigned num1, num2, num3;
        try {
          num1 = CorrectIndex(stoi(f1)), num2 = CorrectIndex(stoi(f2)),
          num3 = CorrectIndex(stoi(f3));
          result_.facetes->push_back(num1 - 1);
          result_.facetes->push_back(num2 - 1);
          result_.facetes->push_back(num2 - 1);
          result_.facetes->push_back(num3 - 1);
          result_.facetes->push_back(num3 - 1);
          while (stream >> f2) {
            num2 = stoi(f2);
            result_.facetes->push_back(num2 - 1);
            result_.facetes->push_back(num2 - 1);
          }
          result_.facetes->push_back(num1 - 1);
        } catch (std::invalid_argument &) {
        }
      }
    }
  }
  in_file_.close();
}

float &OpenFileCommand::FindMaxMin(float &num) &noexcept {
  if (std::isnan(result_.min)) {
    result_.min = num;
    result_.max = num;
  } else if (num > result_.max) {
    result_.max = num;
  } else if (num < result_.min) {
    result_.min = num;
  }
  return num;
}
unsigned int OpenFileCommand::CorrectIndex(const int &num) const noexcept {
  static const unsigned size = result_.vertexes->size() / 3;
  return num < 0 ? num + size : num;
}

void OpenFileCommand::execute() {
  Open();
  ReadObj();
  if (result_.vertexes->empty())
    throw std::runtime_error("Wrong data in the file.");
}
void RotateCommand::execute() {
  S21Matrix mx{S21Matrix::Init4x4fv(matrix_)};

  S21Matrix rotation_matrix(4, 4);
  rotation_matrix(0, 0) = cosf(angle_[1]) * cosf(angle_[2]);
  rotation_matrix(0, 1) = -sinf(angle_[2]) * cosf(angle_[1]);
  rotation_matrix(0, 2) = sinf(angle_[1]);
  rotation_matrix(1, 0) = sinf(angle_[0]) * sinf(angle_[1]) * cosf(angle_[2]) +
                          sinf(angle_[2]) * cosf(angle_[0]);
  rotation_matrix(1, 1) = -sinf(angle_[0]) * sinf(angle_[1]) * sinf(angle_[2]) +
                          cosf(angle_[2]) * cosf(angle_[0]);
  rotation_matrix(1, 2) = -sinf(angle_[0]) * cosf(angle_[1]);
  rotation_matrix(2, 0) = sinf(angle_[0]) * sinf(angle_[2]) -
                          sinf(angle_[1]) * cosf(angle_[0]) * cosf(angle_[2]);
  rotation_matrix(2, 1) = sinf(angle_[0]) * cosf(angle_[2]) +
                          sinf(angle_[1]) * cosf(angle_[0]) * sinf(angle_[2]);
  rotation_matrix(2, 2) = cosf(angle_[0]) * cosf(angle_[1]);
  rotation_matrix(3, 3) = 1.0f;
  mx = rotation_matrix.Transpose() * mx;
  std::copy_n(mx.GetPointer(), 16, matrix_);
}
void ScaleCommand::execute() {
  S21Matrix mx{S21Matrix::Init4x4fv(matrix_)};
  S21Matrix scaling_matrix(4, 4);
  scaling_matrix(0, 0) = factor_;
  scaling_matrix(1, 1) = factor_;
  scaling_matrix(2, 2) = factor_;
  scaling_matrix(3, 3) = 1.0;
  mx *= scaling_matrix;
  std::copy_n(mx.GetPointer(), 16, matrix_);
}
void TranslateCommand::execute() {
  S21Matrix mx{S21Matrix::Init4x4fv(matrix_)};
  S21Matrix translating_matrix = S21Matrix::CreateIdentity(4);
  translating_matrix(3, 0) = vec_[0];
  translating_matrix(3, 1) = vec_[1];
  translating_matrix(3, 2) = vec_[2];
  translating_matrix(3, 3) = 1.0f;
  mx = translating_matrix.Transpose() * mx;
  std::copy_n(mx.GetPointer(), 16, matrix_);
}
void GenOrthoCommand::execute() {
  auto *ortho_matrix = new S21Matrix(S21Matrix::CreateIdentity(4));
  (*ortho_matrix)(0, 0) = 2.0f / (right_ - left_);
  (*ortho_matrix)(1, 1) = 2.0f / (top_ - bottom_);
  (*ortho_matrix)(2, 2) = -2.0f / (far_ - near_);
  (*ortho_matrix)(3, 0) = -(right_ + left_) / (right_ - left_);
  (*ortho_matrix)(3, 1) = -(top_ + bottom_) / (top_ - bottom_);
  (*ortho_matrix)(3, 2) = -(far_ + near_) / (far_ - near_);
  result_ = new float[16];
  std::copy_n(ortho_matrix->GetPointer(), 16, result_);
}
void Model::ExecuteCommand(Command *command) {
  command->execute();
  delete command;
}
void GenPerspectiveCommand::execute() {
  auto *perspective_matrix = new S21Matrix(S21Matrix::CreateIdentity(4));
  (*perspective_matrix)(0, 0) = 1 / (aspect_ * tanf(fov_ / 2));
  (*perspective_matrix)(1, 1) = 1 / (tanf(fov_ / 2));
  (*perspective_matrix)(2, 2) = far_ / (near_ - far_);
  (*perspective_matrix)(2, 3) = -1.0f;
  (*perspective_matrix)(3, 2) = -(2 * far_ * near_) / (far_ - near_);
  result_ = new float[16];
  std::copy_n(perspective_matrix->GetPointer(), 16, result_);
}
}  // namespace s21
