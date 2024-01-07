#pragma once
#include "glm/glm.hpp"

class Transform
{
public:
	Transform();

	glm::mat4 GetMatrix();
	glm::mat4 GetInverseMatrix();

	glm::vec3 scale;
	glm::vec3 position;
	glm::vec3 rotation;
private:

};

