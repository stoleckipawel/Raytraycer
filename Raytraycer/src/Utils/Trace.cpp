#include "Trace.h"


Trace& Trace::Miss()
{
	Result = TraceResult::Miss;
	return *this;
}

Trace& Trace::Hit(float hitDistance, int primitiveId)
{
	Result = TraceResult::Hit;
	HitDistance = hitDistance;
	PrimitiveId = primitiveId;
	return *this;
}
