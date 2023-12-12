#include "WindowHierarchy.h"
#include "App.h"
WindowHierarchy::WindowHierarchy(string name, bool startEnabled, ImGuiWindowFlags flags) : GuiBase(name, startEnabled, flags) {};

void WindowHierarchy::Update() {

    for (auto& item : app->engineManager->GetEngine()->hierarchy)
    {
        if (ImGui::Selectable(item->GetName(), item->selected))
        {
            // Unselect all other items
            for (auto& otherItem : app->engineManager->GetEngine()->hierarchy)
            {
                if (&otherItem != &item)
                {
                    otherItem->selected = false;
                    //cout << item->GetName() << " unselected" << endl;
                }
            }
            cout << item->GetName() << " selected" << endl;
            item->selected = true;
        }
        if (item->selected)
        {
            int meshesAmount = 1;
            ImGui::Indent(); // Opcional: Indentar para mostrar la jerarquía visualmente
            for (auto& meshItem : item->mesh_ptr)
            {
                std:string newName = (std::string)(item->GetName()) + "_Mesh_" + std::to_string(meshesAmount);
                ImGui::Selectable(newName.c_str());
                ++meshesAmount;
            }
            ImGui::Unindent();
        }
    }
}