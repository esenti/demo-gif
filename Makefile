all: build

run: build
	./demo

build:
	g++ main.cpp -o demo -Wl,-Bstatic -lGLEW -Wl,-Bdynamic -lSDL2 -lSDL2main -lGL --std=c++11
