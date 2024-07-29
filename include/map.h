#ifndef MAP_H
#define MAP_H

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <GLFW/glfw3.h>
#include <vector>
#include "texture.h"


const std::string MAP_PATH = "./resources/maps/";

class Map {
public:

	Map();

	Map(const std::string& filename);

	int floor(int index);

	int wall(int index);

	int ceiling(int index);

	void render2D();

	int sX();
	int sY();
	int dim();
	float sA();

	int width();
	int height();


	void debug();

	std::vector<Texture> tex;

protected:
private:

	// Map Dimensions
	int w, h;	// Width & Height
	int len;	// Unwrapped length
	int pDim;	// Pixel dimensions

	// Player spawn location
	int spawnX, spawnY;	// X and Y in MAP COORDINATES
	float spawnA;		// Spawn angle (0 to 2PI)

	// Map arrays
	int* _floor;
	int* _wall;
	int* _ceiling;

	void print(int* arr);

	
};

#endif // !MAP_H