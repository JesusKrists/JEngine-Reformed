# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(JEngine3DTests LANGUAGES CXX)

# ---- Dependencies ----

find_package(Catch2 REQUIRED)
include(Catch)

# ---- Tests ----

add_executable(JEngine3D_test source/JEngine3D_test.cpp)
target_link_libraries(
    JEngine3D_test PRIVATE
    JEngine3D_lib
    Catch2::Catch2WithMain
)
target_compile_features(JEngine3D_test PRIVATE cxx_std_20)

catch_discover_tests(JEngine3D_test)

# ---- End-of-file commands ----

add_folders(Test)
