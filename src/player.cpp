#include "../include/player.h"

Player::Player() {
	x = 0;
	y = 0;
	a = 0;

	mv = 5;
	dx = cos(a) * mv;
	dy = sin(a) * mv;
	rv = .05f;

	fov = 90;
	rays = 1080;
}

void Player::setSpawn(int x, int y, float a, int dim) {
	this->x = (x * dim) + (dim / 2);
	this->y = (y * dim) + (dim / 2);
	this->a = a;

	dx = cos(a) * mv;
	dy = sin(a) * mv;
}

void Player::render2D() {
	// Draw point
	glColor3f(1, 1, 0);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();

	// View Direction
	glColor3f(1, 1, 0);
	glLineWidth(4);
	glBegin(GL_LINES);
	glVertex2i(x, y);
	glVertex2i(x + dx * 10, y + dy * 10);
	glEnd();
}

void Player::render3D(Map* map, int WIDTH, int HEIGHT) {
	int r, mx, my, mi;
	int mDepth = (map->width() > map->height()) ? map->width() : map->height();
	int depth;
	int dim = map->dim(), size = map->width() * map->height();
	float aspect_ratio = map->width() / map->height();


	float aTan, nTan;
	float rx, ry, ra, xo, yo;

	float hx, hy, vx, vy, distH, distV, distF;

	ra = a - degToRad((float)fov / 2.f);
	if (ra < 0) { ra += 2 * PI; }
	if (ra > 2 * PI) { ra -= 2 * PI; }
	float degPerRes = (float)fov / (float)rays;
	float radInc = degToRad(degPerRes);

	for (r = 0; r < rays; ++r, ra += radInc) {
		if (ra < 0) { ra += 2 * PI; }
		if (ra > 2 * PI) { ra -= 2 * PI; }

		/**
		*
		* Horizontal Edge detection
		*
		*/
		depth = 0;
		aTan = -1 / tan(ra);
		// Looking UP
		if (ra > PI) {
			ry = (((int)y / dim) * dim) - .0001;	// -.0001 for accuracy and off by one errors when indexing
			rx = x + (y - ry) * aTan;

			yo = -dim;
			xo = -yo * aTan;
		}
		// Looking DOWN
		else if (ra < PI) {
			ry = (((int)y / dim) * dim) + dim;
			rx = x + (y - ry) * aTan;

			yo = dim;
			xo = -yo * aTan;
		}
		// Parallel to the x axis, won't ever hit a horizontal wall
		else if (ra == 0 || ra == PI) {
			rx = x;
			ry = y;
			depth = 8;
		}
		while (depth < mDepth) {
			// Find our current cell
			mx = (int)rx / dim;
			my = (int)ry / dim;
			mi = my * map->width() + mx;

			// Hit a wall
			if (mi >= 0 && mi < size && map->wall(mi) > 0) {
				depth = mDepth;
			}
			// Cast further
			else {
				rx += xo;
				ry += yo;
				++depth;
			}
		}
		hx = rx, hy = ry;
		distH = dist(x, y, hx, hy);





		/**
		*
		* Vertical Edge Detection
		*
		*/
		depth = 0;
		nTan = -tan(ra);
		// Looking Left
		if (ra > P2 && ra < P3) {
			rx = (((int)x / dim) * dim) - .0001;	// -.0001 for accuracy and off by one errors when indexing
			ry = y + (x - rx) * nTan;

			xo = -dim;
			yo = -xo * nTan;
		}
		// Looking RIGHT
		else if (ra < P2 || ra > P3) {
			rx = (((int)x / dim) * dim) + dim;
			ry = y + (x - rx) * nTan;

			xo = dim;
			yo = -xo * nTan;
		}
		// Parallel to the y axis, won't ever hit a vertical wall
		else if (ra == P2 || ra == P3) {
			rx = x;
			ry = y;
			depth = 8;
		}
		while (depth < mDepth) {
			// Find our current cell
			mx = (int)rx / dim;
			my = (int)ry / dim;
			mi = my * map->width() + mx;

			// Hit a wall
			if (mi >= 0 && mi < size && map->wall(mi) > 0) {
				depth = mDepth;
			}
			// Cast further
			else {
				rx += xo;
				ry += yo;
				++depth;
			}
		}
		vx = rx, vy = ry;
		distV = dist(x, y, vx, vy);

		// Distance Checks
		if (distH < distV) {
			distF = distH;
			rx = hx, ry = hy;
			glColor3f(0, 1, 0);
		}
		else {
			distF = distV;
			rx = vx, ry = vy;
			glColor3f(1, 0, 0);
		}



		/**
		* 
		* Projection
		* 
		*/
		// Fisheye fix
		float da = a - ra;
		if (da < 0) { da += 2 * PI; }
		if (da > 2 * PI) { da -= 2 * PI; }
		distF *= cos(da);

		float lineW = (float)WIDTH / (float)rays;
		float lineH = dim * HEIGHT / (distF * aspect_ratio);
		
		float lx = r * lineW;
		float ly = (HEIGHT / 2) - (lineH / 2);

		// Wall (untextured)
		/*
		glBegin(GL_QUADS);
		glVertex2f(lx, ly);
		glVertex2f(lx + lineW, ly);
		glVertex2f(lx + lineW, ly + lineH);
		glVertex2f(lx, ly + lineH);
		glEnd();
		*/

		/**
		* 
		* Textured walls
		* 
		*/
		mx = (int)rx / dim;
		my = (int)ry / dim;
		mi = my * map->width() + mx;
		int texInd = map->wall(mi) - 1;
		if (texInd < 0) { texInd == 0; }
		else if (texInd > map->tex.size()) { texInd = map->tex.size() - 1; }

		int texW = map->tex[texInd].w, texH = map->tex[texInd].h;
		int texX, texY;

		float darken;
		// Horizontal walls
		if (distH < distV) {
			rx = (int)rx % dim;
			darken = 0;

			// Move along the wall's texture proportionally to the pixels
			texX = (int)mapToTex(rx, 0, dim, 0, texW);

			// Flip the texX if we are facing down
			if (ra < PI) { texX = (texW - 1) - texX; }
		}
		else {
			ry = (int)ry % dim;
			darken = .1f;

			// Move along the wall's texture proportionally to the pixels
			texX = (int)mapToTex(ry, 0, dim, 0, texW);

			// Flip TexX if we are facing left
			if (ra > P2 && ra < P3) { texX = (texW - 1) - texX; }
		}

		// Find the height of each texture's pixel
		float pixY = lineH / texH;
		// Loop down a texture's column
		for (texY = 0; texY < texH; ++texY) {
			color c = map->tex[texInd].colorAt(texX, texY);
			
			glColor3f(c.r - darken, c.g - darken, c.b - darken);
			glBegin(GL_QUADS);
			glVertex2i(lx, ly + (texY * pixY));
			glVertex2i(lx + lineW, ly + (texY * pixY));
			glVertex2i(lx + lineW, ly + (texY * pixY) + pixY);
			glVertex2i(lx, ly + (texY * pixY) + pixY);
			glEnd();

		}


		// FLOORS
		glColor3f(.4, .25, .1);
		glBegin(GL_QUADS);
		glVertex2f(lx, ly + lineH);
		glVertex2f(lx + lineW, ly + lineH);
		glVertex2f(lx + lineW, HEIGHT);
		glVertex2f(lx, HEIGHT);
		glEnd();

		// CEILINGS
		glColor3f(1, .9, .75);
		glBegin(GL_QUADS);
		glVertex2f(lx, ly);
		glVertex2f(lx + lineW, ly);
		glVertex2f(lx + lineW, 0);
		glVertex2f(lx, 0);
		glEnd();
	}
}

void Player::move(int n) {
	switch (n) {
		// W
	case 0:
		x += dx;
		y += dy;
		break;

		// S
	case 1:
		x -= dx;
		y -= dy;
		break;

		// A
	case 2:
		x += dy;
		y -= dx;
		break;

		// D
	case 3:
		x -= dy;
		y += dx;
		break;

		// LEFT ARROW
	case 4:
		a -= rv;
		if (a < 0) { a += 2 * PI; }

		dx = cos(a) * mv;
		dy = sin(a) * mv;
		break;

		// RIGHT ARROW
	case 5:
		a += rv;
		if (a > 2 * PI) { a -= 2 * PI; }

		dx = cos(a) * mv;
		dy = sin(a) * mv;
		break;
	default:
		break;
	}
}

void Player::debug() {
	std::cout << "-----PLAYER-----\n";
	std::cout << "POS: (" << x << ", " << y << ")\n";
	std::cout << "ANGLE: " << a << "\n";
	std::cout << "MOV: " << mv << "\n";
	std::cout << "ROT: " << rv << "\n";
	std::cout << "----------------\n\n";
}

float Player::degToRad(float n) {
	return n * (PI / 180.f);
}

float Player::dist(float x0, float y0, float x1, float y1) {
	return sqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
}

float Player::mapToTex(float wallA, float wallS, float wallE, float texS, float texE) {
	double slope = 1.0 * (texE - texS) / (wallE - wallS);
	return texS + slope * (wallA - wallS);
}