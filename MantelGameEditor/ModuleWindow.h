#pragma once

#include "Module.h"


class ModuleWindow : public Module
{
public:
	ModuleWindow(bool startEnabled);
	~ModuleWindow();

	void Awake();
	void CleanUp();

	SDL_Window* GetWindow() { return window; }
	SDL_GLContext GetContext() { return gl_context; }

private:
	static SDL_Window* SDLWindowInit();

	static SDL_GLContext CreateWindowContext(SDL_Window* window);
	static void OpenGLInit();

private:
	SDL_Window* window;
	SDL_GLContext gl_context;
};