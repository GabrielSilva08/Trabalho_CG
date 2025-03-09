all:
	g++ -Isrc/include -L src/lib -o main main.cpp $(wildcard models/classes/*.cpp) -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
