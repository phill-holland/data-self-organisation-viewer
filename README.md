# Data Self Organisation - 3D Data Viewer

this application relates to the output for;

https://www.github.com/phill-holland/data-self-organisation-vi

This is a tiny, tiny tiny small implementation of a Vulkan graphics engine, it allowed for the dynamic addition and removal of 3d objects within the engine.

You'll be prompt when the program starts to select a input text trace file (stored in the assets/data/traces directory).

You can then step through each iteration of the trace with the space bar.  Each trace demonstrates a program for storing data, which will produce the same output, these programs are found using a genetic algorithm (more details can be found in the repo link above)

The aim is to analysis and find any common patterns in each program generated.

# Control Keys

- space: step through program
- q,w,s,d: move view camera along x & y axis
- i,j,k,l: rotate camera along x & axis
- r,f: zoom in/out

# Todo

- It's difficult at the moment to relate the data each cube visualises (words), this needs to be improved by extending texture mapping implementation

- Difficult to tell when one cube collides with another cube (and thereby generates output)

# Demonstrates

- CMake
- Vulkan
- Shaders

It should be possible to build and run the code with alternative development environments, outside of VSCode.

Host Machine Requirements;

To download VSCode;

https://code.visualstudio.com/download

# Prerequisites

- CMake

- Vulkan Shaders SDK

See below for how to install the Vulkan-SDK (without it, the code will not compile!)

https://vulkan.lunarg.com/doc/view/latest/linux/getting_started_ubuntu.html


