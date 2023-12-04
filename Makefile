# compiler
CC = g++

# compiler flags
FLAGS = -g -Wall -Werror -std=c++17 -Wno-unused-variable

all: main

main: src/main.cpp external/lodepng.cpp src/util/png.cpp
	$(CC) $(FLAGS) -o build/main src/main.cpp external/lodepng.cpp src/util/png.cpp