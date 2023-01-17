# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(JEngine-ReformedTests LANGUAGES CXX)

# ---- Dependencies ----

find_package(Catch2 REQUIRED)
include(Catch)

# ---- Tests ----

add_executable(JEngine-Reformed_test source/JEngine-Reformed_test.cpp)
target_link_libraries(JEngine-Reformed_test PRIVATE JEngine-Reformed::lib
                                                    Catch2::Catch2WithMain)
target_compile_features(JEngine-Reformed_test PRIVATE cxx_std_20)

catch_discover_tests(JEngine-Reformed_test)

# ---- End-of-file commands ----

add_folders(Test)
