# compiler
CC = g++

# compiler flags
FLAGS = -g -Wall -Werror -std=c++17 -Wno-unused-variable -Iexternal

SRCDIR = src/
OBJDIR = obj/
BINDIR = bin/
EXTDIR = external/
SRCFILES = $(shell find $(SRCDIR) -name "*.cpp" -type f)
OBJFILES = $(patsubst $(SRCDIR)%.cpp,$(OBJDIR)%.o,$(SRCFILES))
EXTFILES = $(shell find $(EXTDIR) -maxdepth 1 -name "*.cpp" -type f)
EXTOBJS = $(patsubst $(EXTDIR)%.cpp,$(OBJDIR)%.o,$(EXTFILES))

EXECUTABLE = $(BINDIR)main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJFILES) $(EXTOBJS)
	$(CC) $(FLAGS) -o $@ $^

obj/%.o: $(SRCDIR)%.cpp
	$(CC) $(FLAGS) -c $< -o $@

obj/%.o: $(EXTDIR)%.cpp
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)
	mkdir $(OBJDIR)