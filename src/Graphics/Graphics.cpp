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
	
}

void Graphics::clear()
{
	//Clear
	SDL_RenderSetLogicalSize(p_graphics, resolutionX, resolutionY);
	SDL_SetRenderDrawColor(p_graphics, 0, 0, 0, 255);
	SDL_RenderClear(p_graphics);
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame(p_window);
	ImGui::NewFrame();
}

void Graphics::draw(const Sphere& sphere)
{
	for (size_t y = 0; y < resolutionY; y++)
	{
		for (size_t x = 0; x < resolutionX; x++)
		{
			Vec2 coord((float)x / (float)resolutionX, (float)y / (float)resolutionY);
			coord = coord * 2 - 1;

			float a, b, c, discriminant;
			Ray ray(Vec3(0, 0, -10), Vec3(coord.x, coord.y, -1));
			a = Math::dot(ray.direction, ray.direction);
			b = 2 * Math::dot(ray.origin, ray.direction);
			c = Math::dot(ray.origin, ray.origin) - pow(sphere.radius, 2);

			discriminant = pow(b, 2) - 4 * a * c;
			if (discriminant >= 0)
			{
				SDL_SetRenderDrawColor(p_graphics, 0, 255, 0, 255);
			}
			else
			{
				SDL_SetRenderDrawColor(p_graphics, 0, 0, 0, 255);
			}

			SDL_RenderDrawPoint(p_graphics, x, y);	
		}
	}
}

void Graphics::drawGui(int fps)
{
	SDL_RenderSetLogicalSize(p_graphics, SCREEN_X, SCREEN_Y);
	ImGui::Begin("Render infos");
	ImGui::Text("%i FPS", fps);
	ImGui::InputInt2("Resolution", &resolutionX);
	ImGui::End();
}

void Graphics::render()
{
	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(p_graphics);
}

SDL_Renderer* Graphics::getRenderer() noexcept
{
	return p_graphics;
}