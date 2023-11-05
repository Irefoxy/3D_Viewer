//
// Created by ruslan on 23.05.23.
//

#include "../include/controller.h"

namespace s21 {
controller::controller(Model *model, viewer *view, QObject *parent)
    : QObject(parent), model_{model}, view_{view} {
  connect(view_, &viewer::OpenFileSignal, this, &controller::OpenFile);
  connect(view_, &viewer::RotateMatrix, this, &controller::Rotate);
  connect(view_, &viewer::TranslateMatrix, this, &controller::Translate);
  connect(view_, &viewer::ScaleMatrix, this, &controller::Scale);
  connect(view_, &viewer::GetOrthoMatrix, this, &controller::GetOrtho);
  connect(view_, &viewer::GetPerspectiveMatrix, this,
          &controller::GetPerspective);
}

void controller::OpenFile(const QString &filename) const {
  Obj result;
  Command *command;
  try {
    command = new OpenFileCommand(filename.toStdString(), result);
    model_->ExecuteCommand(command);
    viewer::obj input;
    input.vertexes = result.vertexes;
    input.facetes = result.facetes;
    input.min = result.min;
    input.max = result.max;
    view_->SetResult(input);
  } catch (std::exception &e) {
    view_->SetError(e.what());
  }
}
void controller::Rotate(float *mx, const std::vector<float> &vec) const {
  Command *command;
  command = new RotateCommand(mx, vec);
  model_->ExecuteCommand(command);
}
void controller::Translate(float *mx, const std::vector<float> &vec) const {
  Command *command;
  command = new TranslateCommand(mx, vec);
  model_->ExecuteCommand(command);
}
void controller::Scale(float *mx, const float &factor) const {
  Command *command;
  command = new ScaleCommand(mx, factor);
  model_->ExecuteCommand(command);
}
void controller::GetOrtho(const float &left, const float &right,
                          const float &bottom, const float &top,
                          const float &near, const float &far) const {
  Command *command;
  float *result = nullptr;
  command = new GenOrthoCommand(left, right, bottom, top, near, far, result);
  model_->ExecuteCommand(command);
  view_->SetResultMatrix(result);
}
void controller::GetPerspective(const float &fov, const float &aspect,
                                const float &near, const float &far) const {
  Command *command;
  float *result = nullptr;
  command = new GenPerspectiveCommand(fov, aspect, near, far, result);
  model_->ExecuteCommand(command);
  view_->SetResultMatrix(result);
}

}  // namespace s21
