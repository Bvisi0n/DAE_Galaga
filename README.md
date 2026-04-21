# DAE_Galaga

This is a learning project that spans the entire 4th semester of the course [Programming 4](https://youtu.be/j96Oh6vzhmg) in the [Game Development](https://digitalartsandentertainment.be/page/31/Game+Development) major at [Digital Arts & Entertainment](https://digitalartsandentertainment.be/). Every lesson new patterns and techniques are introduced inspired by the book [Game Programming Patterns](https://gameprogrammingpatterns.com/) by [Robert Nystrom](https://github.com/munificent) which have to be implimented as weekly assignments.

Students are randomly assigned 1 of 10 retro games to recreate by the end of the semester at which points they must present and defend their project.

[![Build Status](https://github.com/Bvisi0n/DAE_Galaga/actions/workflows/cmake.yml/badge.svg)](https://github.com/Bvisi0n/cmake/actions)
[![Build Status](https://github.com/Bvisi0n/DAE_Galaga/actions/workflows/emscripten.yml/badge.svg)](https://github.com/Bvisi0n/emscripten/actions)
[![GitHub Release](https://img.shields.io/github/v/release/Bvisi0n/DAE_Galaga?logo=github&sort=semver)](https://github.com/Bvisi0n/DAE_Galaga/releases/latest)

The Emscripten workflow generates a web version of the project and publishes it [here](https://bvisi0n.github.io/DAE_Galaga/) as a github page.

## Galaga
I was assigned to recreate the arcade game [Galaga](https://en.wikipedia.org/wiki/Galaga) which was released in 1981 by Namco.

It is a fixed shooter game where the player controls a spaceship at the bottom of the screen and has to shoot down waves of alien ships that fly in from the top of the screen. The player can move left and right and shoot bullets upwards to destroy the aliens. The aliens have different movement patterns and can also shoot back at the player. The goal is to score as many points as possible by destroying the aliens while avoiding getting hit by their bullets.

You can find this project's requirements for the [Programming 4](https://youtu.be/j96Oh6vzhmg) course in the [Repository Requirements](RepositoryRequirements.md) document which also functions as my checklist.

## Gameplay Prototyping
I also use this project for the course [Gameplay Prototyping](https://digitalartsandentertainment.be/page/349/Gameplay+Prototyping) where the goal is to iterate on a concept based on feedback gathered through playtesting sessions. For this course code quality is of zero importance so don't expect a high standard in it's branch although I will do my best given the limited time available for this course.

In this prototype I explore whether I can make playing with gravity fun.
Can it be fun to influence the trajectory of projectiles?

I have chosen to do this because it challenges the game engine and helps me learn to identify where the lines lies between game and engine code.

## Noteworthy Patterns, Idioms, Techniques & Libraries
This section is still under construction... For now I list everything with potential even if not yet adopted, I will add details and trim this list as I finish up the project.

### Behavioral Patterns
#### Command
In the course we were challenged to implement a command pattern for the player input, which I did. The pattern could also be used to control game ai or to implement an undo system but these areas currently left unexplored.
#### Observer
#### State
#### Type Object
#### Bytecode
#### Subclass Sandbox

### Creational Patterns
#### Singleton
#### Prototype

### Decoupling Patterns
#### Component
#### Event Queue
#### Service Locator

### Optimization Patterns
#### Dirty Flag
#### Data Locality
#### Object Pool
#### Flyweight
#### Spatial Partition

### Sequencing Patterns
#### Game Loop
#### Update Method
#### Double Buffer

### Idioms, Techniques & Libraries
#### Scenegraph
#### Imgui
#### Pimpl
#### Concurrency
#### Dependency Injection
#### Curiously Recurring Template Pattern (CRTP)


## Minigin

The students are provided with [Minigin](https://github.com/avadae/minigin), a very small project using [SDL3](https://www.libsdl.org/) and [glm](https://github.com/g-truc/glm) for 2D C++ game projects. It is in no way a game engine, only a barebone start project where everything SDL related has been set up. It contains glm for vector math, to aleviate the need to write custom vector and matrix classes.

### Windows Version

Either
- Open the root folder in Visual Studio 2026; this will be recognized as a cmake project.
  
Or
- Install CMake
- Install CMake and CMake Tools extensions in Visual Code
- Open the root folder in Visual Code,  this will be recognized as a cmake project.

Or
- Use whatever editor you like :)

### Emscripten (web) Version

#### On Windows

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

#### On OSX

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

### Github Actions

This project is build with github actions.
- The CMake workflow builds the project in Debug and Release for Windows and serves as a check that the project builds on that platform.
- The Emscripten workflow generates a web version of the project and publishes it as a [github page](https://bvisi0n.github.io/DAE_Galaga/).
  - The url of that page will be `https://<username>.github.io/<repository>/`
- You can embed this page with

```<iframe style="position: absolute; top: 0px; left: 0px; width: 1024px; height: 576px;" src="https://<username>.github.io/<repository>/" loading="lazy"></iframe>```
