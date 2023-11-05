#pragma once

#include "Globals.h"

class GuiBase
{
public:
	GuiBase(string name, bool startEnabled, ImGuiWindowFlags flags = ImGuiWindowFlags_None) : name(name), flags(flags) {
		if (startEnabled) { this->windowState = state::enabled; }
		else if (!startEnabled) { this->windowState = state::disabled; }
	};
	~GuiBase() {}

	void UpdateWindow() {
		this->PreUpdate();

		static bool state = (bool)windowState;
		ImGui::Begin(name.c_str(), &state, flags);
		if (state) { this->windowState = state::enabled; }
		else if (!state) { this->windowState = state::disabled; }

		this->Update();

		ImGui::End();
	}

	state GetWindowState() {
		return windowState;
	}

	void WindowChangeState(state newState) {
		this->windowState = newState;
	}

	string GetName() {
		return this->name;
	}

private:
	virtual void PreUpdate() {};
	virtual void Update() {};

	string name;
	state windowState = state::disabled;

public:
	ImGuiWindowFlags flags;
};