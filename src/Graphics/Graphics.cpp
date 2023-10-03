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

	m_renderOnce = false;
	m_maximumShading = 0;
	m_maxBounce = 6;

	m_isPathtraced = true;
	m_frameCounter = 1;

	m_lightPos = Vec3(0, 0, 1);
	m_position = Vec3(0, 0, 0);
	m_skyColor = Vec3(153, 178.5, 230);
	
	m_spheres.emplace_back(Vec3(0, 46.400, -20), 50, Material(Vec3(0, 0, 0), 0.025));

	m_spheres.emplace_back(Vec3(-0.25, 0, -0.73), 0.25, Material(Vec3(160, 0, 255), 0.1));
	m_spheres.emplace_back(Vec3(0.25, 0, -0.73), 0.25, Material(Vec3(0, 160, 160), 0));

	for (size_t i = 0; i < 5; i++)
	{
		m_spheres.emplace_back(Random::next(-1, 1), Random::nextF(0.1, 1.25), Material(Random::next(0, 255), Random::nextF(0, 0.1)));
	}
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

void Graphics::drawGui(int fps)
{
	SDL_RenderSetLogicalSize(p_graphics, SCREEN_X, SCREEN_Y);
	ImGui::Begin("Graphics settings");
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.6);
		ImGui::Text("fps : %i", fps);
		ImGui::Text("ms  : %f", m_timeToRender);		
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

		ImGui::Checkbox("Pathtracing", &m_isPathtraced);
		if (m_isPathtraced)
		{
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
			{
				m_frameCounter = 1;
			}
		}

		ImGui::InputInt("Bounce limit", &m_maxBounce, 1);
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
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.6);
	ImGui::DragFloat3("Origin", &m_position.x, 0.1f);
	ImGui::DragFloat3("Light position", &m_lightPos.x, 0.1f);
	ImGui::ColorEdit3("Sky color", &m_skyColor.x);
	ImGui::Separator();
	
	for (size_t i = 0; i < m_spheres.size(); i++)
	{
		ImGui::PushID(i);
		ImGui::Text("Sphere[%i]", i);

		if (ImGui::TreeNode("Transform"))
		{
			ImGui::DragFloat3("Position", &m_spheres[i].position.x, 0.01f);
			ImGui::SliderFloat("Radius", &m_spheres[i].radius, 0, 100);
			ImGui::TreePop();
		}
		
		if (ImGui::TreeNode("Material"))
		{
			ImGui::ColorEdit3("Color", &m_spheres[i].material.albedo.x);
			ImGui::DragFloat("Roughness", &m_spheres[i].material.roughness, 0.001, 0, 5);
			ImGui::TreePop();
		}

		ImGui::PopID();
	}
	ImGui::End();
}

void Graphics::render()
{
	SDL_UpdateTexture(p_frontTex, NULL, p_frontBuffer, resolutionX * sizeof(std::uint32_t));
	SDL_RenderCopy(p_graphics, p_frontTex, NULL, NULL);

	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(p_graphics);
}

SDL_Renderer* Graphics::getRenderer() noexcept
{
	return p_graphics;
}

void Graphics::setSpheres(std::vector<Sphere>& spheres) noexcept
{
	m_spheres = spheres;
}

Vec3 Graphics::clamp(Vec3 vec, float min, float max)
{
	return {
		vec.x > 255 ? 255 : vec.x < 0 ? 0 : vec.x,
		vec.y > 255 ? 255 : vec.y < 0 ? 0 : vec.y,
		vec.z > 255 ? 255 : vec.z < 0 ? 0 : vec.z
	};
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

std::uint32_t Graphics::getColor(const Vec3& colorRGB)
{
	return 0xFF000000 | ((0x000000FF & (std::uint8_t)colorRGB.x) << 16) | ((0x000000FF & (std::uint8_t)colorRGB.y) << 8) | (0x000000FF & (std::uint8_t)colorRGB.z);
}

std::uint32_t Graphics::getColor(std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
	return 0xFF000000 | ((0x000000FF & r) << 16) | ((0x000000FF & g) << 8) | (0x000000FF & b);
}

void Graphics::draw()
{
	if (m_frameCounter == 1)
	{
		memset(p_accumulation, 0, resolutionX * resolutionY * sizeof(Vec3));
	}

	for (size_t y = 0; y < resolutionY; y++)
	{
		for (size_t x = 0; x < resolutionX; x++)
		{
            Vec2 nPos = Vec2(x, y) / m_resolution;
			p_accumulation[y * resolutionX + x] += this->perPixel(nPos);
			Vec3 accumulatedColor = this->clamp(p_accumulation[y * resolutionX + x] / m_frameCounter, 0, 255);
			p_backBuffer[y * resolutionX + x] = this->getColor(accumulatedColor);
		}
	}

	if (m_isPathtraced)
	{
		m_frameCounter++;
	}
	else
	{
		m_frameCounter = 1;
	}
}

Vec3 Graphics::perPixel(Vec2& coord)
{	
	coord = coord * 2 - 1;
	coord.x *= m_aspectRatio;
	Ray ray(m_position, Math::normalize(Vec3(coord.x, coord.y, -1)));

	Vec3 pixelColor;
	float colorFactor = 1;

	for (size_t i = 0; i < m_maxBounce; i++)
	{
		RayInfo rayInfo = this->traceRay(ray);
		if (rayInfo.sphere == nullptr)
		{
			pixelColor += m_skyColor * colorFactor;
			break;
		}

		float lightIntensity = Math::dot(rayInfo.normal, Math::normalize(m_lightPos - rayInfo.position));
		lightIntensity = lightIntensity < m_maximumShading ? m_maximumShading : lightIntensity > 1 ? 1 : lightIntensity;
		pixelColor += rayInfo.sphere->material.albedo * lightIntensity * colorFactor;
		
		colorFactor *= 0.5;
		ray = Ray(rayInfo.position, Math::reflect(ray.direction, rayInfo.normal + rayInfo.sphere->material.roughness * Random::next(-0.5, 0.5)));
	}

	return pixelColor;
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
	delete[] p_frontBuffer;
	delete[] p_backBuffer;
	delete[] p_accumulation;
	
	m_frameCounter = 1;
	p_frontBuffer = new std::uint32_t[resolutionX * resolutionY];
	p_backBuffer = new std::uint32_t[resolutionX * resolutionY];
	p_accumulation = new Vec3[resolutionX * resolutionY];
	memset(p_frontBuffer, 0, resolutionX * resolutionY * sizeof(std::uint32_t));
	memset(p_backBuffer, 0, resolutionX * resolutionY * sizeof(std::uint32_t));
	memset(p_accumulation, 0, resolutionX * resolutionY * sizeof(Vec3));
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