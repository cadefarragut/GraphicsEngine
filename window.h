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
	GLFWwindow* window;

	Window(const char* name, int width, int height);

	void createWindow();
	static void changeWindowSize(GLFWwindow* window, int width, int height);
	void initializeGlad();
	void processInput();

};

#endif