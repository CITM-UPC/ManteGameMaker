#pragma once
#include "EGlobals.h"
#include "GraphicObject.h"

#include "Transform.h"
#include "Mesh.h"
#include "Texture2D.h"

class GameObject
{
public:
	GameObject(const std::string &name, const std::string &path);
	~GameObject();

	void Draw();

	using Ptr = std::shared_ptr<Texture2D>;

	Transform transform;
	//static std::vector<Ptr> mesh_ptr;
	static std::vector<Mesh::Ptr> mesh_ptr;
	Texture2D::Ptr texture;

	//important options
	bool selected = false;
	bool visible = true;
	
	//getters and setters
public:
	void setName(std::string &name)
	{
		this->name = name;
	}
	const char* GetName()
	{
		return name.c_str();
	}

private:
	std::string name;
};
