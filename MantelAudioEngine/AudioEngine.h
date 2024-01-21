#pragma once
#include "AudioUtility.h"

class AudioEngine
{
public:
	AudioEngine();
	~AudioEngine();

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