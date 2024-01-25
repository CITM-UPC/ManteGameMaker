#pragma once
#include "AudioUtility.h"

class AudioEvent
{
public:
	AudioEvent();

	bool IsEventPlaying() const
	{
		return playing_id != 0L;
	}

	AkPlayingID playing_id = 0L;						// When Event is played, is different from 0L
	AkCallbackFunc event_call_back = nullptr;			// Call back function
};

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

	//callback stuff
	static void EventCallBack(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo)
	{
		AudioEvent* a_event = (AudioEvent*)in_pCallbackInfo->pCookie;

		switch (in_eType)
		{
		case(AkCallbackType::AK_EndOfEvent):
		{
			a_event->playing_id = 0L;
			break;
		}
		}
	}


private:
	//aka the camera(?
	AkGameObjectID GAME_OBJECT_ID_BACKGROUNDMUSIC = 100;
	//static
	AkGameObjectID GAME_OBJECT_ID_SPATIALSOUND1 = 200;
	//moving
	AkGameObjectID GAME_OBJECT_ID_SPATIALSOUND2 = 300;

private:

	//1st music background
	AudioEvent* music1;
	//2nd music background
	AudioEvent* music2;
	//static
	AudioEvent* spatial1;
	//moving
	AudioEvent* spatial2;

	//true: music1
	//false: music2
	bool nextSong = false;
};