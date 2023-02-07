#pragma once
#include <vector>

#include <SDL/SDL.h>

#include "Ray.hpp"
#include "Sphere.hpp"
#include "RayInfo.hpp"
#include "../Timer.hpp"
#include "../Math/Vec3.hpp"
#include "../Math/Vec2.hpp"

class Graphics
{
public :
	Graphics(SDL_Window* window, SDL_Renderer* graphics);
	void clear();
	void drawGui();
	void render();

	SDL_Renderer* getRenderer() noexcept;
	void setSpheres(std::vector<Sphere>& spheres) noexcept;

	static std::uint32_t getColor(std::uint8_t r, std::uint8_t g, std::uint8_t b);
	static SDL_Color getColor(std::uint32_t colorARGB);

private :
	void draw();
	std::uint32_t perPixel(Vec2& coord);
	RayInfo traceRay(Ray& ray);
	RayInfo closestHit(Ray& ray, float hitDistance, Sphere* hitSphere);

	void resetFrameBuffer();
	void drawSwap();

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
	Vec2 m_resolution;
	float m_timeToRender;
	bool m_renderOnce;

	std::vector<Sphere> m_spheres;
	Vec3 m_lightPos;
	Vec3 m_position;
};