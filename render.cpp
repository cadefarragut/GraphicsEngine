#include "render.h"

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

unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

Render::Render() : shader("shader.vs", "shader.fs") {}

void Render::Init() {
	//vao = VAO();
	vbo = VBO(vertices, sizeof(vertices));
	vao.Bind();
	vao.LinkVBO(vbo, 0, 3, 5, 0);
	vao.LinkVBO(vbo, 2, 2, 5, 3);
	vao.Unbind();


	shader = Shader("shader.vs", "shader.fs");

	Texture texture1("container.jpg", GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, GL_RGB);
	Texture texture2("awesomeface.png", GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR, GL_RGBA);
	// store textures in the member vector so Draw can access them later
	textures = { texture1, texture2 };

	shader.use();

	glEnable(GL_DEPTH_TEST);
}

void Render::BeginWorld(const glm::mat4& viewm, const glm::mat4& proj) {
	shader.use();

	vao.Bind();
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glm::mat4 projection = glm::mat4(1.0f);
	projection = proj;
	shader.setMatrix4("projection", projection);


	glm::mat4 view = viewm;
	shader.setMatrix4("view", view);
}

void Render::Draw(const Box& boxes) {
		if (textures.empty()) return; // nothing to draw if textures not initialized
		// choose texture based on box.textureId (default 0)
		int tid = boxes.textureId;
		if (tid < 0 || tid >= static_cast<int>(textures.size())) tid = 0;
		glBindTexture(GL_TEXTURE_2D, textures[tid].ID);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, boxes.center);
		model = glm::scale(model, boxes.halfExtents);
		shader.setMatrix4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
}