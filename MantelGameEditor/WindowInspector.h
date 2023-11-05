#pragma once
#include "GuiBase.h"
class WindowInspector : public GuiBase
{
public:
	WindowInspector(string name, bool startEnabled = false, ImGuiWindowFlags flags = ImGuiWindowFlags_None);

	void Update();
};

