#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_sdl.h>
#include <ImGui/imgui_impl_sdlrenderer.h>

#include "Application.hpp"
#include "Graphics/Sphere.hpp"
#include "Settings.hpp"

Application* Application::p_app = nullptr;
Application::Application()
{
	m_fps = 0;
	m_displayFPS = 0;
	m_isRunning = true;

	SDL_Init(SDL_INIT_EVERYTHING);

	p_window = SDL_CreateWindow("Raytracing Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_X, SCREEN_Y, SDL_WINDOW_SHOWN);
	m_graphics = new Graphics(p_window, SDL_CreateRenderer(p_window, -1, 0));
	SDL_RenderSetLogicalSize(m_graphics->getRenderer(), resolutionX, resolutionY);

	//Init imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForSDLRenderer(p_window, m_graphics->getRenderer());
	ImGui_ImplSDLRenderer_Init(m_graphics->getRenderer());

	//Init world
	m_spheres.push_back(Sphere(Vec3(0, 0, 0), 2));
}

Application* Application::instance()
{
	if (p_app == nullptr)
	{
		p_app = new Application();
	}
	return p_app;
}

int Application::run()
{
	int x = 0;

	while (m_isRunning)
	{
		while (SDL_PollEvent(&m_event))
		{
			ImGui_ImplSDL2_ProcessEvent(&m_event);
			if (m_event.type == SDL_QUIT)
			{
				m_isRunning = false;
			}
		}

		//Update
		this->countFPS();	
		float deltaTime = m_deltaTimeChrono.getElapsedTime();
		m_deltaTimeChrono.restart();

		//Render frame
		m_graphics->clear();
		m_graphics->drawGui(m_displayFPS, m_spheres[0]);
		m_graphics->render();		
	}

	delete p_app;
	return 0;
}

void Application::countFPS()
{
	m_fps++;
	if (m_framerateChrono.getElapsedTime() >= 1)
	{
		m_displayFPS = m_fps;
		m_fps = 0;
		m_framerateChrono.restart();
	}
}