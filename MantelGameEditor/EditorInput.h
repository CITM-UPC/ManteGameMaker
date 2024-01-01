#pragma once
#include "EditorModule.h"

#define MAX_KEYS 300
#define NUM_MOUSE_BUTTONS 5

enum KeyState {
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class EditorInput : public EditorModule {
public:
	EditorInput();
	~EditorInput();

	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	KeyState GetKey(int id) const {
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const {
		return mouseButtons[id - 1];
	}

	vec2 GetMousePosition() const {
		return vec2(mousePosX, mousePosY);
	}

	vec2 GetMouseMotion() const {
		return vec2(mouseMotionX, mouseMotionY);
	}

	int GetMouseWheelScroll() const {
		return mouseWheelScroll;
	}

	bool WindowSizeHasUpdated() const {
		return windowSizeHasUpdated;
	}

private:
	KeyState* keyboard;
	KeyState mouseButtons[NUM_MOUSE_BUTTONS];

	int mousePosX;
	int mousePosY;

	int	mouseMotionX;
	int mouseMotionY;

	int	mouseWheelScroll;

	bool windowSizeHasUpdated;
};

