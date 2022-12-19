cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
  JEngine3D
  VERSION 0.1.0
  DESCRIPTION "Short description"
  HOMEPAGE_URL "https://example.com/"
  LANGUAGES CXX)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(JEngine3D_lib OBJECT source/lib.cpp)

target_include_directories(
  JEngine3D_lib ${warning_guard}
  PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>")

target_compile_features(JEngine3D_lib PUBLIC cxx_std_20)

find_package(fmt REQUIRED)
find_package(nanosvg REQUIRED)
find_package(tracy REQUIRED)
target_link_libraries(JEngine3D_lib PRIVATE fmt::fmt nanosvg::nanosvg
                                            Tracy::TracyClient)

# ---- Declare executable ----

add_executable(JEngine3D_exe source/main.cpp)
add_executable(JEngine3D::exe ALIAS JEngine3D_exe)

set_property(TARGET JEngine3D_exe PROPERTY OUTPUT_NAME JEngine3D)

target_compile_features(JEngine3D_exe PRIVATE cxx_std_20)

target_link_libraries(JEngine3D_exe PRIVATE JEngine3D_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT JEngine3D_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
    AUTHOR_WARNING "Developer mode is intended for developers of JEngine3D")
endif()

include(cmake/dev-mode.cmake)
