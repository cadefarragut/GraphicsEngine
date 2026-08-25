#ifndef TEXTURE_H
#define TEXTURE_H


#include <iostream>
#include "glad/glad.h"

class Texture {
public:
	GLuint ID;

	Texture(const char* texture, GLenum texture_type, GLenum outside_coord_act, GLenum near_or_lin, GLenum RGB);
	void LinkActiveTexture(GLenum texture_layer, GLenum texture_type);
	void Bind();
	void Unbind();
};

#endif
