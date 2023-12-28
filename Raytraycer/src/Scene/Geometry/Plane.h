#include "Primitive.h"
#pragma once
class Plane : public Primitive
{
public:
	virtual void BuildUI(uint32_t id) override;
	virtual Trace TraceRay(const Ray& ray) const override;
	virtual Trace ResolveTracePayload(Trace& payload, const Ray& ray) const override;
};

