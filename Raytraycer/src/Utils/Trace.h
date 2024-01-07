#pragma once
#include "glm/glm.hpp"

enum class TraceResult : uint8_t { Miss, Hit };

class Trace
{
public:
	float HitDistance = std::numeric_limits<float>::max();
	glm::vec3 WorldNormal = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 WorldPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 LocalPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	int HitGuid = -1;
	TraceResult Result = TraceResult::Miss;
public:
	Trace& Miss();
	Trace& Hit(float hitDistance, int primitiveId);
private:
};