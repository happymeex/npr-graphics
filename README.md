# Non-photorealistic renderer

This repo contains source code for a CPU-based ray tracer
that produces images in watercolor and cel-shaded styles.

## Setup

Download files `lodepng.h` and `lodepng.cpp` from [here](https://github.com/lvandeve/lodepng/tree/d398e0f10d152a5d17fa30463474dc9f56523f9c) and put them in the `external/` directory.
Then, download the `glm/` folder from the [GLM](https://github.com/g-truc/glm/tree/586a402397dd35d66d7a079049856d1e2cbab300) repository and put it in `external/` as well.

## Usage

Run `make` to generate an executable.
Run `./bin/main` to run it.
Any output PNGs should be found in `out/`.
