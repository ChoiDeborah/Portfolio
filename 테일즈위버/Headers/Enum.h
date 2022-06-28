#pragma once

#ifndef __ENUM_H__

// 렌더링 순서
enum RENDER_LAYER { LAYER_BACK, LAYER_OBJ, LAYER_EFFECT, LAYER_UI, LAYER_END };

// 오브젝트 타입
enum OBJID {
	OBJ_ENVIRONMENT, OBJ_COLLISION,	// 충돌 체크 오브젝트
	OBJ_EVENT,
	OBJ_PLAYER,		OBJ_MONSTER,			OBJ_NPC,				// Object
	OBJ_BUDDY,		OBJ_GATE,				OBJ_CREATOR,			OBJ_ITEM,
	OBJ_ATTACK,		OBJ_PLAYER_ATTACK,		OBJ_EFFECT,				// 공격 충돌
	OBJ_UI,															// UI
	OBJ_END
};

// UI 윈도우 아이디
enum WND_ID { WNDID_INFO, WNDID_EQUIP, WNDID_INVEN, WNDID_SKILL, WNDID_COMBO, WNDID_COMBOSET, WNDID_COUNT, WNDID_CHAT, WNDID_QUICK, WNDID_SHOP, WNDID_END };

// 마우스 상태
enum MOUSE_TYPE { MOUSE_NORMAL, MOUSE_ATTACK_GROUND, MOUSE_BATTLE, MOUSE_CHAT, MOUSE_CLICK, MOUSE_GATE, MOUSE_MAGIC, MOUSE_NOPASS, MOUSE_PICK, MOUSE_DELAY, MOUSE_END };

// 생성 오브젝트 이름
enum OBJ_NAME { NAME_PLAYER, NAME_BOSS, NAME_DARKCOW };

// 아이콘 타입
enum ICON_TYPE { ICON_ITEM, ICON_SKILL, ICON_END };

// 아이템 타입
enum ITEM_TYPE { ITEM_EQUIP, ITEM_USE, ITEM_ETC };

// 아이템 아이디
enum ITEMID { ITEMID_HEAD, ITEMID_ARMOR, ITEMID_ARMS, ITEMID_SHIELD, ITEMID_ACCESSOAY1, ITEMID_ACCESSOAY2, ITEMID_ACCESSOAY3, ITEMID_ACCESSOAY4, ITEMID_USE, ITEMID_ETC };

// 스킬 타입
enum SKILL_TYPE { SKILL_ACTIVE, SKILL_PASSIVE };

// 스킬 아이디
enum SKILLID { 
	SKILLID_STAB, SKILLID_VERCUT, 
	SKILLID_ACTIVE0, SKILLID_ACTIVE1, SKILLID_ACTIVE2, SKILLID_ACTIVE3, SKILLID_ACTIVE4, 
	SKILLID_FIREWAVE, SKILLID_HORCUT, SKILLID_RUSHCRASH, SKILLID_STARBUSTSTREAM };


// 이펙트 종류
enum EFFECT_TYPE { MAGIC_EFFECT, THUNDER_EFFECT, BOOM_EFFECT, SWORD_EFFECT, SUN_EFFECT, EFFECT_END };

// 사운드 채널
enum CHANNEL_ID { 
	CHANNEL_BGM, 
	CHANNEL_EFFECT, CHANNEL_WINDOW, CHANNEL_USEITEM, 
	CHANNEL_ATTACK, CHANNEL_HIT,
	CHANNEL_MONSTER_ATTACK, CHANNEL_MONSTER_HIT,
	CHANNEL_END 
};

// 씬 아이디
enum SCENEID { SCENEID_LOGO, SCENEID_TITLE, SCENEID_STAGE, SCENEID_EVENT, SCENEID_END };

// 현재 씬 상태
enum SCENE_STATE { START_SCENE, SCENE_PLAY, NEXT_SCENE };

// 오브젝트 방향
enum DIRECTION { LEFT = -1, UP = 0, RIGHT, DOWN, LEFT_UP, LEFT_DOWN, RIGHT_UP, RIGHT_DOWN, NO_DIRECTION};

// 스프라이트 순환
enum FRAMESTATE { FRAME_ONCE, FRAME_LOOP };

// 퍼즐 타입
enum PUZZLE_TYPE { PIECE_PUZZLE, PUZZLE_END };

// Tool 메뉴
enum TOOL_TAP { SPRITE_TOOL, UNIT_TOOL, MAP_TOOL, WORD_TOOL, EVENT_TOOL };

// UnitTool 메뉴
enum UNITTOOL_TAP { MONSTER_SETTING, ENVIRON_SETTING };

#define __ENUM_H__
#endif