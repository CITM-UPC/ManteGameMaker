#include "EditorApp.h"

#include "EditorWindow.h"

EditorWindow::EditorWindow() : EditorModule() {

}

EditorWindow::~EditorWindow() {

}

bool EditorWindow::Start() {

	// setup SDL window
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// GL 3.1 + GLSL 130
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	Uint32 flags;
	flags = SDL_WINDOW_OPENGL;
	if (isFullscreen) { flags |= SDL_WINDOW_FULLSCREEN; }
	if (isResizable) { flags |= SDL_WINDOW_RESIZABLE; }
	if (isBorderless) { flags |= SDL_WINDOW_BORDERLESS; }

	window = SDL_CreateWindow("SDL+OpenGL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);

	if (!window)
		throw exception(SDL_GetError());

	glContext = SDL_GL_CreateContext(window);
	if (!glContext)
		throw exception(SDL_GetError());

	if (SDL_GL_MakeCurrent(window, glContext) != 0)
		throw exception(SDL_GetError());

	if (SDL_GL_SetSwapInterval(1) != 0)
		throw exception(SDL_GetError());

	// init opengl
	auto glew_init_error = glewInit();
	if (glew_init_error != GLEW_OK)
		throw exception((char*)glewGetErrorString(glew_init_error));
	if (!GLEW_VERSION_3_1)
		throw exception("OpenGL 3.1 Not Supported!");

	editor->AddLog("OpenGL Compiled with " + (string)(const char*)glGetString(GL_VERSION));

	editor->AddLog("GLEW Compiled with " + (string)(const char*)glewGetString(GLEW_VERSION));

	glViewport(0, 0, width, height);
	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

bool EditorWindow::PreUpdate() {

	return true;
}

bool EditorWindow::Update() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	return true;
}

bool EditorWindow::PostUpdate() {

	return true;
}

void EditorWindow::Render() {

	SDL_GL_SwapWindow(window);
}

bool EditorWindow::CleanUp() {

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);

	return true;
}

void EditorWindow::UpdateFullscreen() {

	if (isFullscreen) {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}
	else {
		SDL_SetWindowFullscreen(window, 0);
	}
}

void EditorWindow::UpdateResizable() {

	SDL_SetWindowResizable(window, (SDL_bool)isResizable);
}

void EditorWindow::UpdateBorderless() {

	SDL_SetWindowBordered(window, (SDL_bool)!isBorderless);
}

void EditorWindow::UpdateSizes(int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
	glViewport(0, 0, width, height);
}
