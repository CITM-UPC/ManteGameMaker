#include "EditorApp.h"

#include <GL/glew.h>

#include "EditorObjectSelector.h"
#include "EditorCamera.h"
#include "EditorWindow.h"
#include "EditorInput.h"
#include "EditorUI.h"

EditorObjectSelector::EditorObjectSelector()
{
}

bool EditorObjectSelector::Start()
{
	return true;
}

bool EditorObjectSelector::Update()
{
	if (!editor->editorUI->mouseOnUI) {
		if (editor->editorInput->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && editor->editorInput->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE) {
			GameObject* clicked = DoClickRayCast();
			SetGameObjectSelected(clicked);
		}
	}
	
	if (gameObjectSelected == nullptr) {
		return true;
	}
	glColor3ub(255, 0, 0);
	glPointSize(12);

	TransformComponent* transform = gameObjectSelected->GetComponent<TransformComponent>();
	glBegin(GL_POINTS);
	glVertex3f(transform->getPosition().x, transform->getPosition().y, transform->getPosition().z);
	glEnd();

	return true;
}

bool EditorObjectSelector::PostUpdate()
{
	lastRaycast.DrawRay();

	return true;
}

void EditorObjectSelector::SearchObject(int x, int y)
{
	vector<GameObject*> hitted;
	vector<float> distances;

	if (!RayHitsAABB(x, y, editor->gameApp->scene->aabb)) {
		return;
	}

	for (auto i = editor->gameApp->scene->children.begin(); i != editor->gameApp->scene->children.end(); ++i) {
		if (RayHitsAABB(x, y, (*i)->GetBoundingBox())) {
			CheckRayHitParent(x, y, (*i).get(), hitted, distances);
		}
	}

	if (distances.empty()) {
		return;
	}

	float smallestDistance = distances[0];
	float smallestDistanceIndex = 0;
	for (auto i = 0; i < distances.size(); ++i) {
		if (distances[i] < smallestDistance) {
			smallestDistance = distances[i];
			smallestDistanceIndex = i;
		}
	}

	gameObjectSelected = hitted[smallestDistanceIndex];

}

void EditorObjectSelector::CheckRayHitParent(int x, int y, GameObject* parent, vector<GameObject*> hitted, vector<float> distances)
{
	float distance;
	if (RayHitsGameObject(x, y, parent, distance)) {
		hitted.push_back(parent);
		distances.push_back(distance);

		if (parent->children.empty()) {
			return;
		}

		for (auto i = parent->children.begin(); i != parent->children.end(); ++i) {
			if (RayHitsAABB(x, y, (*i)->GetBoundingBox())) {
				CheckRayHitParent(x, y, (*i).get(), hitted, distances);
			}
		}
	}
}

bool EditorObjectSelector::RayHitsAABB(int x, int y, AABBox aabb)
{
	return false;
}

bool EditorObjectSelector::RayHitsGameObject(int x, int y, GameObject* go, float& distance)
{
	if (go->GetComponent<MeshComponent>() == nullptr) {
		return false;
	}

	// if (ray hits mesh)
	// save distance
	// return true;

	return false;
}

void EditorObjectSelector::SetGameObjectSelected(GameObject* selected)
{
	gameObjectSelected = selected;
}

GameObject* EditorObjectSelector::GetGameObjectSelected()
{
	return gameObjectSelected;
}

void EditorObjectSelector::SetAssetSelected(Paths* selected)
{
	assetSelected = selected;
}

Paths* EditorObjectSelector::GetAssetSelected()
{
	return assetSelected;
}

GameObject* EditorObjectSelector::DoClickRayCast()
{
	Ray ray = CalculateRay();

	lastRaycast = ray;

	std::map<float, GameObject*> hitObjectsMap;

	if (RayAABBIntersection(ray, editor->gameApp->scene->GetBoundingBox())) {
		cout << "Scene AABB hitted" << endl;

		for (const auto& object : editor->gameApp->scene->children)
		{
			if (RayAABBIntersection(ray, object.get()->GetGlobalBoundingBox()))
			{
				cout << "Hit AABB of " << object.get()->name.c_str() << endl;

				CheckMeshCollisionRecursive(ray, object.get(), hitObjectsMap);
			}
		}
	}

	float closestHitPoint = std::numeric_limits<float>::infinity();

	map<float, GameObject*>::iterator it = hitObjectsMap.begin();

	// Iterate through the map and print the elements
	while (it != hitObjectsMap.end()) {

		closestHitPoint = glm::min(closestHitPoint, it->first);
		++it;
	}

	if (hitObjectsMap.size() > 0)
	{
		cout << "Closest hit object is " << hitObjectsMap[closestHitPoint]->name.c_str() << endl;
		return hitObjectsMap[closestHitPoint];
	}

	return nullptr;
}

Ray EditorObjectSelector::CalculateRay()
{
	vec2 nearPlaneSize;
	nearPlaneSize.y = glm::tan(glm::radians(editor->editorCamera->cameraObject->GetComponent<CameraComponent>()->getCamera()->fov * 0.5)) * editor->editorCamera->cameraObject->GetComponent<CameraComponent>()->getCamera()->zNear;
	nearPlaneSize.x = nearPlaneSize.y * editor->editorCamera->cameraObject->GetComponent<CameraComponent>()->getCamera()->aspect;

	vec2 finalPos;
	finalPos.x = (nearPlaneSize.x / editor->editorWindow->width) * (editor->editorInput->GetMousePosition().x - editor->editorWindow->width) * 2 + nearPlaneSize.x;
	finalPos.y = (nearPlaneSize.y / editor->editorWindow->height) * (editor->editorInput->GetMousePosition().y - editor->editorWindow->height) * 2 + nearPlaneSize.y;


	Ray ray;
	ray.origin = (vec3)editor->editorCamera->cameraObject.get()->GetComponent<TransformComponent>()->getPosition();

	ray.origin += (vec3)editor->editorCamera->cameraObject.get()->GetComponent<TransformComponent>()->getForward() * editor->editorCamera->cameraObject->GetComponent<CameraComponent>()->getCamera()->zNear;
	ray.origin += (vec3)-editor->editorCamera->cameraObject.get()->GetComponent<TransformComponent>()->getUp() * finalPos.y;
	ray.origin += (vec3)-editor->editorCamera->cameraObject.get()->GetComponent<TransformComponent>()->getRight() * finalPos.x;

	ray.direction = glm::normalize((vec3)ray.origin - (vec3)editor->editorCamera->cameraObject.get()->GetComponent<TransformComponent>()->getPosition());

	return ray;
}

Triangle EditorObjectSelector::CalculateTriangle(GameObject* triObject, int iterator)
{
	vec4 vert0 = { triObject->GetComponent<MeshComponent>()->getMesh().get()->meshVerts[iterator * 3], 1 };
	vec4 vert1 = { triObject->GetComponent<MeshComponent>()->getMesh().get()->meshVerts[iterator * 3 + 1], 1 };
	vec4 vert2 = { triObject->GetComponent<MeshComponent>()->getMesh().get()->meshVerts[iterator * 3 + 2], 1 };

	vert0 = vert0 * glm::inverse(triObject->GetComponent<TransformComponent>()->getGlobalTransform());
	vert1 = vert1 * glm::inverse(triObject->GetComponent<TransformComponent>()->getGlobalTransform());
	vert2 = vert2 * glm::inverse(triObject->GetComponent<TransformComponent>()->getGlobalTransform());

	auto tri0 = (vec3)vert0 + (vec3)triObject->GetComponent<TransformComponent>()->getGlobalTransform()[3];
	auto tri1 = (vec3)vert1 + (vec3)triObject->GetComponent<TransformComponent>()->getGlobalTransform()[3];
	auto tri2 = (vec3)vert2 + (vec3)triObject->GetComponent<TransformComponent>()->getGlobalTransform()[3];

	// Assuming _format is F_V3 (change if necessary)
	Triangle triangle{ tri0, tri1, tri2 };

	return triangle;
}

void EditorObjectSelector::CheckMeshCollisionRecursive(Ray& ray, GameObject* object, std::map<float, GameObject*>& hitObjects)
{
	float closestIntersection = std::numeric_limits<float>::infinity();

	//Check for a mesh in the parent
	if (object->GetComponent<MeshComponent>() != nullptr)
	{
		for (size_t i = 0; i < object->GetComponent<MeshComponent>()->getMesh().get()->getFacesNum(); ++i)
		{
			Triangle triangle = CalculateTriangle(object, i);

			float currentT;
			if (RayTriangleIntersection(ray, triangle, currentT) && currentT < closestIntersection)
			{
				closestIntersection = currentT;
			}
		}

		if (closestIntersection != std::numeric_limits<float>::infinity())
		{
			hitObjects[closestIntersection] = object;  //Adding to the map an entry which has the distance as an accessor and the GO as the object
		}
	}
	for (const auto& child : object->children)
	{
		CheckMeshCollisionRecursive(ray, child.get(), hitObjects);
	}
}