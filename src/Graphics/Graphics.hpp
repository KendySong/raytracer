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
	void draw(std::uint32_t* backBuffer, const Sphere& sphere);

	void drawGui(int fps, const Sphere& sphere);
	void render();

	SDL_Renderer* getRenderer() noexcept;

private :
	void resetFrameBuffer();

	SDL_Window* p_window;
	SDL_Renderer* p_graphics;

	std::uint32_t* p_frontBuffer;
	SDL_Texture* p_frontTex;

	std::uint32_t* p_backBuffer;
	SDL_Texture* p_backTex;

	int m_showResolutionX;
	int m_showResolutionY;

};