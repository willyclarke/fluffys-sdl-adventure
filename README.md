# fluffys-sdl-adventure

## Description

Various small apps to develop a small C++ library for 3d math.

## License

MIT, see the LICENCE file.

## Build instructions

The project can be built with CMake.

To create the build files:

$ cmake -B build

To build the project:

$ cmake --build build --target package

## Dependencies

* A C++ compiler
* CMake
* Git
* CTest (pulled in by use of CMake's Fetchcontent)
* SDL (pulled in by use of CMake's Fetchcontent)
* SDL_ttf (pulled in by use of CMake's Fetchcontent)

### Linux

#### Ubuntu

* Freetype-dev -> install with 'sudo apt install libfreetype-dev'

#### Rocky / Fedora

* Freetype-dev -> install with 'sudo dnf install freetype-devel'

## Testing

The project utilize CTest which is pulled down when using CMake to create the build files.

## Runs on

* OSX using clang++ to compile.
* Fedora using g++ to compile.

## Know issues

