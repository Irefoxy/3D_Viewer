#include "test.h"

#include "Model.h"

namespace {
TEST_F(ModelTest, open_test_0) {
  s21::Obj result;
  s21::Command *command =
      new s21::OpenFileCommand("./sources/tests/correct_sample.txt", result);
  model_.ExecuteCommand(command);
  std::vector<float> expected_v{
      1, 1, 0, 1, 0, 1, 123.123, 456.321, -342.85, 87.32, -75.45, 12.32};
  std::vector<unsigned> expected_f{1, 2, 2, 3, 3, 1, 3, 4, 4, 1,
                                   1, 2, 2, 3, 1, 3, 3, 2, 2, 1};

  EXPECT_FALSE(result.vertexes->empty());
  EXPECT_FALSE(result.facetes->empty());
  EXPECT_NEAR(result.max, 456.321, 1e-3);
  EXPECT_NEAR(result.min, -342.85, 1e-2);
  for (int i = 0; i < 12; ++i) {
    EXPECT_NEAR((*result.vertexes)[i], expected_v[i], 1e-6);
  }
  for (int i = 0; i < 20; ++i) {
    EXPECT_EQ((*result.facetes)[i], expected_f[i] - 1);
  }
}

TEST_F(ModelTest, open_test_1) {
  s21::Obj result;
  s21::Command *command = new s21::OpenFileCommand("not_exists.obj", result);
  EXPECT_THROW(model_.ExecuteCommand(command), std::runtime_error);
}

TEST_F(ModelTest, open_test_2) {
  s21::Obj result;
  s21::Command *command =
      new s21::OpenFileCommand("./sources/tests/incorrect_sample.txt", result);
  EXPECT_THROW(model_.ExecuteCommand(command), std::runtime_error);
}

TEST_F(ModelTest, get_test_0) {
  float *result = nullptr;
  float fov = (60.0f * M_PI) / 180, aspect = 600.0f / 800.0f, near = 1.0f,
        far = 100.0f;
  float expected[16] = {1 / (aspect * tanf(fov / 2)),
                        0.0f,
                        0.0f,
                        0.0f,
                        0.0f,
                        1 / (tanf(fov / 2)),
                        0.0f,
                        0.0f,
                        0.0f,
                        0.0f,
                        far / (near - far),
                        -1.0f,
                        0.0f,
                        0.0f,
                        -(2 * far * near) / (far - near),
                        1.0f};
  s21::Command *command =
      new s21::GenPerspectiveCommand(fov, aspect, near, far, result);
  model_.ExecuteCommand(command);
  for (int i = 0; i < 16; ++i) {
    EXPECT_NEAR(result[i], expected[i], 1e-3);
  }
}

TEST_F(ModelTest, get_test_1) {
  float *result = nullptr;
  float left = -1.0f, right = 1.0f, bottom = -1.0f, top = 1.0f, near = -1.0f,
        far = 100.0f;
  float expected[16] = {2.0f / (right - left),
                        0.0f,
                        0.0f,
                        0.0f,
                        0.0f,
                        2.0f / (top - bottom),
                        0.0f,
                        0.0f,
                        0.0f,
                        0.0f,
                        -2.0f / (far - near),
                        0.0f,
                        -(right + left) / (right - left),
                        -(top + bottom) / (top - bottom),
                        -(far + near) / (far - near),
                        1.0f};
  s21::Command *command =
      new s21::GenOrthoCommand(left, right, bottom, top, near, far, result);
  model_.ExecuteCommand(command);
  for (int i = 0; i < 16; ++i) {
    EXPECT_NEAR(result[i], expected[i], 1e-3);
  }
}

TEST_F(ModelTest, transform_test_1_over_x) {
  float identity[16] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
  float rotate[16] = {1.0f,        0.0f, 0.0f, 0.0f,       0.0f,      0.997564f,
                      -0.0697565f, 0.0f, 0.0f, 0.0697565f, 0.997564f, 0.0f,
                      0.0f,        0.0f, 0.0f, 1.0f};

  s21::S21Matrix expected{s21::S21Matrix::CreateIdentity(4)};
  expected = s21::S21Matrix::Init4x4fv(rotate).Transpose() * expected;

  s21::Command *command = new s21::RotateCommand(
      identity, std::vector<float>{4.0f * M_PI / 180.0f, 0.0f, 0.0f});
  model_.ExecuteCommand(command);
  for (int i = 0; i < 16; ++i) {
    EXPECT_NEAR(identity[i], expected.GetPointer()[i], 1e-3);
  }
}

TEST_F(ModelTest, transform_test_1_over_y) {
  float identity[16] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
  float rotate[16] = {0.997564f, 0.0f, 0.0697565f,  0.0f, 0.0f,      1.0f,
                      0.0f,      0.0f, -0.0697565f, 0.0f, 0.997564f, 0.0f,
                      0.0f,      0.0f, 0.0f,        1.0f};

  s21::S21Matrix expected{s21::S21Matrix::CreateIdentity(4)};
  expected = s21::S21Matrix::Init4x4fv(rotate).Transpose() * expected;

  s21::Command *command = new s21::RotateCommand(
      identity, std::vector<float>{0.0f, 4.0f * M_PI / 180.0f, 0.0f});
  model_.ExecuteCommand(command);

  for (int i = 0; i < 16; ++i) {
    EXPECT_NEAR(identity[i], expected.GetPointer()[i], 1e-3);
  }
}

TEST_F(ModelTest, transform_test_1_over_z) {
  float identity[16] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
  float rotate[16] = {0.997564f, -0.0697565f, 0.0f, 0.0f, 0.0697565f, 0.997564f,
                      0.0f,      0.0f,        0.0f, 0.0f, 1.0f,       0.0f,
                      0.0f,      0.0f,        0.0f, 1.0f};

  s21::S21Matrix expected{s21::S21Matrix::CreateIdentity(4)};
  expected = s21::S21Matrix::Init4x4fv(rotate).Transpose() * expected;

  s21::Command *command = new s21::RotateCommand(
      identity, std::vector<float>{0.0f, 0.0f, 4.0f * M_PI / 180.0f});
  model_.ExecuteCommand(command);
  for (int i = 0; i < 16; ++i) {
    EXPECT_NEAR(identity[i], expected.GetPointer()[i], 1e-3);
  }
}

TEST_F(ModelTest, transform_test_2) {
  float identity[16] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0,  0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
  float move[16] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   1.0,    0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f, -5.21f, 1.034f, 0.0f, 1.0f};

  s21::S21Matrix expected{s21::S21Matrix::CreateIdentity(4)};
  expected = s21::S21Matrix::Init4x4fv(move).Transpose() * expected;

  s21::Command *command = new s21::TranslateCommand(
      identity, std::vector<float>{-5.21f, 1.034f, 0.0f});
  model_.ExecuteCommand(command);
  for (int i = 0; i < 16; ++i) {
    EXPECT_NEAR(identity[i], expected.GetPointer()[i], 1e-3);
  }
}

TEST_F(ModelTest, transform_test_3) {
  float identity[16] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0,  0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
  float scale[16] = {3.5f, 0.0f, 0.0f, 0.0f, 0.0f, 3.5f, 0.0f, 0.0f,
                     0.0f, 0.0f, 3.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

  s21::S21Matrix expected{s21::S21Matrix::CreateIdentity(4)};
  expected = s21::S21Matrix::Init4x4fv(scale) * expected;

  s21::Command *command = new s21::ScaleCommand(identity, 3.5);
  model_.ExecuteCommand(command);
  for (int i = 0; i < 16; ++i) {
    EXPECT_NEAR(identity[i], expected.GetPointer()[i], 1e-3);
  }
}
}  // namespace
