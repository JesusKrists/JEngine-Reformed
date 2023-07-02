add_executable(JEngine-Reformed_exe source/main.cpp)
add_executable(JEngine-Reformed::exe ALIAS JEngine-Reformed_exe)

set_property(TARGET JEngine-Reformed_exe PROPERTY OUTPUT_NAME JEngine-Reformed)

target_compile_features(JEngine-Reformed_exe PRIVATE cxx_std_20)

target_link_libraries(JEngine-Reformed_exe PRIVATE JEngine-Reformed::lib)