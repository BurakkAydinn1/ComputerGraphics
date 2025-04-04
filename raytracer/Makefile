CXX = g++
CXXFLAGS = -std=c++17 -O2 -Iinclude -Itinyxml2

SRC = $(wildcard src/*.cpp) tinyxml2/tinyxml2.cpp
OBJ = $(SRC:.cpp=.o)

SCENE ?= scenes/scene_001.xml
SCENE_NAME = $(notdir $(basename $(SCENE)))
OUTPUT_NAME = output/render_$(SCENE_NAME).ppm

export OUTPUT_NAME

all: raytracer

raytracer: main.o $(OBJ)
	$(CXX) $(CXXFLAGS) -o raytracer main.o $(OBJ)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

run: raytracer
	OUTPUT_NAME=$(OUTPUT_NAME) ./raytracer $(SCENE)

render: run
	./convert.sh $(OUTPUT_NAME)

clean:
	rm -f *.o src/*.o output/*.* tinyxml2/*.o raytracer

.PHONY: all run render clean

