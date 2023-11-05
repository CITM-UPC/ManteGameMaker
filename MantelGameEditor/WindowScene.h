#pragma once
#include "GuiBase.h"
#include "FrameBuffer.h"

class WindowScene : public GuiBase
{
public:
	WindowScene(string name, bool startEnabled = false, ImGuiWindowFlags flags = ImGuiWindowFlags_None);

	void Update();

	FrameBuffer SceneFBO;
};