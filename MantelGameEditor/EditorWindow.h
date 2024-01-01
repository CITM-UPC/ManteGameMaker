#pragma once
#include "EditorModule.h"

#include "GL/glew.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

using namespace std;
using namespace chrono;

class EditorWindow : public EditorModule {
public:
	EditorWindow();
	~EditorWindow();

	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	void Render();

	void UpdateFullscreen();
	void UpdateResizable();
	void UpdateBorderless();
	void UpdateSizes(int newWidth, int newHeight);

public:
	SDL_Window* window = nullptr;

	SDL_GLContext glContext = nullptr;

	bool isFullscreen = false;
	bool isResizable = true;
	bool isBorderless = false;

	int width = 720 * 16 / 9;
	int height = 720;
};

