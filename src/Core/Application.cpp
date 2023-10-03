#include <windows.h>
#include <iostream>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_sdl.h>
#include <ImGui/imgui_impl_sdlrenderer.h>

#include "Application.hpp"
#include "../Graphics/Sphere.hpp"
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
	while (m_isRunning)
	{
		if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		{
			SDL_WarpMouseInWindow(Application::instance()->getWindow(), SCREEN_X / 2, SCREEN_Y / 2);
		}

		while (SDL_PollEvent(&m_event))
		{
			ImGui_ImplSDL2_ProcessEvent(&m_event);
			switch (m_event.type)
			{
			case SDL_QUIT:
				m_isRunning = false;
				break;

			case SDL_KEYDOWN:
				switch (m_event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					m_isRunning = false;
					break;
				}
				break;
			}
		}

		this->countFPS();

		//Render frame
		m_graphics->clear();
		m_graphics->drawGui(m_displayFPS);
		m_graphics->render();	
	}

	delete p_app;
	return 0;
}

SDL_Window* Application::getWindow() noexcept
{
	return p_window;
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