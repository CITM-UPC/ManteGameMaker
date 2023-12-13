#include "WindowHierarchy.h"
#include "App.h"
WindowHierarchy::WindowHierarchy(string name, bool startEnabled, ImGuiWindowFlags flags) : GuiBase(name, startEnabled, flags) {};

void WindowHierarchy::TreeDeploy(GameObject* gameObject)
{
     bool isParent = gameObject->emptyGameObject && !gameObject->GetChildrenList()->empty();

    // Display del game object y manejo de la selección
    if (ImGui::Selectable(gameObject->GetName(), gameObject->selected))
    {
        // Manejar la selección/deselección
        if (lastSelected != gameObject) {
            // Deseleccionar el último objeto seleccionado (si hay alguno)
            if (lastSelected) {
                lastSelected->selected = false;
                lastSelected = nullptr;
            }

            // Seleccionar el nuevo objeto
            gameObject->selected = true;
            lastSelected = gameObject;

            // Expandir el parent si es un emptyGameObject
            if (isParent) {
                gameObject->expanded = !gameObject->expanded;
            }
        }
        else {
            // Deseleccionar si se hace clic en el mismo objeto nuevamente
            gameObject->selected = false;
            lastSelected = nullptr;

            // Expandir el parent si es un emptyGameObject
            if (isParent) {
                gameObject->expanded = !gameObject->expanded;
            }
        }
    }

    if (gameObject->expanded || gameObject->selected)
    {
        ImGui::Indent();

        int meshesAmount = 1;
        // Display de meshes
        for (auto& meshItem : gameObject->mesh_ptr)
        {
            std::string newName = (std::string)gameObject->GetName() + "_Mesh_" + std::to_string(meshesAmount);
            ImGui::Selectable(newName.c_str());
            ++meshesAmount;
        }

        // Display de hijos si es un empty parent
        if (isParent)
        {
            for (auto& child : *gameObject->GetChildrenList())
            {
                TreeDeploy(child);
            }
        }

        ImGui::Unindent();
    }
}

void WindowHierarchy::Update() {

    for (auto& item : app->engineManager->GetEngine()->hierarchy)
    {
        TreeDeploy(item);
    }

}