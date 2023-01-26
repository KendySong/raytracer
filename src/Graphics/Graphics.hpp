#pragma once
#include <vector>

#include <SDL/SDL.h>

#include "Ray.hpp"
#include "Sphere.hpp"
#include "../Math/Vec3.hpp"

class Graphics
{
public :
	Graphics(SDL_Window* window, SDL_Renderer* graphics);
	void clear();
	void draw(const Sphere& sphere);

	void drawGui(int fps);
	void render();

	SDL_Renderer* getRenderer() noexcept;

private :
	SDL_Window* p_window;
	SDL_Renderer* p_graphics;
};