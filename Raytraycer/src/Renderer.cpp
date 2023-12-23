#include "Renderer.h"
#include "Walnut/Random.h"

void Renderer::Render()
{
	for (uint32_t y = 0; y < m_FrontBuffer->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FrontBuffer->GetWidth(); x++)
		{
			m_FrontBufferData[x + y * m_FrontBuffer->GetWidth()] = PerPixel(glm::vec2(x,y));
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

uint32_t Renderer::PerPixel(glm::vec2 PixelCoord)
{
	glm::vec2 coord = glm::vec2(PixelCoord.x / (float)m_FrontBuffer->GetWidth(), PixelCoord.y / (float)m_FrontBuffer->GetHeight());

	glm::vec3 spherePosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 lightDir = glm::vec3(0.0f, 1.0f, 0.0f);

	float aspectRatio = m_FrontBuffer->GetWidth() / (float)m_FrontBuffer->GetHeight();
	//Ray sphere intersection
	//bx^2 +by^2)t^2 + (2axbx + 2ayby)t + (ax^2 + ay^2 - r^2) = 0
	//a = ray origin
	//b = ray direction
	//r = sphere radius

	glm::vec2 signed_coord;
	signed_coord.x = coord.x * aspectRatio * 2.0f - 1.0f;
	signed_coord.y = coord.y * 2.0f - 1.0f;

	glm::vec3 rayDirection = glm::vec3(signed_coord.x, signed_coord.y, -1.0);
	rayDirection = glm::normalize(rayDirection);

	glm::vec3 rayOrigin = glm::vec3(0.0f, 0.0f, -1.0f);
	float radius = 0.5;
	float radiusSquared = radius * radius;

	float a = glm::dot(rayDirection, rayDirection);
	float b = glm::dot(rayOrigin, rayDirection) * 2.0f;
	float c = glm::dot(rayOrigin, rayOrigin) - radiusSquared;
	
	//Quadriatic formula descriminant
	// x1 = b^2 - 4ac
	float discriminant = b * b - (4.0f * a * c);
	if (discriminant > 0.0f)
	{
		float t0 = (-b - glm::sqrt(discriminant)) / (2 * a);
		float t1 = (-b + glm::sqrt(discriminant)) / (2 * a);

		glm::vec3 hitPos0 = rayOrigin + rayDirection * t0;
		//glm::vec3 hitPos1 = rayOrigin + rayDirection * t1; // we are only interested in the nearest hit for now

		glm::vec3 normal = spherePosition - hitPos0;
		float shading = glm::clamp(glm::dot(normal, lightDir), 0.0f, 1.0f);
		return (uint32_t)(shading * 255.0) | 0xff000000;
	}

	return 0xff000000;
}

