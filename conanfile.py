from conan import ConanFile


class Recipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps", "VirtualRunEnv"

    default_options = {"tracy:shared": True, "tracy:callstack": True}

    def layout(self):
        self.folders.generators = "conan"

    def requirements(self):
        self.requires("fmt/9.1.0")
        self.requires("nanosvg/cci.20210904")
        self.requires("tracy/cci.20220130")

    def build_requirements(self):
        self.test_requires("catch2/3.1.0")
