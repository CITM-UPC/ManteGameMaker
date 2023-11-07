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
        //GraphicObject mesh1(this->mesh_ptr.front());
        //GraphicObject mesh2(this->mesh_ptr.back());

        GraphicObject house;

        for (const auto& item : this->mesh_ptr)
        {
            GraphicObject mesh(item);
            house.addChild(std::move(mesh));
        }


        //house.addChild(std::move(mesh1));
        //house.addChild(std::move(mesh2));

        GraphicObject root;
        root.addChild(std::move(house));

        root.paint();
    }
}
