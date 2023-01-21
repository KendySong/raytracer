#include <cstdint>
#include <iostream>

#include <SDL/SDL_image.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_sdl.h>
#include <ImGui/imgui_impl_sdlrenderer.h>

#include "Graphics.hpp"
#include "../Settings.hpp"

Graphics::Graphics(SDL_Window* window, SDL_Renderer* graphics) : p_window(window), p_graphics(graphics)
{
	srand(time(nullptr));
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

void Graphics::draw()
{
	for (size_t y = 0; y < resolutionY; y++)
	{
		for (size_t x = 0; x < resolutionX; x++)
		{
			

			SDL_SetRenderDrawColor(p_graphics, rand() % 255, rand() % 255, rand() % 255, 255);
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