# CPM Package Lock This file should be committed to version control

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/DisableStaticAnalysis.cmake)

cpmaddpackage("gh:gabime/spdlog#v1.11.0")
cpmaddpackage("gh:libsdl-org/SDL#release-2.28.0")
cpmaddpackage("gh:Dav1dde/glad#v2.0.4")
cpmaddpackage("gh:g-truc/glm#0.9.9.8")
cpmaddpackage("gh:JesusKrists/tracy#master")
cpmaddpackage("gh:catchorg/Catch2#v3.3.2")

add_subdirectory(${glad_SOURCE_DIR}/cmake)
glad_add_library(glad STATIC API gl:core=4.5)
add_library(glad::glad ALIAS glad)

disable_static_analysis(spdlog)
disable_static_analysis(SDL2)
disable_static_analysis(SDL2-static)
disable_static_analysis(SDL2main)
disable_static_analysis(glad)
disable_static_analysis(glm)
disable_static_analysis(TracyClient)
disable_static_analysis(Catch2)
disable_static_analysis(Catch2WithMain)
