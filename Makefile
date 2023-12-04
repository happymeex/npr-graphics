# compiler
CC = g++

# compiler flags
FLAGS = -g -Wall -Werror -std=c++17 -Wno-unused-variable -Iexternal

all: main

main: src/main.cpp external/lodepng.cpp src/util/png.cpp src/util/Mesh.cpp src/util/util.cpp
	$(CC) $(FLAGS) -o build/main $^