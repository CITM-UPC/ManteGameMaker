#pragma once

//basics
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

//wwise info
//#include "Wwise_IDs.h"
#define BANKNAME_INIT L"Init.bank"
#define BANKNAME_RAYCASTER L"RayCast.bank"



//others
using namespace std;