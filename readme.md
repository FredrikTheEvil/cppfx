CppFx C++ Framework X
===========================

Dependencies
------------
 - Required
 	- GLEW - OpenGL Extension Wrangler
 - Optional
 	- SDL2
 	- GLFW
 	- libpng

Note that even though SDL2 and GLFW are marked as optional, atleast one of them MUST be present. 

Building
--------

On linux building is easy, just make sure you have all the dependencies. On ubuntu you can use this command to install all dependencies, clone the repo and build

```
	sudo apt-get install -y cmake git g++ libsdl2-dev libglew-dev libpng-dev
	git clone https://github.com/FredrikTheEvil/cppfx.git
	mkdir cppfx/build
	cd cppfx/build
	cmake -DCMAKE_BUILD_TYPE=Release ..
	make
```

On Windows you need to download binaries for or build the dependencies from source (recommended)
and then use cmake to configure the paths to the dependencies.