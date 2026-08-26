#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

Texture::Texture(const char* texture, GLenum texture_type, GLenum outside_coord_act, GLenum near_or_lin, GLenum RGB) {
	glGenTextures(1, &ID);
	glBindTexture(texture_type, ID);

	glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, outside_coord_act);
	glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, outside_coord_act);
	glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, near_or_lin);
	glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, near_or_lin);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(texture, &width, &height, &nrChannels, 4);

	if (data) {
		glTexImage2D(texture_type, 0, RGB, width, height, 0, RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(texture_type);
	}
	else {
		std::cout << "Failed to load texture.\n";
	}

	stbi_image_free(data);

}


void Texture::LinkActiveTexture(GLenum texture_layer, GLenum texture_type) {
	glActiveTexture(texture_layer);
	glBindTexture(texture_type, ID);
}

void Texture::Bind() {
	glBindTexture(GL_TEXTURE_2D, ID);
}