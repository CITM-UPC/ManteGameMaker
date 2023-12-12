#pragma once

#include "Globals.h"
#include "App.h"

static bool MenuBarUpdate() {

    ImGui::StyleColorsLight();

    if (ImGui::BeginMenuBar()) {
        MyGameEngine* engine_p = app->engineManager->GetEngine();

        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene", "Ctrl+N", false, false)) {          // Temporarly disabled because not implemented (..., false, false)
                // Code to execute when "New Scene" is selected
            }
            if (ImGui::MenuItem("Open Scene", "Ctrl+O", false, false)) {          // Temporarly disabled because not implemented (..., false, false)
                // Code to execute when "Open Scene" is selected
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Save", "Ctrl+S", false, false)) {          // Temporarly disabled because not implemented (..., false, false)
                // Code to execute when "Save" is selected
            }
            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S", false, false)) {          // Temporarly disabled because not implemented (..., false, false)
                // Code to execute when "Save As..." is selected
            }

            ImGui::Separator();

            if (ImGui::MenuItem("New Project...", NULL, false, false)) {          // Temporarly disabled because not implemented (..., false, false)
                // Code to execute when "New Project..." is selected
            }
            if (ImGui::MenuItem("Open Project...", NULL, false, false)) {          // Temporarly disabled because not implemented (..., false, false)
                // Code to execute when "Open Project..." is selected
            }
            if (ImGui::MenuItem("Save Project", NULL, false, false)) {          // Temporarly disabled because not implemented (..., false, false)
                // Code to execute when "Save Project" is selected
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Exit", NULL, false)) {
                return false;
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z", false, false)) {          // Temporarly disabled because not implemented (..., false, false)
                // Code to execute when "Undo" is selected
            }
            if (ImGui::MenuItem("Redo", "Ctrl+Shift+Z", false, false)) {          // Temporarly disabled because not implemented (..., false, false)
                // Code to execute when "Redo" is selected
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Select All", "Ctrl+A", false, false)) {          // Temporarly disabled because not implemented (..., false, false)
                // Code to execute when "Select All" is selected
            }
            if (ImGui::MenuItem("Deselect All", "Shift+D", false, false)) {          // Temporarly disabled because not implemented (..., false, false)
                // Code to execute when "Deselect All" is selected
            }
            if (ImGui::MenuItem("Select Children", "Shift+C", false, false)) {          // Temporarly disabled because not implemented (..., false, false)
                // Code to execute when "Select Children" is selected
            }
            if (ImGui::MenuItem("Invert Selection", "Ctrl+I", false, false)) {          // Temporarly disabled because not implemented (..., false, false)
                // Code to execute when "Invert Selection" is selected
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Duplicate", "Ctrl+D", false, false)) {          // Temporarly disabled because not implemented (..., false, false)
                // Code to execute when "Duplicate" is selected
            }
            if (ImGui::MenuItem("Rename", NULL, false, false)) {          // Temporarly disabled because not implemented (..., false, false)
                // Code to execute when "Rename" is selected
            }
            if (ImGui::MenuItem("Delete", NULL, false, false)) {          // Temporarly disabled because not implemented (..., false, false)
                // Code to execute when "Delete" is selected
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Play", NULL, false, true)) {          // Temporarly disabled because not implemented (..., false, false)
                // Code to execute when "Play" is selected
                app->engineManager->GetEngine()->engineState = EngineState::PLAY;
                for (auto& item : app->engineManager->GetEngine()->hierarchy)
                {
                    if (item->camera)
                    {
                        app->engineManager->GetEngine()->actualCamera = item->GetCamera();
                        break;
                    }
                }
            }
            if (ImGui::MenuItem("Pause", NULL, false, false)) {          // Temporarly disabled because not implemented (..., false, false)
                // Code to execute when "Pause" is selected
            }
            if (ImGui::MenuItem("Stop", NULL, false, true)) {          // Temporarly disabled because not implemented (..., false, false)
                // Code to execute when "Stop" is selected
                app->engineManager->GetEngine()->engineState = EngineState::EDITOR;
                app->engineManager->GetEngine()->actualCamera = &app->engineManager->GetEngine()->camera;
            }

            ImGui::Separator();

            if (ImGui::Button("Settings...")) {
                app->gui->settingsState = true;
                ImGui::OpenPopup("Settings Window");
            }

            if (ImGui::BeginPopupModal("Settings Window", &app->gui->settingsState, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Not working yet...");


                ImGui::EndPopup();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
            //if (ImGui::Checkbox("Grid XY", &engine_p->grid_xy));

            //if (ImGui::Checkbox("Grid XZ", &engine_p->grid_xz));

            ImGui::Separator();

            if (ImGui::BeginMenu("Draw Mode")) {
                static int selectedOption = 0;

                if (ImGui::RadioButton("Mesh", selectedOption == 0)) {
                    selectedOption = 0;
                    // Code to execute when "Mesh" is selected
                }

                if (ImGui::RadioButton("Wireframe", selectedOption == 1)) {
                    selectedOption = 1;
                    // Code to execute when "Wireframe" is selected
                }

                if (ImGui::RadioButton("Vertex", selectedOption == 2)) {
                    selectedOption = 2;
                    // Code to execute when "Vertex" is selected
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Show")) {
                bool selection_vertexNormals;
                if (ImGui::Checkbox("Vertex Normals", &selection_vertexNormals));          // Temporarly disabled because not implemented (..., false, false)

                bool selection_BoundingBoxes;
                if (ImGui::Checkbox("Bounding Boxes", &selection_BoundingBoxes));          // Temporarly disabled because not implemented (..., false, false)

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("GameObjects")) {

            if (ImGui::MenuItem("Create Empty", NULL, false, true))          // Temporarly disabled because not implemented (..., false, false)
            {
                GameObject* emptyGo = new GameObject("EmptyObject", "");
                emptyGo->visible = false;
                app->engineManager->GetEngine()->AddGameObject(emptyGo);
            }

            if (ImGui::MenuItem("Create Parent", NULL, false, true))          // Temporarly disabled because not implemented (..., false, false)
            {
                GameObject* emptyGo = new GameObject("EmptyParent", "", true);
                emptyGo->visible = false;
                app->engineManager->GetEngine()->AddGameObject(emptyGo);
                
                for (auto& item : app->engineManager->GetEngine()->hierarchy)
                {
                    if (item->selected)
                    {
                        //add item as a child of parent created
                        emptyGo->GetChildrenList()->push_back(item);
                    }
                    //remove item from hierarchy principal list
                    app->engineManager->GetEngine()->hierarchy.remove(item);
                    break;
                }
            }

            if (ImGui::MenuItem("Create Camera", NULL, false, true))          // Temporarly disabled because not implemented (..., false, false)
            {
                GameObject* cameraGo = new GameObject("Camera", "", false, true);
                app->engineManager->GetEngine()->AddGameObject(cameraGo);
            }

            GameObject* tGo = new GameObject("", "Assets/BakerHouse.fbx");
            if (ImGui::BeginMenu("3D Object")) {
                if (ImGui::MenuItem("Plane", NULL, false, true))          // Temporarly disabled because not implemented (..., false, false)
                {
                    tGo = new GameObject("Plane", "Assets/Primals/Plane.fbx");
                    app->engineManager->GetEngine()->AddGameObject(tGo);
                }
                if (ImGui::MenuItem("Cube", NULL, false, true))          // Temporarly disabled because not implemented (..., false, false)
                {
                    tGo = new GameObject("Cube", "Assets/Primals/Cube.fbx");
                    app->engineManager->GetEngine()->AddGameObject(tGo);
                }
                if (ImGui::MenuItem("Sphere", NULL, false, true))          // Temporarly disabled because not implemented (..., false, false)
                {
                    tGo = new GameObject("Sphere", "Assets/Primals/Sphere.fbx");
                    app->engineManager->GetEngine()->AddGameObject(tGo);
                }
                ImGui::EndMenu();
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Align View to Selected", "F", false, false)) {          // Temporarly disabled because not implemented (..., false, false)
                // Code to execute when "Play" is selected
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Windows")) {
            bool sceneState = true;
            if (app->gui->w_scene->GetWindowState() == state::disabled) { sceneState = false; }
            if (ImGui::MenuItem("Scene", NULL, sceneState)) {
                if (app->gui->w_scene->GetWindowState() != state::enabled) { app->gui->w_scene->WindowChangeState(state::enabled); }
            }

            bool gameState = false;     // Change to true whwn enabled
            //if (app->gui->w_game->GetWindowState() == States::DISABLED) { gameState = false; }
            if (ImGui::MenuItem("Game", NULL, gameState, false)) {          // Temporarly disabled because not implemented (..., false, false)
                //if (app->gui->w_game->GetWindowState() != States::ENABLED) { app->gui->w_game->WindowChangeState(States::ENABLED); }
            }

            bool hierarchyState = true;
            if (app->gui->w_hierarchy->GetWindowState() == state::disabled) { hierarchyState = false; }
            if (ImGui::MenuItem("Hierarchy", NULL, hierarchyState)) {
                if (app->gui->w_hierarchy->GetWindowState() != state::enabled) { app->gui->w_hierarchy->WindowChangeState(state::enabled); }
            }

            bool inspectorState = true;
            if (app->gui->w_inspector->GetWindowState() == state::disabled) { inspectorState = false; }
            if (ImGui::MenuItem("Inspector", NULL, inspectorState)) {
                if (app->gui->w_inspector->GetWindowState() != state::enabled) { app->gui->w_inspector->WindowChangeState(state::enabled); }
            }

            bool projectState = true;
            if (app->gui->w_project->GetWindowState() == state::disabled) { projectState = false; }
            if (ImGui::MenuItem("Project", NULL, projectState)) {
                if (app->gui->w_project->GetWindowState() != state::enabled) { app->gui->w_project->WindowChangeState(state::enabled); }
            }

            bool consoleState = true;
            if (app->gui->w_console->GetWindowState() == state::disabled) { consoleState = false; }
            if (ImGui::MenuItem("Console", NULL, consoleState)) {
                if (app->gui->w_console->GetWindowState() != state::enabled) { app->gui->w_console->WindowChangeState(state::enabled); }
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")) {
            if (ImGui::Button("About Mantel Game Engine")) {
                app->gui->aboutState = true;
                ImGui::OpenPopup("About this engine...");
            }

            if (ImGui::BeginPopupModal("About this engine...", &app->gui->aboutState, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoSavedSettings))
            {
                ImGui::SetNextWindowSize(ImVec2(50, 50));
                ImGui::SetNextWindowPos(ImVec2(0, 0));

                ImGui::Text("%s\n3D Game Engine developed in Barcelona VideoGame design and development UPC.\nCreated by:\n", app->GetAppDetails().name.c_str());

                for (const std::string& member : app->GetAppDetails().org.members) {
                    ImGui::Text("%s\n", member.c_str());
                }
                ImGui::Separator();

                ImGui::Text("3rd Party Libraries used:\n");
                ImGui::Bullet();
                if (ImGui::SmallButton("STL (C++11)")) {
                    app->RequestBrowser("https://www.geeksforgeeks.org/the-c-standard-template-library-stl/");
                }
                ImGui::Bullet();
                if (ImGui::SmallButton("SDL (v2.0.10)")) {
                    app->RequestBrowser("https://www.libsdl.org/");
                }
                ImGui::Bullet();
                if (ImGui::SmallButton("OpenGL (v3.1.0)")) {
                    app->RequestBrowser("https://www.opengl.org/");
                }
                ImGui::Bullet();
                if (ImGui::SmallButton("DevIL (v1.8.0)")) {
                    app->RequestBrowser("http://openil.sourceforge.net/");
                }
                ImGui::Bullet();
                if (ImGui::SmallButton("Assimp (v5.0.0)")) {
                    app->RequestBrowser("http://assimp.org/");
                }
                ImGui::Bullet();
                if (ImGui::SmallButton("Dear ImGui (v1.73, Base Code)")) {
                    app->RequestBrowser("https://github.com/ocornut/imgui");
                }
                ImGui::Text("  "); ImGui::SameLine();
                if (ImGui::SmallButton("Docking Test Branch (Commit 18/10/19)")) {
                    app->RequestBrowser("https://github.com/ocornut/imgui/tree/7feccf9ab2fad261aa873dfa067e64ad9fab8a03");
                }
                ImGui::Bullet();
                if (ImGui::SmallButton("glew (v2.0)")) {
                    app->RequestBrowser("http://glew.sourceforge.net/");
                }
                ImGui::Bullet();
                if (ImGui::SmallButton("MathGeoLib (v1.5)")) {
                    app->RequestBrowser("https://github.com/juj/MathGeoLib");
                }
                ImGui::Bullet();
                if (ImGui::SmallButton("Par (vN/A)")) {
                    app->RequestBrowser("https://github.com/prideout/par");
                }
                ImGui::Bullet();
                if (ImGui::SmallButton("JSON for Modern C++ (v3.7.0)")) {
                    app->RequestBrowser("https://github.com/nlohmann/json");
                }
                ImGui::Bullet();
                if (ImGui::SmallButton("Brofiler (v1.1.2)")) {
                    app->RequestBrowser("http://www.brofiler.com/");
                }
                ImGui::Bullet();
                if (ImGui::SmallButton("mmgr (vN/A)")) {
                    app->RequestBrowser("http://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml");
                }

                ImGui::Separator();

                if (ImGui::CollapsingHeader("License")) {

                }

                ImGui::EndPopup();
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Development Webpage", NULL, false, false))          // Temporarly disabled because not implemented (..., false, false)
                app->RequestBrowser("https://github.com/CITM-UPC/MantelGameMaker/wiki");

            if (ImGui::MenuItem("Download Latest Version", NULL, false, false))          // Temporarly disabled because not implemented (..., false, false)
                app->RequestBrowser("https://github.com/CITM-UPC/MantelGameMaker/releases");

            ImGui::Separator();

            if (ImGui::MenuItem("Report Bug || Suggest Feature"))
                app->RequestBrowser("https://github.com/CITM-UPC/MantelGameMaker/issues");

            if (ImGui::MenuItem("This Engine's GITHUB repository"))
                app->RequestBrowser("https://github.com/CITM-UPC/MantelGameMaker");



            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Text color

    return true;
}