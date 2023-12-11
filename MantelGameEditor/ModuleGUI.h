#pragma once

#include "Module.h"
#include "GuiBase.h"

#include "WindowScene.h"
#include "WindowHierarchy.h"
#include "WindowInspector.h"
#include "WindowProject.h"
#include "WindowConsole.h"
#include "WindowSettings.h"

// GUI Windows
class WindowScene;
class WindowHierarchy;
class WindowInspector;
class WindowProject;
class WindowConsole;
class WindowSettings;


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

	void AddWindow(GuiBase* window);


private:
	void ImguiInit(SDL_Window* window, SDL_GLContext gl_context);

	bool ImguiDockspace();
	void ImguiSetup();

public:
	ImVec4 clear_color = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

	bool settingsState = false;
	bool aboutState = false;

	bool writing = false;

private:

	list<GuiBase*>		windows;
public:
	WindowScene*		w_scene;
	WindowHierarchy*	w_hierarchy;
	WindowInspector*	w_inspector;
	WindowProject*		w_project;
	WindowConsole*		w_console;
	WindowSettings*		w_settings;
};