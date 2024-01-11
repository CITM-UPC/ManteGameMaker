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
		logs.push_back(l);
	}
	vector<string> GetLogs()
	{
		return logs;
	}
	void ClearLogs()
	{
		logs.clear();
	}

private:
	vector<string> logs;
};