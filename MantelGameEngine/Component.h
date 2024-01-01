#pragma once

enum ComponentType {
	TRANSFORM,
	MESH,
	TEXTURE,
	CAMERA

};

class GameObject;

class Component {
public:
	Component(GameObject* owner) { this->owner = owner; };
	~Component() {};
	virtual void Start() {};
	virtual void Update() {};

	ComponentType componentType = TRANSFORM;

protected:
	// used to connect to other components
	GameObject* owner;

private: 

};
