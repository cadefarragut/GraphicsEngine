#include "window.h"


Window::Window(const char* name, int width, int height) {
	this->name = name;
	this->width = width;
	this->height = height;
	this->window = nullptr;

}

void Window::createWindow() {

	this->window = glfwCreateWindow(width, height, name, NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

}

void Window::changeWindowSize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void Window::initializeGlad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	glViewport(0, 0, width, height);
}

void Window::processInput() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
