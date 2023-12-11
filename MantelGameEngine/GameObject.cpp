#include "GameObject.h"
#include <regex>
#include <list>
GameObject::GameObject(const std::string &name, const std::string &path)
{
    this->fbxPath = path;
    this->texturePath = Mesh::getTexturePathFromFbxPath(path);
    if (name.empty()) {
        std::string correctedPath = path;
        std::regex_replace(correctedPath, std::regex("/"), "\\");
        this->fbxPath = correctedPath;
        // Extract the name from the path
        size_t lastSlashPos = path.find_last_of('\\');
        size_t lastDotPos = path.find_last_of('.');
        if (lastSlashPos != std::string::npos && lastDotPos != std::string::npos) {
            this->name = path.substr(lastSlashPos + 1, lastDotPos - lastSlashPos - 1);
        }
        else {
            // Handle the case when the path doesn't contain a valid name
            this->name = "Unnamed";
        }
    }
    else {
        this->name = name;
    }

    this->mesh_ptr = Mesh::loadFromFile(path);
    this->texture = this->mesh_ptr[0].get()->texture;
}

GameObject::~GameObject()
{
}

void GameObject::Draw()
{
    if (visible)
    {
        GraphicObject house;

        for (const auto& item : this->mesh_ptr)
        {
            GraphicObject mesh(item);
            // set the position from gameobject to the meshes
            mesh.translate(transform.position - mesh.pos());
            // set the rotation from gameobject to the meshes
            mesh.rotate(transform.rotation.x,vec3(1,0,0));
            mesh.rotate(transform.rotation.y,vec3(0,1,0));
            mesh.rotate(transform.rotation.z,vec3(0,0,1));
            // set the scale from gameobject to the meshes
            mesh.scale(transform.scale);

            house.addChild(std::move(mesh));
        }

        GraphicObject root;
        root.addChild(std::move(house));
        
        root.paint();
    }
}
