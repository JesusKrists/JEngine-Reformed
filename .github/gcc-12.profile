[settings]
arch=x86_64
build_type=Release
compiler=gcc
compiler.cppstd=20
compiler.version=12
compiler.libcxx=libstdc++11
os=Linux

[env]
CC=gcc-12
CXX=g++-12

[conf]
tools.system.package_manager:mode=install
tools.system.package_manager:sudo=True
