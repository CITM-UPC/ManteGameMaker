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

#include "../FrogGameEngine/GameApp.h"

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

	GameApp* gameApp;

	list<EditorModule*> modules;

	void AddLog(string l);
	vector<string> GetLogs();
	void ClearLogs();

public: 
	vector<float> FPS_Log;

	bool gameIsOn = false;
private:
	vector<string> logs;
};

extern EditorApp* editor;