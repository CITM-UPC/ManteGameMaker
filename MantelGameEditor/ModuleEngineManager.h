#pragma once

#include "Module.h"

#include "..\MantelGameEngine\MantelGameEngine.h"

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
};


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

    //loop so loadstate can iterate across all children
    void LoadTreeLoop(GameObject* parent, pugi::xml_node& nodeGo);
    //load function
    bool LoadState(pugi::xml_node& data);

    //loop so savestate can iterate across all children
    void SaveTreeLoop(GameObject* item, pugi::xml_node gameObjectNode);
    //save function
    bool SaveState(pugi::xml_node& data);


    //mroe functions
    glm::vec3 ScreenToWorldCoordinates(int mouseX, int mouseY, int screenWidth, int screenHeight, const glm::mat4& viewProjectionMatrix);
    // Función para crear un rayo desde la cámara
    Ray CreateRayFromCamera(const glm::vec3& cameraPosition, const glm::vec3& worldCoords);
    bool RayAABBIntersect(const Ray& ray, const glm::vec3& min, const glm::vec3& max, glm::vec3& intersectionPoint);
    GameObject* CheckIntersection(const Ray& ray);

    MyGameEngine* GetEngine() {
        return &engine;
    }

private:

    Ray ray;

    double camSpeed = 0.1;
    MyGameEngine engine;
};
