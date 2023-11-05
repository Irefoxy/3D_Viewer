//
// Created by ruslan on 23.03.23.
//

#include "OpenGLWidget.h"

namespace s21 {

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

OpenGLWidget::~OpenGLWidget() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &IBO);
  lines_shader.DeleteShader();
  FreeBuffers();
}

void OpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  lines_shader.InitShader("./shaders/line_vertex_shader",
                          "./shaders/line_fragment_shader");
  point_shader.InitShader("./shaders/point_vertex_shader",
                          "./shaders/point_fragment_shader");
  CreateBuffers();
  glEnable(GL_DEPTH_TEST);
  SetStrategy(std::make_unique<s21::VertexStrategy>());
  if (!conf.filename.isEmpty()) emit OpenFileSignal(conf.filename);
}

void OpenGLWidget::SetBuffers() {
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, int(sizeof(float) * vertexes->size()),
               vertexes->data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, int(facetes->size() * sizeof(unsigned)),
               facetes->data(), GL_STATIC_DRAW);
}

void OpenGLWidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void OpenGLWidget::paintGL() {
  glClearColor(conf.colors[0].redF(), conf.colors[0].greenF(),
               conf.colors[0].blueF(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (vertexes) {
    SetPerspectiveMatrix();
    mvp_ = projection_.Transpose() * view_.Transpose() * identity_;
    glBindVertexArray(VAO);

    SetStrategy(std::make_unique<s21::LinesStrategy>());
    current_render_strategy_->Render(lines_shader, mvp_, conf,
                                     int(facetes->size()));

    if (conf.vertices) {
      SetStrategy(std::make_unique<s21::VertexStrategy>());
      current_render_strategy_->Render(point_shader, mvp_, conf,
                                       int(vertexes->size() / 3));
    }
    glBindVertexArray(0);
  }
}
void OpenGLWidget::SetPerspectiveMatrix() {
  if (conf.parallel) {
    emit GetOrthoMatrix(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 100.0f);
  } else {
    emit GetPerspectiveMatrix((60.0f * M_PI) / 180, 600.0f / 800.0f, 1, 100.0f);
  }
  projection_ = std::move(*needed_matrix_);
  delete needed_matrix_;
  needed_matrix_ = nullptr;
}
void OpenGLWidget::SetObj(const vertex *vx, const facet *ft, const float &min,
                          const float &max) {
  FreeBuffers();
  facetes = ft;
  vertexes = vx;
  float norm_half = (max - min) / 2;
  auto norm_mid = float(float(min + norm_half) * 0.75 / norm_half);
  identity_ = s21::S21Matrix::CreateIdentity(4);
  ScaleObject(0.75f / norm_half);
  TranslateObject(std::vector<float>{-norm_mid, -norm_mid, -norm_mid});
  SetBuffers();
}

void OpenGLWidget::FreeBuffers() {
  delete vertexes;
  delete facetes;
}

void OpenGLWidget::mousePressEvent(QMouseEvent *mo) { mPos = mo->pos(); }

void OpenGLWidget::mouseMoveEvent(QMouseEvent *mo) {
  if (mo->buttons() & Qt::RightButton) {
    float x_angle = -float(mo->pos().y() - mPos.y());
    float y_angle = -float(mo->pos().x() - mPos.x());
    RotateObject(
        std::vector<float>{x_angle * 0.0001f, y_angle * 0.0001f, 0.0f});
    update();
  } else if (mo->buttons() & Qt::LeftButton) {
    float x_angle = float(mo->pos().x() - mPos.x());
    float y_angle = -float(mo->pos().y() - mPos.y());
    TranslateObject(
        std::vector<float>{x_angle * 0.0001f, y_angle * 0.0001f, 0});
    update();
  }
}

void OpenGLWidget::wheelEvent(QWheelEvent *event) {
  int angle = event->angleDelta().y() / 8;
  if (event->buttons() & Qt::RightButton) {
    RotateObject(std::vector<float>{0.0f, 0.0f, float(angle) * 0.01f});
  } else {
    float scale = angle < 0 ? 0.9 : 1.1;
    ScaleObject(scale);
  }
  update();
}

void OpenGLWidget::ScaleObject(const float &factor) {
  ScaleMatrix(identity_.GetPointer(), factor);
}

void OpenGLWidget::TranslateObject(const std::vector<float> &vec) {
  TranslateMatrix(identity_.GetPointer(), vec);
}

void OpenGLWidget::RotateObject(const std::vector<float> &vec) {
  RotateMatrix(identity_.GetPointer(), vec);
}

void OpenGLWidget::CreateBuffers() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &IBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
}
void OpenGLWidget::SetResultMatrix(float *result) {
  needed_matrix_ = new s21::S21Matrix(s21::S21Matrix::Init4x4fv(result));
}

QDataStream &operator>>(QDataStream &in, s21::config &conf) {
  QByteArray ba_parallel, ba_solid, ba_vertices, ba_vertices_size,
      ba_edges_thikness;
  in >> conf.filename >> conf.colors[0] >> conf.colors[1] >> conf.colors[2] >>
      ba_parallel >> ba_solid >> ba_vertices >> ba_vertices_size >>
      ba_edges_thikness;
  conf.parallel = ba_parallel.toInt();
  conf.solid = ba_solid.toInt();
  conf.vertices = ba_vertices.toInt();
  conf.vertices_size = ba_vertices_size.toInt();
  conf.edges_thickness = ba_edges_thikness.toInt();
  return in;
}
QDataStream &operator<<(QDataStream &out, const s21::config &conf) {
  out << conf.filename << conf.colors[0] << conf.colors[1] << conf.colors[2]
      << QByteArray::number(conf.parallel) << QByteArray::number(conf.solid)
      << QByteArray::number(conf.vertices)
      << QByteArray::number(conf.vertices_size)
      << QByteArray::number(conf.edges_thickness);
  return out;
}
void s21::LinesStrategy::Render(QtShader shader, const S21Matrix &mvp,
                                const config &conf, const int &size) {
  shader.Use();
  initializeOpenGLFunctions();

  glLineWidth(float(conf.edges_thickness));

  shader.SetUniVariableI("u_solid", conf.solid);

  shader.SetUniVariable("u_mvp", mvp.GetPointer());

  shader.SetUniVec4Fl("u_color",
                      std::vector<float>{(float)conf.colors[1].redF(),
                                         (float)conf.colors[1].greenF(),
                                         (float)conf.colors[1].blueF()});
  glDrawElements(GL_LINES, size, GL_UNSIGNED_INT, nullptr);
}
void s21::VertexStrategy::Render(QtShader shader, const S21Matrix &mvp,
                                 const config &conf, const int &size) {
  shader.Use();
  initializeOpenGLFunctions();

  glPointSize(float(conf.vertices_size));

  shader.SetUniVariableI("u_smooth", int(conf.vertices));

  shader.SetUniVariable("u_mvp", mvp.GetPointer());

  shader.SetUniVec4Fl("u_color",
                      std::vector<float>{(float)conf.colors[2].redF(),
                                         (float)conf.colors[2].greenF(),
                                         (float)conf.colors[2].blueF()});
  glDrawArrays(GL_POINTS, 0, size);
}
}  // namespace s21