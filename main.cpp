#include "window.h"
#include "shader.h"
#include "VAO.h"
#include "EBO.h"


float vertices[] = {
	 // positions			// colors
	 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, // Bottom Right
	-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, // Bottom Left
	 0.0f,  0.5f, 0.0,		0.0f, 0.0f, 1.0f  // Top
};
//unsigned int indices[] = {
//	0, 1, 3,	// First Triangle
//	1, 2, 3		// Second Triangle
//};

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Window display("window", SCREEN_WIDTH, SCREEN_HEIGHT);

	display.createWindow();

	glfwSetFramebufferSizeCallback(display.window, display.changeWindowSize);

	display.initializeGlad();

	VAO VAO1;
	VAO1.Bind();
	VBO VBO1(vertices, sizeof(vertices));
	VAO1.LinkVBO(VBO1, 0, 3, 6, 0);
	VAO1.LinkVBO(VBO1, 1, 3, 6, 3);

	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);


	Shader myShader("shader.vs", "shader.fs");

	myShader.use();
	myShader.setFloat("offset", 0.5f);

	while (!glfwWindowShouldClose(display.window)) {
		// input
		display.processInput();
		// rendering 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	
		// check and call events and swap buffers.
		glfwSwapBuffers(display.window);
		glfwPollEvents();
	}

	glfwTerminate(); 
	return 0;
}
