#include "Trace.h"


Trace& Trace::Miss()
{
	Result = TraceResult::Miss;
	return *this;
}

Trace& Trace::Hit(float hitDistance, int hit_guid)
{
	Result = TraceResult::Hit;
	HitDistance = hitDistance;
	HitGuid = hit_guid;
	return *this;
}
