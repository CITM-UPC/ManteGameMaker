#pragma once
#include "GuiBase.h"
#include <filesystem>

namespace fs = std::filesystem;


class WindowProject : public GuiBase
{
public:
	WindowProject(string name, bool startEnabled = false, ImGuiWindowFlags flags = ImGuiWindowFlags_None);

	void Update();

	void HandleDropEvent();

private:

	void DrawFileTree(const fs::path& path);

public:
	std::string selectedFilePath;
};