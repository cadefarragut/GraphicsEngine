#include "core/window.h"
#include "Config.h"
#include "render/shader.h"
#include "render/VAO.h"
#include "render/EBO.h"
#include "render/Texture.h"
#include "game/camera.h"
#include "game/level.h"
#include "render/render.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

	display.createWindow();
	glfwSetWindowUserPointer(display.window, &camera);
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
		
		for (const Box& x : map1.boxes) {
			render.Draw(x);
		}
		
		//camera.velocityY -= GRAVITY * deltaTime;
		//camera.Position.y += camera.velocityY * deltaTime;


		
		//if (camera.Position.y <= 0) {
		//	camera.Position.y = 0;
		//	camera.velocityY = 0;
		//	camera.onGround = true;
		//}
		//else {
		//	camera.onGround = false;
		//}

		// check and call events and swap buffers.
		glfwSwapBuffers(display.window);
		glfwPollEvents();
	}

	glfwTerminate(); 
	return 0;
}



void processInput(GLFWwindow* window) {
	Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	if (!cam) return;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam->ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam->ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam->ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam->ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && cam->onGround == true) {
		cam->velocityY = 7.0f;
		cam->onGround = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	if (!cam) return;
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	cam->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	if (!cam) return;
	cam->ProcessMouseScroll(static_cast<float>(yoffset));
}