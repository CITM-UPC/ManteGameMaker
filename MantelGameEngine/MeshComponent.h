#pragma once
#include "Component.h"
#include "Mesh.h"

class MeshComponent : public Component {
public:

	MeshComponent(GameObject* owner);
	~MeshComponent();

	void setMesh(Mesh::Ptr mesh);
	Mesh::Ptr getMesh();

protected:

private:
	Mesh::Ptr _mesh = nullptr;
};