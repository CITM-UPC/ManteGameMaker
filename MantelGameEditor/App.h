#pragma once

#include "Globals.h"
#include "Module.h"

#include "ModuleWindow.h"
#include "ModuleEvents.h"
#include "ModuleEngineManager.h"
#include "ModuleGUI.h"
#include "ModuleRenderer.h"

using namespace chrono;

// Modules Classes
class ModuleWindow;
class ModuleEvents;
class ModuleEngineManager;
class ModuleGUI;
class ModuleRenderer;

struct Organization
{
	std::string name;
	std::list<std::string> members;
};

struct AppDetails
{
	std::string name;
	Organization org;
};


class App
{
public:
	// Modules
	ModuleWindow* window;
	ModuleEvents* events;
	ModuleEngineManager* engineManager;
	ModuleGUI* gui;
	ModuleRenderer* renderer;

private:
	int argc;
	char** args;

	list<Module*> modules;

	// FPS control
	duration<double> dt;
	double previousTime, targetFrameTime;
	double frameRate = 60.0; // 60 frames per second

public:
	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	void Awake();

	// Called before the first frame
	void Start();

	// Called each loop iteration and returns false if wants to stop execution
	bool DoUpdate();

	// Called before quitting
	void CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);


	// FPS control
	void ChangeFramerate(double newFrameRate) {
		this->targetFrameTime = 1.0 / newFrameRate;
	}

	duration<double> GetDeltaTime() {
		return dt;
	}

	double GetFrameRate() {
		return frameRate;
	}

	void SetFrameRate(double newFrameRate) {
		frameRate = newFrameRate;
	}

	void RequestBrowser(const char* link)
	{
		ShellExecuteA(NULL, "open", link, NULL, NULL, SW_SHOWNORMAL);
	}

private:

	// Call active modules to perform PreUpdate
	bool PreUpdate();

	// Call active modules to perform Update
	bool Update();

	// Call active modules to perform PostUpdate
	bool PostUpdate();


	//FPS control
	double GetCurrentTime() {
		using namespace std::chrono;
		high_resolution_clock::time_point now = high_resolution_clock::now();
		duration<double> time_span = duration_cast<duration<double>>(now.time_since_epoch());
		return time_span.count();
	}

	void UpdateDeltaTime() {
		double currentTime = GetCurrentTime();
		this->dt = duration<double>(currentTime - this->previousTime);

		this->previousTime = currentTime;
	}


public:
	AppDetails GetAppDetails()
	{
		return details;
	}

private:
	AppDetails details;
};

extern App* app;