#include "EditorApp.h"

#include <filesystem>

#include "GL/glew.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#include "EditorWindow.h"
#include "EditorUI.h"

#include "EditorObjectSelector.h"

#include "EditorInput.h"

EditorInput::EditorInput() : EditorModule()
{
}

EditorInput::~EditorInput()
{
}

bool EditorInput::Start() {

	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouseButtons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);

	return true;
}

bool EditorInput::PreUpdate() {

	string dropped_filedir;

	SDL_Event event;

	windowSizeHasUpdated = false;

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouseButtons[i] == KEY_DOWN)
			mouseButtons[i] = KEY_REPEAT;

		if (mouseButtons[i] == KEY_UP)
			mouseButtons[i] = KEY_IDLE;
	}

	mouseMotionX = 0;
	mouseMotionY = 0;

	mouseWheelScroll = 0;

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
		case SDL_MOUSEBUTTONDOWN:
			mouseButtons[event.button.button - 1] = KEY_DOWN;
			//LOG("Mouse button %d down", event.button.button-1);
			break;

		case SDL_MOUSEBUTTONUP:
			mouseButtons[event.button.button - 1] = KEY_UP;
			//LOG("Mouse button %d up", event.button.button-1);
			break;
		case (SDL_DROPFILE): {      // In case if dropped file
			dropped_filedir = event.drop.file;

			filesystem::path path("Assets");
			path = std::filesystem::absolute(path);
			filesystem::path droppedPath(dropped_filedir);

			if (dropped_filedir.ends_with(".fbx")) {
				if (droppedPath.parent_path() != path)
					filesystem::copy(dropped_filedir, "Assets", filesystem::copy_options::skip_existing);

				auto mesh_ptrs = MeshLoader::loadFromFile(dropped_filedir);
				Paths droppedFile;
				droppedFile.assetsPath = dropped_filedir;
				for (int i = 0; i < mesh_ptrs.size(); ++i)
				{
					droppedFile.libraryPath.push_back(mesh_ptrs[i]);
				}
				droppedFile.name = filesystem::path(dropped_filedir).filename().string();
				editor->gameApp->allAssets.push_back(droppedFile);
				editor->AddLog("Mesh loaded from " + dropped_filedir);

			}
			else if (dropped_filedir.ends_with(".png") || dropped_filedir.ends_with(".dds") || dropped_filedir.ends_with(".tga")) {
				if (droppedPath.parent_path() != path)
					filesystem::copy(dropped_filedir, "Assets", filesystem::copy_options::skip_existing);


			}
			else {

			}
		}
 
						   break;
		case SDL_MOUSEMOTION:
			mousePosX = event.motion.x;
			mousePosY = event.motion.y;

			mouseMotionX = event.motion.xrel;
			mouseMotionY = event.motion.yrel;
			break;
		case SDL_MOUSEWHEEL:
			mouseWheelScroll = event.wheel.y;
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
				windowSizeHasUpdated = true;
				editor->editorWindow->UpdateSizes(event.window.data1, event.window.data2);
			}
			break;
		}
	}



	return true;
}

bool EditorInput::Update() {

	return true;
}

bool EditorInput::PostUpdate() {

	return true;
}

bool EditorInput::CleanUp() {
	delete[] keyboard;

	return true;
}