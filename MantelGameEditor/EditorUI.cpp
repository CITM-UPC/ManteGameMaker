#include "EditorApp.h"

#include <filesystem>

#include "assimp/version.h"
#include "IL/il.h"

#include "EditorInput.h"
#include "EditorWindow.h"
#include "EditorObjectSelector.h"
#include "EditorUI.h"
#include "EditorCamera.h"

#include "../FrogGameEngine/GameApp.h"

#include "../FrogGameEngine/Scene.h"

#include "../FrogGameEngine/Mesh.h"

namespace fs = std::filesystem;

EditorUI::EditorUI() : EditorModule() {

}

bool EditorUI::Start() {

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(editor->editorWindow->window, editor->editorWindow->glContext);
	ImGui_ImplOpenGL3_Init();

	editor->AddLog("ImGui Initialized " + (string)IMGUI_VERSION);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	ImGuizmo::Enable(true);

	clearColor = ImVec4(0.039f, 0.039f, 0.039f, 1.00f);

	return true;
}

bool EditorUI::PreUpdate() {

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	if (dockSpaceEnabled) {
		ImGuiDockNodeFlags dock_flags = 0;
		dock_flags |= ImGuiDockNodeFlags_PassthruCentralNode;
		ImGui::DockSpaceOverViewport(0, dock_flags);
	}

	mouseOnUI = ImGui::GetIO().WantCaptureMouse;

	return true;
}

bool EditorUI::Update() {

	// main menu bar
	UIMainMenuBar();

	if (!editorActivated) {
		return true;
	}

	// about poput (there is a bug in imgui that causes that you cannot open a popup directly from the menu, this is a kinda solution)
	{
		if (showAboutPopup) {
			ImGui::OpenPopup("About");
			showAboutPopup = false;
		}
		UIAboutPopup();
	}


	if (editor->editorInput->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		if (editor->editorObjectSelector->gameObjectSelected != nullptr) {
			editor->gameApp->scene->MoveChildToAnotherParent(editor->editorObjectSelector->gameObjectSelected);
		}
	}

	if (editor->editorInput->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {
		usingGuizmo = false;
	}
	if (editor->editorInput->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		usingGuizmo = true;
		currentGuizmoOperation = ImGuizmo::TRANSLATE;
	}
	if (editor->editorInput->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		usingGuizmo = true;
		currentGuizmoOperation = ImGuizmo::ROTATE;
	}
	if (editor->editorInput->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		usingGuizmo = true;
		currentGuizmoOperation = ImGuizmo::SCALE;
	}

	if (usingGuizmo) {
		/*if (editor->editorObjectSelector->GetGameObjectSelected() != nullptr) {
			const float* viewMatrix = (float*)&editor->editorCamera->camera.computeLookAt()[0][0];
			const float* projectionMatrix = (float*)&glm::perspective(editor->editorCamera->camera.fov, editor->editorCamera->camera.aspect, editor->editorCamera->camera.zNear, editor->editorCamera->camera.zFar)[0][0];



			float* transformMatrix = (float*)&glm::transpose(editor->editorObjectSelector->GetGameObjectSelected()->GetComponent<TransformComponent>()->getTransform())[0][0];

			ImGuizmo::Manipulate(viewMatrix, projectionMatrix, currentGuizmoOperation, currentGuizmoMode, transformMatrix);

		}*/
	}

	// windows
	if (showHardwareWindow) {
		UIHardwareWindow();
	}

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (showDemoWindow) {
		ImGui::ShowDemoWindow(&showDemoWindow);
		ImGui::IsWindowDocked();
	}

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &showDemoWindow);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &showAnotherWindow);
		ImGui::Checkbox("Docking", &dockSpaceEnabled);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clearColor); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (showAnotherWindow)
	{
		ImGui::Begin("Another Window", &showAnotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			showAnotherWindow = false;

		ImGui::End();
	}

	if (showFPSLog) {
		UIFPSLog();
	}

	if (showConfigWindow) {
		ImGui::Begin("Configuration", &showConfigWindow);
		if (ImGui::BeginTabBar("Configs")) {
			UIConfigWindowWindow();

			UIConfigRendererWindow();

			UIConfigInputWindow();

			ImGui::EndTabBar();
		}

		ImGui::End();
	}
	if (showHierarchyWindow) {
		UIHierarchyWindow();
	}

	if (showInspectorWindow) {
		UIInspectorWindow();
	}

	if (showAssetsWindow) {
		UIAssetsWindow();
	}

	if (showConsoleWindow) {
		UIConsoleWindow();
	}

	return true;
}

bool EditorUI::PostUpdate() {

	ImGui::Render();
	//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//SDL_GL_SwapWindow(window);
	return !quitPressed;
}

bool EditorUI::CleanUp() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void EditorUI::UIMainMenuBar() {
	
	if (ImGui::BeginMainMenuBar()) {

		if (ImGui::BeginMenu("General")) {
			if (ImGui::MenuItem("New Project --TODO--")) {

			}

			if (ImGui::MenuItem("Open Project --TODO--")) {

			}

			if (editor->gameIsOn) {
				if (ImGui::MenuItem("STOP")) {
					editor->StopGame();
					editor->editorObjectSelector->SetGameObjectSelected(nullptr);
				}
			}
			else {
				if (ImGui::MenuItem("PLAY")) {
					editor->PlayGame();
				}
			}
			

			ImGui::Separator();

			if (editorActivated) {
				if (ImGui::MenuItem("Deactivate Editor")) {
					editorActivated = !editorActivated;
				}
			}
			else {
				if (ImGui::MenuItem("Activate Editor")) {
					editorActivated = !editorActivated;
				}
			}

			if (ImGui::MenuItem("Go To Github")) {
				editor->WebRequest("https://github.com/CITM-UPC/FrogGameMaker");
			}

			if (ImGui::MenuItem("Quit")) {
				quitPressed = !quitPressed;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Create")) {
			if (ImGui::MenuItem("Empty")) {
				editor->gameApp->scene->AddGameObject();
			}
			if (ImGui::BeginMenu("Basic Shapes")) {
				if (ImGui::MenuItem("Cube")) {
					GameObject* newMesh = editor->gameApp->scene->AddGameObject("Cube");
					auto mesh_ptrs = Mesh::loadFromFile("Cube.sht");
					editor->AddLog("Loaded Cube");
					newMesh->AddMeshWithTexture(mesh_ptrs);
				}
				if (ImGui::MenuItem("Sphere")) {
					GameObject* newMesh = editor->gameApp->scene->AddGameObject("Sphere");
					auto mesh_ptrs = Mesh::loadFromFile("Sphere.sht");
					editor->AddLog("Loaded Sphere");
					newMesh->AddMeshWithTexture(mesh_ptrs);
				}
				if (ImGui::MenuItem("Cone")) {
					GameObject* newMesh = editor->gameApp->scene->AddGameObject("Cone");
					auto mesh_ptrs = Mesh::loadFromFile("Cone.sht");
					editor->AddLog("Loaded Cone");
					newMesh->AddMeshWithTexture(mesh_ptrs);
				}
				if (ImGui::MenuItem("Cylinder")) {
					GameObject* newMesh = editor->gameApp->scene->AddGameObject("Cylinder");
					auto mesh_ptrs = Mesh::loadFromFile("Cylinder.sht");
					editor->AddLog("Loaded Cylinder");
					newMesh->AddMeshWithTexture(mesh_ptrs);
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Configuration")) {

			if (ImGui::MenuItem("Config")) {
				showConfigWindow = !showConfigWindow;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Info")) {

			if (ImGui::MenuItem("Hardware Information")) {
				showHardwareWindow = !showHardwareWindow;
			}
			if (ImGui::MenuItem("FPS Log")) {
				showFPSLog = !showFPSLog;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help")) {

			if (ImGui::MenuItem("Demo Window")) {
				showDemoWindow = !showDemoWindow;
			}

			if (ImGui::MenuItem("About")) {
				showAboutPopup = !showAboutPopup;
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void EditorUI::UIAboutPopup() {

	ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("About")) {
		{
			// name engine + version
			ImGui::SeparatorText("Frog Game Engine v0.1");

			// made by
			ImGui::Text("Made by: ");
			ImGui::Text("   Victor Martin (Github:");
			ImGui::SameLine();
			if (ImGui::SmallButton("VicMarBall")) {
				editor->WebRequest("https://github.com/VicMarBall");
			}
			ImGui::SameLine();
			ImGui::Text(")");

			ImGui::Text("   Ari Sevcik (Github:");
			ImGui::SameLine();
			if (ImGui::SmallButton("AriSevcik")) {
				editor->WebRequest("https://github.com/AriSevcik");
			}
			ImGui::SameLine();
			ImGui::Text(")");



			// external libraries
			{
				ImGui::SeparatorText("External Libraries Used: ");
				// sdl
				SDL_version sdlVersion;
				SDL_GetVersion(&sdlVersion);
				ImGui::Bullet(); ImGui::Text("SDL %d.%d.%d", sdlVersion.major, sdlVersion.minor, sdlVersion.patch);
				
				// opengl
				ImGui::Bullet(); ImGui::Text("OpenGL %s", glGetString(GL_VERSION));

				// glew
				ImGui::Bullet(); ImGui::Text("GLEW %s", glewGetString(GLEW_VERSION));

				// imgui
				ImGui::Bullet(); ImGui::Text("ImGui %s", IMGUI_VERSION);

				// glu
				ImGui::Bullet(); ImGui::Text("GLU %s", gluGetString(GLU_VERSION));

				// devil
				ImGui::Bullet(); ImGui::Text("DevIL %s", ilGetString(IL_VERSION_NUM));

				// assimp
				ImGui::Bullet(); ImGui::Text("Assimp %d.%d.%d", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionRevision());
			}

			{
				ImGui::SeparatorText("License");

				// --TODO-- read directly the license document (?)
				ImGui::Text("MIT License");
				ImGui::Text("");
				ImGui::Text("Copyright(c) 2023 CITM - UPC");
				ImGui::Text("");
				ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy");
				ImGui::Text("of this software and associated documentation files(the \"Software\"), to deal");
				ImGui::Text("in the Software without restriction, including without limitation the rights");
				ImGui::Text("to use, copy, modify, merge, publish, distribute, sublicense, and /or sell");
				ImGui::Text("copies of the Software, and to permit persons to whom the Software is");
				ImGui::Text("furnished to do so, subject to the following conditions :");
				ImGui::Text("");
				ImGui::Text("The above copyright notice and this permission notice shall be included in all");
				ImGui::Text("copies or substantial portions of the Software.");
				ImGui::Text("");
				ImGui::Text("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
				ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
				ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE");
				ImGui::Text("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
				ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
				ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
				ImGui::Text("SOFTWARE.");
			}

			ImGui::Separator();

			if (ImGui::Button("Close")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
}

void EditorUI::UIHardwareWindow() {

	ImGui::Begin("Hardware", &showHardwareWindow);

	ImGui::Text("CPU's Cores:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d (Cache: %d kb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());

	ImGui::Text("System RAM:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d MB", SDL_GetSystemRAM());

	const GLubyte* renderer = glGetString(GL_RENDERER); // Returns a hint to the model

	ImGui::Text("%s", renderer);

	ImGui::Separator();

	int vram;

	glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTION_COUNT_NVX, &vram);
	vram *= 0.001f;
	ImGui::Text("VRAM being used:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d MB", vram);

	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &vram);
	vram *= 0.001f;
	ImGui::Text("VRAM total:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d MB", vram);

	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &vram);
	vram *= 0.001f;
	ImGui::Text("VRAM available:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d MB", vram);

	ImGui::End();
}

void EditorUI::UIFPSLog() {

	ImGui::Begin("FPS Log");
	char title[25];
	sprintf_s(title, 25, "Framerate %.1f", editor->FPS_Log[editor->FPS_Log.size() - 1]);
	ImGui::PlotHistogram("##framerate", &editor->FPS_Log[0], editor->FPS_Log.size(), 0, title, 0.0f, 100.0f, ImVec2(300, 100));

	ImGui::End();
}

void EditorUI::UIConfigWindowWindow() {

	if (ImGui::BeginTabItem("Window")) {
		ImGui::Text("Window Width:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", editor->editorWindow->width);

		ImGui::Text("Window Height:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", editor->editorWindow->height);

		float aspectRatio = (float)editor->editorWindow->width / (float)editor->editorWindow->height;

		ImGui::Text("Aspect Ratio:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%f", aspectRatio);

		if (ImGui::Checkbox("Fullscreen", &editor->editorWindow->isFullscreen)) {
			editor->editorWindow->UpdateFullscreen();
		}
		if (ImGui::Checkbox("Resizable", &editor->editorWindow->isResizable)) {
			editor->editorWindow->UpdateResizable();
		}
		if (ImGui::Checkbox("Borderless", &editor->editorWindow->isBorderless)) {
			editor->editorWindow->UpdateBorderless();
		}

		ImGui::EndTabItem();
	}
}

void EditorUI::UIConfigRendererWindow() {

	if (ImGui::BeginTabItem("Renderer")) {


		ImGui::EndTabItem();
	}
}

void EditorUI::UIConfigInputWindow() {

	if (ImGui::BeginTabItem("Input")) {

		// from ImGui manual
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Inputs"))
		{
			if (ImGui::IsMousePosValid())
				ImGui::Text("Mouse pos: (%g, %g)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
			else
				ImGui::Text("Mouse pos: <INVALID>");
			ImGui::Text("Mouse delta: (%g, %g)", ImGui::GetIO().MouseDelta.x, ImGui::GetIO().MouseDelta.y);
			ImGui::Text("Mouse down:");
			for (int i = 0; i < IM_ARRAYSIZE(ImGui::GetIO().MouseDown); i++) if (ImGui::IsMouseDown(i)) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, ImGui::GetIO().MouseDownDuration[i]); }
			ImGui::Text("Mouse wheel: %.1f", ImGui::GetIO().MouseWheel);

			// We iterate both legacy native range and named ImGuiKey ranges, which is a little odd but this allows displaying the data for old/new backends.
			// User code should never have to go through such hoops! You can generally iterate between ImGuiKey_NamedKey_BEGIN and ImGuiKey_NamedKey_END.
#ifdef IMGUI_DISABLE_OBSOLETE_KEYIO
			struct funcs { static bool IsLegacyNativeDupe(ImGuiKey) { return false; } };
			ImGuiKey start_key = ImGuiKey_NamedKey_BEGIN;
#else
			struct funcs { static bool IsLegacyNativeDupe(ImGuiKey key) { return key < 512 && ImGui::GetIO().KeyMap[key] != -1; } }; // Hide Native<>ImGuiKey duplicates when both exists in the array
			ImGuiKey start_key = (ImGuiKey)0;
#endif
			ImGui::Text("Keys down:");         for (ImGuiKey key = start_key; key < ImGuiKey_NamedKey_END; key = (ImGuiKey)(key + 1)) { if (funcs::IsLegacyNativeDupe(key) || !ImGui::IsKeyDown(key)) continue; ImGui::SameLine(); ImGui::Text((key < ImGuiKey_NamedKey_BEGIN) ? "\"%s\"" : "\"%s\" %d", ImGui::GetKeyName(key), key); }
			ImGui::Text("Keys mods: %s%s%s%s", ImGui::GetIO().KeyCtrl ? "CTRL " : "", ImGui::GetIO().KeyShift ? "SHIFT " : "", ImGui::GetIO().KeyAlt ? "ALT " : "", ImGui::GetIO().KeySuper ? "SUPER " : "");
			ImGui::Text("Chars queue:");       for (int i = 0; i < ImGui::GetIO().InputQueueCharacters.Size; i++) { ImWchar c = ImGui::GetIO().InputQueueCharacters[i]; ImGui::SameLine();  ImGui::Text("\'%c\' (0x%04X)", (c > ' ' && c <= 255) ? (char)c : '?', c); } // FIXME: We should convert 'c' to UTF-8 here but the functions are not public.

			ImGui::TreePop();
		}

		// Display ImGuiIO output flags
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Outputs"))
		{
			ImGui::Text("io.WantCaptureMouse: %d", ImGui::GetIO().WantCaptureMouse);
			ImGui::Text("io.WantCaptureMouseUnlessPopupClose: %d", ImGui::GetIO().WantCaptureMouseUnlessPopupClose);
			ImGui::Text("io.WantCaptureKeyboard: %d", ImGui::GetIO().WantCaptureKeyboard);
			ImGui::Text("io.WantTextInput: %d", ImGui::GetIO().WantTextInput);
			ImGui::Text("io.WantSetMousePos: %d", ImGui::GetIO().WantSetMousePos);
			ImGui::Text("io.NavActive: %d, io.NavVisible: %d", ImGui::GetIO().NavActive, ImGui::GetIO().NavVisible);
			ImGui::EndTabItem();
		}
	}
}

void EditorUI::UIHierarchyNodeWrite(GameObject* GO) {

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
	if (GO->children.empty()) {
		flags |= ImGuiTreeNodeFlags_Leaf;
	}
	if (editor->editorObjectSelector->GetGameObjectSelected() != NULL && editor->editorObjectSelector->GetGameObjectSelected() == GO) {
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	bool nodeIsOpen = ImGui::TreeNodeEx(GO->name.c_str(), flags);

	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
		editor->editorObjectSelector->SetGameObjectSelected(GO);
	}

	// ------

	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("Hierarchy_DragAndDrop", &GO, sizeof(GameObject));

		ImGui::EndDragDropSource();
	}
	if (ImGui::BeginDragDropTarget())
	{
		bool dropped = false;
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Hierarchy_DragAndDrop"))
		{
			GameObject* dragging = *(GameObject**)payload->Data;

			editor->gameApp->scene->MoveChildToAnotherParent(dragging, GO);
			dropped = true;			
		}
		ImGui::EndDragDropTarget();
		if (dropped) {
			return;
		}
	}

	// ------

	//bool elementDeleted = false;

	if (ImGui::BeginPopupContextItem()) {
		editor->editorObjectSelector->SetGameObjectSelected(GO);

		if (ImGui::Selectable("Duplicate")) {
			editor->gameApp->scene->DuplicateGameObject(GO);
		}
		/*if (ImGui::Selectable("Delete")) {
			editor->gameApp->scene->DeleteGameObject(GO);
			elementDeleted = true;
		}*/

		ImGui::EndPopup();
	}

	//if (!elementDeleted) {
		if (nodeIsOpen) {
			for (std::list<unique_ptr<GameObject>>::iterator it = GO->children.begin(); it != GO->children.end(); ++it) {
				UIHierarchyNodeWrite((*it).get());
			}
			ImGui::TreePop();
		}
	//}
}

void EditorUI::UIHierarchyWindow() {

	ImGui::Begin("Hierarchy");
	if (editor->gameApp->scene == nullptr) {
		ImGui::End();
		return;
	}

	Scene* sceneToUI = editor->gameApp->scene;
	// unity style: 
	// get the scene that has as children the rest of game objects
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader(sceneToUI->name.c_str())) {
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Hierarchy_DragAndDrop"))
			{
				GameObject* dragging = *(GameObject**)payload->Data;

				editor->gameApp->scene->MoveChildToAnotherParent(dragging, nullptr);
			}
			ImGui::EndDragDropTarget();
		}

		for (std::list<unique_ptr<GameObject>>::iterator it = sceneToUI->children.begin(); it != sceneToUI->children.end(); ++it) {
			UIHierarchyNodeWrite((*it).get());
		}
	}

	ImGui::End();
}

void EditorUI::UIInspectorWriteTransformNode(TransformComponent* component) {

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader("Transform")) {
		ImGuiInputTextFlags flags = 0;
		ImGui::PushItemWidth(60);

		ImGui::SeparatorText("Position: ");
		ImGui::InputDouble("X", &component->getPosition().x, 0, 0, "%.2f", flags);
		ImGui::SameLine();
		ImGui::PopID();
		ImGui::InputDouble("Y", &component->getPosition().y, 0, 0, "%.2f", flags);
		ImGui::SameLine();
		ImGui::PopID();
		ImGui::InputDouble("Z", &component->getPosition().z, 0, 0, "%.2f", flags);
		ImGui::Spacing();

		ImGui::SeparatorText("Rotation: ");
		ImGui::InputDouble("X", &component->getEulerAngles().x, 0, 0, "%.2f", flags);
		ImGui::SameLine();
		ImGui::PopID();
		ImGui::InputDouble("Y", &component->getEulerAngles().y, 0, 0, "%.2f", flags);
		ImGui::SameLine();
		ImGui::PopID();
		ImGui::InputDouble("Z", &component->getEulerAngles().z, 0, 0, "%.2f", flags);
		ImGui::Spacing();

		ImGui::SeparatorText("Scale: ");
		ImGui::InputDouble("X", &component->getScaleVector().x, 0, 0, "%.2f", flags);
		ImGui::SameLine();
		ImGui::PopID();
		ImGui::InputDouble("Y", &component->getScaleVector().y, 0, 0, "%.2f", flags);
		ImGui::SameLine();
		ImGui::PopID();
		ImGui::InputDouble("Z", &component->getScaleVector().z, 0, 0, "%.2f", flags);
		ImGui::Spacing();

		ImGui::PopItemWidth();

		component->setTransformFromVectorEditing();
		
		/*
		float vec3Position[3] = { (float)component->getPosition().x, (float)component->getPosition().y, (float)component->getPosition().z };
		ImGui::InputFloat3("Position", vec3Position);
		float vec3Rotation[3] = { glm::degrees((float)component->getRotation().x), glm::degrees((float)component->getRotation().y), glm::degrees((float)component->getRotation().z) };
		ImGui::InputFloat3("Rotation", vec3Rotation);
		float vec3Scale[3] = { (float)component->getScale().x, (float)component->getScale().y, (float)component->getScale().z };
		ImGui::InputFloat3("Scale", vec3Scale);*/
	}
}

void EditorUI::UIInspectorWriteMeshNode(MeshComponent* component) {

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader("Mesh")) {
		if (component->getMesh() != nullptr) {
			string s = component->getMesh()->path;
			if (auto n = s.find_last_of("\\"); n != s.npos) {
				s.erase(0, n + 1);
			}
			ImGui::Text("File:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", s.c_str());
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("%s", component->getMesh()->path.c_str());
			}

			ImGui::Text("Vertex:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", component->getMesh()->getVertsNum());

			ImGui::Text("Faces:", component->getMesh()->getFacesNum());
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", component->getMesh()->getFacesNum());

			if (ImGui::Checkbox("Use Checkers Texture", &component->getMesh()->drawChecker)) {}
			if (ImGui::Checkbox("See Vertex Normals", &component->getMesh()->drawNormalsVerts)) {}
			if (ImGui::Checkbox("See Face Normals", &component->getMesh()->drawNormalsFaces)) {}

			ImGui::SliderFloat("Normal's Length", &component->getMesh()->normalLineLength, 0.1f, 2.0f);
			ImGui::SliderInt("Normal's Width", &component->getMesh()->normalLineWidth, 1, 4);
		}
		else {
			ImGui::Text("Mesh not found");
		}
	}
}

void EditorUI::UIInspectorWriteTextureNode(TextureComponent* component) {

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader("Texture")) {
		if (component->getTexture() != nullptr) {
			string s = component->getTexture()->path;
			if (auto n = s.find_last_of("\\"); n != s.npos) {
				s.erase(0, n + 1);
			}
			ImGui::Text("File:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", s.c_str());
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("%s", component->getTexture()->path.c_str());
			}

			ImGui::Text("Size:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%dpx x %dpx", component->getTexture()->width, component->getTexture()->height);
		}
		else {
			ImGui::Text("Texture not found");
		}
	}
}

void EditorUI::UIInspectorWriteCameraNode(CameraComponent* component) {

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader("Camera")) {
		Camera* cam = component->getCamera();
		ImGui::InputDouble("FOV", &cam->fov);
		ImGui::InputDouble("zNear", &cam->zNear);
		ImGui::InputDouble("zFar", &cam->zFar);
		ImGui::InputDouble("Aspect Ratio", &cam->aspect);
	}
}

void EditorUI::UIInspectorNodeWrite(Component* component) {

	switch (component->componentType)
	{
	case ComponentType::TRANSFORM:
		UIInspectorWriteTransformNode((TransformComponent*)component);
		break;
	case ComponentType::MESH:
		UIInspectorWriteMeshNode((MeshComponent*)component);
		break;
	case ComponentType::TEXTURE:
		UIInspectorWriteTextureNode((TextureComponent*)component);
		break;
	case ComponentType::CAMERA:
		UIInspectorWriteCameraNode((CameraComponent*)component);
		break;
	default:
		break;
	}
}

void EditorUI::UIInspectorWindow() {

	ImGui::Begin("Inspector");

	if (editor->editorObjectSelector->GetGameObjectSelected() == nullptr) {
		ImGui::End();
		return;
	}

	// unity style: 
	// get the scene that has as children the rest of game objects
	for (auto iComponent = editor->editorObjectSelector->GetGameObjectSelected()->components.begin(); iComponent != editor->editorObjectSelector->GetGameObjectSelected()->components.end(); ++iComponent) {
		UIInspectorNodeWrite((*iComponent).get());
	}

	ImGui::End();
}

void EditorUI::UIConsoleWindow() {

	ImGui::Begin("Console");

	if (ImGui::Button("Clear")) {
		editor->ClearLogs();
	}

	ImGui::Separator();

	ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	{
		vector<string> editorLogs = editor->GetLogs();

		for (auto it = editorLogs.begin(); it != editorLogs.end(); ++it) {
			ImGui::Text((*it).c_str());
		}
	}
	ImGui::EndChild();

	ImGui::End();
}

void EditorUI::UIAssetsWindow() {

	ImGui::Begin("Assets");

	for (int i = 0; i < editor->gameApp->allAssets.size(); ++i) {

		bool isSelected = false;

		if (editor->editorObjectSelector->GetAssetSelected() != NULL && editor->editorObjectSelector->GetAssetSelected() == &editor->gameApp->allAssets[i]) {
			isSelected = true;
		}
		
		if (ImGui::Selectable(editor->gameApp->allAssets[i].name.c_str(), isSelected)) {
			editor->editorObjectSelector->SetAssetSelected(&editor->gameApp->allAssets[i]);
			if (editor->gameApp->allAssets[i].name.ends_with(".fbx")) {
				GameObject* object = editor->gameApp->scene->AddGameObject();
				auto mesh_ptrs = Mesh::loadFromFile(editor->gameApp->allAssets[i].libraryPath);
				object->AddMeshWithTexture(mesh_ptrs);
			}
			else if (editor->gameApp->allAssets[i].name.ends_with(".png") || editor->gameApp->allAssets[i].name.ends_with(".dds") || editor->gameApp->allAssets[i].name.ends_with(".tga")) {
				if (editor->editorObjectSelector->GetGameObjectSelected() != nullptr) {
					TextureComponent* textureComponent = editor->editorObjectSelector->GetGameObjectSelected()->GetComponent<TextureComponent>();
					MeshComponent* meshComponent = editor->editorObjectSelector->GetGameObjectSelected()->GetComponent<MeshComponent>();
					if (meshComponent->getMesh() != nullptr) {
						meshComponent->getMesh()->loadTextureToMesh(editor->gameApp->allAssets[i].libraryPath[0]);
						textureComponent->setTexture(meshComponent->getMesh()->texture);
						editor->AddLog("Texture " + editor->gameApp->allAssets[i].libraryPath[0] + " applied to " + editor->editorObjectSelector->GetGameObjectSelected()->name);
					}

				}
			}

			editor->gameApp->AddLog(editor->gameApp->allAssets[i].name + " loaded");

		}
		if (ImGui::BeginPopupContextItem()) {
			editor->editorObjectSelector->SetAssetSelected(&editor->gameApp->allAssets[i]);

			if (ImGui::Selectable("Delete")) {
				std::remove(editor->gameApp->allAssets[i].assetsPath.c_str());
				for (int j = 0; j < editor->gameApp->allAssets[i].libraryPath.size(); ++j) {
					std::remove(editor->gameApp->allAssets[i].libraryPath[j].c_str());
				}
				editor->gameApp->allAssets.erase(editor->gameApp->allAssets.begin() + i);

			}
			
			ImGui::EndPopup();
		}
		ImGui::SetItemTooltip(editor->gameApp->allAssets[i].assetsPath.c_str());
		
	}
		

	ImGui::End();
}
