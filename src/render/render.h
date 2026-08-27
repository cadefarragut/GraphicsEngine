#pragma once

#include "VAO.h"
#include "VBO.h"
#include "shader.h"
#include "EBO.h"
#include <vector>
#include "shader.h"
#include "Texture.h"
#include <glm/glm.hpp>

// Simple box primitive used by the renderer (previously in game/level.h)
struct Box {
	glm::vec3 center;
	glm::vec3 halfExtents;
	int textureId = 0;
};

#include <string>
#include <vector>

class Render {
public:
	Render();
	void Init();
	void BeginWorld(const glm::mat4& viewm, const glm::mat4& proj, const glm::vec3& camPos);
	void Draw(const Box& boxes);
	// Load a level/map file and store boxes internally
	bool LoadBoxesFromFile(const std::string& path);
	// Draw all loaded boxes
	void DrawScene();
	// draw a distinct cube mesh for sky/sun
	void DrawSun(const glm::vec3& position, const glm::vec3& scale, int textureId = -1);

private:
	VAO vao;
	VBO vbo;
	EBO ebo;
	Shader shader;
	std::vector<Texture> textures;
	std::vector<Box> sceneBoxes;
	VAO sunVao;
	VBO sunVbo;
};
