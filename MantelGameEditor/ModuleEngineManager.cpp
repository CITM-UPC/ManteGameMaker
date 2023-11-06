#include "ModuleEngineManager.h"
#include "App.h"
#include "Globals.h"

ModuleEngineManager::ModuleEngineManager(bool startEnabled) : Module(startEnabled)
{
    name = "renderer";
}

ModuleEngineManager::~ModuleEngineManager() {}

void ModuleEngineManager::Awake() {
    engine.camera.fov = 60;
    engine.camera.aspectRatio = static_cast<double>(SCREEN_WIDTH) / SCREEN_HEIGHT;
    engine.camera.clippingPlaneViewNear = 0.1;
    engine.camera.clippingPlaneViewFar = 100;
    //engine.camera.eye = vec3(5, 1.75, 5);
    //engine.camera.center = vec3(0, 1, 0);
    //engine.camera.up = vec3(0, 1, 0);
}

void ModuleEngineManager::Start() {

}

bool ModuleEngineManager::PreUpdate() {

    return true;
}

bool ModuleEngineManager::Update(duration<double> dt) {
    engine.step(app->GetDeltaTime());

	DoCameraInput();

    return true;
}

bool ModuleEngineManager::PostUpdate() {
    engine.render(); //engine render
    
    return true;
}

void ModuleEngineManager::CleanUp() {

}


void ModuleEngineManager::DoCameraInput()
{
	DoZoom();

	if (app->input->GetMouseButton(SDL_BUTTON_RIGHT))
	{
		keysInputFPS();
		mouseInputFPS();

		engine.camera.UpdateLookAt();
	}
	if (app->input->GetKey(SDL_SCANCODE_LALT))
	{
		if (app->input->GetMouseButton(SDL_BUTTON_LEFT))
		{
			mouseCamOrbit();
		}
		if (app->input->GetMouseButton(SDL_BUTTON_MIDDLE))
		{
			mouseCameraPan();
		}
	}
}

void ModuleEngineManager::keysInputFPS()
{
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) { camSpeed = 0.2; }
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP) { camSpeed = 0.1; }

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		engine.camera.transform.Move(glm::dvec3(0, 0, camSpeed));
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		engine.camera.transform.Move(glm::dvec3(0, 0, -camSpeed));
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		engine.camera.transform.Move(glm::dvec3(camSpeed, 0, 0));
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		engine.camera.transform.Move(glm::dvec3(-camSpeed, 0, 0));
	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		engine.camera.transform.Move(glm::dvec3(0, camSpeed, 0), Transform::Space::GLOBAL);
	}
	if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		engine.camera.transform.Move(glm::dvec3(0, -camSpeed, 0), Transform::Space::GLOBAL);
	}
}
void ModuleEngineManager::mouseInputFPS()
{
	float sensitivity = 0.1;

	int dx = app->input->GetMouseXMotion();
	int dy = -app->input->GetMouseYMotion();

	engine.camera.transform.Rotate(glm::vec3(0, dx * sensitivity, 0), Transform::Space::GLOBAL);
	engine.camera.transform.Rotate(glm::vec3(dy * sensitivity, 0, 0));
}

void ModuleEngineManager::mouseCamOrbit()
{
	float sensitivity = 0.2;

	int dx = app->input->GetMouseXMotion();
	int dy = -app->input->GetMouseYMotion();

	engine.camera.transform.MoveTo(engine.camera.lookAtPos);

	engine.camera.transform.Rotate(vec3(0, dx * sensitivity, 0), Transform::Space::GLOBAL);
	engine.camera.transform.Rotate(vec3(dy * sensitivity, 0, 0));

	vec3 finalPos = engine.camera.transform.position - (engine.camera.transform.forward * engine.camera.camOffset);
	engine.camera.transform.MoveTo(finalPos);
}
void ModuleEngineManager::mouseCameraPan()
{
	int dx = app->input->GetMouseXMotion();
	int dy = app->input->GetMouseYMotion();

	float panSpeed = 0.01f;

	engine.camera.transform.Move(vec3(dx * panSpeed, 0, 0));
	engine.camera.transform.Move(vec3(0, dy * panSpeed, 0));

	engine.camera.UpdateLookAt();
}
void ModuleEngineManager::DoZoom()
{
	int scrollWheel = app->input->GetMouseZ();

	if (scrollWheel != 0)
	{
		float zoomSensitivity = 0.3f;

		engine.camera.camOffset -= scrollWheel * zoomSensitivity;

		if (engine.camera.camOffset <= 0.1f)
		{
			engine.camera.camOffset = 0.1f;
			return;
		}
		else if (engine.camera.camOffset >= 50.0f)
		{
			engine.camera.camOffset = 50.0f;
			return;
		}

		engine.camera.transform.Move(vec3(0, 0, scrollWheel * zoomSensitivity));
	}
}
