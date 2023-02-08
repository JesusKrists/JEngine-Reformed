from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout
from conan.tools.build import check_min_cppstd
from conan.tools.microsoft import is_msvc
from conan.tools.files import copy

import os


class Recipe(ConanFile):
    name = "JEngine-Reformed"
    version = "latest"
    description = "The next generation of JEngine"
    homepage = "https://jesuskrists.github.io/JEngine-Reformed/"
    url = "https://github.com/JesusKrists/JEngine-Reformed"

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps", "VirtualRunEnv"

    keep_imports = True

    default_options = {
        "spdlog:shared": True,
        "fmt:shared": True,
        "tracy:shared": True,
        "tracy:callstack": True,
    }

    def requirements(self):
        self.requires("spdlog/1.11.0")
        self.requires("sdl/2.26.1")
        self.requires("tracy/0.9")
        self.requires("nanosvg/cci.20210904")

    def build_requirements(self):
        self.test_requires("catch2/3.1.0")

    def imports(self):
        self.copy("*.dll", "build/Debug", "bin")
        self.copy("*.dll", "build/test/Debug", "bin")
        self.copy("*.dll", "build/Release", "bin")
        self.copy("*.dll", "build/test/Release", "bin")

        self.copy("*.dll", "build/dev/", "bin")
        self.copy("*.dll", "build/dev/test", "bin")
        self.copy("*.so*", "build/dev/", "lib")
        self.copy("*.so*", "build/dev/test", "lib")

        self.copy("*.so*", "build/", "lib")
        self.copy("*.so*", "build/test", "lib")

    def config_options(self):
        if not is_msvc(self):
            self.options["sdl"].iconv = False
        if self.settings.os == "Linux":
            self.options["sdl"].pulse = False

    def source(self):
        self.run("git clone https://github.com/JesusKrists/JEngine-Reformed.git src")

    def layout(self):
        cmake_layout(self)
        self.folders.generators = "conan"

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def validate(self):
        if self.info.settings.compiler.get_safe("cppstd"):
            check_min_cppstd(self, 20)

    def build(self):
        cmake = CMake(self)
        cmake.configure(build_script_folder="src")
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
