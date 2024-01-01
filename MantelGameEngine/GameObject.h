#pragma once
#include <string>
#include <list>

#include <memory>

#include "Types.h"

#include "TransformComponent.h"
#include "MeshComponent.h"
#include "TextureComponent.h"
#include "CameraComponent.h"

#include "BoundingBox.h"

using namespace std;

enum GameObjectTypes {
	EMPTY, 
	OBJECT, 
	CAMERA_OBJECT
};

class GameObject {
public:
	GameObject();
	GameObject(string name);
	GameObject(GameObjectTypes type, string name);
	GameObject(GameObject* ref);

	~GameObject();

	GameObject* AddNewChildren();
	GameObject* AddNewChildren(GameObjectTypes GOType);

	void AddChild(unique_ptr<GameObject> child);
	unique_ptr<GameObject> RemoveChild(GameObject* child);

	// void DeleteChild(GameObject* child);

	// THE CHILD IS MOVED, THE REFERENCE IS LOST, THE CHILD HAS TO BE MOVED AGAIN
	unique_ptr<GameObject> FindChild(GameObject* child);

	Component* AddComponent(ComponentType type);

	template <typename T> T* GetComponent();

	void AddMeshWithTexture(std::vector<Mesh::Ptr> meshes);
	void AddMeshWithTexture(Mesh::Ptr meshes);

	void Render(Frustum frustum, bool drawBoundingBox = false);

	void DrawBoundingBox(const AABBox& aabb);
	AABBox GetBoundingBox();

	AABBox GetGlobalBoundingBox();

	// uuid id
	string name;
	// should be a list of uuids
	list<unique_ptr<GameObject>> children;
	GameObject* _parent = nullptr;
	
	list<unique_ptr<Component>> components;
};

template<typename T>
inline T* GameObject::GetComponent()
{
	for (auto component = components.begin(); component != components.end(); ++component) {
		T* returnComponent = dynamic_cast<T*>((*component).get());
		if (returnComponent) {
			return returnComponent;
		}
	}
	return nullptr;
}
