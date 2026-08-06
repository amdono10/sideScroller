# set minimum CMake version
cmake_minimum_required(VERSION 4.3 FATAL_ERROR)
# project name and language
project(sideScroller CXX)

# automatically add the current source and build directories to the include path
# this is useful in out-of-source buildes
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# add source for additional modules here
set(LIB_SOURCE
	math.hpp				math.cpp
	component.hpp			component.cpp
	entity.hpp				entity.cpp
	spriteComponent.hpp		spriteComponent.cpp
	tileMapComponent.hpp	tileMapComponent.cpp
	animSpriteComponent.hpp	animSpriteComponent.cpp
	character.hpp			character.cpp
	game.hpp 				game.cpp)

# add source for Main program loop here
set(MAIN_SOURCE main.cpp)

# ---------------------------------------------------------------------------
# Local install locations (hints for find_package)
# ---------------------------------------------------------------------------
# If you installed SDL3 / SDL3_image to non-standard locations, list the
# install prefixes here (the folder containing each library's "cmake"
# subdirectory). CMAKE_PREFIX_PATH is what find_package() checks first,
# in addition to standard platform locations (CMAKE_SYSTEM_PREFIX_PATH,
# e.g. /usr/local on Linux/macOS or Program Files on Windows), before
# giving up and letting us fall back to FetchContent below.
#
# I use list(APPEND ...) rather than set(...) so I don't clobber any
# value already provided via the command line or environment, e.g.:
#   cmake -B build -DCMAKE_PREFIX_PATH="C:/SomeOtherLib"
# Edit the paths below to match your own machine.
list(APPEND CMAKE_PREFIX_PATH "C:/SDL" "C:/SDL_image")

# Developer override options with -DSDL3_USE_SYSTEM=OFF build flag
# Developer Option 1: Use installed SDL3 if available
# Developer Option 2: Auto-download if not found
option(SDL3_USE_SYSTEM "Use system SDL3 if available" ON)
if(SDL3_USE_SYSTEM)
	# Try to find SDL3 first (checks CMAKE_PREFIX_PATH hints above, plus standard system locations)
	find_package(SDL3 QUIET)
endif()

# SDL3_FOUND is the bool indicating whether the requested version of SDL library was found
# cmake documentation w/ more info https://cmake.org/cmake/help/latest/module/FindSDL.html
if(NOT SDL3_FOUND)
	message(STATUS "SDL3 not found via find_package. Using FetchContent to download from Github and build...")

	include(FetchContent)
	FetchContent_Declare(SDL3
		GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
		GIT_TAG main
	)

	FetchContent_MakeAvailable(SDL3)
	message(STATUS "SDL3 downloaded and configured")
else()
	message(STATUS "SDL3 found: ${SDL3_DIR}")
endif()

# Developer override options with -DSDL_IMAGE_USE_SYSTEM=OFF build flag
# Developer Option 1: Use installed SDL_image if available
# Developer Option 2: Auto-download if not found
option(SDL_IMAGE_USE_SYSTEM "Use system SDL_image if available" ON)
if(SDL_IMAGE_USE_SYSTEM)
	# Try to find SDL3_image first (checks CMAKE_PREFIX_PATH hints above, plus standard system locations)
	find_package(SDL3_image QUIET)
endif()

# SDL3_image_FOUND is the bool indicating whether the requested version of SDL_image library was found
# cmake documentation w/ more info https://cmake.org/cmake/help/latest/module/FindSDL_image.html
if(NOT SDL3_image_FOUND)
	message(STATUS "SDL3_image not found via find_package. Using FetchContent to download from Github and build...")

	# Disable heavy optional image formats so the vendored build doesn't
	# require extra toolchain components (e.g. AVIF pulls in dav1d, which
	# needs a NASM assembler that most dev machines don't have installed).
	set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)
	set(SDLIMAGE_JXL  OFF CACHE BOOL "" FORCE)
	set(SDLIMAGE_TIF  OFF CACHE BOOL "" FORCE)
	set(SDLIMAGE_WEBP OFF CACHE BOOL "" FORCE)

	include(FetchContent)
	FetchContent_Declare(SDL3_image
		GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
		GIT_TAG main
	)

	FetchContent_MakeAvailable(SDL3_image)
	message(STATUS "SDL3_image downloaded and configured (AVIF/JXL/TIFF/WebP disabled)")
else()
	message(STATUS "SDL3_image found: ${SDL3_image_DIR}")
endif()

# set game name/title here
set(EXECUTABLE_NAME sideScroller)

# try to prevent accidental in-source builds, as these cause a lot of problems
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "In-source builds not allowed. Remove any files created thus far and use a different directory for the build.")
endif()

# Compiler standardization
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# executable target and corresponding source file(s)
add_executable(${EXECUTABLE_NAME} ${MAIN_SOURCE} ${LIB_SOURCE})

# link the SDL target (handles include directories and binaries)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE SDL3_image::SDL3_image SDL3::SDL3)

# Copy SDL3.dll to the executable directory after build
add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:SDL3::SDL3>
		$<TARGET_FILE_DIR:${EXECUTABLE_NAME}>
	COMMENT "Copying SDL3.dll to executable directory\n"
)

# Copy SDL3_image.dll to the executable directory after build
add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:SDL3_image::SDL3_image>
		$<TARGET_FILE_DIR:${EXECUTABLE_NAME}>
	COMMENT "Copying SDL3_image.dll to executable directory\n"
)

# Copy Assets directory to executable directory after build
# This is needed to correctly load assets with IMG_load
add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
		${CMAKE_SOURCE_DIR}/Assets
		$<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/Assets
	COMMENT "Copying Assets directory to executable directory\n"
)