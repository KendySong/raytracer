#pragma once
#include <vector>

#include <SDL/SDL.h>

#include "Ray.hpp"
#include "Camera.hpp"
#include "Sphere.hpp"
#include "../Timer.hpp"
#include "../Math/Vec3.hpp"

class Graphics
{
public :
	Graphics(SDL_Window* window, SDL_Renderer* graphics, Camera* camera);
	void clear();
	void draw();

	void drawGui();
	void render();

	static std::uint32_t getColor(std::uint8_t r, std::uint8_t g, std::uint8_t b);
	static SDL_Color getColor(std::uint32_t colorARGB);

	SDL_Renderer* getRenderer() noexcept;
	void setSpheres(std::vector<Sphere>& spheres);

private :
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
	Camera* p_camera;
	Vec3 m_lightPos;
	Sphere* p_closestSphere = nullptr;
	float m_closestDist = FLT_MAX;
};