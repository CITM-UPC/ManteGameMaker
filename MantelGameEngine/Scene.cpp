#include "Scene.h"
#include <GL/glew.h>
#include <fstream>

Scene::Scene()
{
}

Scene::Scene(string name)
{
	this->name = name;
}

Scene::~Scene()
{
}

GameObject* Scene::AddGameObject()
{
	string gameObjectName = "Game Object " + std::to_string(children.size());

	unique_ptr<GameObject> ptrGameObject = make_unique<GameObject>(gameObjectName);
	GameObject* copyGameObject = ptrGameObject.get();

	children.push_back(move(ptrGameObject));

	return copyGameObject;
}

GameObject* Scene::AddGameObject(string name)
{
	unique_ptr<GameObject> ptrGameObject = make_unique<GameObject>(name);
	GameObject* copyGameObject = ptrGameObject.get();

	children.push_back(move(ptrGameObject));

	return copyGameObject;
}

GameObject* Scene::AddGameObjectChildren(GameObject* parent)
{
	string gameObjectName = parent->name + " " + std::to_string(parent->children.size());

	unique_ptr<GameObject> ptrGameObject = make_unique<GameObject>(gameObjectName);
	GameObject* copyGameObject = ptrGameObject.get();

	parent->AddChild(move(ptrGameObject));

	return copyGameObject;

}

GameObject* Scene::DuplicateGameObject(GameObject* original)
{
	unique_ptr<GameObject> ptrGameObject = make_unique<GameObject>(original);
	GameObject* copyGameObject = ptrGameObject.get();

	for (auto c = original->children.begin(); c != original->children.end(); ++c) {
		unique_ptr<GameObject> ptrGameObjectChild = make_unique<GameObject>((*c).get());
		copyGameObject->AddChild(move(ptrGameObjectChild));
	}

	if (original->_parent != nullptr) {
		children.push_back(move(ptrGameObject));
		MoveChildToAnotherParent(copyGameObject, original->_parent);
	}
	else {
		children.push_back(move(ptrGameObject));
	}

	return copyGameObject;
}

//void Scene::DeleteGameObject(GameObject* toDelete)
//{
//	if (toDelete->_parent != nullptr) {
//		toDelete->_parent->DeleteChild(toDelete);
//	}
//	else {
//		for (auto GO = children.begin(); GO != children.end(); ++GO) {
//			if ((*GO).get() == toDelete) {
//				children.erase(GO);
//				return;
//			}
//		}
//	}
//}

unique_ptr<GameObject> Scene::FindChild(GameObject* child)
{
	for (auto GO = children.begin(); GO != children.end(); ++GO) {
		if ((*GO).get() == child) {
			return move(*GO);
		}
	}

	return nullptr;
}

void Scene::MoveChildToAnotherParent(GameObject* child, GameObject* toParent)
{
	list<unique_ptr<GameObject>>::iterator GO;
	
	// look if the child is actually the parent of the toParent
	if (toParent != nullptr) {
		GameObject* parentChecker = toParent;
		while (parentChecker->_parent != nullptr) {
			if (parentChecker->_parent == child) {
				return;
			}
			parentChecker = parentChecker->_parent;
		}
	}

	// if the child has a parent, look in the parent children, if not, look at the scene children
	if (child->_parent != nullptr) {
		for (GO = child->_parent->children.begin(); GO != child->_parent->children.end(); ++GO) {
			if ((*GO).get() == child) {
				break;
			}
		}
		if (toParent != nullptr) {
			toParent->children.splice(toParent->children.begin(), child->_parent->children, GO);
			child->_parent = toParent;
		}
		else {
			children.splice(children.begin(), child->_parent->children, GO);
			child->_parent = nullptr;
		}
	}
	else {
		for (GO = children.begin(); GO != children.end(); ++GO) {
			if ((*GO).get() == child) {
				break;
			}
		}
		if (toParent != nullptr) {
			toParent->children.splice(toParent->children.begin(), children, GO);
			child->_parent = toParent;
		}
		else {
			children.splice(children.begin(), children, GO);
			child->_parent = nullptr;
		}
	}


	
}

void Scene::DebugStart()
{
	{
		AddGameObject();
	}
	{
		GameObject* temp = AddGameObject();
		AddGameObjectChildren(temp);
	}
	{
		AddGameObject();
	}
}

void Scene::Start()
{
}

void Scene::Update()
{
}

void Scene::Render(Frustum frustum, bool drawBoundingBox)
{
	glColor3ub(128, 0, 0);
	if (drawBoundingBox) {
		DrawBoundingBox(GetBoundingBox());
	}

	if (frustum.IsBoundingBoxInFrustum(GetBoundingBox())) {
		for (auto gameObjectI = children.begin(); gameObjectI != children.end(); ++gameObjectI) {
			(*gameObjectI)->Render(frustum, drawBoundingBox);
		}
	}
}

void Scene::CleanUp()
{
	children.clear();
}

static inline void glVec3(const vec3& v) { glVertex3dv(&v.x); }

void Scene::DrawBoundingBox(const AABBox& aabb)
{
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);

	glVec3(aabb.a());
	glVec3(aabb.b());
	glVec3(aabb.c());
	glVec3(aabb.d());
	glVec3(aabb.a());

	glVec3(aabb.e());
	glVec3(aabb.f());
	glVec3(aabb.g());
	glVec3(aabb.h());
	glVec3(aabb.e());
	glEnd();

	glBegin(GL_LINES);
	glVec3(aabb.h());
	glVec3(aabb.d());
	glVec3(aabb.f());
	glVec3(aabb.b());
	glVec3(aabb.g());
	glVec3(aabb.c());
	glEnd();
}

AABBox Scene::GetBoundingBox()
{
	AABBox aabbox;
	aabbox = aabb;
	if (children.empty()) {
		aabbox.min = vec3(0);
		aabbox.max = vec3(0);
	}

	for (auto i = children.begin(); i != children.end(); ++i) {
		const auto child_aabb = ((*i).get()->GetComponent<TransformComponent>()->getTransform() * (*i).get()->GetBoundingBox()).AABB();
		aabbox.min = glm::min(aabbox.min, child_aabb.min);
		aabbox.max = glm::max(aabbox.max, child_aabb.max);
	}

	return aabbox;
}

bool Scene::BoundingBoxInFrustum(Frustum frustum)
{
	return false;
}

void Scene::LoadScene(const std::string& path)
{
	std::ifstream f(path);
	json data = json::parse(f);

	name = data["Name"];
	// for each children, load each child
}

void Scene::SaveTemporalScene()
{
	ofstream("path.json");
}

void Scene::SaveScene()
{
}
