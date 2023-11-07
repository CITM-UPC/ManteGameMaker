#include "ModuleEngineManager.h"
#include "App.h"
#include "Globals.h"

ModuleEngineManager::ModuleEngineManager(bool startEnabled) : Module(startEnabled)
{
    name = "renderer";
}

ModuleEngineManager::~ModuleEngineManager() {}

void ModuleEngineManager::Awake() {
	engine.Start();
    engine.camera.fov = 60;
    engine.camera.aspect = static_cast<double>(SCREEN_WIDTH) / SCREEN_HEIGHT;
    engine.camera.zNear = 0.1;
    engine.camera.zFar = 100;
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

	CameraLogicInput();

    return true;
}

bool ModuleEngineManager::PostUpdate() {
    engine.render(); //engine render
    
    return true;
}

void ModuleEngineManager::CleanUp() {

}


void ModuleEngineManager::CameraLogicInput()
{
	//if right button mouse, different input (principal wasd)
	if (app->input->GetMouseButton(SDL_BUTTON_RIGHT))
	{
		CameraKeyboardInput();
		CameraInputUsingMouse();

		engine.camera.UpdateLookAt();
	}

	//if alt button, all about mouse motion
	if (app->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{

		//commented because it causes bugs in camera control :(

		//IF CURSOR GOES OUT THE WINDOW IT WILL RETURN FROM THE OTHER SIDE
		//if (app->input->GetMouseX() >= SCREEN_WIDTH - 1)
		//{
		//	POINT p;
		//	GetCursorPos(&p);
		//	SetCursorPos(p.x - (SCREEN_WIDTH - 1), p.y);
		//}
		//if (app->input->GetMouseX() <= 1)
		//{
		//	POINT p;
		//	GetCursorPos(&p);
		//	SetCursorPos(p.x + (SCREEN_WIDTH - 1), p.y);
		//}
		//if (app->input->GetMouseY() >= SCREEN_HEIGHT - 1)
		//{
		//	POINT p;
		//	GetCursorPos(&p);
		//	SetCursorPos(p.x, p.y - (SCREEN_HEIGHT - 1));
		//}
		//if (app->input->GetMouseY() <= 1)
		//{
		//	POINT p;
		//	GetCursorPos(&p);
		//	SetCursorPos(p.x, p.y + (SCREEN_HEIGHT - 1));
		//}

		//alt mouse functions
		if (app->input->GetMouseButton(SDL_BUTTON_LEFT))
		{
			CameraOrbitUsingMouse();
		}
		if (app->input->GetMouseButton(SDL_BUTTON_MIDDLE))
		{
			CameraPanUsingMouse();
		}
	}
	//camera can do zoom with no condition
	CameraZoom();
}

void ModuleEngineManager::CameraKeyboardInput()
{
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) camSpeed = 0.2;
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP) camSpeed = 0.1;

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		engine.camera.transform.Move(glm::dvec3(0, 0, camSpeed));

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		engine.camera.transform.Move(glm::dvec3(0, 0, -camSpeed));

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		engine.camera.transform.Move(glm::dvec3(camSpeed, 0, 0));

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		engine.camera.transform.Move(glm::dvec3(-camSpeed, 0, 0));

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
		engine.camera.transform.Move(glm::dvec3(0, camSpeed, 0), Transform::moveType::GLOBAL);

	if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
		engine.camera.transform.Move(glm::dvec3(0, -camSpeed, 0), Transform::moveType::GLOBAL);

}
void ModuleEngineManager::CameraInputUsingMouse()
{
	int mmotionx = app->input->GetMouseXMotion();
	int mmotiony = -app->input->GetMouseYMotion();
	float sense = 0.1;

	engine.camera.transform.Rotate(glm::vec3(0, mmotionx * sense, 0), Transform::moveType::GLOBAL);
	engine.camera.transform.Rotate(glm::vec3(mmotiony * sense, 0, 0));
}

void ModuleEngineManager::CameraOrbitUsingMouse()
{
	int mmotionx = app->input->GetMouseXMotion();
	int mmotiony = -app->input->GetMouseYMotion();
	float sense = 0.2;

	engine.camera.transform.MoveTo(engine.camera.lookAtPos);

	engine.camera.transform.Rotate(vec3(0, mmotionx * sense, 0), Transform::moveType::GLOBAL);
	engine.camera.transform.Rotate(vec3(mmotiony * sense, 0, 0));

	vec3 finalPos = engine.camera.transform.position - (engine.camera.transform.forward * engine.camera.camOffset);
	engine.camera.transform.MoveTo(finalPos);
}
void ModuleEngineManager::CameraPanUsingMouse()
{
	int mmotionx = app->input->GetMouseXMotion();
	int mmotiony = app->input->GetMouseYMotion();
	float speedPan = 0.01f;

	engine.camera.transform.Move(vec3(mmotionx * speedPan, 0, 0));
	engine.camera.transform.Move(vec3(0, mmotiony * speedPan, 0));

	engine.camera.UpdateLookAt();
}
void ModuleEngineManager::CameraZoom()
{
	if (app->input->GetMouseZ() != 0)
	{
		float zoomSens = 0.3f;
		engine.camera.camOffset -= app->input->GetMouseZ() * zoomSens;

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
		engine.camera.transform.Move(vec3(0, 0, app->input->GetMouseZ() * zoomSens));
	}
}
