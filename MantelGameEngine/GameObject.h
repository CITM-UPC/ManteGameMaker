#pragma once
#include "EGlobals.h"
#include "Transform.h"
#include "Mesh.h"
#include "Texture2D.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	using Ptr = std::shared_ptr<Texture2D>;

	Transform transform;
	static std::vector<Ptr> mesh_ptr;
	Texture2D texture;

	bool selected = false;
};
