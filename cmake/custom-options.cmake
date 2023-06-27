add_library(project_options INTERFACE)

if(ENABLE_SANITIZERS)
  target_compile_options(
    project_options INTERFACE -fsanitize=address,undefined
                              -fno-omit-frame-pointer -fno-common)

  target_link_options(project_options INTERFACE -fsanitize=address,undefined)
endif()

if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# We have extra warnings enabled, which clang does not support
if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang" AND NOT CLANG64_MSYS_ENV)
  target_compile_options(
    project_options INTERFACE -Wno-unknown-warning-option
                              -Wno-unused-command-line-argument)
endif()

target_compile_options(project_options INTERFACE "${JENGINE_CXX_FLAGS}")
