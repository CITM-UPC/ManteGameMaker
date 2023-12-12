#include "WindowInspector.h"
#include "App.h"

WindowInspector::WindowInspector(string name, bool startEnabled, ImGuiWindowFlags flags) : GuiBase(name, startEnabled, flags) {};

void WindowInspector::Update() {

	for (auto& item : app->engineManager->GetEngine()->hierarchy)
	{
		if (item->selected)
		{
			if (ImGui::BeginTabBar("InspectorTabs")) {
				// Add tabs here

				if (ImGui::BeginTabItem("Transform")) {
					ImGui::Checkbox("Visible", &item->visible);

					// Content for the "Transform" tab
					//position of transform
					ImGui::Text("Position");
					// Create three squares with "0" inside
					ImGui::Text("x: ");
					ImGui::SameLine(); // Ensure items are on the same line
					//Position parameters
					vec3 tpos = item->transform.position;
					if (ImGui::InputDouble(" ", &(tpos.x))) {
						// enable writing mode
						app->gui->writing = true;
						// Code to modify position parameter x, not yet to add
						if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
						{
							item->transform.MoveTo(tpos);
							cout << "New position x: " << item->transform.position.x << endl;
							//disable writing mode once we finished
							app->gui->writing = false;
						}
					}
					ImGui::Text("y: ");
					ImGui::SameLine(); // Add spacing between the buttons

					if (ImGui::InputDouble("  ", &(tpos.y))) {
						// enable writing mode
						app->gui->writing = true;
						// Code to modify position parameter y, not yet to add
						if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
						{
							item->transform.MoveTo(tpos);
							cout << "New position y: " << item->transform.position.y << endl;
							//disable writing mode once we finished
							app->gui->writing = false;
						}
					}
					ImGui::Text("z: ");
					ImGui::SameLine(); // Add spacing between the buttons

					if (ImGui::InputDouble("   ", &(tpos.z))) {
						// enable writing mode
						app->gui->writing = true;
						// Code to modify position parameter z, not yet to add
						if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
						{
							item->transform.MoveTo(tpos);
							cout << "New position z: " << item->transform.position.z << endl;
							//disable writing mode once we finished
							app->gui->writing = false;
						}
					}

					//rotation of transform
					ImGui::Text("Rotation");
					// Create three squares with "0" inside
					ImGui::Text("x: ");
					ImGui::SameLine(); // Ensure items are on the same line
					//rotation parameters
					//temporary variable for rotation input
					vec3 trot = item->transform.rotation;
					if (ImGui::InputDouble("    ", &(trot.x))) {
						// enable writing mode
						app->gui->writing = true;
						// Code to modify rotation parameter x, not yet to add
						if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
						{
							item->transform.rotation.x = trot.x;
							cout << "New rotation x: " << item->transform.rotation.x << endl;
							//disable writing mode once we finished
							app->gui->writing = false;
						}
					}
					ImGui::Text("y: ");
					ImGui::SameLine(); // Add spacing between the buttons

					if (ImGui::InputDouble("     ", &(trot.y))) {
						// enable writing mode
						app->gui->writing = true;
						// Code to modify rotation parameter y, not yet to add
						if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
						{
							item->transform.rotation.y = trot.y;
							cout << "New rotation y: " << item->transform.rotation.y << endl;
							//disable writing mode once we finished
							app->gui->writing = false;
						}
					}
					ImGui::Text("z: ");
					ImGui::SameLine(); // Add spacing between the buttons

					if (ImGui::InputDouble("      ", &(trot.z))) {
						// enable writing mode
						app->gui->writing = true;
						// Code to modify rotation parameter z, not yet to add
						if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
						{
							item->transform.rotation.z = trot.z;
							cout << "New rotation z: " << item->transform.rotation.z << endl;
							//disable writing mode once we finished
							app->gui->writing = false;
						}
					}

					//scale of transform
					ImGui::Text("Scale");
					// Create three squares with "0" inside
					ImGui::Text("x: ");
					ImGui::SameLine(); // Ensure items are on the same line
					//Position parameters
					//temporary variable for scale input
					vec3 tscale = item->transform.scale;
					if (ImGui::InputDouble("       ", &(tscale.x))) {
						// enable writing mode
						app->gui->writing = true;
						// Code to modify scale parameter x, not yet to add
						if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
						{
							item->transform.scale = (tscale);
							cout << "New scale x: " << item->transform.scale.x << endl;
							//disable writing mode once we finished
							app->gui->writing = false;
						}
					}
					ImGui::Text("y: ");
					ImGui::SameLine(); // Add spacing between the buttons

					if (ImGui::InputDouble("        ", &(tscale.y))) {
						// enable writing mode
						app->gui->writing = true;
						// Code to modify scale parameter y, not yet to add
						if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
						{
							item->transform.scale = (tscale);
							cout << "New scale y: " << item->transform.scale.y << endl;
							//disable writing mode once we finished
							app->gui->writing = false;
						}
					}
					ImGui::Text("z: ");
					ImGui::SameLine(); // Add spacing between the buttons

					if (ImGui::InputDouble("         ", &(tscale.z))) {
						// enable writing mode
						app->gui->writing = true;
						// Code to modify scale parameter z, not yet to add
						if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
						{
							item->transform.scale = (tscale);
							cout << "New scale z: " << item->transform.scale.z << endl;
							//disable writing mode once we finished
							app->gui->writing = false;
						}
					}
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Mesh")) {
					// Content for the "Mesh" tab
					ImGui::Text("Information about mesh:");
					ImGui::Text("  This GameObject has %d meshes\n\n\n",item->mesh_ptr.size());

					for (size_t i = 0; i < item->mesh_ptr.size(); i++)
					{
						ImGui::Text("Information about mesh number: %d", i+1);
						ImGui::Text("  Number of Verts: %d", item->mesh_ptr[i].get()->getNumVerts());
						ImGui::Text("  Number of Indexs: %d", item->mesh_ptr[i].get()->getNumIndexs());
						ImGui::Text("  Number of Triangles: %f\n\n", float((float)item->mesh_ptr[i].get()->getNumIndexs()/3));

					}
					ImGui::Checkbox("Show Normals", &item->showNormals);
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Texture")) {
					// Content for the "Texture" tab
					ImGui::Text("Information about Texture:");
					ImGui::Text("  Size of Texture:\n  1024 x 1024\n\n");
					ImGui::Text("  Path of Texture file:\n  %s", item->texturePath.c_str());
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
	}
}