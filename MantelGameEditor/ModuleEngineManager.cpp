#include "ModuleEngineManager.h"
#include "App.h"

ModuleEngineManager::ModuleEngineManager(bool startEnabled) : Module(startEnabled)
{
    name = "renderer";
}

ModuleEngineManager::~ModuleEngineManager() {}

void ModuleEngineManager::Awake() {
    engine.camera.fov = 60;
    engine.camera.aspect = static_cast<double>(SCREEN_WIDTH) / SCREEN_HEIGHT;
    engine.camera.zNear = 0.1;
    engine.camera.zFar = 100;
    engine.camera.eye = vec3(5, 1.75, 5);
    engine.camera.center = vec3(0, 1, 0);
    engine.camera.up = vec3(0, 1, 0);
}

void ModuleEngineManager::Start() {

}

bool ModuleEngineManager::PreUpdate() {

    return true;
}

bool ModuleEngineManager::Update(duration<double> dt) {
    engine.step(app->GetDeltaTime());

    return true;
}

bool ModuleEngineManager::PostUpdate() {
    engine.render(); //engine render
    
    return true;
}

void ModuleEngineManager::CleanUp() {

}