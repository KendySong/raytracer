#pragma once
#include <vector>

#include <SDL/SDL.h>

#include "../Timer.hpp"

#include "Ray.hpp"
#include "Sphere.hpp"
#include "../Math/Vec3.hpp"

class Graphics
{
public :
	Graphics(SDL_Window* window, SDL_Renderer* graphics);
	void clear();
	void draw(std::uint32_t* backBuffer, const Sphere& sphere);

	void drawGui(const Sphere& sphere);
	void render();

	static std::uint32_t getColor(std::uint8_t r, std::uint8_t g, std::uint8_t b);

	SDL_Renderer* getRenderer() noexcept;

private :
	void resetFrameBuffer();

	SDL_Window* p_window;
	SDL_Renderer* p_graphics;
	float m_aspectRatio;

	std::uint32_t* p_frontBuffer;
	std::uint32_t* p_backBuffer;
	SDL_Texture* p_frontTex;
	SDL_Texture* p_backTex;

	Timer m_frameChrono;
	int m_showResolutionX;
	int m_showResolutionY;
	float m_timeToRender;

	Vec3 pos;
};