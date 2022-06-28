#pragma once

#ifndef __STRUCT_H__

/////////////////////////////////////////////////////////////////////

// 색 정보
typedef struct m_tARGB 
{
	union {
		struct {
			DWORD a, r, g, b;
		};
		DWORD dwColor[4];
	};
	
	m_tARGB(DWORD dwA, DWORD dwR, DWORD dwG, DWORD dwB)
		: a(dwA), r(dwR), g(dwG), b(dwB) {}
}TARGB;

/////////////////////////////////////////////////////////////////////

// 채팅 데이터
typedef struct m_tChatData
{
	wstring wstrName = L"";
	wstring wstrChat = L"";

	DWORD	dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	m_tChatData(const wstring& wstrChatName = L"", const wstring& wstrChatting = L"", DWORD dwFontColor = D3DCOLOR_ARGB(255, 255, 255, 255))
		: wstrName(wstrChatName), wstrChat(wstrChatting), dwColor(dwFontColor) {}
}CHAT_DATA;

/////////////////////////////////////////////////////////////////////

// EventScene에서 쓰이는 텍스트 재생 시간
typedef struct m_tWordEvent
{
	wstring	wstrFileName	= L"";	// 텍스트 파일 이름
	float	fEventTime		= 0.f;	// 재생 시간

	m_tWordEvent(float fTime = 0.f, const wstring& wstrName = L"")
		: fEventTime(fTime), wstrFileName(wstrName) {}
}WORD_EVENT;

/////////////////////////////////////////////////////////////////////

typedef struct m_tWordData 
{
	wstring		wstrWord	= L"";	// 대화
	wstring		wstrName	= L"";	// 이름
	wstring		wstrObjKey	= L"";	// 이미지 오브젝트키
	wstring		wstrStateKey= L"";	// 이미지 상태키

	m_tWordData() {}
	m_tWordData(
		const wstring& wstrNameText,
		const wstring& wstrWordText,
		const wstring& wstrObjKeyText = L"",
		const wstring& wstrStateKeyText = L"")
	: wstrWord(wstrWordText), wstrStateKey(wstrStateKeyText), wstrName(wstrNameText), wstrObjKey(wstrObjKeyText) {}
}WORD_DATA;

/////////////////////////////////////////////////////////////////////

// 데미지 정보
class CGameObject;
typedef struct m_tDamageData 
{
	DWORD		dwDebuff = 0;
	int			iDamage = 0;
	int			iDamageType = 1;
	CGameObject* pFrom = nullptr;

	m_tDamageData(int iDmg = 0, int iDmgType = 1, DWORD dwDeb = 0, CGameObject* pFromObj = nullptr)
		: iDamage(iDmg), iDamageType(iDmgType), dwDebuff(dwDeb), pFrom(pFromObj) {}

}DAMAGE_DATE;

/////////////////////////////////////////////////////////////////////

// 프레임
typedef struct tagFrame
{
	int iFrameEnd;			// 프레임 끝 지점
	int iFrameCur;			// 현재 프레임

	float fFrameTime;		// 프레임 변화 시간
	float fFrameDelay;		// 프레임 변화 속도

	tagFrame() :iFrameCur(0), iFrameEnd(0), fFrameTime(0.f), fFrameDelay(1.f){}
	tagFrame(int iEnd, float fDelay = 0.1f) :iFrameCur(0), iFrameEnd(iEnd), fFrameTime(0.f), fFrameDelay(fDelay) {}
}FRAME;

/////////////////////////////////////////////////////////////////////

typedef struct tagEventNotify
{
	D3DXVECTOR3 vPos = {0.f, 0.f, 0.f};	// 위치. -1.f일 경우 패스
	
	wstring		wstrObjKey		= L"";	// ObjKey
	wstring		wstrStateKey	= L"";	// StateKey

	float		fTime			= 0.f;	// 이벤트 시간

	bool		bIsMove			= false;// 움직임
	bool		bIsActive		= true;	// 활성화 상태

	tagEventNotify() {}
	tagEventNotify(D3DXVECTOR3 Pos, wstring ObjKey, wstring StateKey, float Time, bool Move = false, bool Active = true)
		: vPos(Pos), wstrObjKey(ObjKey), wstrStateKey(StateKey), fTime(Time), bIsMove(Move), bIsActive(Active){}
}EVENT_NOTIFY;

/////////////////////////////////////////////////////////////////////

// 대화 정보
typedef struct tagWord
{
	wstring wstrName		= L"";	// 이름
	wstring wstrWord		= L"";	// 대화

	wstring wstrObjKey		= L"";	// 인물 이미지 ObjKey
	wstring wstrStateKey	= L"";	// 인물 이미지 StateKey
}WORD_INFO;

/////////////////////////////////////////////////////////////////////

// Astar 노드 정보
typedef struct tagAStarNode
{
	tagAStarNode*	pParent = nullptr;
	int				iIndex;
	float			fCost;
}ASTAR_NODE;

/////////////////////////////////////////////////////////////////////

// 타일 정보
typedef struct tagTile
{
	wstring wstrName		= L"";
	wstring	wstrObjKey		= L"Background";		// 오브젝트 키
	wstring	wstrStateKey	= L"TileOption";		// 상태 키

	D3DXVECTOR3 vPos		= {0.f, 0.f, 0.f};	// 타일 위치
	D3DXVECTOR3 vSize		= {1.f, 1.f, 0.f};	// 타일 크기

	BYTE		byDrawID	= 0;	// 타일 이미지 번호
	BYTE		byOption	= 0;	// 타일 이벤트
}TILE_INFO;

/////////////////////////////////////////////////////////////////////

// 스프라이트 정보
typedef struct tagSpriteEdit {
	wstring			wstrFilePath;	// 파일 경로

	wstring			wstrObjKey;		// 오브젝트 키
	wstring			wstrStateKey;	// 상태 키

	D3DXVECTOR3		vOffset		= {0.f, 0.f, 0.f};				// 중점
	TARGB			tBlend		= TARGB{ 255, 255, 255, 255 };	// 블랜드 색 정보
	TARGB			tEraseColor	= TARGB{ 255, 0, 0, 0 };		// 클리어 색 정보

	float			fFrameDelay = 0.1f;	// 프레임 스피드
	int				iCount		= 0;	// 텍스쳐 장 수
	int				iMirrorX	= 1;	// 좌우 반전

	int				iTexType	= 0;	// 멀티 텍스쳐 여부

	DWORD GetBlendColor() { return D3DCOLOR_ARGB(tBlend.a, tBlend.r, tBlend.g, tBlend.b); }
	DWORD GetEraseColor() { return D3DCOLOR_ARGB(tEraseColor.a, tEraseColor.r, tEraseColor.g, tEraseColor.b); }
}SPRITE_INFO;

/////////////////////////////////////////////////////////////////////

// 텍스쳐 정보
typedef struct tagTexture
{
	// IDirect3DTexture9의 포인터
	// IDirect3DTexture9: 불러온 텍스처를 조작하는 Com객체.
	LPDIRECT3DTEXTURE9	pTexture;
	D3DXIMAGE_INFO		tImgInfo;		// 사용할 텍스처의 정보를 갖고있는 구조체.
	D3DXVECTOR3			vOffset;		// 텍스처 중점

	DWORD				dwEraseColor			= D3DCOLOR_ARGB(255, 255, 255, 255);// 지우기 색상 정보
	DWORD				dwBlend			= D3DCOLOR_ARGB(255, 255, 255, 255);// 색 정보

	int					m_iMirror		= 1;								// 좌우 반전
	float				m_fFrameDelay	= 1.f;								// 프레임 기본 스피드
}TEX_INFO;

/////////////////////////////////////////////////////////////////////

// 경험치 정보
typedef struct m_tExpData
{
	int		iMinExp			= 0;	// 최소 레벨업 경험치
	int		iMaxExp			= 0;	// 최대 레벨업 경험치
	int		iAccumulateExp	= 0;	// 누적 경험치
	int		iSkillPoint		= 0;	// 스킬 포인트
}EXP_DATA;

/////////////////////////////////////////////////////////////////////

// 스탯
typedef struct tagStat
{
	int		iHp			= 0;	// 체력
	int		iMp			= 0;	// 마나
	int		iSp			= 0;	// 스테미너

	int		iStab		= 0;	// 찌르기
	int		iHack		= 0;	// 베기
	int		iInt		= 0;	// 마법 공격
	int		iDex		= 0;	// 명중
	int		iDef		= 0;	// 물리 방어
	int		iMr			= 0;	// 마법 방어
	int		iAgi		= 0;	// 회피율

	int		iLightning	= 0;	// 번개 속성
	int		iFire		= 0;	// 불 속성
	int		iEarth		= 0;	// 땅 속성
	int		iLight		= 0;	// 빛 속성

	int		iIce		= 0;	// 얼음 속성
	int		iWind		= 0;	// 바람 속성
	int		iDark		= 0;	// 암흑 속성
	int		iNormal		= 0;	// 무속성

	void Reset()
	{
		iStab = 0;
		iHack = 0;
		iInt = 0;
		iDex = 0;
		iDef = 0;
		iMr = 0;
		iAgi = 0;

		iLightning = 0;
		iFire = 0;
		iEarth = 0;
		iLight = 0;

		iIce = 0;
		iWind = 0;
		iDark = 0;
		iNormal = 0;
	}

	tagStat operator +(const tagStat& tStat)
	{
		tagStat tResult;

		tResult.iStab = iStab + tStat.iStab;
		tResult.iHack = iHack + tStat.iHack;
		tResult.iInt = iInt + tStat.iInt;
		tResult.iDex = iDex + tStat.iDex;
		tResult.iDef = iDef + tStat.iDef;
		tResult.iMr = iMr + tStat.iMr;
		tResult.iAgi = iAgi + tStat.iAgi;

		tResult.iLightning = iLightning + tStat.iLightning;
		tResult.iFire = iFire + tStat.iFire;
		tResult.iEarth = iEarth + tStat.iEarth;
		tResult.iLight = iLight + tStat.iLight;

		tResult.iIce = iIce + tStat.iIce;
		tResult.iWind = iWind + tStat.iWind;
		tResult.iDark = iDark + tStat.iDark;
		tResult.iNormal = iNormal + tStat.iNormal;

		return tResult;
	}

	tagStat& operator +=(const tagStat& tStat)
	{
		iStab += tStat.iStab;
		iHack += tStat.iHack;
		iInt += tStat.iInt;
		iDex += tStat.iDex;
		iDef += tStat.iDef;
		iMr += tStat.iMr;
		iAgi += tStat.iAgi;

		iLightning += tStat.iLightning;
		iFire += tStat.iFire;
		iEarth += tStat.iEarth;
		iLight += tStat.iLight;

		iIce += tStat.iIce;
		iWind += tStat.iWind;
		iDark += tStat.iDark;
		iNormal += tStat.iNormal;

		return *this;
	}
}STAT_INFO;

/////////////////////////////////////////////////////////////////////

// 플레이어 스탯 외 정보
typedef struct m_tIdentity
{
	wstring wstrType = L"";		// 타입
	wstring wstrTitle = L"";	// 칭호

	STAT_INFO tNeedPoint;

	int		iMinExp = 50;	// 최소 레벨업 경험치
	int		iMaxExp = 150;	// 최대 레벨업 경험치

	int		iSeed = 0;	// 돈
	int		iPoint = 0;	// 남은 스탯 포인트
	int		iNp = 0;	// 네냐플 포인트?

	bool	bIsRun = true;
}IDENTITY_DATA;

/////////////////////////////////////////////////////////////////////

// 오브젝트 스탯 정보
typedef struct tagUnit
{
	D3DXVECTOR3 vCollStart	= { 0.f, 0.f, 0.f };	// 충돌 사각형 left, top
	D3DXVECTOR3 vCollEnd	= { 0.f, 0.f, 0.f };	// 충돌 사각형 right, bottom

	wstring	wstrName		= L"";					// 이름
	wstring wstrObjKey		= L"";					// 텍스쳐 오브젝트 키
	wstring wstrStateKey	= L"";					// 텍스쳐 상태 키

	STAT_INFO	tStatInfo;
	STAT_INFO	tAddStatInfo;

	int		iObjID			= OBJ_MONSTER;			// 오브젝트 아이디

	int		iLevel			= 1;	// 레벨
	int		iExp			= 0;	// 경험치

	int		iHp				= 1;	// 체력
	int		iMp				= 1;	// 마나
	int		iSp				= 1;	// 스테미너

	int		iMaxHp			= 1;	// 전체 체력
	int		iMaxMp			= 1;	// 전체 마나
	int		iMaxSp			= 1;	// 전체 스테미너

	int		iIsFirstAtt		= 0;	// 선공/후공

	float	fFindDst		= 0.f;	// 플레이어 발견 거리
}UNIT_INFO;

/////////////////////////////////////////////////////////////////////

// NPC 정보
typedef struct tagNpc
{
	wstring	wstrName		= L"";	// 이름
	wstring wstrObjKey		= L"";	// 텍스쳐 오브젝트 키
	wstring wstrStateKey	= L"";	// 텍스쳐 상태 키
}NPC_INFO;


/////////////////////////////////////////////////////////////////////

// 맵 데이터
typedef struct tagMapData {
	vector<TILE_INFO*>	vecTile;
	vector<TILE_INFO*>	vecObj;

	wstring		wstrMapName;	// 맵 이름
	int			iMapImageNum;	// 맵 이미지 번호
	int			iTileX;			// 타일 X 개수
	int			iTileY;			// 타일 Y 개수

	tagMapData() {}
	tagMapData(wstring MapName, int iImageNum, int iX, int iY)
		: wstrMapName(MapName), iMapImageNum(iImageNum), iTileX(iX), iTileY(iY) {}
	~tagMapData() {
		for_each(vecTile.begin(), vecTile.end(), SafeDelete<TILE_INFO*> );
		vecTile.clear();
		vecTile.shrink_to_fit();

		for_each(vecObj.begin(), vecObj.end(), SafeDelete<TILE_INFO*> );
		vecObj.clear();
		vecObj.shrink_to_fit();
	}
}MAP_DATA;

/////////////////////////////////////////////////////////////////////

// 오브젝트 행렬 정보
typedef struct tagInfo
{
	D3DXVECTOR3	vPos;	// 위치벡터
	D3DXVECTOR3 vDir;	// 방향벡터
	D3DXVECTOR3 vLook;	// 기준벡터
	D3DXVECTOR3 vSize = D3DXVECTOR3(1.f, 1.f, 1.f);	// 크기
	D3DXMATRIX matWorld; // 월드행렬 = 크기행렬 * 자전행렬 * 이동행렬 * 공전행렬 * 부모행렬
}INFO;

/////////////////////////////////////////////////////////////////////

// OBB 충돌 정보
typedef struct tagOBB
{
	D3DXVECTOR3 vCollSize;	// 충돌 박스 크기
	D3DXVECTOR3 vOffset;	// 중점에서의 충돌 박스 오프셋

	D3DXVECTOR3 vRight = {1.f, 0.f, 0.f};
	D3DXVECTOR3 vUp = { 0.f, 1.f, 0.f };
	D3DXVECTOR3	vPos = { 0.f, 0.f, 0.f };	// 위치벡터
	D3DXVECTOR3	vVertex[4] = { 
		{ 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 0.f }
	};					// 각 점 벡터
	D3DXMATRIX matWorld;

	tagOBB() {
		D3DXMatrixIdentity(&matWorld);
	}
}OBB;


/////////////////////////////////////////////////////////////////////

// 커스텀 정점 정보
typedef struct tagCustomVertex
{
	// rhw (Reciprocal Homogeneous W) : 동차좌표계 w의 역수 (1/w)
	float x, y, z, rhw;
	DWORD color;		// Diffuse Color, 정점의 색깔 값
}CUSTOMVERTEX;
// FVF (Flexible Vertex Format) : 사용자가 직접 정점의 구조를 정의해서 사용할 수 있도록 하는 기능
// D3DFVF_XYZRHW : 정점 포맷이 변환된 정점의 위치 좌표를 포함
// D3DFVF_DIFFUSE : 정점 포맷이 Diffuse색 성분을 포함
// x,y,z,w 값과 diffuse color를 나타내는 flag 값을 설정
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

// 정점 정보
typedef struct tagVertex {
	float _x, _y, _z;
	static const DWORD FVF;

	tagVertex() {}
	tagVertex(float x, float y, float z)
		: _x(x), _y(y), _z(z) {}

}Vertex;
const DWORD Vertex::FVF = D3DFVF_XYZ;

#define __STRUCT_H__
#endif