New changes:

A CMake project is now available.
The developer console has been upgraded and you can now type in any lua code (has to be in one line) and have it be
executed immediately.

There are now also 2 global definitions - __DEBUG and __RELEASE that will enable or disable certain debugging features. This, togheter with the dev console eliminates the need for debug code inside gameplay code.

--Lua scripts are now pre-processed before being loaded - strings in certain functions are hashed to integers to avoid hashing/string copying on every frame

--all `inputConf.cfg` and 'asset.list` files now support comments - 
just add a '#' to the beggining of the line you wish to comment out (only supports full line comments)

--fixed a flaw where textures in the openGL renderer weren't being freed

--The message_bus has been redone and message/data lifetimes are now better managed,
basically elimating the flaw that made it completely unusable as subsystems could never be guaranteed access to the data inside messages - it is now guaranteed to have the data be live until the message is destroyed.
