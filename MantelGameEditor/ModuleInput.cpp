#include "Globals.h"
#include "App.h"
#include "ModuleInput.h"

#include "imgui.h" //Testing
#include "imgui_impl_sdl2.h"
#include <iostream>

ModuleInput::ModuleInput(bool start_enabled) : Module(start_enabled)
{
	name = "input";
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
void ModuleInput::Start()
{
	cout << "Init SDL input event system" << endl;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		cout << "SDL_EVENTS could not initialize! SDL_Error: " << SDL_GetError() << endl;
	}
}

// Called every draw update
bool ModuleInput::PreUpdate()
{
	SDL_PumpEvents();

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

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for (int i = 0; i < 5; ++i)
	{
		if (buttons & SDL_BUTTON(i))
		{
			if (mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if (mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	std::string filePath;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

		case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

		case SDL_QUIT:
			quit = true;
			break;

		case SDL_DROPFILE:

			filePath = e.drop.file;

			if (filePath.substr(filePath.find_last_of(".") + 1) == "fbx") {
				cout << ".fbx file detected" << endl;

				app->engineManager->GetEngine()->AddGameObject(new GameObject("", filePath));
			}
			if (filePath.substr(filePath.find_last_of(".") + 1) == "png") {
				cout << ".png file detected" << endl;
				for (auto& item : app->engineManager->GetEngine()->allGameObjects)
				{
					if (item->selected)
					{
						item->texture = nullptr;
						item->texture = make_shared<Texture2D>(filePath);
						for (size_t i = 0; i < item->mesh_ptr.size(); i++)
						{
							item->mesh_ptr[i].get()->texture = item->texture;
							item->texturePath = filePath;
						}
						break;
					}
				}
			}

			SDL_free(e.drop.file);

			break;

		case SDL_WINDOWEVENT:
		{
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				//App->gEngine->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
		}
		}

		//This here works (sends input to ImGui). Maybe should be in ModuleUI.cpp?
		ImGui_ImplSDL2_ProcessEvent(&e);
		if (e.type == SDL_DROPFILE)
		{
			//app->gui->w_project->HandleDropEvent();
		}
	}

	if (quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return false;

	return true;
}

// Called before quitting
void ModuleInput::CleanUp()
{
	cout << "Quitting SDL input event subsystem" << endl;
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
}