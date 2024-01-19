#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
}

AudioEngine::~AudioEngine()
{
}

bool AudioEngine::Start()
{
	// MEMORY MANAGER
	AkMemSettings memSettings;
	AK::MemoryMgr::GetDefaultSettings(memSettings);

	if (AK::MemoryMgr::Init(&memSettings) == AK_Success)
	{
		AddLog("Created Memory manager");
	}
	else
	{
		AddLog("Could not create memory manager");
		return false;
	}

	// STREAM MANAGER
	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);

	if (AK::StreamMgr::Create(stmSettings))
	{
		AddLog("Streaming Manager created");
		return false;
	}
	else
	{
		AddLog("Could not create the Streaming Manager");
		return false;
	}

	// STREAM DEVICE
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	//next line crashes program
	//CAkFilePackageLowLevelIODeferred g_lowLevelIO;

	//if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	//{
	//	AddLog("Could not create the streaming device and Low-Level I/O system");
	//	return false;
	//}

	return true;
}

bool AudioEngine::Update()
{
	return true;
}

bool AudioEngine::CleanUp()
{
	return true;
}
