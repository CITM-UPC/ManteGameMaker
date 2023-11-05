#include "Globals.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(bool startEnabled) : Module(startEnabled)
{
    name = "window";
}

ModuleWindow::~ModuleWindow()
{
}

void ModuleWindow::Awake() {
    this->window = SDLWindowInit();
    this->gl_context = CreateWindowContext(window);
    OpenGLInit();
    SDL_GetWindowSize(window, &windowSize.x, &windowSize.y);
}

bool ModuleWindow::PreUpdate() {
    SDL_GetWindowSize(window, &windowSize.x, &windowSize.y);
    glViewport(0, 0, windowSize.x, windowSize.y);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return true;
}

void ModuleWindow::CleanUp() {

    cout << "Destroying SDL window and quitting all SDL systems" << endl;

    if (window != NULL) { SDL_DestroyWindow(window); }
    if (gl_context != NULL) { SDL_GL_DeleteContext(gl_context); }

    SDL_Quit();
}

SDL_Window* ModuleWindow::SDLWindowInit() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) throw exception(SDL_GetError());

    SDL_version compiled;
    SDL_VERSION(&compiled);
    cout << "SDL Compiled with " << to_string(compiled.major) << '.' << to_string(compiled.minor) << '.' << to_string(compiled.patch) << endl;

    SDL_version linked;
    SDL_GetVersion(&linked);
    cout << "SDL Linked with " << to_string(linked.major) << '.' << to_string(linked.minor) << '.' << to_string(linked.patch) << endl;

    // setup SDL window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // GL 3.1 + GLSL 130
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    Uint32 flags = SDL_WINDOW_SHOWN;
    if (WIN_FULLSCREEN == true) flags |= SDL_WINDOW_FULLSCREEN;
    if (WIN_BORDERLESS == true) flags |= SDL_WINDOW_BORDERLESS;
    if (WIN_RESIZABLE == true) flags |= SDL_WINDOW_RESIZABLE;
    if (WIN_FULLSCREEN_DESKTOP == true) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    if (OPENGL == true) flags |= SDL_WINDOW_OPENGL;

    auto window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, flags);
    if (!window) throw exception(SDL_GetError());

     return window;
}

SDL_GLContext ModuleWindow::CreateWindowContext(SDL_Window* window) {
    auto gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) throw exception(SDL_GetError());
    if (SDL_GL_MakeCurrent(window, gl_context) != 0) throw exception(SDL_GetError());
    if (SDL_GL_SetSwapInterval(1) != 0) throw exception(SDL_GetError());
    return gl_context;
}

void ModuleWindow::UpdateWindowContext(SDL_Window* window, SDL_GLContext gl_context) {
    if (!gl_context) throw exception(SDL_GetError());
    if (SDL_GL_MakeCurrent(window, gl_context) != 0) throw exception(SDL_GetError());
    if (SDL_GL_SetSwapInterval(1) != 0) throw exception(SDL_GetError());
}

void ModuleWindow::OpenGLInit() {
    auto glew_init_error = glewInit();
    if (glew_init_error != GLEW_OK) throw exception((char*)glewGetErrorString(glew_init_error));
    if (!GLEW_VERSION_3_1) throw exception("OpenGL 3.1 Not Supported!");
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glClearColor(1, 1, 1, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}