#pragma once

#include "Module.h"

#include "..\MantelGameEngine\MantelGameEngine.h"

class ModuleEngineManager : public Module
{
public:
    ModuleEngineManager(bool startEnabled);
    ~ModuleEngineManager();

    void Awake();
    void Start();

    bool PreUpdate();
    bool Update(std::chrono::duration<double> dt);
    bool PostUpdate();

    void CleanUp();

private:
    MyGameEngine engine;
};

