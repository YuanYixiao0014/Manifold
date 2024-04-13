# Compiler settings
CXX = clang++
CXXFLAGS = -std=c++17 $(shell sdl2-config --cflags)

# Include directories
INCLUDES = -I./Include -I./Include/glm -I./Include/rapidjson -I./headers -I./Include/SDL2 -I./Include/SDL_image -I./Include/SDL_ttf -I./Include/SDL_mixer -I./Include/box2d/include/box2d -I./Include/box2d/src -I./Include/box2d/include

SDL2_FLAGS = $(shell sdl2-config --libs)
SDL2_MIXER_FLAGS = $(shell pkg-config --libs SDL2_mixer)
SDL2_TTF_FLAGS = $(shell pkg-config --libs SDL2_ttf)
SDL2_IMAGE_FLAGS = $(shell pkg-config --libs SDL2_image)

# Library paths
LDFLAGS=$(SDL2_FLAGS) $(SDL2_MIXER_FLAGS) $(SDL2_TTF_FLAGS) $(SDL2_IMAGE_FLAGS) -llua5.4

# Target executable name
TARGET = game_engine_linux

# Source files
SOURCES = $(wildcard main.cpp source/*.cpp Include/lua/*.c Include/box2d/src/collision/*.cpp Include/box2d/src/common/*.cpp Include/box2d/src/dynamics/*.cpp Include/box2d/src/rope/*.cpp)

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJECTS)
	$(CXX) -v -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

# Compiling
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJECTS) $(TARGET)

# Phony targets
.PHONY: all clean
