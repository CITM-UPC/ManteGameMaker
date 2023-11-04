#pragma once

#include "Globals.h"

class GUI_Window
{
public:
	GUI_Window(string name, bool startEnabled, ImGuiWindowFlags flags = ImGuiWindowFlags_None) : name(name), flags(flags) {
		if (startEnabled) { this->windowState = States::ENABLED; }
		else if (!startEnabled) { this->windowState = States::DISABLED; }
	};
	~GUI_Window() {}

	void UpdateWindow() {
		this->PreUpdate();

		static bool state;
		if (windowState == States::ENABLED) { state = true; }
		else if (windowState == States::DISABLED) { state = false; }
		ImGui::Begin(name.c_str(), &state, flags);
		if (state) { this->windowState = States::ENABLED; }
		else if (!state) { this->windowState = States::DISABLED; }

		this->Update();

		ImGui::End();
	}

	States GetWindowState() {
		return windowState;
	}

	void WindowChangeState(States newState) {
		this->windowState = newState;
	}

	string GetName() {
		return this->name;
	}

private:
	virtual void PreUpdate() {};
	virtual void Update() {};

	string name;
	States windowState = States::DISABLED;

public:
	ImGuiWindowFlags flags;
};