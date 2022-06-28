// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <string>
#include <fstream>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

// Standard Headers
#include <string>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <iostream>



// Sound
#include "fmod.h"
#include "fmod.hpp"
#include <io.h>

#pragma comment(lib, "fmodex_vc.lib")

using namespace std;

// DirectX
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

// User Headers
#include "../Headers/Extern.h"
#include "../Headers/Define.h"
#include "../Headers/Enum.h"
#include "../Headers/Function.h"
#include "../Headers/Functor.h"
#include "../Headers/Typedef.h"
#include "../Headers/Struct.h"
#include "../Headers/AbstractFactory.h"

// Managers
#include "PlayerSubject.h"

#include "CollisionMgr.h"
#include "DeviceMgr.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "FrameMgr.h"
#include "SoundMgr.h"

#include "MouseMgr.h"
#include "KeyMgr.h"

#include "SceneMgr.h"
#include "ObjMgr.h"
#include "ViewMgr.h"
#include "TileMgr.h"
#include "UIMgr.h"
#include "WordMgr.h"

//#pragma comment(linker,"/entry:wWinMainCRTStartup /subsystem:console")

//#ifdef _DEBUG
//#include <vld.h>
//#pragma comment(lib, "vld.lib")
//#endif