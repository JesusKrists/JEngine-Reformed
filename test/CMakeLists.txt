# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(JEngine-ReformedTests LANGUAGES CXX)

# ---- Dependencies ----

find_package(Catch2 REQUIRED)
include(Catch)

# ---- Tests ----

add_executable(JEngine-Reformed_test source/JEngine-Reformed_test.cpp)
target_link_libraries(JEngine-Reformed_test PRIVATE Catch2::Catch2WithMain
                                                    JEngine-Reformed::lib)
target_compile_features(JEngine-Reformed_test PRIVATE cxx_std_20)

# ---- Constexpr tests ----

add_executable(JEngine-Reformed_constexpr_test
               source/JEngine-Reformed_constexpr_test.cpp)
target_link_libraries(JEngine-Reformed_constexpr_test
                      PRIVATE Catch2::Catch2WithMain JEngine-Reformed::lib)
target_compile_features(JEngine-Reformed_constexpr_test PRIVATE cxx_std_20)

# ---- End-of-file commands ----

catch_discover_tests(JEngine-Reformed_test)
catch_discover_tests(JEngine-Reformed_constexpr_test)

# ------------------------------

add_folders(Test)
