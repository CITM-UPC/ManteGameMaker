#pragma once
#include <string>
#include <list>
#include "GameObject.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

using namespace std;

class Scene {
public:
	Scene();
	Scene(string name);
	~Scene();

	GameObject* AddGameObject();
	GameObject* AddGameObject(string name);

	GameObject* AddGameObjectChildren(GameObject* parent);

	GameObject* DuplicateGameObject(GameObject* original);

	// void DeleteGameObject(GameObject* toDelete);

	unique_ptr<GameObject> FindChild(GameObject* child);

	// if toParent is nullptr the child goes to the scene
	void MoveChildToAnotherParent(GameObject* child, GameObject* toParent = nullptr);

	void DebugStart();

	// --TODO--
	void Start();

	// --TODO--
	void Update();

	void Render(Frustum frustum, bool drawBoundingBox = false);

	void CleanUp();

	void DrawBoundingBox(const AABBox& aabb);

	AABBox GetBoundingBox();

	bool BoundingBoxInFrustum(Frustum frustum);

	void LoadScene(const std::string& path);

	void SaveTemporalScene();

	void SaveScene();

	// uuid id
	string name;
	// should be a list of uuids
	list<unique_ptr<GameObject>> children;

	AABBox aabb;

};