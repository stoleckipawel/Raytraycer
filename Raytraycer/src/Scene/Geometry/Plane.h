#include "Primitive.h"
#pragma once
class Plane : public Primitive
{
public:
	Plane();
	virtual void BuildUI(uint32_t id) override;
	virtual Trace Intersect(const Ray& ray) const override;
	virtual Trace ResolveTracePayload(Trace& payload, const Ray& ray) const override;
};

