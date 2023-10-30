#pragma once

class MantelGameEngine;

class EModule
{
private:
	bool enabled;

public:
	MantelGameEngine* gEngine;

	EModule(bool start_enabled = true)
	{}

	virtual ~EModule()
	{}

	virtual bool Init()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual engine_status PreUpdate()
	{
		return ENGINE_UPDATE_CONTINUE;
	}

	virtual engine_status Update()
	{
		return ENGINE_UPDATE_CONTINUE;
	}

	virtual engine_status PostUpdate()
	{
		return ENGINE_UPDATE_CONTINUE;
	}

	virtual bool CleanUp()
	{
		return true;
	}
};