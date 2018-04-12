
Instructions for building the project:

Dependcies needed on Linux:

cmake - you need version 3.9 or higher (or your IDE comes bundled with CMake)

liblua5.3
libSDL2
libSDL2_image
libSDL2_mixer
libSDL2_ttf
libgtest
libglew

if you are on Ubuntu (or another Debian-based distro) you can install all dependencies like this:
```
sudo apt-get install cmake liblua5.3-dev libSDL2-dev libSDL2_image-dev libSDL2_mixer-dev libSDL2_ttf-dev libglew-dev

sudo apt-get install libgtest-dev

cd /usr/src/gtest

sudo cmake CMakeLists.txt

sudo make

sudo cp *.a /usr/lib
```

Or if you are on Fedora:
```
sudo dnf install SDL2-devel SDL2_image-devel SDL2_mixer-devel SDL2_ttf-devel lua-devel glew-devel gtest-devel

cd /usr/src/gtest

sudo cmake CMakeLists.txt

sudo make

sudo cp *.a /usr/lib
```

If you are on linux you can use your favorite IDE and open the folder as a CMake project
(CLion, QtCreator, KDevelop, NetBeans, Eclipse with extensions, etc.) and run the ST_engine target or you can run the following commands in the root of the project:
```
cmake CMakeLists.txt
make ST_engine
cd ST_engine/gamedata
./ST_engine
```

New changes:

The developer console has been upgraded and you can now type in any lua code (has to be in one line) and have it be
executed immediately.

There are now also 2 global definitions - __DEBUG and __RELEASE that will enable or disable certain debugging features. This, togheter with the dev console eliminates the need for debug code inside gameplay code.

--Lua scripts are now pre-processed before being loaded - strings in certain functions are hashed to integers to avoid hashing/string copying on every frame

--all `inputConf.cfg` and 'asset.list` files now support comments - 
just add a '#' to the beggining of the line you wish to comment out (only supports full line comments)

--fixed a flaw where textures in the openGL renderer weren't being freed

--The message_bus has been redone and message/data lifetimes are now better managed,
basically elimating the flaw that made it completely unusable as subsystems could never be guaranteed access to the data inside messages - it is now guaranteed to have the data be live until the message is destroyed.
