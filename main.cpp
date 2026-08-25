#include "window.h"
#include "Config.h"
#include "shader.h"
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"
#include "camera.h"
#include "level.h"
#include "render.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = static_cast<float>(SCREEN_WIDTH) / 2.0f;
float lastY = static_cast<float>(SCREEN_HEIGHT) / 2.0f;
bool firstMouse = true;

void render();
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xpos, double ypos);



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

	Render render;

	render.Init();

	Level map1;
	map1.LoadFromFile("map.txt");

	while (!glfwWindowShouldClose(display.window)) {

		// input
		processInput(display.window);
		// rendering 
		render.BeginWorld(camera.GetViewMatrix(), camera.GetProjectionMatrix());

		float currentframe = glfwGetTime();
		deltaTime = currentframe - lastFrame;
		lastFrame = currentframe;
		
		//std::cout << camera.Position.x << " " << camera.Position.z << std::endl;
		for (const Box& x : map1.boxes) {
			render.Draw(x);
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