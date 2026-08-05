# sideScroller
sideScroller using SDL3

**bold text** In order to build and run, you will need to have SDL3 and SDL_image installed.

Building with CMake with fetch SDL3 from github and incorporate with build.

If you wish to install SDL3 to your machine with CMake (rather than have CMake fetch from remote) follow the instructions on the [SDL Wiki](https://wiki.libsdl.org/SDL3/README-cmake)

If you wish to install SDL_image to your machine with CMake, the following is an example to install on Windows.
- Clone from Repo: [SDL_image Github](https://github.com/libsdl-org/SDL_image/releases)
- `cd SDL_image` 
- `git checkout release-x.x.x 	#3.4.4 is CAO` 
- `git submodule update --init --recursive		#refresh dependencies for build` 
- `cmake -S . -B build -DSDL3_DIR=C:\SDL\cmake -DSDLIMAGE_AVIF=OFF -DSDLIMAGE_JXL=OFF -DSDLIMAGE_TIF=OFF -DSDLIMAGE_WEBP=OFF -DCMAKE_INSTALL_PREFIX=C:\SDL_image` 
- configure's CMake; Developer flags to direct to SDL install, disable AVIF/JXL/TIF/WEBP formats, and where to install SDL_image
- `cmake --build build --config RelWithDebInfo` 
- `cmake --install build --config RelWithDebInfo` 

## Git workflow self reminder note
1. On machine A (create & push)
   - Create local branch named exercise
   ```bash
   git checkout -b exercise
   ``` 
   - alternatively If there are local file conflicts (have the same name as branch)
   ```bash
   git checkout -b exercise origin/exercisei
   ``` 
   - ... make commits ...
   - Push & set upstream tracking
   ```bash
   git push -u origin exercise
   ``` 
2. On Machine B (fetch & work)
   - Update remote tracking branches
   ```bash
   git fetch origin
   ``` 
   - Creates local branch tracking origin/exercise
   ```bash
   git checkout exercise
   ``` 
   -  ... make commits ...
   - Push works without arguments bc tracking is set
   ```bash
   git push
   ``` 

Merging back into main:
1. Finish work on branch (exercise), then push:
	```bash
	git push origin exercise
	```
2. Switch to main and pull latest:
	```bash
	git checkout main
	git pull origin main
	```
3. Merge exercise into main: OR CREATE A PULL REQUEST ON GITHUB
	```bash
	git merge exercise
	```
1. Push main to remote:
	```bash
	git push origin main
	```

Post Merge clean-up:
```bash
git branch -d exercise				# Delete local branch
git push origin --delete exercise	# Delete remote branch
```
   
 