//
// Created by ruslan on 23.03.23.
//

#include "../include/viewer.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QList>
#include <QMessageBox>

#include "../include/gif.h"
#include "ui_viewer.h"

namespace s21 {
constexpr char viewer::config_filename[];

viewer::viewer(QWidget *parent) : QMainWindow(parent), ui(new Ui::viewer) {
  ui->setupUi(this);
  OpenConfigFile();
  SetUiFromConfig();
  screencast_timer = new QTimer;
  connect(ui->open_gl, &OpenGLWidget::OpenFileSignal, this, &viewer::OpenFile);
  connect(ui->open_gl, &OpenGLWidget::RotateMatrix, this,
          &viewer::RotateMatrix);
  connect(ui->open_gl, &OpenGLWidget::TranslateMatrix, this,
          &viewer::TranslateMatrix);
  connect(ui->open_gl, &OpenGLWidget::ScaleMatrix, this, &viewer::ScaleMatrix);
  connect(ui->open_gl, &OpenGLWidget::GetOrthoMatrix, this,
          &viewer::GetOrthoMatrix);
  connect(ui->open_gl, &OpenGLWidget::GetPerspectiveMatrix, this,
          &viewer::GetPerspectiveMatrix);
}

viewer::~viewer() {
  delete ui;
  delete screencast_timer;
}

void viewer::on_open_file_clicked() {
  QString filename = QFileDialog::getOpenFileName(this, tr("Open .obj file"),
                                                  "", tr(".obj (*.obj)"));
  OpenFile(filename);
}

void viewer::on_chooser_buttonClicked(QAbstractButton *button) {
  QColor color = QColorDialog::getColor();
  QList<QLayout *> list = ui->gridLayout->findChildren<QLayout *>();
  for (auto &it : list) {
    if (it->layout()->itemAt(1)->widget() == button) {
      it->layout()->itemAt(0)->widget()->setStyleSheet(
          QString::fromUtf8("background-color: %1").arg(color.name()));
      SetColor(it->layout()->itemAt(0)->widget()->objectName(), color);
    }
  }
  ui->open_gl->update();
}
void viewer::on_parallel_toggled() {
  ui->open_gl->conf.parallel = ui->parallel->isChecked();
  ui->open_gl->update();
}

void viewer::on_solid_toggled() {
  ui->open_gl->conf.solid = ui->solid->isChecked();
  ui->open_gl->update();
}
void viewer::on_vertices_btn_buttonClicked() {
  auto &vertices = ui->open_gl->conf.vertices = 2;
  if (ui->none->isChecked()) {
    vertices = 0;
  } else if (ui->circle->isChecked()) {
    vertices = 1;
  }
  ui->open_gl->update();
}
void viewer::on_size_num_valueChanged() {
  ui->open_gl->conf.vertices_size = ui->size_num->value();
  ui->open_gl->update();
}

void viewer::on_thickness_size_valueChanged() {
  ui->open_gl->conf.edges_thickness = ui->thickness_size->value();
  ui->open_gl->update();
}

void viewer::closeEvent(QCloseEvent *event) {
  QFile file(QFile::decodeName(config_filename));
  if (!file.open(QIODevice::WriteOnly)) {
    QMessageBox::information(nullptr, "error", file.errorString());
    return;
  }
  QDataStream out(&file);
  out << ui->open_gl->conf;
  file.close();
  QMainWindow::closeEvent(event);
}
void viewer::OpenConfigFile() {
  QFile file(config_filename);
  if (file.exists()) {
    if (!file.open(QIODevice::ReadOnly)) {
      QMessageBox::information(nullptr, "error", file.errorString());
      return;
    }
    QDataStream in(&file);

    in >> ui->open_gl->conf;
    file.close();
  }
}
void viewer::SetUiFromConfig() {
  auto &conf = ui->open_gl->conf;
  ui->opened_file->setText(conf.filename);
  ui->back_color->setStyleSheet(
      QString::fromUtf8("background-color: %1").arg(conf.colors[0].name()));
  ui->edge_color->setStyleSheet(
      QString::fromUtf8("background-color: %1").arg(conf.colors[1].name()));
  ui->vertices_color->setStyleSheet(
      QString::fromUtf8("background-color: %1").arg(conf.colors[2].name()));
  if (conf.solid) {
    ui->solid->setChecked(true);
  } else {
    ui->dotted->setChecked(true);
  }
  if (conf.parallel) {
    ui->parallel->setChecked(true);
  } else {
    ui->central->setChecked(true);
  }
  ui->thickness_size->setValue((int)conf.edges_thickness);
  ui->size_num->setValue((int)conf.vertices_size);
  unsigned vertices_type = conf.vertices;
  if (!vertices_type) {
    ui->none->setChecked(true);
  } else if (vertices_type == 1) {
    ui->circle->setChecked(true);
  } else {
    ui->sqare->setChecked(true);
  }
}
void viewer::on_move_clicked() {
  if (ui->move_combo->currentIndex() == 0) {
    ui->open_gl->TranslateObject(
        std::vector<float>{float(ui->move_spin->value()), 0.0f, 0.0f});
  } else if (ui->move_combo->currentIndex() == 1) {
    ui->open_gl->TranslateObject(
        std::vector<float>{0.0f, float(ui->move_spin->value()), 0.0f});
  } else {
    ui->open_gl->TranslateObject(
        std::vector<float>{0.0f, 0.0f, float(ui->move_spin->value())});
  }

  ui->open_gl->update();
}
void viewer::on_rotate_clicked() {
  if (ui->rotate_combo->currentIndex() == 0) {
    ui->open_gl->RotateObject(
        std::vector<float>{float(ui->rotate_spin->value()), 0.0f, 0.0f});
  } else if (ui->rotate_combo->currentIndex() == 1) {
    ui->open_gl->RotateObject(
        std::vector<float>{0.0f, float(ui->rotate_spin->value()), 0.0f});
  } else {
    ui->open_gl->RotateObject(
        std::vector<float>{0.0f, 0.0f, float(ui->rotate_spin->value())});
  }
  ui->open_gl->update();
}
void viewer::on_scale_clicked() {
  ui->open_gl->ScaleObject(ui->scale_spin->value());
  ui->open_gl->update();
}

void viewer::on_save_clicked() {
  QString selected_filter;
  auto img_local = ui->open_gl->grabFramebuffer();
  auto filename = QFileDialog::getSaveFileName(
      this, tr("Save as..."), "model", tr("BMP (*.bmp);; JPEG (*.jpg *.jpeg)"),
      &selected_filter);
  if (selected_filter.contains("BMP")) {
    filename += ".bmp";
  } else {
    filename += ".jpeg";
  }
  img_local.save(filename);
}

void viewer::on_screencast_clicked() {
  auto filename = "ScreenCast.gif";
  screencast_timer->setInterval(20);
  connect(screencast_timer, &QTimer::timeout, this,
          QOverload<>::of(&viewer::screencast_timer_tick));
  GifBegin(&g, filename, ui->open_gl->width(), ui->open_gl->height(), 0);
  screencast_timer->start();
}

void viewer::screencast_timer_tick() {
  counter++;
  QImage frame = ui->open_gl->grabFramebuffer();
  GifWriteFrame(&g,
                frame.convertToFormat(QImage::Format_Indexed8)
                    .convertToFormat(QImage::Format_RGBA8888)
                    .constBits(),
                ui->open_gl->width(), ui->open_gl->height(), 0);
  if (counter > 200) {
    screencast_timer->stop();
    GifEnd(&g);
  }
}

void viewer::SetError(const std::string &message) {
  error = true;
  ui->opened_file->setText(QString::fromStdString(message));
}

void viewer::SetColor(const QString &name, const QColor &color) {
  int index = 2;
  if (name == "back_color") {
    index = 0;
  } else if (name == "edge_color") {
    index = 1;
  }
  ui->open_gl->conf.colors[index] = color;
}

void viewer::SetResult(const viewer::obj &input) {
  ui->edges_number->setText(QString::number(input.facetes->size() / 2));
  ui->vertices_number->setText(QString::number(input.vertexes->size() / 3));
  ui->open_gl->SetObj(input.vertexes, input.facetes, input.min, input.max);
}
void viewer::OpenFile(const QString &filename) {
  error = false;
  emit OpenFileSignal(filename);
  if (!error) {
    ui->opened_file->setText(filename);
    ui->open_gl->conf.filename = filename;
    ui->open_gl->update();
  }
}
void viewer::SetResultMatrix(float *result) {
  ui->open_gl->SetResultMatrix(result);
}
}  // namespace s21