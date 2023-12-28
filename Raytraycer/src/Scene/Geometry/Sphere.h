#pragma once
#include "Primitive.h"

class Sphere : public Primitive
{
public:
	float Radius = 0.5f;
public:
	virtual void BuildUI(uint32_t id) override;
	virtual Trace Intersect(const Ray& ray) const override;
	virtual Trace ResolveTracePayload(Trace& payload, const Ray& ray) const override;
private:
	Trace GeometricSolution(const Ray& ray) const;
	Trace AnalyticSolution(const Ray& ray) const;
};