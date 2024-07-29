#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>
#include <fstream>

struct color {
	float r, g, b;
};

const std::string TEX_PATH = "./resources/textures/";

class Texture {
public:
	Texture(std::string filename);

	int w, h;
	color* pixels;

	color colorAt(int x, int y);
};

#endif // !TEXTURE_H