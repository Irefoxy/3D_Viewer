//
// Created by ruslan on 08.07.23.
//

#ifndef INC_3DVIEWER_TEST_H
#define INC_3DVIEWER_TEST_H

#include <gtest/gtest.h>

#include "Model.h"

namespace {
class ModelTest : public ::testing::Test {
 protected:
  s21::Model model_;

  void SetUp() override {}

  void TearDown() override {}
};
}  // namespace

#endif  // INC_3DVIEWER_TEST_H
