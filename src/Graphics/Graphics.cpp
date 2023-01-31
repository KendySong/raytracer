#include <cstdint>
#include <iostream>

#include <SDL/SDL_image.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_sdl.h>
#include <ImGui/imgui_impl_sdlrenderer.h>

#include "Graphics.hpp"
#include "../Math/Math.hpp"
#include "../Settings.hpp"

Graphics::Graphics(SDL_Window* window, SDL_Renderer* graphics) : p_window(window), p_graphics(graphics)
{
	m_showResolutionX = resolutionX;
	m_showResolutionY = resolutionY;
	m_aspectRatio = (float)resolutionX / (float)resolutionY;

	p_frontBuffer = new std::uint32_t[0];
	p_backBuffer = new std::uint32_t[0];
	this->resetFrameBuffer();

	m_lightDir = Vec3(-1, -1, 1);
	m_cameraPosition = Vec3(0, 0, -10);
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

void Graphics::draw(std::uint32_t* backBuffer, const Sphere& sphere)
{
	for (size_t y = 0; y < resolutionY; y++)
	{
		for (size_t x = 0; x < resolutionX; x++)
		{
			Vec2 coord((float)x / (float)resolutionX, (float)y / (float)resolutionY);
			coord = coord * 2 - 1;
			coord.x *= m_aspectRatio;

			//ax^2 + bx + c
			float a, b, c, discriminant;
			Ray ray(Vec3(-m_cameraPosition.x, m_cameraPosition.y, m_cameraPosition.z), Math::normalize(Vec3(coord.x, coord.y, -1)));
			a = Math::dot(ray.direction, ray.direction);
			b = 2 * Math::dot(ray.origin, ray.direction);
			c = Math::dot(ray.origin, ray.origin) - pow(sphere.radius, 2);

			discriminant = pow(b, 2) - 4 * a * c;
			std::uint32_t pixelColor;

			if (discriminant >= 0)
			{	
				float tp = (-b + sqrt(discriminant)) / (2 * a);
				float tm = (-b - sqrt(discriminant)) / (2 * a);
				Vec3 atp = ray.at(tp);
				Vec3 atm = ray.at(tm);

				Vec3 normal = atp - Vec3(0, 0, 0);
				float intensity = Math::dot(Math::normalize(normal), Math::normalize(-m_lightDir));
				float intensityCheck = intensity;

				if (intensityCheck < 0)
				{
					intensityCheck = 0;
				}
				else if (intensityCheck > 1)
				{
					intensityCheck = 1;
				}

				std::uint8_t colorIntensity = intensityCheck * 255;	
				backBuffer[y * resolutionX + x] = this->getColor(colorIntensity, 0, 0);
			}
			else
			{
				backBuffer[y * resolutionX + x] = 0;
			}
		}
	}
}

void Graphics::drawGui(const Sphere& sphere)
{
	SDL_RenderSetLogicalSize(p_graphics, SCREEN_X, SCREEN_Y);
	ImGui::Begin("Render");
		ImGui::Text("ms : %f", m_timeToRender);

		if (ImGui::Button("Render frame"))
		{
			//Draw sphere and swap buffers
			m_frameChrono.restart();		
			this->draw(p_backBuffer, sphere);
			m_timeToRender = m_frameChrono.getElapsedTime() * 1000;

			std::uint32_t* tempBuffer = p_frontBuffer;
			p_frontBuffer = p_backBuffer;
			p_backBuffer = tempBuffer;
		}		

		ImGui::TextUnformatted("Resolution");
		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		ImGui::InputInt2("##",  &m_showResolutionX);
		ImGui::PopItemWidth();
		if (ImGui::Button("Apply"))
		{
			resolutionX = m_showResolutionX;
			resolutionY = m_showResolutionY;
			m_aspectRatio = (float)resolutionX / (float)resolutionY;
			this->resetFrameBuffer();
		}

		ImGui::InputFloat3("Camera position", &m_cameraPosition.x);
		ImGui::InputFloat3("Light position", &m_lightDir.x);
	ImGui::End();
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
	std::uint32_t red = (0x000000FF & r) << 16;
	std::uint32_t green = (0x000000FF & g) << 8;
	std::uint32_t blue = (0x000000FF & b);
	std::uint32_t result = 0xFF000000 | red | green | blue;
	return result;
}

SDL_Renderer* Graphics::getRenderer() noexcept
{
	return p_graphics;
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