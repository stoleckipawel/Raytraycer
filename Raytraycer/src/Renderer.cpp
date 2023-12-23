#include "Renderer.h"
#include "Walnut/Random.h"

static uint32_t ConvertColorToUInt32(glm::vec4 color)
{
	color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
	color *= 255.0f;
	return ((uint8_t)color.x) | ((uint8_t)color.y << 8) | ((uint8_t)color.z << 16) | ((uint8_t)color.w << 24);
}

void Renderer::Render(const Scene& scene, const Camera& camera)
{
	if (scene.Spheres.size() == 0.0)
		return;
	
	Ray ray;
	ray.Origin = camera.GetPosition();
	
	for (uint32_t y = 0; y < m_FrontBuffer->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FrontBuffer->GetWidth(); x++)
		{
			ray.Direction = camera.GetRayDirections()[x + y * m_FrontBuffer->GetWidth()];

			glm::vec4 color = TraceRay(scene, ray);

			m_FrontBufferData[x + y * m_FrontBuffer->GetWidth()] = ConvertColorToUInt32(color);
		}
	}

	m_FrontBuffer->SetData(m_FrontBufferData);
}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (!m_FrontBuffer)
	{
		m_FrontBuffer = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}
	else if (width != m_FrontBuffer->GetWidth() || height != m_FrontBuffer->GetHeight())
	{
		m_FrontBuffer->Resize(width, height);
	}
	else
	{
		return;
	}

	delete[] m_FrontBufferData;
	m_FrontBufferData = new uint32_t[width * height];

}

glm::vec4 Renderer::TraceRay(const Scene& scene, const Ray& ray)
{
	//Ray sphere intersection
	//bx^2 +by^2)t^2 + (2axbx + 2ayby)t + (ax^2 + ay^2 - r^2) = 0
	//a = ray origin
	//b = ray direction
	//r = sphere radius

	const Sphere* closestSphere = nullptr;
	float closestHitDistance = 9999.0;

	for (const Sphere& sphere : scene.Spheres)
	{
		float radiusSquared = sphere.Radius * sphere.Radius;
		glm::vec3 origin = ray.Origin - sphere.Position;//optimization for sphere placement

		float a = glm::dot(ray.Direction, ray.Direction);
		float b = glm::dot(origin, ray.Direction) * 2.0f;
		float c = glm::dot(origin, origin) - radiusSquared;


		//Quadriatic formula descriminant
		// x1 = b^2 - 4ac
		float discriminant = b * b - (4.0f * a * c);
		if (discriminant > 0.0f)
		{
			float hitDistance = (-b - glm::sqrt(discriminant)) / (2.0f * a);//closest
			//float t1 = (-b + glm::sqrt(discriminant)) / (2 * a);//not needed now
			if (hitDistance < closestHitDistance)
			{
				closestHitDistance = hitDistance;
				closestSphere = &sphere;
			}
		}
	}

	if (closestSphere == nullptr)
	{
		return glm::vec4(0.15f, 0.65f, 1.0f, 1.0f);
	}

	glm::vec3 origin = ray.Origin - closestSphere->Position;//optimization for sphere placement
	glm::vec3 closestHitPos = origin + ray.Direction * closestHitDistance;
	glm::vec3 normal = glm::normalize(closestHitPos);//assumes that each sphere is in the center of the world
	

	glm::vec3 lightDir = glm::vec3(-1.0f, -1.0f, -1.0f);
	lightDir = glm::normalize(lightDir);
	float shading = glm::max(glm::dot(normal, -lightDir), 0.0f);

	return glm::vec4(closestSphere->Albedo * shading, 1.0f);
}

