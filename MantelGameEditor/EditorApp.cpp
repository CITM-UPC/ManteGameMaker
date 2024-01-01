#include "EditorApp.h"

#include "EditorWindow.h"
#include "EditorInput.h"
#include "EditorUI.h"
#include "EditorCamera.h"
#include "EditorObjectSelector.h"

EditorApp::EditorApp() {

	editorInput = new EditorInput();
	editorWindow = new EditorWindow();
	editorCamera = new EditorCamera();
	editorObjectSelector = new EditorObjectSelector();
	editorUI = new EditorUI();

	modules.push_back(editorInput);
	modules.push_back(editorWindow);
	modules.push_back(editorCamera);
	modules.push_back(editorObjectSelector);
	modules.push_back(editorUI);

	FPS_Log.push_back(0.0f);

	gameApp = new GameApp();
}

EditorApp::~EditorApp()
{
}

bool EditorApp::Start() {

	bool ret = true;

	// sdl init
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw exception(SDL_GetError());

	SDL_version compiled;
	SDL_VERSION(&compiled);
	cout << "SDL Compiled with " << to_string(compiled.major) << '.' << to_string(compiled.minor) << '.' << to_string(compiled.patch);
	AddLog("SDL Compiled with " + to_string(compiled.major) + '.' + to_string(compiled.minor) + '.' + to_string(compiled.patch));

	SDL_version linked;
	SDL_GetVersion(&linked);
	cout << "SDL Linked with " << to_string(linked.major) << '.' << to_string(linked.minor) << '.' << to_string(linked.patch);
	AddLog("SDL Linked with " + to_string(linked.major) + '.' + to_string(linked.minor) + '.' + to_string(linked.patch));

	// module start
	{
		auto item = modules.begin();

		while (item != modules.end() && ret == true) {
			ret = (*item)->Start();
			++item;
		}
	}

	gameApp->EditorStart();

	return ret;
}

bool EditorApp::Update() {

	const auto frame_start = steady_clock::now();

	bool ret = true;

	// pre update
	{
		auto item = modules.begin();

		while (item != modules.end() && ret == true) {
			ret = (*item)->PreUpdate();
			++item;
		}
	}

	// update
	{
		auto item = modules.begin();

		while (item != modules.end() && ret == true) {
			ret = (*item)->Update();
			++item;
		}

		gameApp->EditorStep(FDT);
		if (gameIsOn) {
			gameApp->GameStep(FDT);
		}
	}

	// post update
	{
		if (gameIsOn) {
			gameApp->GameRender(editorCamera->cameraObject.get()->GetComponent<CameraComponent>());
		}
		else {
			gameApp->EditorRender(editorCamera->cameraObject.get()->GetComponent<CameraComponent>());

		}

		auto item = modules.begin();

		while (item != modules.end() && ret == true) {
			ret = (*item)->PostUpdate();
			++item;
		}

		editorWindow->Render();
	}

	// get game logs
	{
		vector<string> gameLogs = gameApp->GetLogs();

		for (auto it = gameLogs.begin(); it != gameLogs.end(); ++it) {
			AddLog((*it));
		}

		gameApp->ClearLogs();
	}

	const auto frame_end = steady_clock::now();
	const auto frame_duration = frame_end - frame_start;

	if (frame_duration < FDT)
		this_thread::sleep_for(FDT - frame_duration);

	const auto frameEndAfterSleep = steady_clock::now();
	const auto frameDurationAfterSleep = frameEndAfterSleep - frame_start;

	float lastFPS = 1.0f / (frameDurationAfterSleep.count() * 0.000000001f);

	FPS_Log.push_back(lastFPS);
	if (FPS_Log.size() > 100) {
		FPS_Log.erase(FPS_Log.begin());
	}

	return ret;
}

bool EditorApp::Cleanup() {

	bool ret = true;

	gameApp->CleanUp();

	// clean up is made in reverse
	modules.reverse();

	auto item = modules.begin();

	while (item != modules.end() && ret == true) {
		ret = (*item)->CleanUp();
		++item;
	}

	SDL_Quit();

	return true;
}

void EditorApp::PlayGame()
{
	gameApp->GameStart();
	gameIsOn = true;
}

void EditorApp::StopGame()
{
	gameApp->CleanUp();
	gameIsOn = false;
	gameApp->EditorStart();
}

void EditorApp::WebRequest(const char* url) {

	//ShellExecute(0, 0, url, 0, 0, SW_SHOW);
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

void EditorApp::AddLog(string l) { 
	logs.push_back(l); 
}

vector<string> EditorApp::GetLogs() { 
	return logs; 
}

void EditorApp::ClearLogs() { 
	logs.clear(); 
}