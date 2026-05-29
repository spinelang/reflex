setup:
    CXX=clang++ CXX_LD=mold meson setup build

setup-release:
    CXX=clang++ CXX_LD=mold meson setup build --buildtype=release

build:
    meson compile -C build

test: build
    meson test -C build

run: build
    ./build/reader
