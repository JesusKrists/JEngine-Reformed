if(ENABLE_FORTIFY_SOURCE)
  add_compile_definitions(_FORTIFY_SOURCE=3)
endif()

if(ENABLE_SANITIZERS)
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer
                      -fno-common)
endif()

if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# We have extra warnings enabled, which clang does not support
if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  set(CMAKE_CXX_FLAGS "-Wno-unknown-warning-option ${CMAKE_CXX_FLAGS}")
endif()
