#include "window.h"
#include "shader.h"
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"
#include "camera.h"
#include "level.h"

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

#define SCREEN_WIDTH 1920.0f
#define SCREEN_HEIGHT 1080.0f


float deltaTime = 0.0f;
float lastFrame = 0.0f;

float fov = 45.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xpos, double ypos);

Box floorBox = {
	.center = glm::vec3(0.0f, -1.0f, 0.0f),
	.halfExtents = glm::vec3(40.0f, 0.0f, 40.0f)
};
Box wall1 = {
	.center = glm::vec3(40.0f, -1.0f, 40.0f),
	.halfExtents = glm::vec3(40.0f, 40.0f, 0.0f)
};

std::vector<Box> boxes = {floorBox, wall1};

float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};


glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
};


unsigned int indices[] = {
	0, 1, 3, 
	1, 2, 3  
};


int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Window display("window", SCREEN_WIDTH, SCREEN_HEIGHT);

	display.createWindow();

	glfwSetFramebufferSizeCallback(display.window, display.changeWindowSize);
	glfwSetCursorPosCallback(display.window, mouse_callback);
	glfwSetScrollCallback(display.window, scroll_callback);

	glfwSetInputMode(display.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	display.initializeGlad();

	VAO VAO1;
	VAO1.Bind();
	VBO VBO1(vertices, sizeof(vertices));
	VAO1.LinkVBO(VBO1, 0, 3, 5, 0);
	VAO1.LinkVBO(VBO1, 2, 2, 5, 3);
	VAO1.Unbind();

	Shader myShader("shader.vs", "shader.fs");

	
	Texture texture1("container.jpg", GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, GL_RGB);
	Texture texture2("awesomeface.png", GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, GL_RGBA);
	myShader.use();
	myShader.setInt("texture1", 0);
	myShader.setInt("texture2", 1);
		
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(display.window)) {
		myShader.use();

		texture1.LinkActiveTexture(GL_TEXTURE0, GL_TEXTURE_2D);
		texture2.LinkActiveTexture(GL_TEXTURE1, GL_TEXTURE_2D);

		VAO1.Bind();
		// input
		processInput(display.window);
		// rendering 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float currentframe = glfwGetTime();
		deltaTime = currentframe - lastFrame;
		lastFrame = currentframe;

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.Zoom), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
		myShader.setMatrix4("projection", projection);


		glm::mat4 view = camera.GetViewMatrix();
		myShader.setMatrix4("view", view);
		
		for (int i = 0; i < boxes.size(); i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, boxes[i].center);
			model = glm::scale(model, boxes[i].halfExtents);
			myShader.setMatrix4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		camera.velocityY -= GRAVITY * deltaTime;
		camera.Position.y += camera.velocityY * deltaTime;


		
		if (camera.Position.y <= 0) {
			camera.Position.y = 0;
			camera.velocityY = 0;
			camera.onGround = true;
		}
		else {
			camera.onGround = false;
		}

		std::cout << camera.Position.y << std::endl;

		// check and call events and swap buffers.
		glfwSwapBuffers(display.window);
		glfwPollEvents();
	}

	glfwTerminate(); 
	return 0;
}



void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	//float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && camera.onGround == true) {
		camera.velocityY = 7.0f;
		camera.onGround = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}