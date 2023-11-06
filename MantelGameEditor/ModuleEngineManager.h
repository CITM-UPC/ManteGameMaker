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

    void DoCameraInput();
    void keysInputFPS();
    void mouseInputFPS();
    void mouseCamOrbit();
    void mouseCameraPan();
    void DoZoom();


    MyGameEngine* GetEngine() {
        return &engine;
    }

private:

    double camSpeed = 0.1;
    MyGameEngine engine;
};

