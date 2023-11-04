#include "Globals.h"
#include "ModuleEvents.h"

ModuleEvents::ModuleEvents(bool startEnabled) : Module(startEnabled)
{
    name = "events";
}

ModuleEvents::~ModuleEvents() {}

void ModuleEvents::Awake() {
    SDL_Init(0);

    if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0) throw exception(SDL_GetError());
}

bool ModuleEvents::PreUpdate() {
    bool ret;

    ret = this->SDLEventsProcess();

    return ret;
}

bool ModuleEvents::SDLEventsProcess() {
    SDL_PumpEvents();

    static SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type) {
        case SDL_QUIT: return false;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE: return false;
            }
            break;
        }
    }

    return true;
}