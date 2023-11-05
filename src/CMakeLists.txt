cmake_minimum_required(VERSION 3.24)
project(3dViewer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
set(CMAKE_PREFIX_PATH "/home/ruslan/Qt/6.4.2/gcc_64/lib/cmake/")
find_package(Qt6 COMPONENTS Core Widgets OpenGLWidgets Gui REQUIRED)
find_package(GTest REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/sources/gui)

add_compile_options(-Werror -Wextra -Wall)

add_executable(3dViewer
        include/gif.h
        sources/viewer.cc include/viewer.h sources/gui/viewer.ui
        sources/main.cc
        sources/OpenGLWidget.cc include/OpenGLWidget.h
        sources/Model.cc include/Model.h
        include/controller.h sources/controller.cc
        include/qtshader.h sources/qtshader.cc
        sources/s21_matrix_oop.cc include/s21_matrix_oop.h
        sources/gif.cpp)
target_link_libraries(3dViewer Qt6::Core Qt6::Widgets Qt6::OpenGLWidgets Qt::Gui)

add_executable(model_test
        sources/Model.cc include/Model.h
        sources/tests/test.cc include/test.h
        include/s21_matrix_oop.h sources/s21_matrix_oop.cc)

target_compile_options(model_test PRIVATE --coverage)

target_link_libraries(model_test GTest::gtest_main gcov)