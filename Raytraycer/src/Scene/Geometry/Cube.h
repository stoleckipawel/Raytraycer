#pragma once
#include "Primitive.h"
class Box : public Primitive
{
public:
	virtual void BuildUI(uint32_t id) override;
	virtual Trace Intersect(const Ray& ray) const override;
	virtual Trace ResolveTracePayload(Trace& payload, const Ray& ray) const override;
	glm::vec3 GetExtentMin() const;
	glm::vec3 GetExtentMax() const;
};

