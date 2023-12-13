#include "GameObject.h"
#include <GL/glew.h>
#include <regex>
#include <list>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
        tCamera.transform = this->transform;

        this->cameraGo = &tCamera;
    }
}

GameObject::~GameObject()
{
}

void DrawBoundingBox(const glm::vec3& min, const glm::vec3& max) {
    // Dibuja las líneas de la bounding box
    glLineWidth(2);
    glBegin(GL_LINES);

    // horizontal lines bounding box
    glVertex3f(min.x, min.y, min.z);
    glVertex3f(max.x, min.y, min.z);

    glVertex3f(min.x, max.y, min.z);
    glVertex3f(max.x, max.y, min.z);

    glVertex3f(min.x, max.y, max.z);
    glVertex3f(max.x, max.y, max.z);

    glVertex3f(min.x, min.y, max.z);
    glVertex3f(max.x, min.y, max.z);

    // vertical lines bounding box
    glVertex3f(min.x, min.y, min.z);
    glVertex3f(min.x, max.y, min.z);

    glVertex3f(max.x, min.y, min.z);
    glVertex3f(max.x, max.y, min.z);

    glVertex3f(max.x, min.y, max.z);
    glVertex3f(max.x, max.y, max.z);

    glVertex3f(min.x, min.y, max.z);
    glVertex3f(min.x, max.y, max.z);

    glEnd();
}

//void GameObject::PrintMeshBoundingBox()
//{
//    for (const auto& mesh : this->mesh_ptr) {
//        glm::vec3 minBounds = mesh->boundingBoxMin;
//        glm::vec3 maxBounds = mesh->boundingBoxMax;
//
//        float width = maxBounds.x - minBounds.x;
//        float height = maxBounds.y - minBounds.y;
//        float depth = maxBounds.z - minBounds.z;
//
//        //std::cout << "Mesh Size: Width = " << width << ", Height = " << height << ", Depth = " << depth << std::endl;
//    }
//}



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
    if (selected)
    {
        for (const auto& mesh : mesh_ptr)
        {
            DrawBoundingBox(mesh.get()->boundingBoxMin, mesh.get()->boundingBoxMax);
        }
    }
}
