#pragma once
#include "EditorModule.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include "imguizmo.h"

#include "../FrogGameEngine/GameObject.h"

class EditorUI : public EditorModule {
public:
	EditorUI();

	// TODO: change window and gl_context to pick it from app
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

public:
	// if the mouse pressed is drag to ui this still is false
	bool mouseOnUI;


	bool dockSpaceEnabled = true;

	bool editorActivated = true;

	bool quitPressed = false;

	bool showDemoWindow = false;
	bool showAnotherWindow = false;

	bool showHardwareWindow = false;
	bool showAboutPopup = false;
	bool showFPSLog = true;

	bool showConfigWindow = false;

	bool showHierarchyWindow = true;
	bool showInspectorWindow = true;

	bool showConsoleWindow = true;
	bool showAssetsWindow = true;
	
	bool usingGuizmo = false;
	ImGuizmo::OPERATION currentGuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	ImGuizmo::MODE currentGuizmoMode = ImGuizmo::MODE::WORLD;
	bool usingSnap;

	ImVec4 clearColor;

private:
	void UIMainMenuBar();

	void UIAboutPopup();
	void UIHardwareWindow();
	void UIFPSLog();

	void UIConfigWindowWindow();
	void UIConfigRendererWindow();
	void UIConfigInputWindow();

	void UIHierarchyNodeWrite(GameObject* GO);
	void UIHierarchyWindow();

	void UIInspectorWriteTransformNode(TransformComponent* component);
	void UIInspectorWriteMeshNode(MeshComponent* component);
	void UIInspectorWriteTextureNode(TextureComponent* component);
	void UIInspectorWriteCameraNode(CameraComponent* component);
	void UIInspectorNodeWrite(Component* component);
	void UIInspectorWindow();

	void UIConsoleWindow();
	void UIAssetsWindow();

};