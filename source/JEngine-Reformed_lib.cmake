macro(default name)
  if(NOT DEFINED "${name}")
    set("${name}" "${ARGN}")
  endif()
endmacro()

add_library(JEngine-Reformed_lib OBJECT source/lib.cpp)
add_library(JEngine-Reformed::lib ALIAS JEngine-Reformed_lib)

target_include_directories(
  JEngine-Reformed_lib ${warning_guard}
  PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>")

target_compile_features(JEngine-Reformed_lib PUBLIC cxx_std_20)

target_link_libraries(JEngine-Reformed_lib
                      PUBLIC spdlog::spdlog Tracy::TracyClient nanosvg::nanosvg)

default(JE_PLATFORM_WINDOWS_VALUE 0)
default(JE_PLATFORM_UNIX_VALUE 0)
default(JE_PLATFORM_APPLE_VALUE 0)

if(WIN32)
  set(JE_PLATFORM_WINDOWS_VALUE 1)
elseif(UNIX)
  set(JE_PLATFORM_UNIX_VALUE 1)
  if(APPLE)
    set(JE_PLATFORM_APPLE_VALUE 1)
  endif()
endif()

if(MSVC)
  target_compile_definitions(JEngine-Reformed_lib
                             PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()

target_compile_definitions(
  JEngine-Reformed_lib
  PUBLIC JE_PLATFORM_WINDOWS_VALUE=${JE_PLATFORM_WINDOWS_VALUE})

target_compile_definitions(
  JEngine-Reformed_lib PUBLIC JE_PLATFORM_UNIX_VALUE=${JE_PLATFORM_UNIX_VALUE})

target_compile_definitions(
  JEngine-Reformed_lib
  PUBLIC JE_PLATFORM_APPLE_VALUE=${JE_PLATFORM_APPLE_VALUE})

target_compile_definitions(
  JEngine-Reformed_lib
  PUBLIC
    "JE_ASSERTS_ENABLED_VALUE=$<NOT:$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>>"
)

target_compile_definitions(
  JEngine-Reformed_lib
  PUBLIC
    "JE_RELEASE_BUILD_VALUE=$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>")
