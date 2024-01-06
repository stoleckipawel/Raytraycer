#pragma once
#include "glm/glm.hpp"
#include "Material/Material.h"
#include "../../Utils/Trace.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "../Ray.h"


class Primitive
{
public:
	virtual void BuildUI(uint32_t id) {};
	virtual Trace Intersect(const Ray& ray) const { return Trace(); };//intentionally not pure
	virtual Trace ResolveTracePayload(Trace& payload, const Ray& ray) const { return Trace(); };//intentionally not pure
public:
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	Material* Material = nullptr;
	int Guid = 0;
};
