#include "App.h"

App::App(int argc, char* args[]) : argc(argc), args(args) {
    
    // Create the modules here (module = new Module(true);)
    window = new ModuleWindow(true);
    events = new ModuleEvents(true);
    engineManager = new ModuleEngineManager(true);
    gui = new ModuleGUI(true);
    renderer = new ModuleRenderer(true);

    // Add the modules to the modules list in the proper order (AddModule(module);)
    AddModule(window);
    AddModule(events);
    AddModule(engineManager);

    AddModule(gui);
    AddModule(renderer);
}

App::~App()
{
    // Release modules
    this->modules.clear();
}

void App::AddModule(Module* module) {
    this->modules.push_back(module);
}

void App::Awake() {
    targetFrameTime = 1 / frameRate;

    for (const auto& item : modules)
    {
        item->Awake();
        cout << "Performed Awake of Module " << item->name << endl;
    }
}

void App::Start() {

    for (const auto& item : modules)
    {
        if (item->GetState() == States::DISABLED)
            continue;

        item->Start();
        cout << "Performed Start of Module " << item->name << endl;
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

    cout << "==============================================================" << endl;

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
        if (item->GetState() == States::DISABLED)
            continue;

        ret = item->PreUpdate();
        cout << "Performed PreUpdate of Module " << item->name << endl;

        if (!ret) {
            cout << "Stopped execution on the PreUpdate of Module " << item->name << endl;
            break;
        }
    }
    cout << endl;

    return ret;
}

bool App::Update() {
    bool ret;
    for (const auto& item : modules)
    {
        if (item->GetState() == States::DISABLED)
            continue;

        ret = item->Update(this->dt);
        cout << "Performed Update of Module " << item->name << endl;

        if (!ret) {
            cout << "Stopped execution on the Update of Module " << item->name << endl;
            break;
        }
    }
    cout << endl;

    return ret;
}

bool App::PostUpdate() {
    bool ret;
    for (const auto& item : modules)
    {
        if (item->GetState() == States::DISABLED)
            continue;

        ret = item->PostUpdate();
        cout << "Performed PostUpdate of Module " << item->name << endl;

        if (!ret) {
            cout << "Stopped execution on the PostUpdate of Module " << item->name << endl;
            break;
        }
    }
    cout << endl;

    return ret;
}

void App::CleanUp() {

    for (const auto& item : modules)
    {
        item->CleanUp();
        cout << "Performed CleanUp of Module " << item->name << endl;
    }
}