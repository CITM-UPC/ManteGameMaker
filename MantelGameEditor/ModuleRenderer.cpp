#include "ModuleRenderer.h"
#include "App.h"

ModuleRenderer::ModuleRenderer(bool startEnabled) : Module(startEnabled)
{
    name = "renderer";
}

ModuleRenderer::~ModuleRenderer() {}

void ModuleRenderer::Awake() {

}

void ModuleRenderer::Start() {
    
}

bool ModuleRenderer::PreUpdate() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    return true;
}

bool ModuleRenderer::Update(duration<double> dt) {

    return true;
}

bool ModuleRenderer::PostUpdate() {
    //render of modules


    SDL_GL_SwapWindow(app->window->GetWindow());

    return true;
}

void ModuleRenderer::CleanUp() {

}