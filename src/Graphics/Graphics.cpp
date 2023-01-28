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
			Ray ray(Vec3(0, 0, -10), Vec3(coord.x, coord.y, -1));
			a = Math::dot(ray.direction, ray.direction);
			b = 2 * Math::dot(ray.origin, ray.direction);
			c = Math::dot(ray.origin, ray.origin) - pow(sphere.radius, 2);

			discriminant = pow(b, 2) - 4 * a * c;
			std::uint32_t pixelColor;
			if (discriminant >= 0)
			{
				backBuffer[y * resolutionX + x] = sphere.color;
			}
			else
			{
				backBuffer[y * resolutionX + x] = 0;
			}
		}
	}
}

void Graphics::drawGui(int fps, const Sphere& sphere)
{
	SDL_RenderSetLogicalSize(p_graphics, SCREEN_X, SCREEN_Y);
	ImGui::Begin("Render");
		ImGui::Text("%i FPS", fps);
		if (ImGui::Button("Render frame"))
		{
			//Draw sphere and swap buffers
			this->draw(p_backBuffer, sphere);

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