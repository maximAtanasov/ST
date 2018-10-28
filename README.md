<b>Slavic Tales</b>
===================

A 2D Platformer project built with C++, SDL2 and Lua.
=====================================================

![](ST_engine/docs/screenshots/screenshot1.png?raw=true)

![](ST_engine/docs/screenshots/screenshot2.png?raw=true)

![](ST_engine/docs/screenshots/screenshot3.png?raw=true)

[A short video](https://youtu.be/7citcBJ2SI4)



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

Also, have a look at the [Documentation](http://maximatanasov.github.io/slavicTales/index.html).
