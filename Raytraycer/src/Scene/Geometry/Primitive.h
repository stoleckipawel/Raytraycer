#pragma once
#include "glm/glm.hpp"
#include "Material/Material.h"
#include "../../Utils/Trace.h"
#include "../Ray.h"


class Primitive
{
public:
	virtual void BuildUI(uint32_t id) {};
	virtual Trace TraceRay(const Ray& ray) const { return Trace(); };//intentionally not pure
	virtual Trace ResolveTrace(Trace& payload, const Ray& ray) const { return Trace(); };//intentionally not pure
public:
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	Material* Material = nullptr;
	int Guid = 0;
};
