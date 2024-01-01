#include "GameApp.h"
#include <GL\glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <vector>
#include <IL/il.h>
#include "MeshLoader.h"

using namespace std;

static double angle = 0.0;

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
        glVertex2i(i, grid_size);
        glVertex2i(-grid_size, i);
        glVertex2i(grid_size, i);

        //XZ plane
        glVertex3i(i, 0, -grid_size);
        glVertex3i(i, 0, grid_size);
        glVertex3i(-grid_size, 0, i);
        glVertex3i(grid_size, 0, i);
    }
    glEnd();
}

GameApp::GameApp()
{
    ilInit();
    AddLog("IL Init");

    scene = new Scene("TestScene");
}

GameApp::~GameApp()
{

}

void GameApp::EditorStart() {

    std::filesystem::create_directories("Library/Meshes/");
    std::filesystem::create_directories("Library/Materials/");

    std::string path = "Assets";
    
    for (const auto& entry : filesystem::directory_iterator(path)) {
        Paths currentAsset;
        if (entry.path().string().ends_with(".fbx")) {
            vector<string> libraryStrings = MeshLoader::loadFromFile(entry.path().generic_string());
            for (int i = 0; i < libraryStrings.size(); ++i)
                currentAsset.libraryPath.push_back(libraryStrings[i]);
        }
        else {
            currentAsset.libraryPath.push_back(Texture2D::transformToDDS(entry.path().string()));
        }

        currentAsset.assetsPath = entry.path().generic_string();
        currentAsset.name = entry.path().filename().string();

        allAssets.push_back(currentAsset);
    }

    // scene->DebugStart();
    std::vector<std::string> housePath, streetPath;
    for (int i = 0; i < allAssets.size(); ++i) {
        if (allAssets[i].name == std::string("Street_environment.fbx")) {
            streetPath = allAssets[i].libraryPath;
        }
        else if (allAssets[i].name == std::string("BakerHouse.fbx")) {
            housePath = allAssets[i].libraryPath;
        }
    }

    house = scene->AddGameObject("House Object");
    auto mesh_ptrs = Mesh::loadFromFile(housePath);
    house->AddMeshWithTexture(mesh_ptrs);

    AddLog("BakerHouse.fbx loaded");
    
    street = scene->AddGameObject("Street Object");
    mesh_ptrs = Mesh::loadFromFile(streetPath);
    street->AddMeshWithTexture(mesh_ptrs);
    street->GetComponent<TransformComponent>()->rotate(-90, {1, 0, 0});

    AddLog("Street environment_V01.fbx loaded");

    auto transformHouse = house->GetComponent<TransformComponent>();
    transformHouse->rotate(30, vec3(1, 0, 1));
    transformHouse->translate(vec3(0, -10, 10));
    transformHouse->scale(vec3(1, 1, 1));

    //auto transformStreet = street->GetComponent<TransformComponent>();
    //transformStreet->rotate(-90, vec3(1, 0, 0));

    basicCamera = scene->AddGameObject("Camera Testing");
    basicCamera->AddComponent(CAMERA);
    {
        Camera* cameraToSet = basicCamera->GetComponent<CameraComponent>()->getCamera();
        cameraToSet->zFar = 20;
        cameraToSet->fov = 10;
        basicCamera->GetComponent<CameraComponent>()->setCamera(*cameraToSet);
    }
    basicCamera->GetComponent<TransformComponent>()->translate({ 0, 10, -5 }, GLOBAL);
    basicCamera->GetComponent<TransformComponent>()->rotate(10, { 0, 0, 1 });

}

void GameApp::EditorStep(std::chrono::duration<double> dt)
{

}

void GameApp::GameStart()
{

}

void GameApp::GameStep(std::chrono::duration<double> dt)
{
    //auto childHouse = house->children.begin()->get();
    //childHouse->GetComponent<TransformComponent>()->rotate(1, vec3(1, 0, 0));
    //house->GetComponent<TransformComponent>()->rotate(1, vec3(0, 1, 0));

    basicCamera->GetComponent<TransformComponent>()->translate({0, -0.1, 0}, GLOBAL);

}

// (i think) camera has to be global -> not being a child --TODO--
void GameApp::EditorRender(CameraComponent* camera)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera->getCamera()->fov, camera->getCamera()->aspect, camera->getCamera()->zNear, camera->getCamera()->zFar);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    vec3 center = camera->getTransform()->getPosition() + camera->getTransform()->getForward();

    gluLookAt(camera->getTransform()->getPosition().x, camera->getTransform()->getPosition().y, camera->getTransform()->getPosition().z,
        center.x, center.y, center.z,
        camera->getTransform()->getUp().x, camera->getTransform()->getUp().y, camera->getTransform()->getUp().z);

    drawGrid(100, 1);
    drawAxis();

#pragma region Draw Sandbox

    if (useBasicCameraWithFrustum) {
        // only for debug/showing purposes
        scene->Render(basicCamera->GetComponent<CameraComponent>()->getCamera()->createFrustum(basicCamera->GetComponent<TransformComponent>()->getTransform()), true);
    }
    else {
        // this camera is what it should be used normally
        scene->Render(camera->getCamera()->createFrustum(camera->getTransform()->getTransform()), true);
    }

#pragma endregion


    assert(glGetError() == GL_NONE);
}

// (i think) camera has to be global -> not being a child --TODO--
void GameApp::GameRender(CameraComponent* camera)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera->getCamera()->fov, camera->getCamera()->aspect, camera->getCamera()->zNear, camera->getCamera()->zFar);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // this position we have to make it global for it to be able to be as a child
    vec3 center = camera->getTransform()->getPosition() + camera->getTransform()->getForward();

    gluLookAt(camera->getTransform()->getPosition().x, camera->getTransform()->getPosition().y, camera->getTransform()->getPosition().z,
        center.x, center.y, center.z,
        camera->getTransform()->getUp().x, camera->getTransform()->getUp().y, camera->getTransform()->getUp().z);

    drawGrid(100, 1);
    drawAxis();

#pragma region Draw Sandbox

    if (useBasicCameraWithFrustum) {
        // only for debug/showing purposes
        scene->Render(basicCamera->GetComponent<CameraComponent>()->getCamera()->createFrustum(basicCamera->GetComponent<TransformComponent>()->getTransform()), true);
    }
    else {
        // this camera is what it should be used normally
        scene->Render(camera->getCamera()->createFrustum(camera->getTransform()->getTransform()), true);
    }
#pragma endregion


    assert(glGetError() == GL_NONE);
}

void GameApp::CleanUp()
{
    // memory leaks --TODO--
    scene->CleanUp();
}

void GameApp::AddLog(string l)
{
    logs.push_back(l);
}

vector<string> GameApp::GetLogs()
{
    return logs;
}

void GameApp::ClearLogs()
{
    logs.clear();
}

