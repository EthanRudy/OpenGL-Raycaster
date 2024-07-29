#include "../include/texture.h"

Texture::Texture(std::string filename) {
	std::ifstream file;
	file.open(TEX_PATH + filename);

	if (!file.good()) { std::cout << "Error opening file: " << filename << "\n"; }
	else {
		std::string line;
		int sPos, channel = 0, index = 0;
		color newColor;

		// Skip the first two lines
		std::getline(file, line);
		std::getline(file, line);

		std::getline(file, line);
		sPos = line.find(' ');
		w = stoi(line.substr(0, sPos));
		h = stoi(line.substr(sPos));
		// Allocate memory
		pixels = new color[w * h];

		// Skip the next line
		std::getline(file, line);

		while (std::getline(file, line)) {
			switch (channel) {
			case 0:	// Red
				newColor.r = stof(line) / 255.f;
				break;
			case 1:	// Green
				newColor.g = stof(line) / 255.f;
				break;
			case 2: // Blue
				newColor.b = stof(line) / 255.f;
				break;
			default: break;
			}

			++channel;
			if (channel == 3) { 
				channel = 0;
				pixels[index] = newColor;
				++index;
			}
		}
	}
}

color Texture::colorAt(int x, int y) {
	return pixels[y * w + x];
}