# sideScroller
sideScroller using SDL3

**bold text** In order to build and run, you will need to have SDL3 and SDL_image installed.

Building with CMake with fetch SDL3 from github and incorporate with build.

If you wish to install SDL3 to your machine with CMake (rather than have CMake fetch from remote) follow the instructions on the [SDL Wiki](https://wiki.libsdl.org/SDL3/README-cmake)

If you wish to install SDL_image to your machine with CMake, the following is an example to install on Windows.
- Clone from Repo: [SDL_image Github](https://github.com/libsdl-org/SDL_image/releases)
- `code` cd SDL_image
- `code` git checkout release-x.x.x 	#3.4.4 is CAO
- `code` git submodule update --init --recursive		#refresh dependencies for build
- `code` cmake -S . -B build -DSDL3_DIR=C:\SDL\cmake -DSDLIMAGE_AVIF=OFF -DSDLIMAGE_JXL=OFF -DSDLIMAGE_TIF=OFF -DSDLIMAGE_WEBP=OFF -DCMAKE_INSTALL_PREFIX=C:\SDL_image
- configure's CMake; Developer flags to direct to SDL install, disable AVIF/JXL/TIF/WEBP formats, and where to install SDL_image
- `code` cmake --build build --config RelWithDebInfo
- `code` cmake --install build --config RelWithDebInfo
  
