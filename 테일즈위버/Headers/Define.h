#pragma once
#ifndef __DEFINE_H__

#define WINCX				1920
#define WINCY				1080

#define CLIENTCX			1600
#define CLIENTCY			900

#define VIEWCX				1080
#define VIEWCY				720

#define TILECX				94 // 120 // 146
#define TILECY				48 // 66 // 146

#define MIN_STR				64
#define MID_STR				128
#define MAX_STR				256

// Update Event
#define NO_EVENT			0
#define OBJ_DEAD			1
#define SCENE_CHANGE		2

// Active Event
#define ACTIVE_OFF			0
#define ACTIVE_ON			1

#define SCENE_MAXNUM		15

// Math
#define PI					3.1415f
#define GRAVITY				9.8f

// Key Setting
#define KEY_LCLICK			VK_LBUTTON
#define KEY_RCLICK			VK_RBUTTON

#define	KEY_COMUNICATION	VK_SPACE

#define KEY_ATTACK_GROUND	VK_LCONTROL //'A'

#define KEY_QUICK1			VK_F1
#define KEY_QUICK2			VK_F2
#define KEY_QUICK3			VK_F3
#define KEY_QUICK4			VK_F4

#define KEY_QUICK5			VK_F5
#define KEY_QUICK6			VK_F6
#define KEY_QUICK7			VK_F7
#define KEY_QUICK8			VK_F8

#define KEY_CHAT			VK_RETURN

#define KEY_DASH			'F'

#define KEY_GET_ITEM		'Z'
#define KEY_RUN				'R'

#define KEY_WND_COMBO		'A'
#define KEY_WND_INFO		'P'
#define KEY_WND_EQUIP		'E'
#define KEY_WND_INVEN		'I'
#define KEY_WND_SKILL		'K'


// 치트
#define KEY_ROOM_RESTART	'1'
#define KEY_ROOM_PREV		'2'
#define KEY_ROOM_NEXT		'3'
#define KEY_ROOM_EVENT		'4'

#define	KEY_LEVEL_UP		'5'
#define KEY_VIEW_ZOOM_IN	'6'
#define KEY_VIEW_ZOOM_OUT	'7'
#define KEY_VIEW_INIT		VK_F10


#define KEY_CREATE_ITEM		'8'
#define KEY_WORD_EVENT		'X'
#define KEY_RECT_RENDER		'9'
#define KEY_TILE_RENDER		'0'

// 디버프 상태
#define DEBUFF_NORMAL				0x0000	// 아무상태 없음

#define DEBUFF_FREZE				0x0001	// 빙결
#define DEBUFF_IMMOVABLE			0x0002	// 이동 불가
#define DEBUFF_CANT_ATTACT			0x0004	// 공격 불가
#define DEBUFF_IRRECOVERABLE		0x0008	// 회복 불가

#define DEBUFF_CONTINUOUS_DAMAGE	0x0010	// 지속 데미지
#define DEBUFF_DOWN_ATTACK			0x0020	// 공격력 저하
#define	DEBUFF_DOWN_DEFENSE			0x0040	// 방어력 저하
#define	DEBUFF_BLINED				0x0080	// 시야암흑

#define DEBUFF_CURSE				0x0100	// 저주 (능력치 낮춤)
#define DEBUFF_POISION				0x0200	// 중독 (지속 데미지)
#define	DEBUFF_UNDEAD				0x0400	// 언데드 (회복->데미지 변경)
#define DEBUFF_ELECTRIC_SHOCK		0x0800	// 감전


// Event Value
// Scene Event
#define EVENT_ALL_STOP				0x0001
#define EVENT_PLAYER_STOP			0x0002
#define EVENT_XPLAYER_ALL_STOP		0x0004
#define EVENT_PLAYER_NO_DEAD		0x0008
#define EVENT_FADE_DARK				0x0010
#define EVENT_TILE_RENDER			0x0020
#define EVENT_RECT_RENDER			0x0040
#define EVENT_WORD_EVENT			0x0080

// Quest
#define EVENT_QUEST0				0x0001
#define EVENT_QUEST1				0x0002
#define EVENT_QUEST2				0x0004
#define EVENT_QUEST3				0x0008

// Event Scene
#define EVENT_SCENE0				0x0001
#define EVENT_SCENE1				0x0002
#define EVENT_SCENE2				0x0004
#define EVENT_SCENE3				0x0008


// 다음 씬 플레이어 위치
#define SCENE_LOAD_POS	-1.f

/////////////////////////////////
// Get Color
#define D3DCOLOR_GET_A(Color) \
(DWORD((Color) >> 24))

#define D3DCOLOR_GET_R(Color) \
(DWORD((Color << 8) >> 24))

#define D3DCOLOR_GET_G(Color) \
(DWORD((Color << 16) >> 24))

#define D3DCOLOR_GET_B(Color) \
(DWORD((Color << 24) >> 24))

// Null Check
#define NULL_CHECK(ptr)	\
if(nullptr == (ptr)) { return; }

#define NULL_CHECK_MSG(ptr, msg)	\
if(nullptr == (ptr)) { ::MessageBox(0, msg, L"System Error", MB_OK); return; }

#define NULL_CHECK_RETURN(ptr, val)	\
if(nullptr == (ptr)) { return val; }

#define NULL_CHECK_MSG_RETURN(ptr, msg, val)	\
if(nullptr == (ptr)) { ::MessageBox(0, msg, L"System Error", MB_OK); return val; }

#define FAILED_CHECK(hr)	\
if(FAILED(hr)) { return; }

#define FAILED_CHECK_MSG(hr, msg)	\
if(FAILED(hr)) { ::MessageBox(0, msg, L"System Error", MB_OK); return; }

#define FAILED_CHECK_RETURN(hr, val)	\
if(FAILED(hr)) { return val; }

#define FAILED_CHECK_MSG_RETURN(hr, msg, val)	\
if(FAILED(hr)) { ::MessageBox(0, msg, L"System Error", MB_OK); return val; }

#define NO_COPY(ClassName)						\
private:										\
	ClassName(const ClassName&);				\
	ClassName& operator=(const ClassName&);

#define DECLARE_SINGLETON(ClassName)			\
		NO_COPY(ClassName)						\
public:											\
	static ClassName* GetInstance();			\
	void DestroyInstance();						\
private:										\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)			\
ClassName* ClassName::m_pInstance = nullptr;	\
ClassName* ClassName::GetInstance()				\
{												\
	if(nullptr == m_pInstance)					\
		m_pInstance = new ClassName;			\
	return m_pInstance;							\
}												\
void ClassName::DestroyInstance()				\
{												\
	if(m_pInstance)								\
	{											\
		delete m_pInstance;						\
		m_pInstance = nullptr;					\
	}											\
}

#define __DEFINE_H__
#endif