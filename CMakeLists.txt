cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
  JEngine-Reformed
  VERSION 0.0.1
  DESCRIPTION "The next generation of JEngine"
  HOMEPAGE_URL "https://github.com/JesusKrists/JEngine-Reformed"
  LANGUAGES CXX)

# We have extra warnings enabled, which clang does not support
if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  set(CMAKE_CXX_FLAGS "-Wno-unknown-warning-option ${CMAKE_CXX_FLAGS}")
endif()

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

find_package(spdlog REQUIRED)
find_package(SDL2 REQUIRED)
find_package(nanosvg REQUIRED)
find_package(Tracy REQUIRED)

# ---- Declare library ----

include(${CMAKE_CURRENT_SOURCE_DIR}/source/JEngine-Reformed_lib.cmake)

# ---- Declare executable ----

include(${CMAKE_CURRENT_SOURCE_DIR}/source/JEngine-Reformed_exe.cmake)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT JEngine-Reformed_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
    AUTHOR_WARNING
      "Developer mode is intended for developers of JEngine-Reformed")
endif()

include(cmake/dev-mode.cmake)
