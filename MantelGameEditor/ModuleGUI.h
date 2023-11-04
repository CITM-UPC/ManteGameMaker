#pragma once

#include "Module.h"

class ModuleGUI : public Module
{
public:
	ModuleGUI(bool startEnabled);
	~ModuleGUI();

	void Awake();

	void Start();

	bool PreUpdate();

	bool Update(std::chrono::duration<double> dt);

	bool PostUpdate();

	void CleanUp();

private:
	void ImguiInit(SDL_Window* window, SDL_GLContext gl_context);

	void ImguiExample();

public:
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

private:
	bool show_demo_window = true;
	bool show_another_window = false;
};