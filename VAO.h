#ifndef VAO_H
#define VAO_H

#include "VBO.h"

class VAO {
public:
	GLuint ID;

	VAO();
	void LinkVBO(VBO VBO, GLuint layout, GLint vdata, GLint stride, GLuint offset);
	void Bind();
	void Unbind();
	void Delete();
};

#endif

