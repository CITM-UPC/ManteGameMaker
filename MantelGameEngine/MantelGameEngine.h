#pragma once

#include <chrono>
#include <list>

#include "types.h"
#include "Camera.h"
#include "GameObject.h"

enum EngineState
{
	EDITOR = 0,
	PLAY,
	PAUSE,
	STOP

};

class MyGameEngine
{
public:
	Camera* actualCamera;

	Camera camera, cameraStatic;

	EngineState engineState = EngineState:: EDITOR;

	MyGameEngine();
	void step(std::chrono::duration<double> dt);
	void Start();
	void render();

	//restart cam options
	void ResetActualCamera();

	// adds a gameobject, check out bools
	void AddGameObject(GameObject* go, bool isChildren = false, GameObject* parentGo = nullptr);

	//delete all gameobjects from scene
	void ClearScene();

	//defenitive metod to delete a game object from everywhere in the scene
	void DeleteGameObject(GameObject* go);

	int selectedGameObject = 0;

	std::list<GameObject*> hierarchy;
	std::list<GameObject*> allGameObjects;
	std::list<GameObject*> toDeleteGo;

	bool deleteList = false;
};

