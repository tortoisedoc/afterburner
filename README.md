# afterburner

A simple old-school example of a fire simulation. Written in C++ and C.

## Requirements

  - A Linux w/build-essential (gcc) OR git bash shell + relevant MINGW compilers, as well as Qt
(5.15 on my setup)

  - OpenCV development libraries, on debian `apt-get install -y libopencv-dev`

  To build the cmd line testing tool:

  - SFML - on debian, install via `apt-get install -y libsfml-dev`

## Building

  1. To build the _SMFL sample_ (which runs on the command line and uses CMake)

  - Open a terminal on the root of the repo
  - Run cmake command with `./cmake.sh`
  - `cd build && make -j 4`

  2. To build the actual demo, you will need Qt (5.15 on my laptop currently). This is built by qmake

  - Open the project under `firehose/firehose.pro`
  - compile
 
## Running

  To run the SMFL sample, `cd build && ./fire_simulation`

  To run the Qt sample, simply press F5 from QtCreator
