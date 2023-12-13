#pragma once
#include "GuiBase.h"

class GameObject;

class WindowHierarchy : public GuiBase
{
public:
    WindowHierarchy(string name, bool startEnabled = false, ImGuiWindowFlags flags = ImGuiWindowFlags_None);

    void TreeDeploy(GameObject* item);

    void Update();


    GameObject* lastSelected = nullptr;
};