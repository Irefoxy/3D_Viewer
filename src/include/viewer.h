//
// Created by ruslan on 23.03.23.
//

#ifndef C8_3DVIEWER_V1_0_1_SRC_SOURCES_QT_VIEWER_H_
#define C8_3DVIEWER_V1_0_1_SRC_SOURCES_QT_VIEWER_H_

#include <QAbstractButton>
#include <QMainWindow>
#include <cmath>

#include "gif.h"

/**
 * @file viewer.cc - contains this class' definitions
 */

namespace Ui {
class viewer;
}

namespace s21 {

/**
 * @class viewer
 * @brief base vieweer class
 */
class viewer : public QMainWindow {
  Q_OBJECT

 public:
  static constexpr char config_filename[] = "settings.conf";

  /**
   * @struct obj
   * @brief Same result class as in model
   */
  struct obj {
    std::vector<float> *vertexes = nullptr;
    std::vector<unsigned> *facetes = nullptr;
    float min = std::nanf("NAN");
    float max = std::nanf("NAN");
  };

  /**
   * Qt class ctor. Sets ui and connects
   * @param parent
   */
  explicit viewer(QWidget *parent = nullptr);

  /**
   * Corresponding dtor
   */
  ~viewer() override;

  /**
   * Publec func to set error
   * @param message - error message
   */
  void SetError(const std::string &message);

  /**
   * Public func to set result
   * @param input - result object
   */
  void SetResult(const obj &input);

  /**
   * Public func to set result in opengl class
   * @param result - result matrix to be set
   */
  void SetResultMatrix(float *result);
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
 private slots:
  void on_open_file_clicked();
  void on_chooser_buttonClicked(QAbstractButton *button);
  void on_parallel_toggled();
  void on_solid_toggled();
  void on_vertices_btn_buttonClicked();
  void on_size_num_valueChanged();
  void on_move_clicked();
  void on_rotate_clicked();
  void on_scale_clicked();
  void on_save_clicked();
  void on_screencast_clicked();
  /**
   * Slot called by timer
   */
  void screencast_timer_tick();
  void on_thickness_size_valueChanged();

 private:
  /**
   * Overriding close event to save config
   */
  void closeEvent(QCloseEvent *event) override;

  /**
   * Method for setting color by name
   * @param name
   * @param color
   */
  void SetColor(const QString &name, const QColor &color);

  /**
   * Slot to open the config file amd get config
   */
  void OpenConfigFile();

  /**
   * Method to open the file from ui
   * @param filename - file
   */
  void OpenFile(const QString &filename);

  /**
   * Method for setting ui values from config
   */
  void SetUiFromConfig();

 private:
  bool error = false;
  Ui::viewer *ui;
  GifWriter g;
  QTimer *screencast_timer;
  int counter = 0;
};
}  // namespace s21
#endif  // C8_3DVIEWER_V1_0_1_SRC_SOURCES_QT_VIEWER_H_
