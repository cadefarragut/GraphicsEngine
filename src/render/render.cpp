#include "render.h"
#include <fstream>
#include <sstream>
#include <iostream>

float vertices[] = {
	// position            uv       normal
	// front (+z)
	-0.5f,-0.5f, 0.5f,   0.0f,0.0f,   0.0f, 0.0f, 1.0f,
	 0.5f,-0.5f, 0.5f,   1.0f,0.0f,   0.0f, 0.0f, 1.0f,
	 0.5f, 0.5f, 0.5f,   1.0f,1.0f,   0.0f, 0.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,   0.0f,1.0f,   0.0f, 0.0f, 1.0f,

	// back (-z)
	 0.5f,-0.5f,-0.5f,   0.0f,0.0f,   0.0f, 0.0f,-1.0f,
	-0.5f,-0.5f,-0.5f,   1.0f,0.0f,   0.0f, 0.0f,-1.0f,
	-0.5f, 0.5f,-0.5f,   1.0f,1.0f,   0.0f, 0.0f,-1.0f,
	 0.5f, 0.5f,-0.5f,   0.0f,1.0f,   0.0f, 0.0f,-1.0f,

	 // left (-x)
	 -0.5f,-0.5f,-0.5f,   0.0f,0.0f,  -1.0f, 0.0f, 0.0f,
	 -0.5f,-0.5f, 0.5f,   1.0f,0.0f,  -1.0f, 0.0f, 0.0f,
	 -0.5f, 0.5f, 0.5f,   1.0f,1.0f,  -1.0f, 0.0f, 0.0f,
	 -0.5f, 0.5f,-0.5f,   0.0f,1.0f,  -1.0f, 0.0f, 0.0f,

	 // right (+x)
	  0.5f,-0.5f, 0.5f,   0.0f,0.0f,   1.0f, 0.0f, 0.0f,
	  0.5f,-0.5f,-0.5f,   1.0f,0.0f,   1.0f, 0.0f, 0.0f,
	  0.5f, 0.5f,-0.5f,   1.0f,1.0f,   1.0f, 0.0f, 0.0f,
	  0.5f, 0.5f, 0.5f,   0.0f,1.0f,   1.0f, 0.0f, 0.0f,

	  // top (+y)
	  -0.5f, 0.5f, 0.5f,   0.0f,0.0f,   0.0f, 1.0f, 0.0f,
	   0.5f, 0.5f, 0.5f,   1.0f,0.0f,   0.0f, 1.0f, 0.0f,
	   0.5f, 0.5f,-0.5f,   1.0f,1.0f,   0.0f, 1.0f, 0.0f,
	  -0.5f, 0.5f,-0.5f,   0.0f,1.0f,   0.0f, 1.0f, 0.0f,

	  // bottom (-y)
	  -0.5f,-0.5f,-0.5f,   0.0f,0.0f,   0.0f,-1.0f, 0.0f,
	   0.5f,-0.5f,-0.5f,   1.0f,0.0f,   0.0f,-1.0f, 0.0f,
	   0.5f,-0.5f, 0.5f,   1.0f,1.0f,   0.0f,-1.0f, 0.0f,
	  -0.5f,-0.5f, 0.5f,   0.0f,1.0f,   0.0f,-1.0f, 0.0f,
};

float sunVertices[] = {
	-0.5f, -0.5f, -0.5f, 
	 0.5f, -0.5f, -0.5f, 
	 0.5f,  0.5f, -0.5f,  
	 0.5f,  0.5f, -0.5f, 
	-0.5f,  0.5f, -0.5f, 
	-0.5f, -0.5f, -0.5f,  

	-0.5f, -0.5f,  0.5f,  
	 0.5f, -0.5f,  0.5f, 
	 0.5f,  0.5f,  0.5f,  
	 0.5f,  0.5f,  0.5f, 
	-0.5f,  0.5f,  0.5f, 
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,  
	-0.5f,  0.5f, -0.5f,  
	-0.5f, -0.5f, -0.5f, 
	-0.5f, -0.5f, -0.5f, 
	-0.5f, -0.5f,  0.5f,  
	-0.5f,  0.5f,  0.5f, 

	 0.5f,  0.5f,  0.5f,  
	 0.5f,  0.5f, -0.5f,  
	 0.5f, -0.5f, -0.5f, 
	 0.5f, -0.5f, -0.5f, 
	 0.5f, -0.5f,  0.5f,  
	 0.5f,  0.5f,  0.5f,  

	-0.5f, -0.5f, -0.5f, 
	 0.5f, -0.5f, -0.5f,  
	 0.5f, -0.5f,  0.5f, 
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
};

unsigned int indices[] = {
	 0,  1,  2,   2,  3,  0,     // front
	 4,  5,  6,   6,  7,  4,     // back
	 8,  9, 10,  10, 11,  8,     // left
	12, 13, 14,  14, 15, 12,     // right
	16, 17, 18,  18, 19, 16,     // top
	20, 21, 22,  22, 23, 20,     // bottom
};
Render::Render() : shader("src/assets/shader.vs", "src/assets/shader.fs") {}

void Render::Init() {
	
	vbo = VBO(vertices, sizeof(vertices));
	vao.Bind();
	ebo = EBO(indices, sizeof(indices));
	vao.LinkVBO(vbo, 0, 3, 8, 0);
	vao.LinkVBO(vbo, 1, 2, 8, 3);
	vao.LinkVBO(vbo, 2, 3, 8, 5);
	vao.Unbind();

	Texture texture1("src/assets/sand.jpg", GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, GL_RGBA);
	Texture texture2("src/assets/travertine.jpg", GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, GL_RGBA);
	Texture texture3("src/assets/concrete.jpg", GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, GL_RGBA);
	Texture texture4("src/assets/bricks.jpg", GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, GL_RGBA);
	
	textures = { texture1, texture2, texture3, texture4 };

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);


	shader.use();
	shader.setInt("texture1", 0);
	glEnable(GL_DEPTH_TEST);
}

bool Render::LoadBoxesFromFile(const std::string& path) {
	std::ifstream in(path);
	if (!in) {
		std::cout << "Failed to access: " << path << std::endl;
		return false;
	}
	sceneBoxes.clear();
	std::string line;
	while (std::getline(in, line)) {
		if (line.empty() || line[0] == '#') continue;

		std::istringstream ss(line);
		glm::vec3 c, size;
		int text;
		if (ss >> c.x >> c.y >> c.z >> size.x >> size.y >> size.z >> text) {
			sceneBoxes.push_back({ c, size, text });
		}
	}
	return !sceneBoxes.empty();
}

void Render::DrawScene() {
	for (const Box& b : sceneBoxes) {
		Draw(b);
	}
}

void Render::BeginWorld(const glm::mat4& viewm, const glm::mat4& proj, const glm::vec3& camPos) {
	shader.use();

	glm::vec3 skyColor = glm::vec3(0.45f, 0.58f, 0.75f);

	glm::vec3 sunDir = glm::normalize(glm::vec3(-0.55f, -0.42f, -0.25f));
	shader.setVec3("sunDir", sunDir);
	shader.setVec3("sunColor", glm::vec3(1.15f, 1.02f, 0.82f));
	shader.setVec3("skyColor", glm::vec3(0.45f, 0.58f, 0.75f));

	vao.Bind();
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glm::mat4 projection = glm::mat4(1.0f);
	projection = proj;
	shader.setMatrix4("projection", projection);

	glm::mat4 view = glm::mat4(1.0f);
	view = viewm;
	shader.setMatrix4("view", view);
}

void Render::Draw(const Box& boxes) {
		if (textures.empty()) return; // nothing to draw if textures not initialized
		shader.setVec2("uvscale", glm::vec2(boxes.halfExtents.x, boxes.halfExtents.z) * 0.5f);
		int tid = boxes.textureId;
		if (tid < 0 || tid >= static_cast<int>(textures.size())) tid = 0;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[tid].ID);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, boxes.center);
		model = glm::scale(model, boxes.halfExtents);
		shader.setMatrix4("model", model);
		glm::mat3 nrm = glm::mat3(glm::transpose(glm::inverse(model)));
		shader.setMatrix3("normalMatrix", nrm);
		// draw using the main cube VAO
		vao.Bind();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		vao.Unbind();

}



void Render::DrawSun(const glm::vec3& position, const glm::vec3& scale, int textureId) {
    if (textures.empty()) return;
    shader.setVec2("uvscale", glm::vec2(1.0f, 1.0f));
    int tid = textureId;
    if (tid < 0 || tid >= static_cast<int>(textures.size())) tid = 0;
    glBindTexture(GL_TEXTURE_2D, textures[tid].ID);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    shader.setMatrix4("model", model);
	
	// draw sun using the same cube VAO (same vertex format)
	vao.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	vao.Unbind();
}