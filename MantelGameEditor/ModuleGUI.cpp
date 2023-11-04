#include "ModuleGUI.h"
#include "App.h"

ModuleGUI::ModuleGUI(bool startEnabled) : Module(startEnabled)
{
	show_demo_window = true;
	show_another_window = false;
	name = "GUI";
}

ModuleGUI::~ModuleGUI() {}

void ModuleGUI::Awake() {
	this->ImguiInit(app->window->GetWindow(), app->window->GetContext());
}

void ModuleGUI::Start() {

}

bool ModuleGUI::PreUpdate() {


	return true;
}

bool ModuleGUI::Update(std::chrono::duration<double> dt) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow(); // Show demo window! :)
	this->ImguiExample();
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);

	return true;
}

bool ModuleGUI::PostUpdate() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return true;
}

void ModuleGUI::ImguiInit(SDL_Window *window, SDL_GLContext gl_context){
	// Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup ImGui for SDL and OpenGL3
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	// Set up ImGui style (optional)
	ImGui::StyleColorsDark();


}

void ModuleGUI::ImguiExample() {

	if (show_demo_window)
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("MantelGameMaker");						// Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");				// Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
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