#include "render.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cmath>
#include <GLFW/glfw3.h>

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

unsigned int indices[] = {
	 0,  1,  2,   2,  3,  0,     // front
	 4,  5,  6,   6,  7,  4,     // back
	 8,  9, 10,  10, 11,  8,     // left
	12, 13, 14,  14, 15, 12,     // right
	16, 17, 18,  18, 19, 16,     // top
	20, 21, 22,  22, 23, 20,     // bottom
};

// seconds for a full dawn -> dusk -> dawn sweep
static const float kDayLength = 90.0f;

// sky gradient endpoints, mixed by the sun's height
static const glm::vec3 kDayTop  = glm::vec3(0.20f, 0.44f, 0.78f);
static const glm::vec3 kDayBot  = glm::vec3(0.80f, 0.87f, 0.95f);
static const glm::vec3 kDuskTop = glm::vec3(0.05f, 0.08f, 0.18f);
static const glm::vec3 kDuskBot = glm::vec3(0.85f, 0.40f, 0.22f);

struct Material { glm::vec3 tint; float shininess; float specStrength; };

static Material materialFor(int texId) {
	switch (texId) {
	case 0: return { glm::vec3(0.70f, 0.64f, 0.48f),  4.0f, 0.04f }; // sand
	case 1: return { glm::vec3(1.00f, 0.96f, 0.90f), 64.0f, 0.55f }; // travertine
	case 2: return { glm::vec3(0.95f, 0.95f, 0.97f),  8.0f, 0.12f }; // concrete
	case 3: return { glm::vec3(0.92f, 0.86f, 0.82f), 16.0f, 0.22f }; // bricks
	default: return { glm::vec3(1.0f),               16.0f, 0.15f };
	}
}

// A tiny, fixed height offset per box so no two faces are ever exactly
// coplanar - otherwise the stacked seating blocks z-fight. Big flat boxes
// (the arena floor) get a much smaller offset so it stays flush.
static float depthNudge(const glm::vec3& center, const glm::vec3& halfExtents) {
	float h = std::sin(center.x * 127.1f + center.y * 311.7f + center.z * 74.7f) * 43758.5453f;
	float unit = ((h - std::floor(h)) * 2.0f - 1.0f);
	float amount = (halfExtents.x > 8.0f || halfExtents.z > 8.0f) ? 0.0025f : 0.015f;
	return unit * amount;
}

// A cube with position + colour, using the shared index buffer. Top verts get
// topColor, bottom verts get bottomColor.
static std::vector<float> makeColorCube(const glm::vec3& topColor, const glm::vec3& bottomColor) {
	std::vector<float> out;
	for (int i = 0; i < 24; ++i) {
		float x = vertices[i * 8 + 0];
		float y = vertices[i * 8 + 1];
		float z = vertices[i * 8 + 2];
		const glm::vec3& c = (y > 0.0f) ? topColor : bottomColor;
		out.insert(out.end(), { x, y, z, c.r, c.g, c.b });
	}
	return out;
}

Render::Render()
	: shader("src/assets/shader.vs", "src/assets/shader.fs"),
	  flatShader("src/assets/flat.vs", "src/assets/flat.fs") {}

void Render::Init() {

	vbo = VBO(vertices, sizeof(vertices));
	vao.Bind();
	ebo = EBO(indices, sizeof(indices));
	vao.LinkVBO(vbo, 0, 3, 8, 0);
	vao.LinkVBO(vbo, 1, 2, 8, 3);
	vao.LinkVBO(vbo, 2, 3, 8, 5);
	vao.Unbind();

	std::vector<float> skyData = makeColorCube(kDayTop, kDayBot);
	skyVbo = VBO(skyData.data(), skyData.size() * sizeof(float));
	skyVao.Bind();
	ebo.Bind();
	skyVao.LinkVBO(skyVbo, 0, 3, 6, 0);
	skyVao.LinkVBO(skyVbo, 1, 3, 6, 3);
	skyVao.Unbind();

	std::vector<float> sunData = makeColorCube(glm::vec3(1.0f, 0.93f, 0.75f), glm::vec3(1.0f, 0.93f, 0.75f));
	sunVbo = VBO(sunData.data(), sunData.size() * sizeof(float));
	sunVao.Bind();
	ebo.Bind();
	sunVao.LinkVBO(sunVbo, 0, 3, 6, 0);
	sunVao.LinkVBO(sunVbo, 1, 3, 6, 3);
	sunVao.Unbind();

	Texture texture1("src/assets/sand.jpg", GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, GL_RGBA);
	Texture texture2("src/assets/travertine.jpg", GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, GL_RGBA);
	Texture texture3("src/assets/concrete.jpg", GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, GL_RGBA);
	Texture texture4("src/assets/bricks.jpg", GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, GL_RGBA);
	textures = { texture1, texture2, texture3, texture4 };

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	shader.use();
	shader.setInt("texture1", 0);
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
	shader.use();
	for (const Box& b : sceneBoxes) {
		Draw(b);
	}
}

void Render::BeginWorld(const glm::mat4& viewm, const glm::mat4& proj, const glm::vec3& camPos) {
	mView = viewm;
	mProj = proj;

	shader.use();

	// sweep the sun across the sky, dawn -> noon -> dusk, then loop
	float dayT = std::fmod(static_cast<float>(glfwGetTime()) / kDayLength, 1.0f);
	float ang  = glm::radians(10.0f + dayT * 160.0f);
	mSunPos = glm::normalize(glm::vec3(std::cos(ang), std::sin(ang), 0.32f)) * 160.0f;
	float elev = glm::clamp(std::sin(ang), 0.0f, 1.0f);   // 0 at the horizon, 1 overhead

	glm::vec3 sunColor = glm::mix(glm::vec3(1.15f, 0.42f, 0.16f),
	                              glm::vec3(1.00f, 0.95f, 0.85f), elev);

	shader.setVec3("sun.direction", glm::normalize(-mSunPos));
	shader.setVec3("sun.ambient", glm::vec3(0.05f) + glm::vec3(0.12f) * elev);
	shader.setVec3("sun.diffuse", sunColor * (0.16f + 0.66f * elev));
	shader.setVec3("sun.specular", glm::vec3(0.10f + 0.30f * elev));

	// cool fill light from above so shadowed faces don't go black
	float fill = 0.25f + 0.55f * elev;
	shader.setVec3("skyFill.direction", glm::vec3(0.30f, -0.90f, 0.20f));
	shader.setVec3("skyFill.ambient", glm::vec3(0.14f, 0.16f, 0.21f) * fill);
	shader.setVec3("skyFill.diffuse", glm::vec3(0.22f, 0.27f, 0.38f) * fill);
	shader.setVec3("skyFill.specular", glm::vec3(0.0f));

	mSkyTop    = glm::mix(kDuskTop, kDayTop, elev);
	mSkyBottom = glm::mix(kDuskBot, kDayBot, elev);

	shader.setVec3("viewPos", camPos);

	vao.Bind();
	glClearColor(mSkyBottom.r, mSkyBottom.g, mSkyBottom.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.setMatrix4("projection", proj);
	shader.setMatrix4("view", viewm);
}

void Render::DrawSky() {
	// update the gradient for the current time of day
	std::vector<float> skyData = makeColorCube(mSkyTop, mSkyBottom);
	glBindBuffer(GL_ARRAY_BUFFER, skyVbo.ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, skyData.size() * sizeof(float), skyData.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// cube centred on the camera so you can never reach its edge
	glm::vec3 camPos = glm::vec3(glm::inverse(mView)[3]);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), camPos);
	model = glm::scale(model, glm::vec3(10.0f));

	flatShader.use();
	flatShader.setMatrix4("projection", mProj);
	flatShader.setMatrix4("view", mView);
	flatShader.setMatrix4("model", model);

	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	skyVao.Bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	skyVao.Unbind();
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
}

void Render::Draw(const Box& box) {
	if (textures.empty()) return;

	int tid = box.textureId;
	if (tid < 0 || tid >= static_cast<int>(textures.size())) tid = 0;

	glBindTexture(GL_TEXTURE_2D, textures[tid].ID);

	Material mat = materialFor(tid);
	shader.setVec3("material.tint", mat.tint);
	shader.setFloat("material.shininess", mat.shininess);
	shader.setFloat("material.specStrength", mat.specStrength);
	shader.setVec2("uvscale", glm::vec2(box.halfExtents.x, box.halfExtents.z) * 0.28f);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), box.center + glm::vec3(0.0f, depthNudge(box.center, box.halfExtents), 0.0f));
	model = glm::scale(model, box.halfExtents);
	shader.setMatrix4("model", model);
	shader.setMatrix3("normalMatrix", glm::mat3(glm::transpose(glm::inverse(model))));

	vao.Bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	vao.Unbind();
}

void Render::DrawSun() {
	flatShader.use();
	flatShader.setMatrix4("projection", mProj);
	flatShader.setMatrix4("view", mView);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), mSunPos);
	model = glm::scale(model, glm::vec3(12.0f));
	flatShader.setMatrix4("model", model);

	sunVao.Bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	sunVao.Unbind();
}
