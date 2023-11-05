#pragma once
#include "GuiBase.h"
class WindowProject : public GuiBase
{
public:
	WindowProject(string name, bool startEnabled = false, ImGuiWindowFlags flags = ImGuiWindowFlags_None);

	void Update();
};