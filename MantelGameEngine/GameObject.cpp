#include "GameObject.h"
#include <regex>
#include <list>
#include <iostream>

using namespace std;

GameObject::GameObject(const std::string& name, const std::string& path, bool emptyGameObject, bool camera)
{
    this->fbxPath = path;
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

    if (!path.empty())
    {
        this->texturePath = Mesh::getTexturePathFromFbxPath(path);
        this->mesh_ptr = Mesh::loadFromFile(path);
        this->texture = this->mesh_ptr[0].get()->texture;
    }
    else
    {
        cout << "Creating GameObject with no fbx" << endl;
    }
    this->emptyGameObject = emptyGameObject;
    if (camera)
    {
        this->camera = camera;
        Camera tCamera;
        tCamera.fov = 60;
        tCamera.aspect = static_cast<double>(SCREEN_WIDTH) / SCREEN_HEIGHT;
        tCamera.zNear = 0.1;
        tCamera.zFar = 100;

        this->cameraGo = &tCamera;
    }
}

GameObject::~GameObject()
{
}

void GameObject::Draw()
{
    if (visible)
    {
        GraphicObject house;

        if (!emptyGameObject)
        {

            for (const auto& item : this->mesh_ptr)
            {
                GraphicObject mesh(item);
                // set the position from gameobject to the meshes
                mesh.translate(transform.position - mesh.pos());
                // set the rotation from gameobject to the meshes
                mesh.rotate(transform.rotation.x, vec3(1, 0, 0));
                mesh.rotate(transform.rotation.y, vec3(0, 1, 0));
                mesh.rotate(transform.rotation.z, vec3(0, 0, 1));
                // set the scale from gameobject to the meshes
                mesh.scale(transform.scale);

                house.addChild(std::move(mesh));
            }

            GraphicObject root;
            root.addChild(std::move(house));

            root.paint();
        }
        else
        {
            for (const auto& child : this->childrenList)
            {
                for (const auto& item : child->mesh_ptr)
                {
                    GraphicObject mesh(item);
                    // set the position from gameobject to the meshes
                    mesh.translate(transform.position - mesh.pos());
                    // set the rotation from gameobject to the meshes
                    mesh.rotate(transform.rotation.x, vec3(1, 0, 0));
                    mesh.rotate(transform.rotation.y, vec3(0, 1, 0));
                    mesh.rotate(transform.rotation.z, vec3(0, 0, 1));
                    // set the scale from gameobject to the meshes
                    mesh.scale(transform.scale);

                    house.addChild(std::move(mesh));
                }
            }
        }
    }
}
