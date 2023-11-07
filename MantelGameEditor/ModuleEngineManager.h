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

    //camera functions
    void CameraLogicInput();
    void CameraKeyboardInput();
    void CameraInputUsingMouse();
    void CameraOrbitUsingMouse();
    void CameraPanUsingMouse();
    void CameraZoom();


    MyGameEngine* GetEngine() {
        return &engine;
    }

private:

    double camSpeed = 0.1;
    MyGameEngine engine;
};
