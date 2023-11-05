#pragma once
#include "GuiBase.h"

class WindowSettings : public GuiBase
{
public:
	WindowSettings(string name, bool startEnabled = false, ImGuiWindowFlags flags = ImGuiWindowFlags_None);

	void Update();
};

