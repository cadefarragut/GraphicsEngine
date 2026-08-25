#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <fstream>
#include<iostream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


struct Box {
	glm::vec3 center;
	glm::vec3 halfExtents;
	int textureId = 0;
};

struct Level {
	std::vector<Box> boxes;
	bool LoadFromFile(const std::string& path);
};

//bool LoadFromFile(const std::string& path);
#endif 

