#include "Renderer.h"
#include "Walnut/Random.h"

static uint32_t ConvertColorToUInt32(glm::vec4 color)
{
	color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
	color *= 255.0f;
	return ((uint8_t)color.x) | ((uint8_t)color.y << 8) | ((uint8_t)color.z << 16) | ((uint8_t)color.w << 24);
}

void Renderer::Render(const Camera& camera)
{
	Ray ray;
	ray.Origin = camera.GetPosition();
	
	for (uint32_t y = 0; y < m_FrontBuffer->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FrontBuffer->GetWidth(); x++)
		{
			ray.Direction = camera.GetRayDirections()[x + y * m_FrontBuffer->GetWidth()];
			glm::vec4 color = TraceRay(ray);
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

glm::vec4 Renderer::TraceRay(const Ray& ray)
{
	glm::vec3 spherePosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 lightDir = glm::vec3(1.0f, 1.0f, 0.0f);

	float radius = 0.5;
	float radiusSquared = radius * radius;

	//Ray sphere intersection
	//bx^2 +by^2)t^2 + (2axbx + 2ayby)t + (ax^2 + ay^2 - r^2) = 0
	//a = ray origin
	//b = ray direction
	//r = sphere radius

	float a = glm::dot(ray.Direction, ray.Direction);
	float b = glm::dot(ray.Origin, ray.Direction) * 2.0f;
	float c = glm::dot(ray.Origin, ray.Origin) - radiusSquared;
	
	//Quadriatic formula descriminant
	// x1 = b^2 - 4ac
	float discriminant = b * b - (4.0f * a * c);
	if (discriminant > 0.0f)
	{
		float t0 = (-b - glm::sqrt(discriminant)) / (2 * a);
		float t1 = (-b + glm::sqrt(discriminant)) / (2 * a);

		glm::vec3 hitPos0 = ray.Origin + ray.Direction * t0;
		//glm::vec3 hitPos1 = rayOrigin + rayDirection * t1; // we are only interested in the nearest hit for now

		glm::vec3 normal = glm::normalize(spherePosition - hitPos0);
		float shading = glm::clamp(glm::dot(normal, lightDir), 0.0f, 1.0f);

		glm::vec3 color = glm::vec3(shading, shading, shading);
		return glm::vec4(color, 1.0f);
	}

	return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

