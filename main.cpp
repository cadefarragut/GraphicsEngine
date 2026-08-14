#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window.h"


int main() {

	Window display("window", 1920, 1080);
	GLFWwindow* window = display.createWindow();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout<< "Failed to initialize GLAD" << std::endl;
		return 1;
	}

	glViewport(0, 0, 1920, 1080);

	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate(); 
	return 0;
}
