#include "../include/qtshader.h"

namespace s21 {

void QtShader::InitShader(const std::string &vshader_filepath,
                          const std::string &fshader_filepath) {
  initializeOpenGLFunctions();

  unsigned int prog = glCreateProgram();

  unsigned int vs =
      CompileShader(GetShader(vshader_filepath), GL_VERTEX_SHADER);

  unsigned int fs =
      CompileShader(GetShader(fshader_filepath), GL_FRAGMENT_SHADER);

  glAttachShader(prog, vs);
  glAttachShader(prog, fs);
  glLinkProgram(prog);
  glValidateProgram(prog);

  int success;
  char log[512];
  glGetProgramiv(prog, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(prog, 512, nullptr, log);
    std::cout << "Shader linking failed\n" << log << std::endl;
  };

  glDeleteShader(vs);
  glDeleteShader(fs);
  id_ = prog;
}

void QtShader::SetUniVariable(const char *name, const float *value) {
  int location = glGetUniformLocation(id_, name);
  glUniformMatrix4fv(location, 1, GL_TRUE, value);
}

void QtShader::SetUniVariable(const char *name, const float &value) {
  int location = glGetUniformLocation(id_, name);
  glUniform1f(location, value);
}

unsigned int QtShader::CompileShader(const std::string &shader_source,
                                     unsigned int shader_type) {
  unsigned int id = glCreateShader(shader_type);
  const char *src = shader_source.c_str();
  glShaderSource(id, 1, &src, NULL);
  glCompileShader(id);
  int success;
  char log[512];
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(id, 512, NULL, log);
    std::cout << "Shader compilation failed\n" << log << std::endl;
  };
  return id;
}

std::string QtShader::GetShader(const std::string &filepath) {
  std::ifstream shader_file;
  shader_file.open(filepath);
  std::stringstream stream;
  stream << shader_file.rdbuf();
  shader_file.close();
  return stream.str();
}

void QtShader::SetUniVec4Fl(const char *name, const std::vector<float> &vec) {
  int location = glGetUniformLocation(id_, name);
  glUniform4f(location, vec[0], vec[1], vec[2], 1.0f);
}
void QtShader::SetUniVariableI(const char *name, const int &value) {
  int location = glGetUniformLocation(id_, name);
  glUniform1i(location, value);
}
}  // namespace s21