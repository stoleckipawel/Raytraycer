#pragma once
#include "Primitive.h"

class Sphere : public Primitive
{
public:
	float Radius = 0.5f;
public:
	virtual void BuildUI(uint32_t id) override;
	virtual Trace TraceRay(const Ray& ray) const override;
	virtual Trace ResolveTrace(Trace& payload, const Ray& ray) const override;
};