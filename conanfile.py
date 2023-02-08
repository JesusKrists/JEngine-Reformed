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

    options = {
        "dev_build": [True, False],
        "coverage_build": [True, False],
        "sanitize_build": [True, False],
    }

    default_options = {
        "dev_build": False,
        "coverage_build": False,
        "sanitize_build": False,
        ##########################
        "spdlog:shared": True,
        "fmt:shared": True,
        "tracy:shared": True,
        "tracy:callstack": True,
    }

    def requirements(self):
        self.requires("spdlog/1.11.0")
        self.requires("sdl/2.26.1")
        self.requires("nanosvg/cci.20210904")
        self.requires("tracy/0.9")

    def build_requirements(self):
        self.test_requires("catch2/3.1.0")

    def imports(self):
        build_folder = "build/"
        test_folder = "build/test"
        if self.options.dev_build:
            build_folder = "build/dev"
            test_folder = "build/dev/test"
        if self.options.coverage_build:
            build_folder = "build/coverage"
            test_folder = "build/coverage/test"
        if self.options.sanitize_build:
            build_folder = "build/sanitize"
            test_folder = "build/sanitize/test"
        if (
            is_msvc(self)
            and not self.options.dev_build
            and not self.options.coverage_build
            and not self.options.sanitize_build
        ):
            build_folder = f"build/{str(self.settings.build_type)}"
            test_folder = f"build/test/{str(self.settings.build_type)}"
        if self.settings.os == "Windows":
            self.copy("*.dll", build_folder, "bin")
            self.copy("*.dll", test_folder, "bin")
        if self.settings.os == "Linux":
            self.copy("*.so*", build_folder, "lib")
            self.copy("*.so*", test_folder, "lib")

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
