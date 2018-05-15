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

cmake - you need version 3.9 or higher (or your IDE comes bundled with CMake)

liblua5.3
libsd
libSDL2_image
libSDL2_mixer
libSDL2_ttf
libgtest
libglew

if you are on Ubuntu (or another Debian-based distro) you can install all dependencies like this:
```
sudo apt-get install cmake liblua5.3-dev libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev

sudo apt-get install libgtest-dev

cd /usr/src/gtest

sudo cmake CMakeLists.txt

sudo make

sudo cp *.a /usr/lib
```

Or if you are on Fedora:
```
sudo dnf install SDL2-devel SDL2_image-devel SDL2_mixer-devel SDL2_ttf-devel lua-devel gtest-devel

```

If you are on linux you can use your favourite IDE and open the folder as a CMake project
(CLion, QtCreator, KDevelop, NetBeans, Eclipse with extensions, etc.) and run the ST_engine target or you can run the following commands in the root of the project:
```
cmake CMakeLists.txt
make ST_engine
cd ST_engine/gamedata
./ST_engine
```


Also, have a look at the [Documentation](http://maximatanasov.github.io/slavicTales/index.html).
