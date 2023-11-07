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
					if (ImGui::Button(std::to_string(item->transform.position.x).c_str())) {
						// Code to modify position parameter x, not yet to add
					}
					ImGui::Text("y: ");
					ImGui::SameLine(); // Add spacing between the buttons

					if (ImGui::Button(std::to_string(item->transform.position.y).c_str())) {
						// Code to modify position parameter x, not yet to add
					}
					ImGui::Text("z: ");
					ImGui::SameLine(); // Add spacing between the buttons

					if (ImGui::Button(std::to_string(item->transform.position.z).c_str())) {
						// Code to modify position parameter x, not yet to add
					}

					//rotation of transform
					ImGui::Text("Rotation");
					// Create three squares with "0" inside
					ImGui::Text("x: ");
					ImGui::SameLine(); // Ensure items are on the same line
					//Position parameters
					if (ImGui::Button(std::to_string(item->transform.rotation.x).c_str())) {
						// Code to modify position parameter x, not yet to add
					}
					ImGui::Text("y: ");
					ImGui::SameLine(); // Add spacing between the buttons

					if (ImGui::Button(std::to_string(item->transform.rotation.y).c_str())) {
						// Code to modify position parameter x, not yet to add
					}
					ImGui::Text("z: ");
					ImGui::SameLine(); // Add spacing between the buttons

					if (ImGui::Button(std::to_string(item->transform.rotation.z).c_str())) {
						// Code to modify position parameter x, not yet to add
					}

					//scale of transform
					ImGui::Text("Scale");
					// Create three squares with "0" inside
					ImGui::Text("x: ");
					ImGui::SameLine(); // Ensure items are on the same line
					//Position parameters
					if (ImGui::Button(std::to_string(item->transform.scale.x).c_str())) {
						// Code to modify position parameter x, not yet to add
					}
					ImGui::Text("y: ");
					ImGui::SameLine(); // Add spacing between the buttons

					if (ImGui::Button(std::to_string(item->transform.scale.y).c_str())) {
						// Code to modify position parameter x, not yet to add
					}
					ImGui::Text("z: ");
					ImGui::SameLine(); // Add spacing between the buttons

					if (ImGui::Button(std::to_string(item->transform.scale.z).c_str())) {
						// Code to modify position parameter x, not yet to add
					}
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Mesh")) {
					// Content for the "Mesh" tab
					ImGui::Text("Information about mesh:");
					ImGui::Text("  This GameObject has %d meshes\n\n\n",item->mesh_ptr.size());

					for (size_t i = 0; i < item->mesh_ptr.size(); i++)
					{
						ImGui::Text("Information about mesh number: %d", i);
						ImGui::Text("  Number of Verts: %d", item->mesh_ptr[i].get()->getNumVerts());
						ImGui::Text("  Number of Indexs: %d", item->mesh_ptr[i].get()->getNumIndexs());
						ImGui::Text("  Number of Triangles: %f\n\n", float((float)item->mesh_ptr[i].get()->getNumIndexs()/3));

					}
					ImGui::Checkbox("Show Normals", &item->showNormals);
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Texture")) {
					// Content for the "Texture" tab
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
	}
}