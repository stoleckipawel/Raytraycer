#include "Box.h"

void Box::BuildUI(uint32_t id)
{
	ImGui::PushID(id);
	ImGui::DragFloat3("Position", glm::value_ptr(Position), 0.01f);
	ImGui::DragFloat3("Rotation", glm::value_ptr(Rotation), 5.01f);
	ImGui::DragFloat3("Scale", glm::value_ptr(Scale), 0.01f);
	ImGui::Separator();
	ImGui::PopID();
}

glm::vec3 Box::GetExtentMin() const
{
	return glm::vec3(-1.0f, -1.0f, -1.0f);
}

glm::vec3 Box::GetExtentMax() const 
{
	return glm::vec3(1.0f, 1.0f, 1.0f);
}

Trace Box::Intersect(const Ray& ray) const
{
	glm::vec3 rayOriginLocal = glm::vec3(InvWorldMtx * glm::vec4(ray.Origin, 1.0f));
	glm::vec3 rayDirectionLocal = glm::vec3(InvWorldMtx * glm::vec4(ray.Direction, 0.0f));
	rayDirectionLocal = glm::normalize(rayDirectionLocal);

	glm::vec3 lb = GetExtentMin();
	glm::vec3 rt = GetExtentMax();
	glm::vec rayDirInv = glm::vec3(1.0f, 1.0f, 1.0f) / rayDirectionLocal;
	
	float t1 = (lb.x - rayOriginLocal.x) * rayDirInv.x;
	float t2 = (rt.x - rayOriginLocal.x) * rayDirInv.x;

	float t3 = (lb.y - rayOriginLocal.y) * rayDirInv.y;
	float t4 = (rt.y - rayOriginLocal.y) * rayDirInv.y;

	float t5 = (lb.z - rayOriginLocal.z) * rayDirInv.z;
	float t6 = (rt.z - rayOriginLocal.z) * rayDirInv.z;

	float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
	float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	if (tmax < 0)
	{
		return Trace().Miss();
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		return Trace().Miss();
	}

	//We have hit!
	Trace trace;
	trace.Result = TraceResult::Hit;
	trace.LocalPosition = rayOriginLocal + tmin * rayDirectionLocal;;
	trace.WorldPosition = WorldMtx * glm::vec4(trace.LocalPosition, 1.0f);
	float t = glm::length(trace.WorldPosition - ray.Origin);// we need t in world position ! thats why we cant use regular tmin
	trace.HitDistance = t;
	trace.HitGuid = Guid;
	return trace;
}

glm::vec3 Box::ComputeAABBNormal(const Ray& ray, Trace trace) const
{
	glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);

	// Check which face the intersection point lies on
	float epsilon = 0.001;
	for (uint8_t i = 0; i < 3; i++)
	{
		if (abs(trace.LocalPosition[i] - GetExtentMin()[i]) < epsilon)
		{
			normal[i] = -1.0f;
		}
		else if (abs(trace.LocalPosition[i] - GetExtentMax()[i]) < epsilon)
		{
			normal[i] = 1.0f;
		}
	}

	// Transform the normal to world space
	return glm::normalize(glm::vec3(glm::mat3(WorldRotationMtx) * glm::vec4(normal, 0.0f)));
}

Trace Box::ResolveTracePayload(Trace& payload, const Ray& ray) const
{
	payload.WorldNormal = ComputeAABBNormal(ray, payload);
	return payload;
}
