from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout
from conan.tools.build import check_min_cppstd


class Recipe(ConanFile):
    name = "JEngine-Reformed"
    version = "0.0.1"
    description = "The next generation of JEngine"
    homepage = "https://jesuskrists.github.io/JEngine-Reformed/"
    url = "https://github.com/JesusKrists/JEngine-Reformed"

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps", "VirtualRunEnv"

    default_options = {"tracy:shared": True, "tracy:callstack": True}

    def source(self):
        self.run("git clone https://github.com/JesusKrists/JEngine-Reformed.git")

    def layout(self):
        cmake_layout(self, src_folder="source")
        self.folders.generators = "conan"

    def requirements(self):
        self.requires("fmt/9.1.0")
        self.requires("nanosvg/cci.20210904")
        self.requires("tracy/cci.20220130")

    def build_requirements(self):
        self.test_requires("catch2/3.1.0")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def validate(self):
        if self.info.settings.compiler.get_safe("cppstd"):
            check_min_cppstd(self, 20)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
