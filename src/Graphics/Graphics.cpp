#include <cstdint>
#include <iostream>

#include <SDL/SDL_image.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_sdl.h>
#include <ImGui/imgui_impl_sdlrenderer.h>

#include "Graphics.hpp"
#include "../Math/Math.hpp"
#include "../Core/Settings.hpp"

Graphics::Graphics(SDL_Window* window, SDL_Renderer* graphics) : p_window(window), p_graphics(graphics)
{
	m_showResolutionX = resolutionX;
	m_showResolutionY = resolutionY;
	m_resolution = Vec2(resolutionX, resolutionY);
	m_aspectRatio = m_resolution.x / m_resolution.y;

	p_frontBuffer = new std::uint32_t[0];
	p_backBuffer = new std::uint32_t[0];
	this->resetFrameBuffer();

	m_lightPos = Vec3(-1, -1, 1);
	m_renderOnce = false;
	m_position = Vec3(0, 0, 0);
	m_maximumShading = 0;
}

void Graphics::clear()
{
	SDL_RenderSetLogicalSize(p_graphics, resolutionX, resolutionY);
	SDL_SetRenderDrawColor(p_graphics, 0, 0, 0, 255);
	SDL_RenderClear(p_graphics);

	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame(p_window);
	ImGui::NewFrame();
}

void Graphics::drawGui()
{
	SDL_RenderSetLogicalSize(p_graphics, SCREEN_X, SCREEN_Y);
	ImGui::Begin("Render");
		ImGui::Text("ms : %f", m_timeToRender);
		ImGui::Checkbox("Render frame per frame", &m_renderOnce);

		if (!m_renderOnce)
		{
			this->drawSwap();
		}
		else
		{
			if (ImGui::Button("Render frame"))
			{
				this->drawSwap();
			}
		}

		ImGui::InputInt("Bounce limit", &bounceLimit, 1);
		ImGui::DragFloat("Maximum shading", &m_maximumShading, 0.001f, 0, 1);
		ImGui::Separator();

		ImGui::InputInt2("Resolution", &m_showResolutionX);	
		if (ImGui::Button("Apply resolution"))
		{
			resolutionX = m_showResolutionX;
			resolutionY = m_showResolutionY;
			m_resolution.x = resolutionX;
			m_resolution.y = resolutionY;
			m_aspectRatio = m_resolution.x / m_resolution.y;
			this->resetFrameBuffer();
		}

	ImGui::End();

	ImGui::Begin("Scene");
	
	ImGui::TextUnformatted("Origin");
	ImGui::SameLine();
	ImGui::DragFloat3("Position", &m_position.x, 0.1f);
	ImGui::TextUnformatted("Light ");
	ImGui::SameLine();
	ImGui::PushID(m_spheres.size());
	ImGui::DragFloat3("Position", &m_lightPos.x, 0.1f);
	ImGui::PopID();
	ImGui::Separator();

	for (size_t i = 0; i < m_spheres.size(); i++)
	{
		ImGui::PushID(i);
		ImGui::Text("Sphere[%i]", i);
		ImGui::DragFloat3("Position", &m_spheres[i].position.x, 0.01f);
		ImGui::SliderFloat("Radius", &m_spheres[i].radius, 0, 10);
		ImGui::Separator();
		ImGui::PopID();
	}
	ImGui::End();
}

SDL_Color Graphics::getColor(std::uint32_t colorARGB)
{
	SDL_Color color;
	color.a = (colorARGB & 0xFF000000) >> 24;
	color.r = (colorARGB & 0x00FF0000) >> 16;
	color.g = (colorARGB & 0x0000FF00) >> 8;
	color.b = (colorARGB & 0x00FF00FF);
	return color;
}

SDL_Renderer* Graphics::getRenderer() noexcept
{
	return p_graphics;
}

void Graphics::render()
{
	SDL_UpdateTexture(p_frontTex, NULL, p_frontBuffer, resolutionX * sizeof std::uint32_t);
	SDL_RenderCopy(p_graphics, p_frontTex, NULL, NULL);

	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(p_graphics);
}

std::uint32_t Graphics::getColor(std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
	return 0xFF000000 | ((0x000000FF & r) << 16) | ((0x000000FF & g) << 8) | (0x000000FF & b);
}

void Graphics::setSpheres(std::vector<Sphere>& spheres) noexcept
{
	m_spheres = spheres;
}

void Graphics::draw()
{
	for (int y = 0; y < resolutionY; y++)
	{
		for (int x = 0; x < resolutionX; x++)
		{
			p_backBuffer[y * resolutionX + x] = this->perPixel(Vec2(x, y) / m_resolution);
		}
	}
}

std::uint32_t Graphics::perPixel(Vec2& coord)
{
	//Normalize coordinates
	coord = coord * 2 - 1;
	coord.x *= m_aspectRatio;

	Ray ray(m_position, Math::normalize(Vec3(coord.x, coord.y, -1)));
	RayInfo rayInfo = this->traceRay(ray);
	if (rayInfo.sphere == nullptr)
	{
		return this->getColor(0, 170, 255);
	}

	//Compute light intensity
	float lightIntensity = Math::dot(rayInfo.normal, Math::normalize(m_lightPos - rayInfo.position));
	lightIntensity = lightIntensity < m_maximumShading ? m_maximumShading : lightIntensity > 1 ? 1 : lightIntensity;

	SDL_Color pixelColor = this->getColor(rayInfo.sphere->color);
	ray = Ray(rayInfo.position, Math::reflect(ray.direction, rayInfo.normal));

	for (size_t i = 0; i < bounceLimit; i++)
	{			
		rayInfo = this->traceRay(ray);
		if (rayInfo.sphere != nullptr)
		{
			rayInfo = this->traceRay(ray);
			ray = Ray(rayInfo.position, Math::reflect(ray.direction, rayInfo.normal));
			pixelColor = this->getColor(rayInfo.sphere->color);
		}
		else
		{
			break;
		}
	}

	return this->getColor(pixelColor.r * lightIntensity, pixelColor.g * lightIntensity, pixelColor.b * lightIntensity);
}

RayInfo Graphics::traceRay(const Ray& ray)
{
	Sphere* closestSphere = nullptr;
	float closestDist = FLT_MAX;

	for (Sphere& sphere : m_spheres)
	{
		Vec3 origin = ray.origin - sphere.position;

		//ax^2 + bx + c
		//Resolve equation for detect if the ray have an intersection with the sphere
		float a, b, c, discriminant;
		a = Math::dot(ray.direction, ray.direction);
		b = 2 * Math::dot(origin, ray.direction);
		c = Math::dot(origin, origin) - pow(sphere.radius, 2);
		discriminant = pow(b, 2) - 4 * a * c;

		if (discriminant > 0)
		{
			float t = (-b - sqrt(discriminant)) / (2 * a);

			if (t > 0 && t < closestDist)
			{
				closestDist = t;
				closestSphere = &sphere;
			}
		}
	}

	if (closestSphere == nullptr)
	{
		return RayInfo();
	}

	return this->closestHit(ray, closestDist, closestSphere);
}
RayInfo Graphics::closestHit(const Ray& ray, float hitDistance, Sphere* hitSphere)
{
	Vec3 hit = ray.at(hitDistance);
	Vec3 normal = Math::normalize(hit - hitSphere->position);
	return RayInfo(hitSphere, hitDistance, hit, normal);
}

void Graphics::resetFrameBuffer()
{
	delete p_frontBuffer;
	delete p_backBuffer;
	p_frontBuffer = new std::uint32_t[resolutionX * resolutionY];
	p_backBuffer = new std::uint32_t[resolutionX * resolutionY];
	memset(p_frontBuffer, 0, resolutionX * resolutionY * sizeof std::uint32_t);
	memset(p_backBuffer, 0, resolutionX * resolutionY * sizeof std::uint32_t);
	p_frontTex = SDL_CreateTexture(p_graphics, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, resolutionX, resolutionY);
	p_backTex = SDL_CreateTexture(p_graphics, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, resolutionX, resolutionY);
}

void Graphics::drawSwap()
{
	//Draw sphere and swap buffers
	m_frameChrono.restart();
	this->draw();
	m_timeToRender = m_frameChrono.getElapsedTime() * 1000;

	std::uint32_t* tempBuffer = p_frontBuffer;
	p_frontBuffer = p_backBuffer;
	p_backBuffer = tempBuffer;
}