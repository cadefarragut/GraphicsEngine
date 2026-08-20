#include "window.h"
#include "shader.h"
#include "VAO.h"
#include "EBO.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"


float vertices[] = {
	 // positions			// colors			 // texture coords	
	 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	 1.0f, 1.0f,  // Top Right
	 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 	 1.0f, 0.0f,  // Bottom Right
	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	 0.0f, 0.0f,  // Bottom Left
	-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,    0.0f, 1.0f   // Top Left
};
//unsigned int indices[] = {
// Uncommented indices for EBO
unsigned int indices[] = {
	0, 1, 3, // First Triangle
	1, 2, 3  // Second Triangle
};


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
	VAO1.LinkVBO(VBO1, 0, 3, 8, 0);
	VAO1.LinkVBO(VBO1, 1, 3, 8, 3);
	VAO1.LinkVBO(VBO1, 2, 2, 8, 6);
	EBO EBO1(indices, sizeof(indices));
	VAO1.LinkEBO(EBO1);

	VAO1.Unbind();



	Shader myShader("shader.vs", "shader.fs");

	myShader.use();
	myShader.setFloat("offset", 0.5f);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture.\n";
	}

	stbi_image_free(data);

	while (!glfwWindowShouldClose(display.window)) {
		VAO1.Bind();
		// input
		display.processInput();
		// rendering 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		// check and call events and swap buffers.
		glfwSwapBuffers(display.window);
		glfwPollEvents();
	}

	glfwTerminate(); 
	return 0;
}
