cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
  JEngine-Reformed
  VERSION 0.1.0
  DESCRIPTION "The next generation of JEngine"
  HOMEPAGE_URL "https://github.com/JesusKrists/JEngine-Reformed"
  LANGUAGES CXX)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(JEngine-Reformed_lib OBJECT source/lib.cpp)

target_include_directories(
  JEngine-Reformed_lib ${warning_guard}
  PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>")

target_compile_features(JEngine-Reformed_lib PUBLIC cxx_std_20)

find_package(fmt REQUIRED)
find_package(nanosvg REQUIRED)
find_package(tracy REQUIRED)
target_link_libraries(JEngine-Reformed_lib PRIVATE fmt::fmt nanosvg::nanosvg
                                            Tracy::TracyClient)

# ---- Declare executable ----

add_executable(JEngine-Reformed_exe source/main.cpp)
add_executable(JEngine-Reformed::exe ALIAS JEngine-Reformed_exe)

set_property(TARGET JEngine-Reformed_exe PROPERTY OUTPUT_NAME JEngine-Reformed)

target_compile_features(JEngine-Reformed_exe PRIVATE cxx_std_20)

target_link_libraries(JEngine-Reformed_exe PRIVATE JEngine-Reformed_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT JEngine-Reformed_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
    AUTHOR_WARNING "Developer mode is intended for developers of JEngine-Reformed")
endif()

include(cmake/dev-mode.cmake)
