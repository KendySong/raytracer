#pragma once
#include <SDL/SDL.h>

class Graphics
{
public :
	Graphics(SDL_Window* window, SDL_Renderer* graphics);
	void clear();
	void draw();
	void drawGui(int fps);
	void render();

	SDL_Renderer* getRenderer() noexcept;

private :
	SDL_Window* p_window;
	SDL_Renderer* p_graphics;
};