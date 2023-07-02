project(JEngine-Reformed_Tests LANGUAGES CXX)

# ---- Dependencies ----
include(${CMAKE_SOURCE_DIR}/cmake/Catch.cmake)

# ---- Tests ----

add_executable(JEngine-Reformed_test src/JEngine-Reformed_test.cpp)
target_link_system_libraries(JEngine-Reformed_test PRIVATE Catch2::Catch2
                             Catch2::Catch2WithMain)
target_link_libraries(JEngine-Reformed_test PRIVATE JEngine-Reformed::lib)
target_compile_features(JEngine-Reformed_test PRIVATE cxx_std_20)

# ---- Constexpr tests ----

add_executable(JEngine-Reformed_constexpr_test
               src/JEngine-Reformed_constexpr_test.cpp)
target_link_system_libraries(JEngine-Reformed_constexpr_test PRIVATE
                             Catch2::Catch2 Catch2::Catch2WithMain)
target_link_libraries(JEngine-Reformed_constexpr_test
                      PRIVATE JEngine-Reformed::lib)
target_compile_features(JEngine-Reformed_constexpr_test PRIVATE cxx_std_20)

# ---- End-of-file commands ----

catch_discover_tests(JEngine-Reformed_test)
catch_discover_tests(JEngine-Reformed_constexpr_test)

# ------------------------------

add_folders(Test)
