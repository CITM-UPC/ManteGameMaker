#pragma once
#include "AudioUtility.h"

//registered id in banks
#define GAME_OBJECT_ID_MUSIC1 100
#define GAME_OBJECT_ID_MUSIC2 200
#define GAME_OBJECT_ID_SPATIALSOUND1 300
#define GAME_OBJECT_ID_SPATIALSOUND2 400



class AudioEngine
{
public:
	AudioEngine();
	~AudioEngine();

	bool InitEngine();
	bool Start();
	bool Update();
	bool CleanUp();


	void AddLog(string l)
	{
		l = "AudioEngine: " + l;
		cout << l << endl;
	}

private:
};