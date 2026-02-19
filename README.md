# DAE_Galaga

A learning project that spans the entire 4th semester of the course [Programming 4](https://youtu.be/j96Oh6vzhmg) in the [Game Development](https://digitalartsandentertainment.be/page/31/Game+Development) major at [Digital Arts & Entertainment](https://digitalartsandentertainment.be/). Every lesson new patterns and techniques are introduced inspired by the book [Game Programming Patterns](https://gameprogrammingpatterns.com/) by [Robert Nystrom](https://github.com/munificent) which have to be implimented as weekly assignments. Students are randomly assigned 1 of 10 retro games to recreate by the end of the semester at which points they must present and defend their project.

[![Build Status](https://github.com/Bvisi0n/DAE_Galaga/actions/workflows/cmake.yml/badge.svg)](https://github.com/Bvisi0n/cmake/actions)
[![Build Status](https://github.com/Bvisi0n/DAE_Galaga/actions/workflows/emscripten.yml/badge.svg)](https://github.com/Bvisi0n/emscripten/actions)
[![GitHub Release](https://img.shields.io/github/v/release/Bvisi0n/DAE_Galaga?logo=github&sort=semver)](https://github.com/Bvisi0n/DAE_Galaga/releases/latest)

The Emscripten workflow generates a web version of the project and publishes it [here](https://bvisi0n.github.io/DAE_Galaga/) as a github page.

## Minigin

[Minigin](https://github.com/avadae/minigin) is a very small project using [SDL3](https://www.libsdl.org/) and [glm](https://github.com/g-truc/glm) for 2D C++ game projects. It is in no way a game engine, only a barebone start project where everything SDL related has been set up. It contains glm for vector math, to aleviate the need to write custom vector and matrix classes.

## Windows version

Either
- Open the root folder in Visual Studio 2026; this will be recognized as a cmake project.
  
Or
- Install CMake
- Install CMake and CMake Tools extensions in Visual Code
- Open the root folder in Visual Code,  this will be recognized as a cmake project.

Or
- Use whatever editor you like :)

## Emscripten (web) version

### On windows

For installing all of the needed tools on Windows I recommend using [Chocolatey](https://chocolatey.org/). You can then run the following in a terminal to install what is needed:

    choco install -y cmake
    choco install -y emscripten
    choco install -y ninja
    choco install -y python

In a terminal, navigate to the root folder. Run this:

    mkdir build_web
    cd build_web
    emcmake cmake ..
    emmake ninja

To be able to see the webpage you can start a python webserver in the build_web folder

    python -m http.server

Then browse to http://localhost:8000 and you're good to go.

### On OSX

On Mac you can use homebrew

    brew install cmake
    brew install emscripten
    brew install python

In a terminal on OSX, navigate to the root folder. Run this:

    mkdir build_web
    cd build_web
    emcmake cmake .. -DCMAKE_OSX_ARCHITECTURES=""
    emmake make

To be able to see the webpage you can start a python webserver in the build_web folder

    python3 -m http.server

Then browse to http://localhost:8000 and you're good to go.

## Github Actions

This project is build with github actions.
- The CMake workflow builds the project in Debug and Release for Windows and serves as a check that the project builds on that platform.
- The Emscripten workflow generates a web version of the project and publishes it as a [github page](https://bvisi0n.github.io/DAE_Galaga/).
  - The url of that page will be `https://<username>.github.io/<repository>/`
- You can embed this page with

```<iframe style="position: absolute; top: 0px; left: 0px; width: 1024px; height: 576px;" src="https://<username>.github.io/<repository>/" loading="lazy"></iframe>```
