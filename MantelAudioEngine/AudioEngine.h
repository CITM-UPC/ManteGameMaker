#pragma once
#include "AudioUtility.h"

//#define GAME_OBJECT_ID_BACKGROUNDMUSIC 100 //aka the camera?
//#define GAME_OBJECT_ID_SPATIALSOUND1 200
//#define GAME_OBJECT_ID_SPATIALSOUND2 300

class AudioEngine
{
public:
	AudioEngine();
	~AudioEngine();

	bool InitEngine();
	bool Start();
	bool Update();
	bool CleanUp();

	void PlayEngine();
	void PauseEngine();

	void SetListenerTransform(float posx, float posy, float posz, float ofx, float ofy, float ofz, float otx, float oty, float otz);
	void SetSpatial1Transform(float posx, float posy, float posz);
	void SetSpatial2Transform(float posx, float posy, float posz);

	void AddLog(string l)
	{
		l = "AudioEngine: " + l;
		cout << l << endl;
	}

	bool isGameOn = false;

private:
	//aka the camera(?
	AkGameObjectID GAME_OBJECT_ID_BACKGROUNDMUSIC = 100;
	//static
	AkGameObjectID GAME_OBJECT_ID_SPATIALSOUND1 = 200;
	//moving
	AkGameObjectID GAME_OBJECT_ID_SPATIALSOUND2 = 300;

private:

	bool music1eventFinished = true;
	bool music2eventFinished = false;
	bool spatial1eventFinished = true;
	bool spatial2eventFinished = true;

	void toggleMusic1EventFinished()
	{
		music1eventFinished = !music1eventFinished;
	}
	void toggleMusic2EventFinished()
	{
		music2eventFinished = !music2eventFinished;
	}
	void toggleSpatial1EventFinished()
	{
		spatial1eventFinished = !spatial1eventFinished;
	}
	void toggleSpatial2EventFinished()
	{
		spatial2eventFinished = !spatial2eventFinished;
	}
};