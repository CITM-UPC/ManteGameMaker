#pragma once
#include "EGlobals.h"
#include "GraphicObject.h"
#include "Camera.h"

#include "Transform.h"
#include "Mesh.h"
#include "Texture2D.h"
#include "BBox.hpp"

struct Camera;

class GameObject
{
public:
	GameObject(const std::string &name, const std::string &path, bool emptyGameObject = false, bool camera = false);
	~GameObject();



	void Draw();

	using Ptr = std::shared_ptr<Texture2D>;

public:
	void setName(std::string& name)
	{
		this->name = name;
	}
	const char* GetName()
	{
		return name.c_str();
	}

private:
	std::string name;

public:

	Transform transform;
	//static std::vector<Ptr> mesh_ptr;
	/*static*/ std::vector<Mesh::Ptr> mesh_ptr;
	Texture2D::Ptr texture;

	//important options
	bool selected = false;
	bool visible = true;
	bool showNormals = false;
	
	bool emptyGameObject = false;
	bool expanded = false;

	bool camera = false;

	//paths
	std::string fbxPath;
	std::string texturePath;


	//getters and setters
public:

	void PrintMeshBoundingBox();

	// if gameobject is NOT an EMPTY game object do NOT use this function
	list<GameObject*>* GetChildrenList()
	{
		if (!emptyGameObject)
		{
			cout << "WARNING: GetChildrenList() function called on a NON EMPTY game object, returned list is USELESS" << endl;
		}
		return (emptyGameObject ? &childrenList : nullptr);
	}

	// if gameobject is NOT an CAMERA do NOT use this function
	Camera* GetCamera()
	{
		return (camera ? cameraGo : nullptr);
	}

	GameObject* GetParent()
	{
		return parent;
	}

	void SetParent(GameObject* parent)
	{
		this->parent = parent;
	}

private:

	list<GameObject*> childrenList;

	GameObject* parent = nullptr;

	Camera* cameraGo;
};
