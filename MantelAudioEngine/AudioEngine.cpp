#include "AudioEngine.h"

CAkFilePackageLowLevelIODeferred g_lowLevelIO;

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
	}
	else
	{
		AddLog("Could not create the Streaming Manager");
		return false;
	}

	// STREAM DEVICE
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	if (g_lowLevelIO.Init(deviceSettings) == AK_Success)
	{
		AddLog("Created stream device and Low-Level I/O System");
	}
	else
	{
		AddLog("Could not create the streaming device and Low-Level I/O system");
		return false;
	}

	//sound engine
	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) == AK_Success)
	{
		AddLog("Sound Engine Initialized");
	}
	else
	{
		AddLog("Could not initialize Sound Engine");
		return false;
	}


	//music engine
	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if (AK::MusicEngine::Init(&musicInit) == AK_Success)
	{
		AddLog("Succes on initialize the Music Engine.");
	}
	else
	{
		AddLog("Could not initialize the Music Engine");
		return false;
	}

	//spatial audio
	//AkSpatialAudioInitSettings settings; // The constructor fills AkSpatialAudioInitSettings with the recommended default settings. 

	//if (AK::SpatialAudio::Init(settings) != AK_Success)
	//{
	//	AddLog("Succes on initialize the Spatial Audio");
	//}
	//else
	//{
	//	AddLog("Could not initialize the Spatial Audio.");
	//	return false;
	//}

	//communications
#ifndef AK_OPTIMIZED
	// Initialize communications (not in release build!)

	//AkCommSettings commSettings;
	//AK::Comm::GetDefaultInitSettings(commSettings);
	//if (AK::Comm::Init(commSettings) != AK_Success)
	//{
	//	AddLog("Succes on initialize communication.");
	//}
	//else
	//{
	//	AddLog("Could not initialize communication");
	//	return false;
	//}
#endif // AK_OPTIMIZED


	//TODO: SET AS DYNAMIC PATH
	g_lowLevelIO.SetBasePath(AKTEXT("D:\\repos\\Github\\MantelGameMaker\\Wwise\\SDK\\samples\\IntegrationDemo\\WwiseProject\\GeneratedSoundBanks\\Windows"));
	AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(us)"));

	//bank init
	AkBankID bankID;
	if (AK::SoundEngine::LoadBank(BANKNAME_INIT, bankID) == AK_Success)
	{
		AddLog("Init bank loaded");
	}
	else
	{
		AddLog("Could not load init bank");
		return false;
	}

	if (AK::SoundEngine::LoadBank(BANKNAME_RAYCASTER, bankID) == AK_Success)
	{
		AddLog("Raycast bank loaded");
	}
	else
	{
		AddLog("Could not load raycast bank");
		return false;
	}

	AddLog("Audio System initialization completed");
	return true;
}

bool AudioEngine::Update()
{
	//if (AK::SoundEngine::RegisterGameObj(, "Theme") == AK_Success)
	//{

	//}

	return true;
}

bool AudioEngine::CleanUp()
{
	AK::SoundEngine::Term();
	AK::MusicEngine::Term();
#ifndef AK_OPTIMIZED
	AK::Comm::Term();
#endif // AK_OPTIMIZED

	g_lowLevelIO.Term();

	if (AK::IAkStreamMgr::Get())
	{
		AK::IAkStreamMgr::Get()->Destroy();
	}

	AK::MemoryMgr::Term();

	return true;
}
