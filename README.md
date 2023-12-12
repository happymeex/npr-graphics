# Non-photorealistic renderer

This repo contains source code for a CPU-based ray tracer
that produces images in watercolor and cel-shaded styles.

![ball-cell](https://i.imgur.com/7KzTXLx.png)
![ball-watercolor](https://i.imgur.com/JTKmvTA.png)

![teapot-cell](https://imgur.com/gFNSB6i.png)
![teapot-watercolor](https://imgur.com/CBWA0X2.png)

## Setup

Download the following and put them in the `external/` directory:

-   Files `lodepng.h` and `lodepng.cpp` from [here](https://github.com/lvandeve/lodepng/tree/d398e0f10d152a5d17fa30463474dc9f56523f9c),
-   The `glm/` folder from the [GLM](https://github.com/g-truc/glm/tree/586a402397dd35d66d7a079049856d1e2cbab300) repository,
-   The file `PerlinNoise.hpp` from [here](https://github.com/Reputeless/PerlinNoise/tree/bdf39fe92b2a585cdef485bcec2bca8ab5614095).

## Usage

Run `make` to generate an executable and `./bin/main` to run it.
This renders four example scenes whose output PNGs can be found in `out/`.
You can find the code and API usage in `src/examples.cpp`.

## Acknowledgments

This code was written by Maxwell Jiang and Daniel Hu for MIT's 6.4400 Computer Graphics course.
The watercolor algorithms are based on those described in Santiago Montesdeoca's [thesis](https://artineering.io/publications/real-time-watercolor-rendering-of-3d-objects-and-animation-with-enhanced-control).
