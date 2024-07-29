#ifndef PLAYER_H
#define PLAYER_H

#include <GLFW/glfw3.h>
#include <iostream>
#include "map.h"

#define PI 3.1415926535
#define P2 PI / 2
#define P3 3 * PI / 2

class Player {
public:

	Player();

	void setSpawn(int x, int y, float a, int dim);

	void render2D();
	void render3D(Map* map, int WIDTH, int HEIGHT);

	void move(int n);

	void debug();
protected:
private:

	int x, y;
	float dx, dy;
	float a;

	float mv;
	float rv;

	int fov;
	int rays;

	float degToRad(float n);

	float dist(float x0, float y0, float x1, float y1);
	float mapToTex(float wallA, float wallS, float wallE, float texS, float texE);
};

#endif // !PLAYER_H