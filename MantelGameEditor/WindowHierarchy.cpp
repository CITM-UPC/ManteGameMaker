#include "WindowHierarchy.h"
#include "App.h"
WindowHierarchy::WindowHierarchy(string name, bool startEnabled, ImGuiWindowFlags flags) : GuiBase(name, startEnabled, flags) {};

void WindowHierarchy::TreeDeploy(GameObject* gameObject)
{
    //for (auto& item2 : *item->GetChildrenList())
    //{
    //    if (ImGui::Selectable(item2->GetName(), item2->selected))
    //    {
    //        // Unselect all other items
    //        for (auto& otherItem2 : *item->GetChildrenList())
    //        {
    //            if (&otherItem2 != &item2)
    //            {
    //                otherItem2->selected = false;
    //                //cout << item->GetName() << " unselected" << endl;
    //            }
    //        }
    //        cout << item2->GetName() << " selected" << endl;
    //        item2->selected = true;
    //    }
    //    if (item2->selected)
    //    {
    //        int meshesAmount = 1;
    //        ImGui::Indent();
    //        for (auto& meshItem : item2->mesh_ptr)
    //        {
    //            std::string newName = (std::string)(item2->GetName()) + "_Mesh_" + std::to_string(meshesAmount);
    //            ImGui::Selectable(newName.c_str());
    //            ++meshesAmount;

    //            if (item->emptyGameObject && !item->GetChildrenList()->empty())
    //            {
    //                TreeDeploy(item);
    //            }
    //        }
    //        ImGui::Unindent();
    //    }
    //}




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

    //for (auto& item : app->engineManager->GetEngine()->hierarchy)
    //{
    //    if (ImGui::Selectable(item->GetName(), item->selected))
    //    {
    //        // Unselect all other items
    //        for (auto& otherItem : app->engineManager->GetEngine()->hierarchy)
    //        {
    //            if (&otherItem != &item)
    //            {
    //                otherItem->selected = false;
    //                //cout << item->GetName() << " unselected" << endl;
    //            }
    //        }
    //        cout << item->GetName() << " selected" << endl;
    //        item->selected = true;
    //    }
    //    if (item->selected)
    //    {
    //        int meshesAmount = 1;
    //        ImGui::Indent(); 
    //        for (auto& meshItem : item->mesh_ptr)
    //        {
    //            std:string newName = (std::string)(item->GetName()) + "_Mesh_" + std::to_string(meshesAmount);
    //            ImGui::Selectable(newName.c_str());
    //            ++meshesAmount;
    //        }

    //        if (item->emptyGameObject && !item->GetChildrenList()->empty())
    //        {



    //            /////////////////////////////////////////////////////////////////////////////////////
    //             
    //            TreeDeploy(item);

    //            
    //            /////////////////////////////////////////////////////////////////////////////////////
    //        }

    //        ImGui::Unindent();
    //    }
    //}




    for (auto& item : app->engineManager->GetEngine()->hierarchy)
    {
        TreeDeploy(item);
    }

}