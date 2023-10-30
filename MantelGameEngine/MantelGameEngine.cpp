#include "MantelGameEngine.h"

MantelGameEngine::MantelGameEngine()
{
	//input = new ModuleInput(this);
	renderer3D = new EModuleRenderer3D(this);

	//AddModule(input);
	AddModule(renderer3D);
}

MantelGameEngine::~MantelGameEngine()
{
	list_modules.clear();
}

bool MantelGameEngine::Init()
{
	bool ret = true;

	for (auto const& item : list_modules)
	{
		ret = item->Init();
	}

	return ret;
}

bool MantelGameEngine::Start()
{
	bool ret = true;

	ELOG("Game Engine Start --------------");
	for (auto const& item : list_modules)
	{
		ret = item->Start();
	}

	return ret;
}

void MantelGameEngine::PrepareUpdate()
{
}

void MantelGameEngine::FinishUpdate()
{
}

engine_status MantelGameEngine::PreUpdate()
{
	engine_status ret = ENGINE_UPDATE_CONTINUE;

	for (auto const& item : list_modules)
	{
		ret = item->PreUpdate();
		if (ret != ENGINE_UPDATE_CONTINUE) return ret;
	}

	return ret;
}

engine_status MantelGameEngine::Update()
{
	engine_status ret = ENGINE_UPDATE_CONTINUE;
	PrepareUpdate();

	for (auto const& item : list_modules)
	{
		ret = item->Update();
		if (ret != ENGINE_UPDATE_CONTINUE) return ret;
	}

	FinishUpdate();
	return ret;
}

engine_status MantelGameEngine::PostUpdate()
{
	engine_status ret = ENGINE_UPDATE_CONTINUE;

	for (auto const& item : list_modules)
	{
		ret = item->PostUpdate();
		if (ret != ENGINE_UPDATE_CONTINUE) return ret;
	}

	return ret;
}

bool MantelGameEngine::CleanUp()
{
	bool ret = true;

	for (auto const& item : list_modules)
	{
		ret = item->CleanUp();
		if (ret != true) return ret;
	}

	return ret;
}

void MantelGameEngine::AddModule(EModule* mod)
{
	list_modules.push_back(mod);
}