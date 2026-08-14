#include "window.h"


int main() {

	Window display("window", 1920, 1080);

	GLFWwindow* window = display.createWindow();

	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate(); 
	return 0;
}
