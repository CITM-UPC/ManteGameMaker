#include "ModuleGUI.h"
#include "App.h"

#include "GuiMenuBar.h"

ModuleGUI::ModuleGUI(bool startEnabled) : Module(startEnabled)
{
	name = "GUI";

	// Create the windows
	w_scene = new WindowScene("Scene", false);
	w_hierarchy = new WindowHierarchy("Hierarchy", true);
	w_inspector = new WindowInspector("Inspector", true);
	w_project = new WindowProject("Project", true);
	w_console = new WindowConsole("Console", true);
	w_settings = new WindowSettings("Settings", false);

	// Add the windows to the queue
	AddWindow(w_scene);
	AddWindow(w_hierarchy);
	AddWindow(w_inspector);
	AddWindow(w_project);
	AddWindow(w_console);
	AddWindow(w_settings);
}

ModuleGUI::~ModuleGUI() {}

void ModuleGUI::AddWindow(GuiBase* window) {
	this->windows.push_back(window);
}


void ModuleGUI::Awake() {
	w_scene->SceneFBO.Create_FrameBuffer(app->window->GetWindowSize().x, app->window->GetWindowSize().y);
	this->ImguiInit(app->window->GetWindow(), app->window->GetContext());
}

void ModuleGUI::Start() {

}

bool ModuleGUI::PreUpdate() {

	bool ret = true;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ret = this->ImguiDockspace();

	return ret;
}

bool ModuleGUI::Update(std::chrono::duration<double> dt) {

	this->ImguiSetup();

	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);

	return true;
}

bool ModuleGUI::PostUpdate() {
	ImGuiIO& io = ImGui::GetIO();

	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);

	io.DisplaySize = ImVec2(app->window->GetWindowSize().x, app->window->GetWindowSize().y);

	///
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	///

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		app->window->UpdateWindowContext(app->window->GetWindow(), app->window->GetContext());
	}

	return true;
}

void ModuleGUI::CleanUp() {
	// Cleanup ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	// Cleanup SDL
	/*SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();*/
}


void ModuleGUI::ImguiInit(SDL_Window *window, SDL_GLContext gl_context){
	// Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable window DockSpaces
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// Setup ImGui for SDL and OpenGL3
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	// Set up ImGui style (optional)
	ImGui::StyleColorsDark();
}

bool ModuleGUI::ImguiDockspace() {
	bool ret = true;

	// Enable docking (if not already enabled by default)
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	// Create a main dockspace
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
	{
		window_flags |= ImGuiWindowFlags_NoBackground;
	}

	if (!ImGui::Begin("MyDockSpace", NULL, window_flags)) throw exception("ImGui Window Begin returned false (Error on creating the Imgui Window)");
	ImGui::PopStyleVar(3);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		static auto deployed = false;  // Only done in the first dockspace setup (no Update needed)
		if (!deployed) {
			deployed = true;

			//ImGui::DockBuilderRemoveNode(dockspace_id); // clear previous layouts
			//ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
			//ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

			//ImGui::DockBuilderDockWindow("Scene", dockspace_id);
			//ImGui::DockBuilderDockWindow("Inspector", ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.2f, nullptr, &dockspace_id));
			//auto dock_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.3f, nullptr, &dockspace_id);
			//ImGui::DockBuilderDockWindow("Project", dock_down);
			//ImGui::DockBuilderDockWindow("Console", dock_down);
			//ImGui::DockBuilderDockWindow("Hierarchy", ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.25f, nullptr, &dockspace_id));
			//ImGui::DockBuilderFinish(dockspace_id);
		}
	}

	ret = MenuBarUpdate();

	ImGui::End();

	return ret;
}


void ModuleGUI::ImguiSetup() {
	for (const auto& item : windows)
	{
		if (item->GetWindowState() == state::disabled)
			continue;

		item->UpdateWindow();
	}
}
