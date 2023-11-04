#pragma once
#include "Module.h"

class ModuleEvents : public Module
{
public:
	ModuleEvents(bool startEnabled);
	~ModuleEvents();

	void Awake();

	bool PreUpdate();

private:
	static bool SDLEventsProcess();
};