#ifndef QTSHADER_H
#define QTSHADER_H

#include <QtOpenGL>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

/**
 * @file qtshader.cc - contains shader class' definitions
 */

namespace s21 {

/**
 * @class QtShader
 * @brief This is a custom shader class created for the 3DViewer2.0 School21
 * project.
 */

class QtShader : public QOpenGLFunctions {
 public:
  QtShader() = default;

  /**
  @brief This function initializes shader by compiling shader program with given
  sources files.
  @param vshader_filepath - filepath for the vertex shader.
  @param fshader_filepath - filepath for the fragment shader.
  */
  void InitShader(const std::string &vshader_filepath,
                  const std::string &fshader_filepath);

  /**
  @brief This function setups a uniform variable inside the shader.
  @param name - variable name.
  @param value - its' value.
  */
  void SetUniVariable(const char *name, const float *value);

  /**
  @brief This function setups a uniform vec4 variable inside the shader.
  @details vec4 is a GLSL dapa type of 4 coordinates.
  @param name - variable name.
  @param vec - its' data.
  */
  void SetUniVec4Fl(const char *name, const std::vector<float> &vec);

  /**
  @brief This function returns shader program id.
  */
  unsigned int GetShaderId() { return id_; }

  /**
  @brief This function deletes shader program.
  */
  void DeleteShader() { glDeleteShader(id_); }

  /**
  @brief This function invokes glUseProgram to render the scene with this
  shader.
  */
  void Use() { glUseProgram(id_); }

  /**
  @brief This is an overloaded function, which setups a uniform mat4 variable
  inside the shader.
  @details mat4 is a GLSL dapa type of 4x4 matrix.
  @param name - variable name.
  @param value - pointer to the first element.
  */
  void SetUniVariable(const char *name, const float &value);

  /**
  @brief This function setups a uniform int variable inside the shader.
  @param name - variable name.
  @param value - its' value.
  */
  void SetUniVariableI(const char *name, const int &value);

 private:
  /**
  @brief This private function setups shader compilation process.
  @param shader_source - as it is.
  @param shader_type - vertex/fragment.
  */
  unsigned int CompileShader(const std::string &shader_source,
                             unsigned int shader_type);
  /**
  @brief This function loads shader source code from the file.
  @details using filestream.

  */
  std::string GetShader(const std::string &filepath);

  unsigned int id_;
};
}  // namespace s21
#endif  // QTSHADER_H
