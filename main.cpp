#include "window.h"

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Window display("window", 1920, 1080);

	display.createWindow();

	glfwSetFramebufferSizeCallback(display.window, display.changeWindowSize);

	display.initializeGlad();

	while (!glfwWindowShouldClose(display.window)) {
		// input
		display.processInput();
		// rendering 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// check and call events and swap buffers.
		glfwSwapBuffers(display.window);
		glfwPollEvents();
	}

	glfwTerminate(); 
	return 0;
}
