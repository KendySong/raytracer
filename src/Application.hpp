#pragma once
#include <vector>

#include <SDL/SDL.h>

#include "Timer.hpp"
#include "Math/Vec3.hpp"
#include "Graphics/Sphere.hpp"
#include "Graphics/Graphics.hpp"

class Application
{
private :
	Application();

public :
	static Application* instance();
	int run();

private :
	void countFPS();

	static Application* p_app;
	SDL_Window* p_window;	
	SDL_Event m_event;
	bool m_isRunning;

	Graphics* m_graphics;
	std::vector<Sphere> m_spheres;

	Timer m_deltaTimeChrono;	
	Timer m_framerateChrono;
	int m_displayFPS;
	int m_fps;
};