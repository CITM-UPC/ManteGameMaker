#pragma once

#include <list>
#include "EGlobals.h"
#include "EModule.h"
//#include "ModuleInput.h"
#include "EModuleRenderer3D.h"
//#include "ModuleAudio.h"

#include "Camera.h"

class MantelGameEngine
{
public:
	//ModuleInput* input;
	EModuleRenderer3D* renderer3D;
	//ModuleAudio* audio;

	Camera cam;

private:

	std::list<EModule*> list_modules;

public:

	MantelGameEngine();
	~MantelGameEngine();

	bool Init();
	bool Start();
	engine_status PreUpdate();
	engine_status Update();
	engine_status PostUpdate();
	bool CleanUp();

private:

	void AddModule(EModule* mod);
	void PrepareUpdate();
	void FinishUpdate();
};
