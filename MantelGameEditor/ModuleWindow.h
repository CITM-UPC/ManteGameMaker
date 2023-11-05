#pragma once

#include "Module.h"
#include "Globals.h"

class ModuleWindow : public Module
{
public:
	ModuleWindow(bool startEnabled);
	~ModuleWindow();

	void Awake();
	bool PreUpdate();
	void CleanUp();

	SDL_Window* GetWindow() { return window; }
	SDL_GLContext GetContext() { return gl_context; }
	void UpdateWindowContext(SDL_Window* window, SDL_GLContext gl_context);

private:
	static SDL_Window* SDLWindowInit();

	static SDL_GLContext CreateWindowContext(SDL_Window* window);
	static void OpenGLInit();

public:

	iPoint GetWindowSize() {
		return windowSize;
	}


private:
	SDL_Window* window;
	SDL_GLContext gl_context;

	iPoint windowSize;
};