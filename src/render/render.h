#pragma once

#include "VAO.h"
#include "VBO.h"
#include "shader.h"
#include "EBO.h"
#include <vector>
#include "shader.h"
#include "Texture.h"
#include "../game/level.h"

class Render {
public:
	Render();
	void Init();
	void BeginWorld(const glm::mat4& view, const glm::mat4& proj);
	void Draw(const Box& boxes);

private:
	VAO vao;
	VBO vbo;
	EBO ebo;
	Shader shader;
	std::vector<Texture> textures;
};
