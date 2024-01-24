#include "AudioEngine.h"

CAkFilePackageLowLevelIODeferred g_lowLevelIO;

AudioEngine::AudioEngine()
{
}

AudioEngine::~AudioEngine()
{
}

bool AudioEngine::InitEngine()
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

	g_lowLevelIO.SetBasePath(AKTEXT("Assets\\wwise\\WwiseProject\\GeneratedSoundBanks\\Windows"));
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

	if (AK::SoundEngine::LoadBank(BANKNAME_MANTELENGINE, bankID) == AK_Success)
	{
		AddLog("MantelEngine bank loaded");
	}
	else
	{
		AddLog("Could not load MantelEngine bank");
		return false;
	}

	//communications
#ifndef AK_OPTIMIZED
	// Initialize communications (not in release build!)

	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if (AK::Comm::Init(commSettings) == AK_Success)
	{
		AddLog("Succes on initialize communication.");
	}
	else
	{
		AddLog("Could not initialize communication");
		return false;
	}
#endif // AK_OPTIMIZED


	//spatial audio
	AkSpatialAudioInitSettings settings; // The constructor fills AkSpatialAudioInitSettings with the recommended default settings. 

	if (AK::SpatialAudio::Init(settings) == AK_Success)
	{
		AddLog("Succes on initialize the Spatial Audio");
	}
	else
	{
		AddLog("Could not initialize the Spatial Audio.");
		return false;
	}

	return true;
}

bool AudioEngine::Start()
{
	//init engine itself
	if (InitEngine())
	{
		AddLog("Audio System initialization completed");
	}
	else
	{
		AddLog("Audio System initialization ERROR");
		//return false;
	}

	//registering music to game object
	if (AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_BACKGROUNDMUSIC, "Music1") == AK_Success)
	{
		AddLog("Game Object BackgroundMusic Succesfully Registered");
	}
	else
	{
		AddLog("Game Object BackgroundMusic ERROR on Register");
	}
	//registering spatialsound1 to game object
	if (AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_SPATIALSOUND1, "SpatialSound1") == AK_Success)
	{
		AddLog("Game Object Spatial Sound 1 Succesfully Registered");
	}
	else
	{
		AddLog("Game Object Spatial Sound 1 ERROR on Register");
	}
	//registering spatialsound2 to game object
	if (AK::SoundEngine::RegisterGameObj(GAME_OBJECT_ID_SPATIALSOUND2, "SpatialSound2") == AK_Success)
	{
		AddLog("Game Object Spatial Sound 2 Succesfully Registered");
	}
	else
	{
		AddLog("Game Object Spatial Sound 2 ERROR on Register");
	}
	//set default listener
	AK::SoundEngine::SetDefaultListeners(&GAME_OBJECT_ID_BACKGROUNDMUSIC,1);

	return true;
}

void AudioEngine::PlayEngine()
{
	//music 1
	if (music2eventFinished)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::MUSIC1, GAME_OBJECT_ID_BACKGROUNDMUSIC/*, AkCallbackType::AK_EndOfEvent, toggleMusic1EventFinished()*/);
	}
	//music 2
	else if (music1eventFinished)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::MUSIC2, GAME_OBJECT_ID_BACKGROUNDMUSIC/*, AkCallbackType::AK_EndOfEvent, toggleMusic2EventFinished()*/);
	}

	//spatial sound 1
	if (spatial1eventFinished)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::SPATIAL1, GAME_OBJECT_ID_SPATIALSOUND1/*, AkCallbackType::AK_EndOfEvent, toggleSpatial1EventFinished()*/);
	}

	//spatial sound 2
	if (spatial2eventFinished)
	{
		AK::SoundEngine::PostEvent(AK::EVENTS::SPATIAL2, GAME_OBJECT_ID_SPATIALSOUND2/*, AkCallbackType::AK_EndOfEvent, toggleSpatial2EventFinished()*/);
	}
}

void AudioEngine::PauseEngine()
{
	isGameOn = false;
	AK::SoundEngine::StopAll(GAME_OBJECT_ID_BACKGROUNDMUSIC);
	AK::SoundEngine::StopAll(GAME_OBJECT_ID_SPATIALSOUND1);
	AK::SoundEngine::StopAll(GAME_OBJECT_ID_SPATIALSOUND2);
}

// SETTERS
void AudioEngine::SetListenerTransform(float posx, float posy, float posz, float ofx, float ofy, float ofz, float otx, float oty, float otz)
{
	//SINCE OPENGL AND WWISE USE DIFFERENT POSITIVE X AND Z POSITIONS HERE WILL BE CHANGED HERE
	AkSoundPosition tTransform;
	tTransform.SetPosition({ -posx, posy, -posz });
	tTransform.SetOrientation({ ofx, ofy, ofz }, { otx, oty, otz });
	if (AK::SoundEngine::SetPosition(GAME_OBJECT_ID_BACKGROUNDMUSIC, tTransform) != AK_Success)
	{
		AddLog("ERROR setting position to backgroundmusic (default listener)");
	}

}
void AudioEngine::SetSpatial1Transform(float posx, float posy, float posz)
{
	//SINCE OPENGL AND WWISE USE DIFFERENT POSITIVE X AND Z POSITIONS HERE WILL BE CHANGED HERE
	AkSoundPosition tTransform;
	tTransform.SetPosition({ -posx, posy, -posz });
	tTransform.SetOrientation({ 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f });
	if (AK::SoundEngine::SetPosition(GAME_OBJECT_ID_SPATIALSOUND1, tTransform) != AK_Success)
	{
		AddLog("ERROR setting position to spatialsound1 (emiter 1)");
	}
}
void AudioEngine::SetSpatial2Transform(float posx, float posy, float posz)
{
	//SINCE OPENGL AND WWISE USE DIFFERENT POSITIVE X AND Z POSITIONS HERE WILL BE CHANGED HERE
	AkSoundPosition tTransform;
	tTransform.SetPosition({ -posx, posy, -posz });
	tTransform.SetOrientation({ 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f });
	if (AK::SoundEngine::SetPosition(GAME_OBJECT_ID_SPATIALSOUND2, tTransform) != AK_Success)
	{
		AddLog("ERROR setting position to spatialsound2 (emiter 2)");
	}
}
//END OF SETTERS

bool AudioEngine::Update()
{
	//always call this function on update to make things work
	AK::SoundEngine::RenderAudio();
	if (isGameOn)
	{


	}

	return true;
}

bool AudioEngine::CleanUp()
{
#ifndef AK_OPTIMIZED
	AK::Comm::Term();
#endif // AK_OPTIMIZED
	//commented cz theres no term function xd
	//AK::SpatialAudio::Term();
	AK::MusicEngine::Term();
	AK::SoundEngine::Term();

	g_lowLevelIO.Term();

	if (AK::IAkStreamMgr::Get())
	{
		AK::IAkStreamMgr::Get()->Destroy();
	}

	AK::MemoryMgr::Term();

	return true;
}