<b>ST</b>
===================

[![CircleCI](https://circleci.com/gh/maximAtanasov/ST/tree/master.svg?style=svg&circle-token=2e636633b20ca5cdbe02468a9593a485014bd88a)](https://circleci.com/gh/maximAtanasov/ST/tree/master)
[![Build status](https://ci.appveyor.com/api/projects/status/iqte4fwebiml3xv6?svg=true)](https://ci.appveyor.com/project/maximAtanasov/slavictales)

A 2D game engine project built with C++, SDL2 and Lua.
=====================================================

The engine supports the loading of assets in multiple formats, as well as
a custom binary format. It also supports audio playback, basic multitheading and
physics and the ability to program games fully in Lua.

![](ST_engine/docs/screenshots/screenshot1.png?raw=true)

![](ST_engine/docs/screenshots/screenshot2.png?raw=true)

![](ST_engine/docs/screenshots/screenshot3.png?raw=true)


Instructions for building the project:
==============================================

<b><big>Linux:</big></b>

Dependcies needed on Linux:

libSDL2
libSDL2_image
libSDL2_mixer
libSDL2_ttf

if you are on Ubuntu (or another Debian-based distro) you can install all dependencies like this:
```
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
```

Or if you are on Fedora:
```
sudo dnf install SDL2-devel SDL2_image-devel SDL2_mixer-devel SDL2_ttf-devel

```

You can also open the project with any IDE that supports CMake projects or run these commands on the command line: 

```
cmake CMakeLists.txt && make all
cd ST_engine/gamedata
./ST_engine
```
 
<b><big>Windows:</big></b>

Building using the MSVC toolchain is supported on Windows.
You can use Visual Studio 2017 to open the folder as a CMake Project (or alternatively CLion with Visual Studio as the selected toolchain). Select the `ST_engine.exe` target and run it. The game should compile and run without any additional dependencies.

Also, have a look at the [Documentation](https://maximatanasov.github.io/ST/).
