#include "ModuleEngineManager.h"
#include "App.h"
#include "Globals.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ModuleEngineManager::ModuleEngineManager(bool startEnabled) : Module(startEnabled)
{
    name = "engineManager";
}

ModuleEngineManager::~ModuleEngineManager() {}

void ModuleEngineManager::Awake() {
	engine.Start();
    engine.camera.fov = 60;
    engine.camera.aspect = static_cast<double>(SCREEN_WIDTH) / SCREEN_HEIGHT;
    engine.camera.zNear = 0.1;
    engine.camera.zFar = 100;
}

void ModuleEngineManager::Start() {
	
}



glm::vec3 ModuleEngineManager::ScreenToWorldCoordinates(int mouseX, int mouseY, int screenWidth, int screenHeight, const glm::mat4& viewProjectionMatrix) {
	// Normalizar las coordenadas del ratón
	float normalizedX = (2.0f * mouseX) / screenWidth - 1.0f;
	float normalizedY = 1.0f - (2.0f * mouseY) / screenHeight;

	// Coordenadas del clip space
	glm::vec4 clipCoords(normalizedX, normalizedY, -1.0f, 1.0f);

	// Inversa de la matriz de proyección y vista
	glm::mat4 invViewProjMatrix = glm::inverse(viewProjectionMatrix);

	// Coordenadas en el espacio de la cámara
	glm::vec4 eyeCoords = invViewProjMatrix * clipCoords;
	eyeCoords.z = -1.0f;
	eyeCoords.w = 0.0f;

	// Inversa de la matriz de modelo-vista
	glm::mat4 invViewMatrix = glm::inverse(engine.camera.GetViewMatrix()); // Debes tener acceso a la matriz de vista

	// Coordenadas en el espacio del mundo
	glm::vec3 worldCoords = glm::vec3(invViewMatrix * eyeCoords);

	return worldCoords;
}

// Función para crear un rayo desde la cámara
Ray ModuleEngineManager::CreateRayFromCamera(const glm::vec3& cameraPosition, const glm::vec3& worldCoords) {
	Ray ray;
	ray.origin = cameraPosition;
	ray.direction = glm::normalize(worldCoords - cameraPosition);
	return ray;
}


bool ModuleEngineManager::RayAABBIntersect(const Ray& ray, const glm::vec3& min, const glm::vec3& max, glm::vec3& intersectionPoint) {
	float tMin = (min.x - ray.origin.x) / ray.direction.x;
	float tMax = (max.x - ray.origin.x) / ray.direction.x;

	if (tMin > tMax) std::swap(tMin, tMax);

	float tyMin = (min.y - ray.origin.y) / ray.direction.y;
	float tyMax = (max.y - ray.origin.y) / ray.direction.y;

	if (tyMin > tyMax) std::swap(tyMin, tyMax);

	if (tMin > tyMax || tyMin > tMax)
		return false;

	if (tyMin > tMin) tMin = tyMin;

	float tzMin = (min.z - ray.origin.z) / ray.direction.z;
	float tzMax = (max.z - ray.origin.z) / ray.direction.z;

	if (tzMin > tzMax) std::swap(tzMin, tzMax);

	if (tMin > tzMax || tzMin > tMax)
		return false;

	if (tzMin > tMin) tMin = tzMin;

	intersectionPoint = ray.origin + tMin * ray.direction;

	return true;
}

// Función para verificar la intersección con los objetos de la escena
GameObject* ModuleEngineManager:: CheckIntersection(const Ray& ray) {
	// Itera sobre tus objetos de la escena y verifica la intersección con cada uno
	for (const auto& gameObject : engine.allGameObjects) {
		// Supongamos que tus objetos tienen información sobre los límites (bounding box)
		for (const auto& mesh : gameObject->mesh_ptr)
		{
			const glm::vec3& boundingBoxMin = mesh.get()->boundingBoxMin;
			const glm::vec3& boundingBoxMax = mesh.get()->boundingBoxMax;

			glm::vec3 intersectionPoint;
			if (RayAABBIntersect(ray, boundingBoxMin, boundingBoxMax, intersectionPoint)) {
				// Devuelve el objeto intersectado
				return gameObject;
			}
		}
	}

	// No hubo intersección con ningún objeto
	return nullptr;
}




bool ModuleEngineManager::PreUpdate() {
	//first lets delete toDeleteList
	if (engine.deleteList)
	{
		for (auto& item : engine.toDeleteGo)
		{
			delete item;
			item = nullptr;
		}
		engine.toDeleteGo.clear();
		engine.deleteList = false;
	}

	if (app->gui->writing)
	{
		return true;
	}
	//delete a GameObject if so
	if (app->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
	{
		for (auto& item : app->engineManager->GetEngine()->allGameObjects)
		{
			if (item->selected)
			{
				app->engineManager->GetEngine()->DeleteGameObject(item);
				break;
			}
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		for (auto& item : app->engineManager->GetEngine()->allGameObjects)
		{
			if (item->selected)
			{
				app->engineManager->GetEngine()->camera.LookAtGameObject(item);
			}
		}
	}

	//code to select gameobject from opengl environment
	if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) {

		// Obtener las coordenadas del ratón
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		glm::mat4 projectionMatrix = engine.camera.GetProjectionMatrix();
		glm::mat4 viewMatrix = engine.camera.GetViewMatrix();
		glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

		// Convertir coordenadas de pantalla a coordenadas de mundo
		glm::vec3 worldCoords = ScreenToWorldCoordinates(mouseX, mouseY, SCREEN_WIDTH, SCREEN_HEIGHT, viewProjectionMatrix);

		// Crear un rayo desde la cámara
		ray = CreateRayFromCamera(engine.camera.transform.position, worldCoords);

		// Verificar intersección con los objetos de la escena
		GameObject* selectedObject = CheckIntersection(ray);

		// enable selected on focused go and unable it on all others
		for (const auto& go : engine.allGameObjects)
		{
			go->selected = false;
		}
		if (selectedObject != nullptr)
		{
			selectedObject->selected = true;
		}
	}

	//draw ray

	glColor3f(1.0f, 0.0f, 0.0f); // rojo

	glBegin(GL_LINES);
	glVertex3fv(glm::value_ptr(ray.origin));

	// Calcula el extremo del rayo
	int length = 10000;
	glm::vec3 rayEnd = ray.origin + glm::vec3(length, length, length) * ray.direction;
	glVertex3fv(glm::value_ptr(rayEnd));

	glEnd();

	//end of draw ray

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
	if (app->engineManager->GetEngine()->engineState == EngineState::EDITOR)
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

void ModuleEngineManager::LoadTreeLoop(GameObject* parent, pugi::xml_node& nodeGo)
{
	GameObject* tgo = new GameObject(
					nodeGo.attribute("name").as_string(),
					nodeGo.attribute("fbxPath").as_string(),
					nodeGo.attribute("emptyGameObject").as_bool(),
					nodeGo.attribute("camera").as_bool());
	engine.AddGameObject(tgo);
	
	//here becaus its 2nd iteration we know its already a children
	//add child to parent and delete it from hierarchy (it was setted to hierarchy as default)
	app->engineManager->GetEngine()->hierarchy.remove(tgo);
	parent->GetChildrenList()->push_back(tgo);
	tgo->SetParent(parent);


	tgo->transform.position = vec3(nodeGo.attribute("p_x").as_double(), nodeGo.attribute("p_y").as_double(), nodeGo.attribute("p_z").as_double());
	tgo->transform.rotation = vec3(nodeGo.attribute("r_x").as_double(), nodeGo.attribute("r_y").as_double(), nodeGo.attribute("r_z").as_double());
	tgo->transform.scale = vec3(nodeGo.attribute("s_x").as_double(), nodeGo.attribute("s_y").as_double(), nodeGo.attribute("s_z").as_double());
	tgo->visible = nodeGo.attribute("visible").as_bool();
	tgo->emptyGameObject = nodeGo.attribute("emptyGameObject").as_bool();
	tgo->selected = nodeGo.attribute("selected").as_bool();
	tgo->camera = nodeGo.attribute("camera").as_bool();
	tgo->expanded = nodeGo.attribute("expanded").as_bool();

	if (tgo->camera)
	{
		tgo->GetCamera()->fov = nodeGo.attribute("cam_fov").as_double();
		tgo->GetCamera()->aspect = nodeGo.attribute("cam_aspect").as_double();
		tgo->GetCamera()->zNear = nodeGo.attribute("cam_zNear").as_double();
		tgo->GetCamera()->zFar = nodeGo.attribute("cam_zFar").as_double();
		tgo->GetCamera()->camOffset = nodeGo.attribute("cam_offset").as_double();
		tgo->GetCamera()->transform.position = vec3(
					nodeGo.attribute("cam_p_x").as_double(),
					nodeGo.attribute("cam_p_y").as_double(),
					nodeGo.attribute("cam_p_z").as_double());
		tgo->GetCamera()->transform.rotation = vec3(
					nodeGo.attribute("cam_r_x").as_double(),
					nodeGo.attribute("cam_r_y").as_double(),
					nodeGo.attribute("cam_r_z").as_double());
		tgo->GetCamera()->transform.scale = vec3(
					nodeGo.attribute("cam_s_x").as_double(),
					nodeGo.attribute("cam_s_y").as_double(),
					nodeGo.attribute("cam_s_z").as_double());
		tgo->GetCamera()->lookAtPos = vec3(nodeGo.attribute("cam_lookAtPos_x").as_double(), nodeGo.attribute("cam_lookAtPos_y").as_double(), nodeGo.attribute("cam_lookAtPos_z").as_double());
	}

	tgo->texturePath = nodeGo.attribute("texturePath").as_string();

	for (auto& childNode : nodeGo.children("GameObject"))
	{
		LoadTreeLoop(tgo, childNode);
	}
}

bool ModuleEngineManager::LoadState(pugi::xml_node& data)
{
	engine.ClearScene();

	for (auto& nodeGo : data.children("GameObject"))
	{
		GameObject* tgo = new GameObject(
			nodeGo.attribute("name").as_string(),
			nodeGo.attribute("fbxPath").as_string(),
			nodeGo.attribute("emptyGameObject").as_bool(),
			nodeGo.attribute("camera").as_bool());
		engine.AddGameObject(tgo);

		tgo->transform.position = vec3(nodeGo.attribute("p_x").as_double(), nodeGo.attribute("p_y").as_double(), nodeGo.attribute("p_z").as_double());
		tgo->transform.rotation = vec3(nodeGo.attribute("r_x").as_double(), nodeGo.attribute("r_y").as_double(), nodeGo.attribute("r_z").as_double());
		tgo->transform.scale = vec3(nodeGo.attribute("s_x").as_double(), nodeGo.attribute("s_y").as_double(), nodeGo.attribute("s_z").as_double());
		tgo->visible = nodeGo.attribute("visible").as_bool();
		tgo->emptyGameObject = nodeGo.attribute("emptyGameObject").as_bool();
		tgo->selected = nodeGo.attribute("selected").as_bool();
		tgo->camera = nodeGo.attribute("camera").as_bool();
		tgo->expanded = nodeGo.attribute("expanded").as_bool();

		if (tgo->camera)
		{
			tgo->GetCamera()->fov = nodeGo.attribute("cam_fov").as_double();
			tgo->GetCamera()->aspect = nodeGo.attribute("cam_aspect").as_double();
			tgo->GetCamera()->zNear = nodeGo.attribute("cam_zNear").as_double();
			tgo->GetCamera()->zFar = nodeGo.attribute("cam_zFar").as_double();
			tgo->GetCamera()->camOffset = nodeGo.attribute("cam_offset").as_double();
			tgo->GetCamera()->transform.position = vec3(
				nodeGo.attribute("cam_p_x").as_double(),
				nodeGo.attribute("cam_p_y").as_double(),
				nodeGo.attribute("cam_p_z").as_double());
			tgo->GetCamera()->transform.rotation = vec3(
				nodeGo.attribute("cam_r_x").as_double(),
				nodeGo.attribute("cam_r_y").as_double(),
				nodeGo.attribute("cam_r_z").as_double());
			tgo->GetCamera()->transform.scale = vec3(
				nodeGo.attribute("cam_s_x").as_double(),
				nodeGo.attribute("cam_s_y").as_double(),
				nodeGo.attribute("cam_s_z").as_double());
			tgo->GetCamera()->lookAtPos = vec3(nodeGo.attribute("cam_lookAtPos_x").as_double(), nodeGo.attribute("cam_lookAtPos_y").as_double(), nodeGo.attribute("cam_lookAtPos_z").as_double());
		}
		//once we have texture path
		tgo->texturePath = nodeGo.attribute("texturePath").as_string();
		//we apply it to the recently created GameObject
		tgo->texture = nullptr;
		tgo->texture = make_shared<Texture2D>(tgo->texturePath);
		for (size_t i = 0; i < tgo->mesh_ptr.size(); i++)
		{
			tgo->mesh_ptr[i].get()->texture = tgo->texture;
		}

		for (auto& childNode : nodeGo.children("GameObject"))
		{
			LoadTreeLoop(tgo, childNode);
		}
	}

	return true;
}

void ModuleEngineManager::SaveTreeLoop(GameObject* item, pugi::xml_node gameObjectNode)
{
	// Añadir atributos específicos del GameObject (posición, rotación, escalado, etc.)
	gameObjectNode.append_attribute("name").set_value(item->GetName());
	gameObjectNode.append_attribute("p_x").set_value(item->transform.position.x);
	gameObjectNode.append_attribute("p_y").set_value(item->transform.position.y);
	gameObjectNode.append_attribute("p_z").set_value(item->transform.position.z);
	gameObjectNode.append_attribute("r_x").set_value(item->transform.rotation.x);
	gameObjectNode.append_attribute("r_y").set_value(item->transform.rotation.y);
	gameObjectNode.append_attribute("r_z").set_value(item->transform.rotation.z);
	gameObjectNode.append_attribute("s_x").set_value(item->transform.scale.x);
	gameObjectNode.append_attribute("s_y").set_value(item->transform.scale.y);
	gameObjectNode.append_attribute("s_z").set_value(item->transform.scale.z);
	gameObjectNode.append_attribute("visible").set_value(item->visible);
	gameObjectNode.append_attribute("emptyGameObject").set_value(item->emptyGameObject);
	gameObjectNode.append_attribute("selected").set_value(item->selected);
	gameObjectNode.append_attribute("camera").set_value(item->camera);
	gameObjectNode.append_attribute("expanded").set_value(item->expanded);

	//if its a camera we should add all camera info
	if (item->camera)
	{
		gameObjectNode.append_attribute("cam_fov").set_value(item->GetCamera()->fov);
		gameObjectNode.append_attribute("cam_aspect").set_value(item->GetCamera()->aspect);
		gameObjectNode.append_attribute("cam_zNear").set_value(item->GetCamera()->zNear);
		gameObjectNode.append_attribute("cam_zFar").set_value(item->GetCamera()->zFar);
		gameObjectNode.append_attribute("cam_offset").set_value(item->GetCamera()->camOffset);
		gameObjectNode.append_attribute("cam_p_x").set_value(item->GetCamera()->transform.position.x);
		gameObjectNode.append_attribute("cam_p_y").set_value(item->GetCamera()->transform.position.y);
		gameObjectNode.append_attribute("cam_p_z").set_value(item->GetCamera()->transform.position.z);
		gameObjectNode.append_attribute("cam_r_x").set_value(item->GetCamera()->transform.rotation.x);
		gameObjectNode.append_attribute("cam_r_y").set_value(item->GetCamera()->transform.rotation.y);
		gameObjectNode.append_attribute("cam_r_z").set_value(item->GetCamera()->transform.rotation.z);
		gameObjectNode.append_attribute("cam_s_x").set_value(item->GetCamera()->transform.scale.x);
		gameObjectNode.append_attribute("cam_s_y").set_value(item->GetCamera()->transform.scale.y);
		gameObjectNode.append_attribute("cam_s_z").set_value(item->GetCamera()->transform.scale.z);
		gameObjectNode.append_attribute("cam_lookAtPos_x").set_value(item->GetCamera()->lookAtPos.x);
		gameObjectNode.append_attribute("cam_lookAtPos_y").set_value(item->GetCamera()->lookAtPos.y);
		gameObjectNode.append_attribute("cam_lookAtPos_z").set_value(item->GetCamera()->lookAtPos.z);
	}

	gameObjectNode.append_attribute("fbxPath").set_value(item->fbxPath.c_str());
	gameObjectNode.append_attribute("texturePath").set_value(item->texturePath.c_str());

	//if its a parent we should add all its children
	if (item->emptyGameObject && !item->GetChildrenList()->empty())
	{
		for (const auto& child : *item->GetChildrenList())
		{
			SaveTreeLoop(child, gameObjectNode.append_child("GameObject"));
		}
	}
}

bool ModuleEngineManager::SaveState(pugi::xml_node& data)
{
	//lets save the name, position, rotation and more info of all of GameObjects of the scene

	//it is a gameobject so

	for (const auto& item : engine.hierarchy)
	{
		pugi::xml_node gameObjectNode = data.append_child("GameObject");
		SaveTreeLoop(item, gameObjectNode);
	}
	return true;
}