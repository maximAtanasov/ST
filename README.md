<b>Slavic Tales</b>
===================

A 2D Platformer project built with C++, SDL2 and Lua.
=====================================================

![](https://raw.githubusercontent.com/maximAtanasov/slavicTales/master/ST_engine/docs/screenshots/screenshot1.png)

![](https://raw.githubusercontent.com/maximAtanasov/slavicTales/master/ST_engine/docs/screenshots/screenshot2.png)

![](https://raw.githubusercontent.com/maximAtanasov/slavicTales/master/ST_engine/docs/screenshots/screenshot3.png)

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
sudo apt-get install liblua5.3-dev libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
```

Or if you are on Fedora:
```
sudo dnf install SDL2-devel SDL2_image-devel SDL2_mixer-devel SDL2_ttf-devel lua-devel

```

If you do not have a recent version of cmake installed, you can point your IDE to the included version inside the `cmake` folder. 

If you are on linux you can use your favourite IDE and open the folder as a CMake project
(CLion, QtCreator, KDevelop, NetBeans, Eclipse with extensions, etc.) and run the ST_engine target or you can run the following commands in the root of the project:

```
./cmakew CMakeLists.txt
make all
cd ST_engine/gamedata
./ST_engine
```

Also, have a look at the [Documentation](http://maximatanasov.github.io/slavicTales/index.html).
