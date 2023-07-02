cmake_minimum_required(VERSION 3.25)

include(cmake/prelude.cmake)

if(CLANG_ENV)
  set(CMAKE_CXX_FLAGS
      "-Wno-unknown-warning-option -Wno-unused-command-line-argument ${CMAKE_CXX_FLAGS}"
  )
endif()

project(
  JEngine-Reformed
  VERSION 0.0.1
  DESCRIPTION "The next generation of JEngine"
  HOMEPAGE_URL "https://github.com/JesusKrists/JEngine-Reformed"
  LANGUAGES CXX C)

include(cmake/custom-options.cmake)
include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)
include(cmake/SystemLink.cmake)

include(cmake/DownloadCPM.cmake)
cpmusepackagelock(${CMAKE_CURRENT_SOURCE_DIR}/package-lock.cmake)

# find_package(nanosvg REQUIRED)

# ---- Declare library ----

include(${CMAKE_CURRENT_SOURCE_DIR}/src/JEngine-Reformed_lib.cmake)

# ---- Declare executable ----

include(${CMAKE_CURRENT_SOURCE_DIR}/src/JEngine-Reformed_exe.cmake)

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
