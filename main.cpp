#include "window.h"
#include "shader.h"
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"




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

	Texture texture1("container.jpg", GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, GL_RGB);
	Texture texture2("awesomeface.png", GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, GL_RGBA);

	texture1.LinkActiveTexture(GL_TEXTURE0, GL_TEXTURE_2D);
	texture2.LinkActiveTexture(GL_TEXTURE1, GL_TEXTURE_2D);
	
	myShader.setInt("texture1", 0);
	myShader.setInt("texture2", 1);
	
	//glm::mat4 trans = glm::mat4(1.0f);
	//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	
	


	while (!glfwWindowShouldClose(display.window)) {
		VAO1.Bind();
		// input
		display.processInput();
		// rendering 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		myShader.setMatrix4("transform", trans);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// check and call events and swap buffers.
		glfwSwapBuffers(display.window);
		glfwPollEvents();
	}

	glfwTerminate(); 
	return 0;
}
