#include "GameObject.h"
#include <list>
GameObject::GameObject(const std::string &name, const std::string &path)
{
    this->name = name;
    this->mesh_ptr = Mesh::loadFromFile(path);
    //this->texture = this->mesh_ptr[0].get()->texture;
}

GameObject::~GameObject()
{
}

void GameObject::Draw()
{
    if (visible)
    {
        GraphicObject mesh1(this->mesh_ptr.front());
        GraphicObject mesh2(this->mesh_ptr.back());

        GraphicObject house;

        house.addChild(std::move(mesh1));
        house.addChild(std::move(mesh2));

        GraphicObject root;
        root.addChild(std::move(house));

        root.paint();
    }
}
