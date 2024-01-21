#pragma once

//basics
#include <iostream>
#include <list>
#include <string>
#include <vector>
//memory management
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>

//streaming
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include <Common/AkFilePackageLowLevelIODeferred.h> 

//file packaging
#include <../Common/AkFilePackage.h>
#include <../Common/AkFilePackageLUT.h>

//communications
// Include for communication between Wwise and the game -- Not needed in the release version
#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h>
#endif // AK_OPTIMIZED

//music engine
#include <AK/MusicEngine/Common/AkMusicEngine.h>

//spatial audio
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>

//wwise info
//#include "Wwise_IDs.h"
#include "../samples/IntegrationDemo/WwiseProject/GeneratedSoundBanks/Wwise_IDs.h"

#define BANKNAME_INIT L"Init.bnk"
#define BANKNAME_RAYCASTER L"RayCast.bnk"



//others
using namespace std;