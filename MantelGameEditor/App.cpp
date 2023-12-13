#include "App.h"

App::App(int argc, char* args[]) : argc(argc), args(args) {
    
    list<string> members = { "Luis Gonzalez", "Adrian Ponce" };
    details.name = "MantelGameEngine";
    details.org.name = "Mantel";
    details.org.members = members;

    // Create the modules here (module = new Module(true);)
    window = new ModuleWindow(true);
    input = new ModuleInput(true);
    events = new ModuleEvents(true);
    engineManager = new ModuleEngineManager(true);
    gui = new ModuleGUI(true);
    renderer = new ModuleRenderer(true);

    // Add the modules to the modules list in the proper order (AddModule(module);)
    AddModule(window);
    AddModule(input);
    AddModule(events);
    AddModule(engineManager);

    AddModule(gui);
    AddModule(renderer);

    dualOutputBuffer = new DualOutputBuffer(std::cout.rdbuf(), consoleOutput);

    std::cout.rdbuf(dualOutputBuffer);
}

App::~App()
{
    // Release modules
    this->modules.clear();
}

void App::AddModule(Module* module) {
    //push modules to the list
    this->modules.push_back(module);
}

void App::Awake() {
    targetFrameTime = 1 / frameRate;

    for (const auto& item : modules)
    {
        item->Awake();
        cout << "    --Awake of Module " << item->name << "--" << endl;
    }
}

void App::Start() {

    for (const auto& item : modules)
    {
        if (item->GetState() == state::disabled)
            continue;

        item->Start();
        cout << "    --Start of Module " << item->name << "--" << endl;
    }
}

bool App::DoUpdate() {
	// Record the start time of the frame
	auto frameStartTime = std::chrono::high_resolution_clock::now();

    // Update the delta time for this frame execution
    bool ret = true;

    this->UpdateDeltaTime();

	// Update the active modules
    if (ret) { ret = this->PreUpdate(); }

    if (ret) { ret = this->Update(); }    

    if (ret) { ret = this->PostUpdate(); }

    // Calculate the elapsed time for the frame
    auto frameEndTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> frameDuration = frameEndTime - frameStartTime;

    // Calculate the sleep time based on the current target frame rate
    double sleepTime = targetFrameTime - frameDuration.count();

    if (sleepTime > 0) {
        std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
    }

    return ret;
}

bool App::PreUpdate() {
    bool ret;
    for (const auto& item : modules)
    {
        if (item->GetState() == state::disabled)
            continue;

        ret = item->PreUpdate();

        if (!ret) {
            cout << " !!!! Stopped execution on the PreUpdate of Module " << item->name << endl;
            break;
        }
    }

    return ret;
}

bool App::Update() {
    bool ret;
    for (const auto& item : modules)
    {
        if (item->GetState() == state::disabled)
            continue;

        ret = item->Update(this->dt);

        if (!ret) {
            cout << " !!!! Stopped execution on the Update of Module " << item->name << endl;
            break;
        }
    }

    return ret;
}

bool App::PostUpdate() {

    bool ret;
    for (const auto& item : modules)
    {
        if (item->GetState() == state::disabled)
            continue;

        ret = item->PostUpdate();

        if (!ret) {
            cout << " !!!! Stopped execution on the PostUpdate of Module " << item->name << endl;
            break;
        }
    }


    //lets save/load once postupdate is finished
    if (loadGameRequested == true) LoadFromFile();
    if (saveGameRequested == true) SaveToFile();
    
    return ret;
}

void App::CleanUp() {

    for (const auto& item : modules)
    {
        item->CleanUp();
        cout << "    --CleanUp of Module " << item->name << "--" << endl;
    }

    if (dualOutputBuffer != nullptr)
    {
        std::cout.rdbuf(dualOutputBuffer->GetOriginalStream());
        RELEASE(dualOutputBuffer);
    }
}


void App::LoadGameRequest(string filePath)
{
    this->loadPathFile = filePath;
    // NOTE: We should check if SAVE_STATE_FILENAME actually exist
    loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest()
{
    // NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
    saveGameRequested = true;
}


bool App::LoadFromFile()
{
    bool ret = true;

    pugi::xml_document gameStateFile;
    pugi::xml_parse_result result = gameStateFile.load_file(this->loadPathFile.c_str());

    if (result == NULL)
    {
        cout << "Could not load xml file savedScene.mantel. pugi error: " << result.description() << endl;
        ret = false;
    }
    else
    {
        for (auto& item : modules)
        {
            pugi::xml_node childNode = gameStateFile.child("savedScene").child(item->name.c_str());
            ret = item->LoadState(childNode);
        }
    }

    loadGameRequested = false;

    return ret;
}

bool App::SaveToFile()
{
    bool ret = false;

    pugi::xml_document* saveDoc = new pugi::xml_document();
    pugi::xml_node saveStateNode = saveDoc->append_child("savedScene");


    for (auto& item : modules)
    {
        pugi::xml_node childNode = saveStateNode.append_child(item->name.c_str());
        ret = item->SaveState(childNode);
    }

    ret = saveDoc->save_file(this->loadPathFile.c_str());

    saveGameRequested = false;

    return ret;
}