#ifndef VAO_H
#define VAO_H

#include "VBO.h"
#include "EBO.h"

class VAO {
public:
	GLuint ID;

	VAO();
	void LinkVBO(VBO& VBO, GLuint layout, GLint vdata, GLint stride, GLuint offset);
	void LinkEBO(EBO EBO);
	void Bind();
	void Unbind();
	void Delete();
};

#endif

