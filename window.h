#ifndef WINDOW_H
#define WINDOW_H


#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Window {
public:

	int height;
	int width;
	const char* name;

	Window(const char* name, int width, int height);

	GLFWwindow* createWindow();
	void changeWindowSize(GLFWwindow* window, int width, int height);

};

#endif