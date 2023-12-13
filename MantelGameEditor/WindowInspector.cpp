#include "WindowInspector.h"
#include "App.h"

WindowInspector::WindowInspector(string name, bool startEnabled, ImGuiWindowFlags flags) : GuiBase(name, startEnabled, flags) {};

void WindowInspector::Update() {

	for (auto& item : app->engineManager->GetEngine()->allGameObjects)
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
							if (item->camera && app->engineManager->GetEngine()->engineState == EngineState::PLAY)
							{
								item->GetCamera()->transform = item->transform;
								app->engineManager->GetEngine()->actualCamera->transform = item->transform;
							}
							//for (const auto& mesh : item->mesh_ptr)
							//{
							//	mesh.get()->boundingBoxMin -= tpos;
							//}
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

							if (item->camera && app->engineManager->GetEngine()->engineState == EngineState::PLAY)
							{
								item->GetCamera()->transform = item->transform;
								app->engineManager->GetEngine()->actualCamera->transform = item->transform;
							}

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

							if (item->camera && app->engineManager->GetEngine()->engineState == EngineState::PLAY)
							{
								item->GetCamera()->transform = item->transform;
								app->engineManager->GetEngine()->actualCamera->transform = item->transform;
							}


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

							if (item->camera && app->engineManager->GetEngine()->engineState == EngineState::PLAY)
							{
								item->GetCamera()->transform = item->transform;
								app->engineManager->GetEngine()->actualCamera->transform = item->transform;
							}


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

							if (item->camera && app->engineManager->GetEngine()->engineState == EngineState::PLAY)
							{
								item->GetCamera()->transform = item->transform;
								app->engineManager->GetEngine()->actualCamera->transform = item->transform;
							}


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

							if (item->camera && app->engineManager->GetEngine()->engineState == EngineState::PLAY)
							{
								item->GetCamera()->transform = item->transform;
								app->engineManager->GetEngine()->actualCamera->transform = item->transform;
							}

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

					if (item->camera)
					{
						ImGui::Text("Camera");
						// Create three squares with "0" inside
						ImGui::Text("FOV: ");
						ImGui::SameLine(); // Ensure items are on the same line
						//Position parameters
						double fov = app->engineManager->GetEngine()->actualCamera->fov;
						if (ImGui::InputDouble("          ", &fov)) {
							// enable writing mode
							app->gui->writing = true;
							// Code to modify position parameter x, not yet to add
							if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
							{
								if (item->camera && app->engineManager->GetEngine()->engineState == EngineState::PLAY)
								{
									item->GetCamera()->fov = fov;
									app->engineManager->GetEngine()->actualCamera->fov = fov;
								}
								cout << "New fov: " << fov << endl;
								//disable writing mode once we finished
								app->gui->writing = false;
							}
						}


						ImGui::Text("Aspect: ");
						ImGui::SameLine(); // Ensure items are on the same line
						//Position parameters
						double aspect = app->engineManager->GetEngine()->actualCamera->aspect;
						if (ImGui::InputDouble("           ", &aspect)) {
							// enable writing mode
							app->gui->writing = true;
							// Code to modify position parameter x, not yet to add
							if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
							{
								if (item->camera && app->engineManager->GetEngine()->engineState == EngineState::PLAY)
								{
									item->GetCamera()->aspect = aspect;
									app->engineManager->GetEngine()->actualCamera->aspect = aspect;
								}
								cout << "New aspect: " << aspect << endl;
								//disable writing mode once we finished
								app->gui->writing = false;
							}
						}

						ImGui::Text("zNear: ");
						ImGui::SameLine(); // Ensure items are on the same line
						//Position parameters
						double zNear = app->engineManager->GetEngine()->actualCamera->zNear;
						if (ImGui::InputDouble("            ", &zNear)) {
							// enable writing mode
							app->gui->writing = true;
							// Code to modify position parameter x, not yet to add
							if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
							{
								if (item->camera && app->engineManager->GetEngine()->engineState == EngineState::PLAY)
								{
									item->GetCamera()->zNear = zNear;
									app->engineManager->GetEngine()->actualCamera->zNear = zNear;
								}
								cout << "New zNear: " << zNear << endl;
								//disable writing mode once we finished
								app->gui->writing = false;
							}
						}

						ImGui::Text("zFar: ");
						ImGui::SameLine(); // Ensure items are on the same line
						//Position parameters
						double zFar = app->engineManager->GetEngine()->actualCamera->zFar;
						if (ImGui::InputDouble("             ", &zFar)) {
							// enable writing mode
							app->gui->writing = true;
							// Code to modify position parameter x, not yet to add
							if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
							{
								if (item->camera && app->engineManager->GetEngine()->engineState == EngineState::PLAY)
								{
									item->GetCamera()->zFar = zFar;
									app->engineManager->GetEngine()->actualCamera->zFar = zFar;
								}
								cout << "New zFar: " << zFar << endl;
								//disable writing mode once we finished
								app->gui->writing = false;
							}
						}

						ImGui::Text("camOffset: ");
						ImGui::SameLine(); // Ensure items are on the same line
						//Position parameters
						double camOffset = app->engineManager->GetEngine()->actualCamera->camOffset;
						if (ImGui::InputDouble("              ", &camOffset)) {
							// enable writing mode
							app->gui->writing = true;
							// Code to modify position parameter x, not yet to add
							if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
							{
								if (item->camera && app->engineManager->GetEngine()->engineState == EngineState::PLAY)
								{
									item->GetCamera()->camOffset = camOffset;
									app->engineManager->GetEngine()->actualCamera->camOffset = camOffset;
								}
								cout << "New camOffset: " << camOffset << endl;
								//disable writing mode once we finished
								app->gui->writing = false;
							}
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