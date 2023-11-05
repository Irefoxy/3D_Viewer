//
// Created by ruslan on 02.06.23.
//

#ifndef INC_3DVIEWER_MODEL_H
#define INC_3DVIEWER_MODEL_H

#include <cmath>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "s21_matrix_oop.h"

/**
 * @file Model.cc - Model - part method's definitions
 */

namespace s21 {

using vertex = std::vector<float>;
using facet = std::vector<unsigned>;

/**
 * @struct Command
 * @brief Base class for Command pattern
 */
struct Command {
  virtual ~Command() = default;

  /**
   * Virtual method
   */
  virtual void execute() = 0;
};

/**
 * @struct Obj
 * @brief result struct
 */
struct Obj {
  vertex *vertexes = nullptr;
  facet *facetes = nullptr;
  float min = std::nanf("NAN");
  float max = std::nanf("NAN");
};

/**
 * @class OpenFileCommand
 * @brief Command pattern's class for open file command
 */
class OpenFileCommand : public Command {
 public:
  /**
   * Ctor for initializing private vars
   */
  OpenFileCommand(std::string filename, Obj &result)
      : filename_(std::move(filename)), result_(result) {}

  void execute() override;

 private:
  void Open();

  void ReadObj();

  float &FindMaxMin(float &num) &noexcept;

  unsigned int CorrectIndex(const int &num) const noexcept;

 private:
  std::string filename_;
  Obj &result_;
  std::ifstream in_file_;
};

/**
 * @class RotateCommand
 * @brief Command pattern's class for rotate command
 */
class RotateCommand : public Command {
 public:
  void execute() override;
  /**
   * Ctor for initializing private vars
   */
  RotateCommand(float *matrix, const std::vector<float> &vec)
      : matrix_(matrix), angle_(vec) {}

 private:
  float *matrix_;
  const std::vector<float> angle_;
};

/**
 * @class ScaleCommand
 * @brief Command pattern's class for scale command
 */
class ScaleCommand : public Command {
 public:
  void execute() override;
  /**
   * Ctor for initializing private vars
   */
  ScaleCommand(float *matrix, const float &factor)
      : matrix_(matrix), factor_(factor) {}

 private:
  float *matrix_;
  const float factor_;
};

/**
 * @class TranslateCommand
 * @brief Command pattern's class for translate command
 */
class TranslateCommand : public Command {
 public:
  void execute() override;
  /**
   * Ctor for initializing private vars
   */
  TranslateCommand(float *matrix, const std::vector<float> &vec)
      : matrix_(matrix), vec_(vec) {}

 private:
  float *matrix_;
  const std::vector<float> vec_;
};

/**
 * @class GenOrthoCommand
 * @brief Command pattern's class for creating an orthogonal projection
 */
class GenOrthoCommand : public Command {
 public:
  void execute() override;
  /**
   * Ctor for initializing private vars
   */
  GenOrthoCommand(const float &left, const float &right, const float &bottom,
                  const float &top, const float &near, const float &far,
                  float *&result)
      : result_(result),
        left_(left),
        right_(right),
        bottom_(bottom),
        top_(top),
        near_(near),
        far_(far) {}

 private:
  float *&result_;
  const float &left_, &right_, &bottom_, &top_, &near_, &far_;
};

/**
 * @class GenPerspectiveCommand
 * @brief Command pattern's class for creating an perspective projection
 */
class GenPerspectiveCommand : public Command {
 public:
  void execute() override;
  /**
   * Ctor for initializing private vars
   */
  GenPerspectiveCommand(const float &fov, const float &aspect,
                        const float &near, const float &far, float *&result)
      : result_(result), fov_(fov), aspect_(aspect), near_(near), far_(far) {}

 private:
  float *&result_;
  const float &fov_, &aspect_, &near_, &far_;
};

/**
 * @class Model
 * @brief Wrapper for Command pattern
 */
class Model {
 public:
  /**
   * Execution method
   * @param command - command to execute
   */
  static void ExecuteCommand(Command *command);
};

}  // namespace s21

#endif  // INC_3DVIEWER_MODEL_H
