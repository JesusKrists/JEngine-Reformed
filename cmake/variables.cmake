# ---- Developer mode ----

# Developer mode enables targets and code paths in the CMake scripts that are
# only relevant for the developer(s) of JEngine-Reformed Targets necessary to
# build the project must be provided unconditionally, so consumers can trivially
# build and package the project
if(PROJECT_IS_TOP_LEVEL)
  option(JEngine-Reformed_DEVELOPER_MODE "Enable developer mode" OFF)
endif()

# ---- Warning guard ----

# target_include_directories with the SYSTEM modifier will request the compiler
# to omit warnings from the provided paths, if the compiler supports that This
# is to provide a user experience similar to find_package when add_subdirectory
# or FetchContent is used to consume this project
set(warning_guard "")
if(NOT PROJECT_IS_TOP_LEVEL)
  option(
    JEngine-Reformed_INCLUDES_WITH_SYSTEM
    "Use SYSTEM modifier for JEngine-Reformed's includes, disabling warnings"
    ON)
  mark_as_advanced(JEngine-Reformed_INCLUDES_WITH_SYSTEM)
  if(JEngine-Reformed_INCLUDES_WITH_SYSTEM)
    set(warning_guard SYSTEM)
  endif()
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS
      ${CMAKE_CXX_FLAGS}
      -Wmisleading-indentation # warn if indentation implies blocks where blocks
                               # do not exist
      -Wduplicated-cond # warn if if / else chain has duplicated conditions
      -Wduplicated-branches # warn if if / else branches have duplicated code
      -Wlogical-op # warn about logical operations being used where bitwise were
                   # probably wanted
      -Wuseless-cast # warn if you perform a cast to the same type
  )
endif()
