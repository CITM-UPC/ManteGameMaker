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
        //debug text to know if selected bool worked well
        //ImGui::Text("       Selected: %s", item->selected ? "true" : "false");
    }
}