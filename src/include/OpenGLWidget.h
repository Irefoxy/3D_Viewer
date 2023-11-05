//
// Created by ruslan on 23.03.23.
//

#ifndef INC_3DVIEWER_SRC_INCLUDE_OPENGLWIDGET_H_
#define INC_3DVIEWER_SRC_INCLUDE_OPENGLWIDGET_H_

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include "qtshader.h"
#include "s21_matrix_oop.h"

/**
 * @file OpenGlWidget.cc - file with opengl part implementation's definitions
 */

namespace s21 {

/**
 * @struct config
 * @brief keeps config values
 */
struct config {
  QString filename = "";
  QColor colors[3] = {QColor("midnightblue"), QColor("red"), QColor("yellow")};
  bool parallel = true;
  bool solid = true;
  unsigned vertices = 0;
  unsigned vertices_size = 10;
  unsigned edges_thickness = 5;
};

/**
 * @class Strategy
 * @brief implements Strategy pattern
 */
class Strategy {
 public:
  Strategy() = default;
  virtual ~Strategy() = default;
  virtual void Render(QtShader shader, const S21Matrix &mvp, const config &conf,
                      const int &size) = 0;
};

/**
 * @class VertexStrategy
 * @brief Implements vertex strategy rendering
 */
class VertexStrategy : public Strategy, protected QOpenGLExtraFunctions {
 public:
  void Render(QtShader shader, const S21Matrix &mvp, const config &conf,
              const int &size) override;
};

/**
 * @class LinesStrategy
 * @brief Implements lines strategy rendering
 */
class LinesStrategy : public Strategy, protected QOpenGLExtraFunctions {
 public:
  void Render(QtShader shader, const S21Matrix &mvp, const config &conf,
              const int &size) override;
};

/**
 * @class OpenGLWidget
 * @brief Base opengl implementation qt class
 */
class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
  Q_OBJECT
 public:
  s21::config conf;

 signals:
  /**
   * Signal to open the file
   * @param filename - file
   */
  void OpenFileSignal(const QString &filename);

  /**
   * Signal to rotate model matrix
   * @param mx - matrix to rotate
   * @param - rotation vector
   */
  void RotateMatrix(float *, const std::vector<float> &);

  /**
   * Signal to translate model matrix
   * @param mx - matrix to translate
   * @param vec - translation vector
   */
  void TranslateMatrix(float *, const std::vector<float> &);

  /**
   * Signal to scale model matrix
   * @param mx - matrix to scale
   * @param factor - scale factor
   */
  void ScaleMatrix(float *, const float &);

  /**
   * Signal to get Ortho matrix based in provided values
   */
  void GetOrthoMatrix(const float &, const float &, const float &,
                      const float &, const float &, const float &);

  /**
   * Signal to get Perspective matrix based in provided values
   */
  void GetPerspectiveMatrix(const float &, const float &, const float &,
                            const float &);

 public:
  using vertex = std::vector<float>;
  using facet = std::vector<unsigned>;

  /**
   * opengl class ctor
   * @param parent Qwidget parent
   */
  explicit OpenGLWidget(QWidget *parent = nullptr);

  /**
   * Corresponding dtor
   */
  ~OpenGLWidget() override;

  /**
   * default qt opengl initialize func
   */
  void initializeGL() override;

  /**
   * default qt opengl resize func
   */
  void resizeGL(int w, int h) override;

  /**
   * default qt opengl paint func
   */
  void paintGL() override;

  /**
   * Method sets class private vars with input data from newly opened file
   * @param vx - pointer to vertex vector
   * @param ft - pointer to facets vector
   * @param min - min vertex value
   * @param max - max vertex value
   */
  void SetObj(const vertex *vx, const facet *ft, const float &min,
              const float &max);

  /**
   * Public method for rotation. Needed to cal from ui.
   * @param factor
   */
  void ScaleObject(const float &factor);

  /**
   * Public method for translation. Needed to cal from ui.
   * @param vec
   */
  void TranslateObject(const std::vector<float> &vec);

  /**
   * Public method for scale. Needed to cal from ui.
   * @param vec
   */
  void RotateObject(const std::vector<float> &vec);

  /**
   * Public method called from viewer to set result matrix provided by
   * controller
   * @param factor
   */
  void SetResultMatrix(float *result);

 private:
  /**
   * Setting Opengl buffers
   */
  void SetBuffers();

  /**
   * Frees private vars
   */
  void FreeBuffers();

  /*
   * Get position of pressed mouse button
   */
  void mousePressEvent(QMouseEvent *mo) override;

  /**
   * Method to rotate and move object with mouse
   * @param mo
   */
  void mouseMoveEvent(QMouseEvent *mo) override;

  /**
   * Creates opengl buffers
   */
  void CreateBuffers();

  /**
   * Method to rotate and scale object with mouse
   * @param mo
   */
  void wheelEvent(QWheelEvent *event) override;

  /**
   * Choosing and setting perspective matrix
   */
  void SetPerspectiveMatrix();

  /**
   * Sets strategy fro Strategy pattern
   * @param strategy
   */
  void SetStrategy(std::unique_ptr<s21::Strategy> &&strategy) {
    current_render_strategy_ = std::move(strategy);
  }

 private:
  const s21::S21Matrix view_ = {
      4, 4, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, -1.0f, 1}};
  const vertex *vertexes = nullptr;
  const facet *facetes = nullptr;
  s21::S21Matrix projection_, mvp_,
      identity_ = s21::S21Matrix::CreateIdentity(4), *needed_matrix_ = nullptr;
  GLuint VAO = 0, VBO = 0, IBO = 0;
  s21::QtShader lines_shader, point_shader;
  QPoint mPos;
  std::unique_ptr<s21::Strategy> current_render_strategy_;
};

/**
 * Overloads >> to save conf to file
 */
QDataStream &operator>>(QDataStream &in, s21::config &conf);

/**
 * Overloads << to save conf to file
 */
QDataStream &operator<<(QDataStream &out, const s21::config &conf);
}  // namespace s21
#endif  // INC_3DVIEWER_SRC_INCLUDE_OPENGLWIDGET_H_
