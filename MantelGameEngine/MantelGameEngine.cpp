#include "MantelGameEngine.h"
#include <GL\glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <vector>
#include <IL/il.h>

#include "CubeImmediateMode.h"
#include "CubeVertexArray.h"
#include "CubeVertexBuffer.h"
#include "CubeInterleavedVBO.h"
#include "CubeWireframeIVBO.h"

#include "Mesh.h"
#include "Transform.h"
#include "GameObject.h"

#include "EGlobals.h"

#include "GraphicObject.h"
#include <iostream>

using namespace std;

static double angle = 0.0;

MyGameEngine::MyGameEngine() {

    ilInit();
    //auto glew_init_error = glewInit();
    //if (glew_init_error != GLEW_OK) throw exception((char*)glewGetErrorString(glew_init_error));
    //if (!GLEW_VERSION_3_1) throw exception("OpenGL 3.1 Not Supported!");
}

void MyGameEngine::step(std::chrono::duration<double> dt) {
    const double angle_vel = 90.0; // degrees per second
	angle += angle_vel * dt.count();
}

static void drawAxis() {
    glLineWidth(4.0);
    glBegin(GL_LINES);
    glColor3ub(255, 0, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(0.8, 0, 0);
    glColor3ub(0, 255, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 0.8, 0);
    glColor3ub(0, 0, 1);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 0, 0.8);
    glEnd();
}

static void drawGrid(int grid_size, int grid_step) {
    glLineWidth(1.0);
    glColor3ub(128, 128, 128);

    glBegin(GL_LINES);
    for (int i = -grid_size; i <= grid_size; i += grid_step) {
        //XY plane
        glVertex2i(i, -grid_size);
        glVertex2i(i,  grid_size);
        glVertex2i(-grid_size, i);
        glVertex2i( grid_size, i);

        //XZ plane
        glVertex3i(i, 0, -grid_size);
        glVertex3i(i, 0, grid_size);
        glVertex3i(-grid_size, 0, i);
        glVertex3i( grid_size, 0, i);
    }
    glEnd();
}

void MyGameEngine::Start()
{
    //default fbx loaded
    GameObject* BakerHouse = new GameObject("BakerHouse", "Assets/BakerHouse.fbx");
    AddGameObject(BakerHouse);
    //default play camera
    GameObject* playCamera = new GameObject("Camera", "", false, true);
    AddGameObject(playCamera);
    //playCamera->GetCamera()->fov = 60;
    //playCamera->GetCamera()->aspect = static_cast<double>(SCREEN_WIDTH) / SCREEN_HEIGHT;
    //playCamera->GetCamera()->zNear = 0.1;
    //playCamera->GetCamera()->zFar = 100;

    actualCamera = &camera;
}


void MyGameEngine::AddGameObject(GameObject* go, bool isChildren, GameObject* parentGo)
{
    std::string originalName = go->GetName();
    std::string newName = originalName;
    int counter = 1;

    while (true)
    {
        bool nameTaken = false;

        for (const auto& item : allGameObjects)
        {
            if (item->GetName() == newName)
            {
                nameTaken = true;
                break;
            }
        }

        if (!nameTaken)
        {
            go->setName(newName);
            if (!isChildren)
            {
                hierarchy.push_back(go);
            }
            else if (isChildren && parentGo->emptyGameObject)
            {
                parentGo->GetChildrenList()->push_back(go);
                go->SetParent(parentGo);
            }
            else
            {
                cout << "Unable to add " << originalName << endl;
            }
            allGameObjects.push_back(go);

            break;
        }

        newName = originalName + "_" + std::to_string(counter);
        counter++;
    }
    std::cout << go->GetName() << " created" << endl;

}

void MyGameEngine::ClearScene()
{
    for (auto it = allGameObjects.begin(); it != allGameObjects.end(); ++it) {
        toDeleteGo.push_back(*it);
    }
    deleteList = true;
    allGameObjects.clear();
    hierarchy.clear();
}

void MyGameEngine::DeleteGameObject(GameObject* go)
{
    std::cout << go->GetName() << " deleted" << endl;

    // Find and remove the GameObject with the same pointer from the hierarchy list
    allGameObjects.remove(go);
    //if it's a parent, also delete its children
    if (go->emptyGameObject && !go->GetChildrenList()->empty())
    {
        auto childrenList = *go->GetChildrenList(); // Copia la lista antes de la modificación

        for (auto& child : childrenList)
        {
            DeleteGameObject(child);
        }
    }
    //if has no parent delete from hierarchy
    if (go->GetParent() == nullptr)
    {
        hierarchy.remove(go);
    }
    //if has parent, delete from parent children's list
    else
    {
        go->GetParent()->GetChildrenList()->remove(go);
    }

    // Now, you should delete the GameObject to free the memory if necessary
    //delete go;
}


void MyGameEngine::render() {


        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(actualCamera->fov, actualCamera->aspect, actualCamera->zNear, actualCamera->zFar);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(actualCamera->transform.position.x, actualCamera->transform.position.y, actualCamera->transform.position.z,
            actualCamera->lookAtPos.x, actualCamera->lookAtPos.y, actualCamera->lookAtPos.z,
            actualCamera->transform.up.x, actualCamera->transform.up.y, actualCamera->transform.up.z);

        drawGrid(100, 1);
        drawAxis();

#pragma region Draw Sandbox

    if (engineState == EngineState::EDITOR)
    {


    }
    if (engineState == EngineState::PLAY)
    {

    }

    //out of conditions cz always renders
    //draw using GameObject class
    for (const auto& item : allGameObjects)
    {
        item->Draw();
    }

#pragma endregion


    //assert(glGetError() ==GL_NONE);
}
