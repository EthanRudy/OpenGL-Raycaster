#include <GLFW/glfw3.h>
#include <iostream>
#include "../include/map.h"
#include "../include/player.h"
#include "../include/texture.h"

// Display Settings
const unsigned WIDTH = 1920;
const unsigned HEIGHT = 1080;
const float FPS_LIMIT = 1.f / 60.f;

bool ACTIVE_KEYS[512];

void clearKeys();
void handle_input(GLFWwindow* window, int, int, int, int);

int main() {
	Map map("map1");
	map.debug();
    Player player;

    // Set player spawn location
    int sX = map.sX(), sY = map.sY(), sD = map.dim();
    float sA = map.sA();
    player.setSpawn(sX, sY, sA, sD);
    player.debug();

    // Set keys
    clearKeys();

	GLFWwindow* window;
	if (!glfwInit()) { return -1; }

	window = glfwCreateWindow(WIDTH, HEIGHT, "Maze Game", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glOrtho(0, WIDTH, HEIGHT, 0, 0, 1);
    glfwSetKeyCallback(window, handle_input);

    double lastUpdateTime = 0, lastFrameTime = 0;
    double now, deltaTime;
    while (!glfwWindowShouldClose(window)) {
        // deltaTime
        now = glfwGetTime();
        deltaTime = now - lastUpdateTime;

        // Poll inputs
        glfwPollEvents();

        // Framerate locker
        // ANYTHING LOCKED TO FPS GOES INSIDE
        if ((now - lastFrameTime) >= FPS_LIMIT) {
            // Clear screen
            glClear(GL_COLOR_BUFFER_BIT);

            /**
            * 
            * Player Movement
            * 
            */
            if (ACTIVE_KEYS[GLFW_KEY_W]) {  // Move forward
                player.move(0);
            }
            if (ACTIVE_KEYS[GLFW_KEY_S]) {  // Move backward
                player.move(1);
            }
            if (ACTIVE_KEYS[GLFW_KEY_A]) {  // Strafe Left
                player.move(2);
            }
            if (ACTIVE_KEYS[GLFW_KEY_D]) {  // Strafe Right
                player.move(3);
            }
            if (ACTIVE_KEYS[GLFW_KEY_LEFT]) {   // Turn Left
                player.move(4);
            }
            if (ACTIVE_KEYS[GLFW_KEY_RIGHT]) {  // Turn Right
                player.move(5);
            }

            // Render 2D
            //map.render2D();
            //player.render2D();
            // Render 3D
            player.render3D(&map, WIDTH, HEIGHT);

            // Swap buffers
            glfwSwapBuffers(window);

            lastFrameTime = now;
        }
        lastUpdateTime = now;

        // ANYTHING UNLOCKED TO FPS GOES HERE
    }

    glfwTerminate();
    return 0;
}

void clearKeys() {
    for (int i = 0; i < 512; ++i) {
        ACTIVE_KEYS[i] = 0;
    }
}

void handle_input(GLFWwindow* window, int, int, int, int) {
    // W
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        ACTIVE_KEYS[GLFW_KEY_W] = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
        ACTIVE_KEYS[GLFW_KEY_W] = false;
    }

    // S
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        ACTIVE_KEYS[GLFW_KEY_S] = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
        ACTIVE_KEYS[GLFW_KEY_S] = false;
    }

    // A
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        ACTIVE_KEYS[GLFW_KEY_A] = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
        ACTIVE_KEYS[GLFW_KEY_A] = false;
    }

    // D
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        ACTIVE_KEYS[GLFW_KEY_D] = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
        ACTIVE_KEYS[GLFW_KEY_D] = false;
    }

    // RIGHT ARROW
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        ACTIVE_KEYS[GLFW_KEY_RIGHT] = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE) {
        ACTIVE_KEYS[GLFW_KEY_RIGHT] = false;
    }

    // LEFT ARROW
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        ACTIVE_KEYS[GLFW_KEY_LEFT] = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE) {
        ACTIVE_KEYS[GLFW_KEY_LEFT] = false;
    }
}