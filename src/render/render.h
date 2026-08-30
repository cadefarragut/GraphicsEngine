#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "shader.h"
#include "Texture.h"

struct Box {
	glm::vec3 center;
	glm::vec3 halfExtents;
	int textureId = 0;
};

class Render {
public:
	Render();
	void Init();
	bool LoadBoxesFromFile(const std::string& path);

	void BeginWorld(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& camPos);
	void DrawScene();
	void DrawSky();
	void DrawSun();
	void Draw(const Box& box);

private:
	VAO vao;
	VBO vbo;
	EBO ebo;
	Shader shader;
	Shader flatShader;   // unlit, for the sky and light markers
	std::vector<Texture> textures;
	std::vector<Box> sceneBoxes;

	VAO skyVao;
	VBO skyVbo;
	VAO sunVao;
	VBO sunVbo;

	// set by BeginWorld, used by the Draw* calls
	glm::mat4 mView = glm::mat4(1.0f);
	glm::mat4 mProj = glm::mat4(1.0f);
	glm::vec3 mSunPos = glm::vec3(0.0f);
	glm::vec3 mSkyTop = glm::vec3(0.2f, 0.44f, 0.78f);
	glm::vec3 mSkyBottom = glm::vec3(0.8f, 0.87f, 0.95f);
};
