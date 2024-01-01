#pragma once
#include <chrono>
#include <string>
#include <filesystem>
#include <list>

#include "Types.h"
#include "GameRenderer.h"
#include "GL/glew.h"
#include "glm/ext/matrix_transform.hpp"

#include "Scene.h"

struct Paths {

	std::string name;
	std::string assetsPath;
	std::vector<std::string> libraryPath;

};

class GameApp
{
public:
	GameApp();
	~GameApp();

	//enum class RenderModes { RELEASE, DEBUG };

	void EditorStart();
	void EditorStep(std::chrono::duration<double> dt);
	void GameStart();
	void GameStep(std::chrono::duration<double> dt);
	void EditorRender(CameraComponent* camera);
	void GameRender(CameraComponent* camera);
	void CleanUp();

	vec3 positionCube = { 0, 0, 0 }, rotationCube = { 0, 0, 0 }, scaleCube = { 1, 1, 1 };

	Scene* scene = nullptr;
	
	GameObject* house = nullptr;
	GameObject* street = nullptr;

	GameObject* basicCamera = nullptr;
	bool useBasicCameraWithFrustum = false;

	vector<Paths> allAssets;

	void AddLog(string l);

	vector<string> GetLogs();

	void ClearLogs();

private:
	vector<string> logs;

};



