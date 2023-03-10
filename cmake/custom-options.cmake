if(ENABLE_SANITIZERS)
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer
                      -fno-common)

  add_link_options(-fsanitize=address,undefined)
endif()

if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# We have extra warnings enabled, which clang does not support
if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang" AND NOT CLANG64_MSYS_ENV)
  set(CMAKE_CXX_FLAGS
      "-Wno-unknown-warning-option -Wno-unused-command-line-argument ${CMAKE_CXX_FLAGS}"
  )
endif()
