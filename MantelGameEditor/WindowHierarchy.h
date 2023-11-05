#pragma once
#include "GuiBase.h"
class WindowHierarchy : public GuiBase
{
public:
    WindowHierarchy(string name, bool startEnabled = false, ImGuiWindowFlags flags = ImGuiWindowFlags_None);

    void Update();
};