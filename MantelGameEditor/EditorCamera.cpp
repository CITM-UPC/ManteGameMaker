#include "EditorApp.h"

#include "EditorInput.h"
#include "EditorWindow.h"
#include "EditorObjectSelector.h"

#include "EditorCamera.h"
#include "EditorUI.h"

#include "../FrogGameEngine/GameObject.h"

EditorCamera::EditorCamera() : EditorModule() { }

bool EditorCamera::Start() {

	cameraObject = make_unique<GameObject>(CAMERA_OBJECT, "Editor Camera");

	cameraObject.get()->GetComponent<TransformComponent>()->getPosition() = { 10, 2, 10 };
	cameraObject.get()->GetComponent<TransformComponent>()->rotate(-120, { 0, 1, 0 });

	focusPosition = cameraObject.get()->GetComponent<TransformComponent>()->getPosition() + (cameraObject.get()->GetComponent<TransformComponent>()->getForward() * 10.0);

	cameraObject.get()->GetComponent<CameraComponent>()->getCamera()->aspect = (double)editor->editorWindow->width / (double)editor->editorWindow->height;

	return true;
}

bool EditorCamera::Update() {

	if (editor->editorInput->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		editor->gameApp->useBasicCameraWithFrustum = !editor->gameApp->useBasicCameraWithFrustum;
	}

	if (editor->editorInput->WindowSizeHasUpdated()) {
		cameraObject.get()->GetComponent<CameraComponent>()->getCamera()->aspect = (double)editor->editorWindow->width / (double)editor->editorWindow->height;
	}

	// Holding SHIFT duplicates movement speed
	float speed = baseSpeed;
	if (editor->editorInput->GetKey(SDL_SCANCODE_LSHIFT)) {
		speed = speed * 2.0 /*multiplying factor*/;
	}

	if (editor->editorUI->mouseOnUI) {
		return true;
	}

	// Alt+Left click should orbit the object
	if (editor->editorInput->GetKey(SDL_SCANCODE_LALT)) {
		if (editor->editorInput->GetMouseButtonDown(SDL_BUTTON_LEFT)) {
			OrbitAround(editor->editorInput->GetMouseMotion());
		}
	}

	// While Right clicking, “WASD” fps-like movement and free look around must be enabled
	else if (editor->editorInput->GetMouseButtonDown(SDL_BUTTON_RIGHT)) {
		if (editor->editorInput->GetKey(SDL_SCANCODE_W)) {
			Translate(vec3(0, 0, speed));
		}
		if (editor->editorInput->GetKey(SDL_SCANCODE_A)) {
			Translate(vec3(speed, 0, 0));
		}
		if (editor->editorInput->GetKey(SDL_SCANCODE_S)) {
			Translate(vec3(0, 0, -speed));
		}
		if (editor->editorInput->GetKey(SDL_SCANCODE_D)) {
			Translate(vec3(-speed, 0, 0));
		}
		if (editor->editorInput->GetKey(SDL_SCANCODE_SPACE)) {
			Translate(vec3(0, speed, 0), GLOBAL);
		}

		LookAround(editor->editorInput->GetMouseMotion());
	}

	

	// Mouse wheel should zoom in and out
	if (editor->editorInput->GetMouseWheelScroll() != 0) {
		CameraZoom(-editor->editorInput->GetMouseWheelScroll());
	}

	// Pressing “f” should focus the camera around the geometry
	if (editor->editorInput->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
		if (editor->editorObjectSelector->GetGameObjectSelected() != nullptr) {
			if (editor->editorObjectSelector->GetGameObjectSelected()->GetComponent<TransformComponent>() != nullptr) {
				TransformComponent* gameObjectTransform = editor->editorObjectSelector->GetGameObjectSelected()->GetComponent<TransformComponent>();
				vec3 focusPoint = gameObjectTransform->getPosition();
				FocusOn(focusPoint);
			}
		}
	}

	return true;
}

void EditorCamera::Translate(vec3 translation, ReferenceAxis ref) {

	cameraObject.get()->GetComponent<TransformComponent>()->translate(translation, ref);

	double focusDistance = glm::distance(focusPosition, cameraObject.get()->GetComponent<TransformComponent>()->getPosition());
	focusPosition = cameraObject.get()->GetComponent<TransformComponent>()->getPosition() + cameraObject.get()->GetComponent<TransformComponent>()->getForward() * focusDistance;
}

void EditorCamera::OrbitAround(vec2 motion) {

	double sensibility = 0.1;

	double focusDistance = glm::distance(cameraObject.get()->GetComponent<TransformComponent>()->getPosition(), focusPosition);	

	cameraObject.get()->GetComponent<TransformComponent>()->getPosition() = focusPosition;

	cameraObject.get()->GetComponent<TransformComponent>()->rotate((-motion.x) * sensibility, vec3(0, 1.0f, 0), GLOBAL);
	cameraObject.get()->GetComponent<TransformComponent>()->rotate((motion.y) * sensibility, vec3(1.0f, 0, 0));

	cameraObject.get()->GetComponent<TransformComponent>()->getPosition() = cameraObject.get()->GetComponent<TransformComponent>()->getPosition() - (glm::normalize(cameraObject.get()->GetComponent<TransformComponent>()->getForward()) * focusDistance);

}

void EditorCamera::LookAround(vec2 motion) {

	double sensibility = 0.1;


	if (motion.x != 0) {
		cameraObject.get()->GetComponent<TransformComponent>()->rotate(-motion.x * sensibility, {0, 1, 0}, GLOBAL);
	}

	if (motion.y != 0) {
		cameraObject.get()->GetComponent<TransformComponent>()->rotate(motion.y * sensibility, {1, 0, 0});
	}

	double focusDistance = glm::distance(focusPosition, cameraObject.get()->GetComponent<TransformComponent>()->getPosition());
	focusPosition = cameraObject.get()->GetComponent<TransformComponent>()->getPosition() + cameraObject.get()->GetComponent<TransformComponent>()->getForward() * focusDistance;
}

void EditorCamera::CameraZoom(double amount) {
	
	double focusDistance = glm::distance(focusPosition, cameraObject.get()->GetComponent<TransformComponent>()->getPosition());
	cameraObject.get()->GetComponent<TransformComponent>()->translate(vec3(0, 0, 1) * -amount * (focusDistance * 0.1f));
}

void EditorCamera::FocusOn(vec3 focusPoint) {

	// move center (looking point) to the selected game object position
	// then move eye (position) to a distance on the line vector z crossing center (looking point)
	// --TODO-- (?) is there a way to get how big the 3d model is? to put the vision around the whole mesh and not only from one point
	focusPosition = focusPoint;
	cameraObject.get()->GetComponent<TransformComponent>()->getPosition() = focusPoint - (cameraObject.get()->GetComponent<TransformComponent>()->getForward() * 10.0);
}
