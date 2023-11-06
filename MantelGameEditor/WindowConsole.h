#pragma once
#include "GuiBase.h"

class WindowConsole : public GuiBase
{
public:
	WindowConsole(string name, bool startEnabled = false, ImGuiWindowFlags flags = ImGuiWindowFlags_None);
	~WindowConsole();

	void Update();
	

};