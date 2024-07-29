#include "../include/map.h"

Map::Map() {
	w = 0, h = 0;
	len = 0;
	pDim = 0;

	spawnX = 0; spawnY = 0;
	spawnA = 0;

	_floor = 0;
	_wall = 0;
	_ceiling = 0;
}

Map::Map(const std::string& filename) {
	std::ifstream file;
	file.open(MAP_PATH + filename);

	if (!file.good()) { std::cout << "Error opening file: " << filename << "\n"; }
	else {
		int section = 0;
		
		int sPos, x = 0;
		std::stringstream ss;
		std::string line, word;

		while (section < 5) {
			switch (section) {
			
				// Map Dimensions
			case 0:
				// Width and Height
				std::getline(file, line);
				sPos = line.find(' ');
				w = stoi(line.substr(0, sPos));
				h = stoi(line.substr(sPos));
				
				// Pixel dimensions
				std::getline(file, line);
				pDim = stoi(line);
				
				// Allocate Memory
				len = w * h;
				_floor = new int[len];
				_wall = new int[len];
				_ceiling = new int[len];

				// Next section
				++section;
				break;

				// Player Spawn
			case 1:
				// X, Y, and angle
				std::getline(file, line);

				// X
				sPos = line.find(' ');
				spawnX = stoi(line.substr(0, sPos));
				line = line.substr(sPos + 1);

				// Y
				sPos = line.find(' ');
				spawnY = stoi(line.substr(0, sPos));
				

				// Angle
				spawnA = stof(line.substr(sPos + 1));


				// Next section
				++section;
				break;

				// Floors
			case 2:
				for (int y = 0; y < h; ++y) {
					x = 0;
					std::getline(file, line);
					ss = std::stringstream(line);
					while (ss >> word) {
						_floor[y * w + x] = stoi(word);
						++x;
					}
				}

				// Next section
				++section;
				break;

				// Walls
			case 3:
				for (int y = 0; y < h; ++y) {
					x = 0;
					std::getline(file, line);
					ss = std::stringstream(line);
					while (ss >> word) {
						_wall[y * w + x] = stoi(word);
						++x;
					}
				}

				// Next section
				++section;
				break;

				// Ceilings
			case 4:
				for (int y = 0; y < h; ++y) {
					x = 0;
					std::getline(file, line);
					ss = std::stringstream(line);
					while (ss >> word) {
						_ceiling[y * w + x] = stoi(word);
						++x;
					}
				}

				// Next section
				++section;
				break;

			default:
				break;
			}

			// Skip the whitespace
			std::getline(file, line);
		}
	}


	/**
	* 
	* Texture loading
	* 
	*/
	tex.push_back(Texture("brick_1.ppm"));
	tex.push_back(Texture("stone_2.ppm"));
}

int Map::floor(int index) {
	return _floor[index];
}

int Map::wall(int index) {
	return _wall[index];
}

int Map::ceiling(int index) {
	return _ceiling[index];
}

int Map::width() {
	return w;
}
int Map::height() {
	return h;
}

void Map::render2D() {
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			int index = y * w + x;
			if (_wall[index] == 0) {
				glColor3f(.1f, .1f, .1f);
			}
			else {
				glColor3f(1.f, 1.f, 1.f);
			}

			glBegin(GL_QUADS);
			glVertex2i(x * pDim + 1, y * pDim + 1);
			glVertex2i(x * pDim + pDim - 1, y * pDim + 1);
			glVertex2i(x * pDim + pDim - 1, y * pDim + pDim - 1);
			glVertex2i(x * pDim + 1, y * pDim + pDim - 1);
			glEnd();
		}
	}
}

int Map::sX() {
	return spawnX;
}
int Map::sY() {
	return spawnY;
}
int Map::dim() {
	return pDim;
}
float Map::sA() {
	return spawnA;
}

void Map::debug() {
	std::cout << "----------MAP----------\n";
	std::cout << "DIM: (" << w << ", " << h << ")\n";
	std::cout << "PIXEL DIM: " << pDim << "\n";
	std::cout << "PLAYER SPAWN: (" << spawnX << ", " << spawnY << ")\n";
	std::cout << "\tAngle: " << spawnA << "\n";
	std::cout << "FLOOR:\n";
	print(_floor);
	std::cout << "WALL:\n";
	print(_wall);
	std::cout << "CEILING:\n";
	print(_ceiling);
	std::cout << "-----------------------\n\n";
}

void Map::print(int* arr) {
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			std::cout << arr[y * w + x] << " ";
		}
		std::cout << "\n";
	}
}