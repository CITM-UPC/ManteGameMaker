#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>
#include <list>

#include <windows.h>
#include <shellapi.h>

#include "GL/glew.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#include "EditorModule.h"

#include "DualOutputBuffer.h"

#include "../MantelGameEngine/GameApp.h"
#include "../MantelAudioEngine/AudioEngine.h"

using namespace std;
using namespace chrono;

static const unsigned int FPS = 60;
static const auto FDT = 1.0s / FPS;

class EditorInput;
class EditorWindow;
class EditorUI;
class EditorCamera;
class EditorObjectSelector;

class EditorApp {
public:
	EditorApp();
	~EditorApp();

	bool Start();
	bool Update();
	bool Cleanup();

	void PlayGame();
	void StopGame();

	void WebRequest(const char* url);

	EditorInput* editorInput;
	EditorWindow* editorWindow;
	EditorCamera* editorCamera;
	EditorObjectSelector* editorObjectSelector;
	EditorUI* editorUI;

	// Audio Engine
	AudioEngine* audioEngine;

	// Game Engine
	GameApp* gameApp;

	list<EditorModule*> modules;

	void AddLog(string l);

public: 
	vector<float> FPS_Log;

	bool gameIsOn = false;

	//console output access
	std::stringstream consoleOutput;
	DualOutputBuffer* dualOutputBuffer;


private:
};

extern EditorApp* editor;