//
// Created by ruslan on 23.05.23.
//

#ifndef SMARTCALC2_CONTROLLER_H
#define SMARTCALC2_CONTROLLER_H

#include <QObject>

#include "Model.h"
#include "viewer.h"

/**
 * @file controller.cc controller class definitions
 */

/**
 * @namespace school 21 namespace
 */
namespace s21 {

/**
 * @class controller
 * @brief implements the MVC pattern
 */
class controller final : public QObject {
  Q_OBJECT
 public:
  /**
   * Constructor. Get pointers to model and view(MVC). Connects their signals to
   * self slots.
   * @param model - pointer to model
   * @param view - pointer to ciew
   * @param parent - parent fot QObject
   */
  controller(Model *model, viewer *view, QObject *parent = nullptr);

  ~controller() override = default;

 private slots:
  /**
   * Slot for file opening
   * @param filename - file to open
   */
  void OpenFile(const QString &filename) const;

  /**
   * Slot to rotate model matrix
   * @param mx - matrix to rotate
   * @param - rotation vector
   */
  void Rotate(float *mx, const std::vector<float> &vec) const;

  /**
   * Slot to translate model matrix
   * @param mx - matrix to translate
   * @param vec - translation vector
   */
  void Translate(float *mx, const std::vector<float> &vec) const;

  /**
   * Slot to scale model matrix
   * @param mx - matrix to scale
   * @param factor - scale factor
   */
  void Scale(float *, const float &) const;

  /**
   * Slot to get Ortho matrix based in provided values
   */
  void GetOrtho(const float &, const float &, const float &, const float &,
                const float &, const float &) const;

  /**
   * Slot to get Perspective matrix based in provided values
   */
  void GetPerspective(const float &, const float &, const float &,
                      const float &) const;

 private:
  Model *model_;

  viewer *view_;
};

}  // namespace s21

#endif  // SMARTCALC2_CONTROLLER_H
