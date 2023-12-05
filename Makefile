# compiler
CC = g++

# compiler flags
FLAGS = -g -Wall -Werror -std=c++17 -Wno-unused-variable -Iexternal

all: main

main: src/main.cpp external/lodepng.cpp src/Mesh.cpp src/util.cpp src/Scene.cpp src/Tracer.cpp src/Object.cpp src/Image.cpp
	$(CC) $(FLAGS) -o build/main $^
