from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout
from conan.tools.build import check_min_cppstd
from conan.tools.microsoft import is_msvc
from conan.tools.files import copy
from conan.errors import ConanInvalidConfiguration

import os


class Recipe(ConanFile):
    name = "jengine-reformed"
    version = "latest"
    description = "The next generation of JEngine"
    homepage = "https://jesuskrists.github.io/JEngine-Reformed/"
    url = "https://github.com/JesusKrists/JEngine-Reformed"

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps", "VirtualRunEnv"

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
        "spdlog*:shared": True,
        "fmt*:shared": True,
        "glad*:fPIC": False,
        "glad*:no_loader": True,
        "glad*:gl_profile": "core",
        "glad*:gl_version": "4.5",
        "tracy*:shared": True,
        "tracy*:callstack": True,
    }

    def requirements(self):
        self.requires("spdlog/1.11.0")
        self.requires("sdl/2.26.1")
        self.requires("glad/0.1.36")
        self.requires("nanosvg/cci.20210904")
        self.requires("tracy/0.9")

    def build_requirements(self):
        self.test_requires("catch2/3.3.1")

    def config_options(self):
        if not is_msvc(self) and not self.settings.os == "Macos":
            self.options["sdl"].iconv = False
        if self.settings.os == "Linux":
            self.options["sdl"].pulse = False
        if self.settings.os == "Macos":
            self.options["glad"].gl_version = "4.1"

    def source(self):
        self.run("git clone https://github.com/JesusKrists/JEngine-Reformed.git .")

    def layout(self):
        cmake_layout(self)
        self.folders.generators = "conan"

    def import_shared_libraries(self):
        build_folders = ["build/"]
        test_folders = ["build/test"]
        if self.options.dev_build:
            build_folders[0] = "build/dev"
            test_folders[0] = "build/dev/test"
        if self.options.coverage_build:
            build_folders[0] = "build/coverage"
            test_folders[0] = "build/coverage/test"
        if self.options.sanitize_build:
            build_folders[0] = "build/sanitize"
            test_folders[0] = "build/sanitize/test"
        if (
            is_msvc(self)
            and not self.options.dev_build
            and not self.options.coverage_build
            and not self.options.sanitize_build
        ):
            build_folders[0] = "build/Debug"
            test_folders[0] = "build/test/Debug"
            build_folders.append("build/Release")
            test_folders.append("build/test/Release")

        for dep in self.dependencies.values():
            dirs = []
            extension_pattern = ""

            if self.settings.os == "Windows":
                dirs = dep.cpp_info.bindirs
                extension_pattern = "*.dll"
            if self.settings.os == "Linux":
                dirs = dep.cpp_info.libdirs
                extension_pattern = "*.so*"
            if self.settings.os == "Macos":
                dirs = dep.cpp_info.libdirs
                extension_pattern = "*.dylib"

            for dir in dirs:
                for i, _ in enumerate(build_folders):
                    copy(
                        self,
                        extension_pattern,
                        dir,
                        os.path.join(self.source_path, build_folders[i]),
                    )
                    copy(
                        self,
                        extension_pattern,
                        dir,
                        os.path.join(self.source_path, test_folders[i]),
                    )

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

        self.import_shared_libraries()

    def validate(self):
        if self.settings.get_safe("compiler.cppstd"):
            check_min_cppstd(self, 20)

    def build(self):
        cmake = CMake(self)
        cmake.configure(build_script_folder="src")
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
