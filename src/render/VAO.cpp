#include "VAO.h"


VAO::VAO() {
	glGenVertexArrays(1, &ID);
}
void VAO::LinkVBO(VBO VBO, GLuint layout, GLint vdata, GLint stride, GLuint offset) {
	VBO.Bind();
	glVertexAttribPointer(layout, vdata, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::LinkEBO(EBO EBO) {
	EBO.Bind();
}

void VAO::Bind() {
	glBindVertexArray(ID);
}

void VAO::Unbind() {
	glBindVertexArray(0);
}

void VAO::Delete() {
	glDeleteVertexArrays(1, &ID);
}