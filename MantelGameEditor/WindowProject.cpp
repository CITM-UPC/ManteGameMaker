#include "App.h"
#include "WindowProject.h"
#include <filesystem>

namespace fs = std::filesystem;

WindowProject::WindowProject(string name, bool startEnabled, ImGuiWindowFlags flags) : GuiBase(name, startEnabled, flags) {};

void WindowProject::DrawFileTree(const fs::path& path) {
    bool selected = false;
    for (const auto& entry : fs::directory_iterator(path)) {
        ImGuiTreeNodeFlags node_flags =
            ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

        if (entry.is_directory()) {
            node_flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
            bool node_open = ImGui::TreeNodeEx(entry.path().filename().string().c_str(), node_flags);

            if (node_open) {
                DrawFileTree(entry.path());
                ImGui::TreePop();
            }
        }
        else {
            //drag and drop doesnt work, so lets add items when double click
            if (ImGui::Selectable(entry.path().filename().string().c_str(), &selected) && ImGui::IsMouseDoubleClicked(0))
            {
                cout << entry.path().filename().string().c_str() << " selected" << endl;
                app->engineManager->GetEngine()->AddGameObject(new GameObject("", entry.path().string()));
            }
            //if (ImGui::Selectable(entry.path().filename().string().c_str()), &selected)
            //{

            //    selectedFilePath = entry.path().string();
            //    if (ImGui::BeginDragDropSource()) {
            //        ImGui::SetDragDropPayload("DRAG_FILE", selectedFilePath.c_str(), selectedFilePath.size() + 1);
            //        ImGui::Text("Dragging %s file", entry.path().filename().string().c_str());
            //        ImGui::EndDragDropSource();
            //    }
            //}
        }
    }
}

void WindowProject::HandleDropEvent() {

    if (selectedFilePath.substr(selectedFilePath.find_last_of(".") + 1) == "fbx") {
        cout << ".fbx file detected" << endl;

        app->engineManager->GetEngine()->AddGameObject(new GameObject("", selectedFilePath));
    }
    if (selectedFilePath.substr(selectedFilePath.find_last_of(".") + 1) == "png") {
        cout << ".png file detected" << endl;
        for (auto& item : app->engineManager->GetEngine()->hierarchy)
        {
            if (item->selected)
            {
                item->texture = nullptr;
                item->texture = make_shared<Texture2D>(selectedFilePath);
                for (size_t i = 0; i < item->mesh_ptr.size(); i++)
                {
                    item->mesh_ptr[i].get()->texture = item->texture;
                    item->texturePath = selectedFilePath;
                }
                break;
            }
        }
    }
}


void WindowProject::Update() {

    fs::path assets_path = fs::current_path() / "Assets";
    ////drag and drop from imgui not working
    //if (selectedFilePath.size() != 0)
    //{
    //    cout << "Dragging and dropping " << selectedFilePath << " file." << endl;
    //}

    if (ImGui::TreeNode("Assets")) {
        DrawFileTree(assets_path);
        ImGui::TreePop();
    }
}