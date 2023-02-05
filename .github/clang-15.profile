[settings]
arch=x86_64
build_type=Release
compiler=clang
compiler.cppstd=20
compiler.version=15
compiler.libcxx=libstdc++11
os=Linux

[env]
CC=clang-15
CXX=clang++-15

[conf]
tools.system.package_manager:mode=install
tools.system.package_manager:sudo=True
