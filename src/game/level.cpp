#include "level.h"

bool Level::LoadFromFile(const std::string& path) {
	std::ifstream in(path);
	if (!in) {
		std::cout << "Failed to access: " << path << std::endl;
		return false;
	}
	boxes.clear();
	std::string line;
	while (std::getline(in, line)) {
		if (line.empty() || line[0] == '#') continue;
		
		std::istringstream ss(line);
		glm::vec3 c, size;
		int text;
		if (ss >> c.x >> c.y >> c.z >> size.x >> size.y >> size.z >> text) {
			boxes.push_back({ c, size, text });
		}
	}
	return boxes.empty();
}