CXX = g++
CXXFLAGS = -Wall -O2 -Iinclude

all:
	$(CXX) $(CXXFLAGS) src/main.cpp -o football_pure_cpp
	./football_pure_cpp

opengl:
	$(CXX) $(CXXFLAGS) -DUSE_OPENGL src/main.cpp -o football_opengl -lGL -lGLU -lglut
	./football_opengl

clean:
	rm -f football_pure_cpp football_opengl output/*.ppm
