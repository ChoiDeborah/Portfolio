#pragma once

#ifndef __STRUCT_H__

/////////////////////////////////////////////////////////////////////

// �� ����
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

// ä�� ������
typedef struct m_tChatData
{
	wstring wstrName = L"";
	wstring wstrChat = L"";

	DWORD	dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	m_tChatData(const wstring& wstrChatName = L"", const wstring& wstrChatting = L"", DWORD dwFontColor = D3DCOLOR_ARGB(255, 255, 255, 255))
		: wstrName(wstrChatName), wstrChat(wstrChatting), dwColor(dwFontColor) {}
}CHAT_DATA;

/////////////////////////////////////////////////////////////////////

// EventScene���� ���̴� �ؽ�Ʈ ��� �ð�
typedef struct m_tWordEvent
{
	wstring	wstrFileName	= L"";	// �ؽ�Ʈ ���� �̸�
	float	fEventTime		= 0.f;	// ��� �ð�

	m_tWordEvent(float fTime = 0.f, const wstring& wstrName = L"")
		: fEventTime(fTime), wstrFileName(wstrName) {}
}WORD_EVENT;

/////////////////////////////////////////////////////////////////////

typedef struct m_tWordData 
{
	wstring		wstrWord	= L"";	// ��ȭ
	wstring		wstrName	= L"";	// �̸�
	wstring		wstrObjKey	= L"";	// �̹��� ������ƮŰ
	wstring		wstrStateKey= L"";	// �̹��� ����Ű

	m_tWordData() {}
	m_tWordData(
		const wstring& wstrNameText,
		const wstring& wstrWordText,
		const wstring& wstrObjKeyText = L"",
		const wstring& wstrStateKeyText = L"")
	: wstrWord(wstrWordText), wstrStateKey(wstrStateKeyText), wstrName(wstrNameText), wstrObjKey(wstrObjKeyText) {}
}WORD_DATA;

/////////////////////////////////////////////////////////////////////

// ������ ����
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

// ������
typedef struct tagFrame
{
	int iFrameEnd;			// ������ �� ����
	int iFrameCur;			// ���� ������

	float fFrameTime;		// ������ ��ȭ �ð�
	float fFrameDelay;		// ������ ��ȭ �ӵ�

	tagFrame() :iFrameCur(0), iFrameEnd(0), fFrameTime(0.f), fFrameDelay(1.f){}
	tagFrame(int iEnd, float fDelay = 0.1f) :iFrameCur(0), iFrameEnd(iEnd), fFrameTime(0.f), fFrameDelay(fDelay) {}
}FRAME;

/////////////////////////////////////////////////////////////////////

typedef struct tagEventNotify
{
	D3DXVECTOR3 vPos = {0.f, 0.f, 0.f};	// ��ġ. -1.f�� ��� �н�
	
	wstring		wstrObjKey		= L"";	// ObjKey
	wstring		wstrStateKey	= L"";	// StateKey

	float		fTime			= 0.f;	// �̺�Ʈ �ð�

	bool		bIsMove			= false;// ������
	bool		bIsActive		= true;	// Ȱ��ȭ ����

	tagEventNotify() {}
	tagEventNotify(D3DXVECTOR3 Pos, wstring ObjKey, wstring StateKey, float Time, bool Move = false, bool Active = true)
		: vPos(Pos), wstrObjKey(ObjKey), wstrStateKey(StateKey), fTime(Time), bIsMove(Move), bIsActive(Active){}
}EVENT_NOTIFY;

/////////////////////////////////////////////////////////////////////

// ��ȭ ����
typedef struct tagWord
{
	wstring wstrName		= L"";	// �̸�
	wstring wstrWord		= L"";	// ��ȭ

	wstring wstrObjKey		= L"";	// �ι� �̹��� ObjKey
	wstring wstrStateKey	= L"";	// �ι� �̹��� StateKey
}WORD_INFO;

/////////////////////////////////////////////////////////////////////

// Astar ��� ����
typedef struct tagAStarNode
{
	tagAStarNode*	pParent = nullptr;
	int				iIndex;
	float			fCost;
}ASTAR_NODE;

/////////////////////////////////////////////////////////////////////

// Ÿ�� ����
typedef struct tagTile
{
	wstring wstrName		= L"";
	wstring	wstrObjKey		= L"Background";		// ������Ʈ Ű
	wstring	wstrStateKey	= L"TileOption";		// ���� Ű

	D3DXVECTOR3 vPos		= {0.f, 0.f, 0.f};	// Ÿ�� ��ġ
	D3DXVECTOR3 vSize		= {1.f, 1.f, 0.f};	// Ÿ�� ũ��

	BYTE		byDrawID	= 0;	// Ÿ�� �̹��� ��ȣ
	BYTE		byOption	= 0;	// Ÿ�� �̺�Ʈ
}TILE_INFO;

/////////////////////////////////////////////////////////////////////

// ��������Ʈ ����
typedef struct tagSpriteEdit {
	wstring			wstrFilePath;	// ���� ���

	wstring			wstrObjKey;		// ������Ʈ Ű
	wstring			wstrStateKey;	// ���� Ű

	D3DXVECTOR3		vOffset		= {0.f, 0.f, 0.f};				// ����
	TARGB			tBlend		= TARGB{ 255, 255, 255, 255 };	// ���� �� ����
	TARGB			tEraseColor	= TARGB{ 255, 0, 0, 0 };		// Ŭ���� �� ����

	float			fFrameDelay = 0.1f;	// ������ ���ǵ�
	int				iCount		= 0;	// �ؽ��� �� ��
	int				iMirrorX	= 1;	// �¿� ����

	int				iTexType	= 0;	// ��Ƽ �ؽ��� ����

	DWORD GetBlendColor() { return D3DCOLOR_ARGB(tBlend.a, tBlend.r, tBlend.g, tBlend.b); }
	DWORD GetEraseColor() { return D3DCOLOR_ARGB(tEraseColor.a, tEraseColor.r, tEraseColor.g, tEraseColor.b); }
}SPRITE_INFO;

/////////////////////////////////////////////////////////////////////

// �ؽ��� ����
typedef struct tagTexture
{
	// IDirect3DTexture9�� ������
	// IDirect3DTexture9: �ҷ��� �ؽ�ó�� �����ϴ� Com��ü.
	LPDIRECT3DTEXTURE9	pTexture;
	D3DXIMAGE_INFO		tImgInfo;		// ����� �ؽ�ó�� ������ �����ִ� ����ü.
	D3DXVECTOR3			vOffset;		// �ؽ�ó ����

	DWORD				dwEraseColor			= D3DCOLOR_ARGB(255, 255, 255, 255);// ����� ���� ����
	DWORD				dwBlend			= D3DCOLOR_ARGB(255, 255, 255, 255);// �� ����

	int					m_iMirror		= 1;								// �¿� ����
	float				m_fFrameDelay	= 1.f;								// ������ �⺻ ���ǵ�
}TEX_INFO;

/////////////////////////////////////////////////////////////////////

// ����ġ ����
typedef struct m_tExpData
{
	int		iMinExp			= 0;	// �ּ� ������ ����ġ
	int		iMaxExp			= 0;	// �ִ� ������ ����ġ
	int		iAccumulateExp	= 0;	// ���� ����ġ
	int		iSkillPoint		= 0;	// ��ų ����Ʈ
}EXP_DATA;

/////////////////////////////////////////////////////////////////////

// ����
typedef struct tagStat
{
	int		iHp			= 0;	// ü��
	int		iMp			= 0;	// ����
	int		iSp			= 0;	// ���׹̳�

	int		iStab		= 0;	// ���
	int		iHack		= 0;	// ����
	int		iInt		= 0;	// ���� ����
	int		iDex		= 0;	// ����
	int		iDef		= 0;	// ���� ���
	int		iMr			= 0;	// ���� ���
	int		iAgi		= 0;	// ȸ����

	int		iLightning	= 0;	// ���� �Ӽ�
	int		iFire		= 0;	// �� �Ӽ�
	int		iEarth		= 0;	// �� �Ӽ�
	int		iLight		= 0;	// �� �Ӽ�

	int		iIce		= 0;	// ���� �Ӽ�
	int		iWind		= 0;	// �ٶ� �Ӽ�
	int		iDark		= 0;	// ���� �Ӽ�
	int		iNormal		= 0;	// ���Ӽ�

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

// �÷��̾� ���� �� ����
typedef struct m_tIdentity
{
	wstring wstrType = L"";		// Ÿ��
	wstring wstrTitle = L"";	// Īȣ

	STAT_INFO tNeedPoint;

	int		iMinExp = 50;	// �ּ� ������ ����ġ
	int		iMaxExp = 150;	// �ִ� ������ ����ġ

	int		iSeed = 0;	// ��
	int		iPoint = 0;	// ���� ���� ����Ʈ
	int		iNp = 0;	// �׳��� ����Ʈ?

	bool	bIsRun = true;
}IDENTITY_DATA;

/////////////////////////////////////////////////////////////////////

// ������Ʈ ���� ����
typedef struct tagUnit
{
	D3DXVECTOR3 vCollStart	= { 0.f, 0.f, 0.f };	// �浹 �簢�� left, top
	D3DXVECTOR3 vCollEnd	= { 0.f, 0.f, 0.f };	// �浹 �簢�� right, bottom

	wstring	wstrName		= L"";					// �̸�
	wstring wstrObjKey		= L"";					// �ؽ��� ������Ʈ Ű
	wstring wstrStateKey	= L"";					// �ؽ��� ���� Ű

	STAT_INFO	tStatInfo;
	STAT_INFO	tAddStatInfo;

	int		iObjID			= OBJ_MONSTER;			// ������Ʈ ���̵�

	int		iLevel			= 1;	// ����
	int		iExp			= 0;	// ����ġ

	int		iHp				= 1;	// ü��
	int		iMp				= 1;	// ����
	int		iSp				= 1;	// ���׹̳�

	int		iMaxHp			= 1;	// ��ü ü��
	int		iMaxMp			= 1;	// ��ü ����
	int		iMaxSp			= 1;	// ��ü ���׹̳�

	int		iIsFirstAtt		= 0;	// ����/�İ�

	float	fFindDst		= 0.f;	// �÷��̾� �߰� �Ÿ�
}UNIT_INFO;

/////////////////////////////////////////////////////////////////////

// NPC ����
typedef struct tagNpc
{
	wstring	wstrName		= L"";	// �̸�
	wstring wstrObjKey		= L"";	// �ؽ��� ������Ʈ Ű
	wstring wstrStateKey	= L"";	// �ؽ��� ���� Ű
}NPC_INFO;


/////////////////////////////////////////////////////////////////////

// �� ������
typedef struct tagMapData {
	vector<TILE_INFO*>	vecTile;
	vector<TILE_INFO*>	vecObj;

	wstring		wstrMapName;	// �� �̸�
	int			iMapImageNum;	// �� �̹��� ��ȣ
	int			iTileX;			// Ÿ�� X ����
	int			iTileY;			// Ÿ�� Y ����

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

// ������Ʈ ��� ����
typedef struct tagInfo
{
	D3DXVECTOR3	vPos;	// ��ġ����
	D3DXVECTOR3 vDir;	// ���⺤��
	D3DXVECTOR3 vLook;	// ���غ���
	D3DXVECTOR3 vSize = D3DXVECTOR3(1.f, 1.f, 1.f);	// ũ��
	D3DXMATRIX matWorld; // ������� = ũ����� * ������� * �̵���� * ������� * �θ����
}INFO;

/////////////////////////////////////////////////////////////////////

// OBB �浹 ����
typedef struct tagOBB
{
	D3DXVECTOR3 vCollSize;	// �浹 �ڽ� ũ��
	D3DXVECTOR3 vOffset;	// ���������� �浹 �ڽ� ������

	D3DXVECTOR3 vRight = {1.f, 0.f, 0.f};
	D3DXVECTOR3 vUp = { 0.f, 1.f, 0.f };
	D3DXVECTOR3	vPos = { 0.f, 0.f, 0.f };	// ��ġ����
	D3DXVECTOR3	vVertex[4] = { 
		{ 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 0.f }
	};					// �� �� ����
	D3DXMATRIX matWorld;

	tagOBB() {
		D3DXMatrixIdentity(&matWorld);
	}
}OBB;


/////////////////////////////////////////////////////////////////////

// Ŀ���� ���� ����
typedef struct tagCustomVertex
{
	// rhw (Reciprocal Homogeneous W) : ������ǥ�� w�� ���� (1/w)
	float x, y, z, rhw;
	DWORD color;		// Diffuse Color, ������ ���� ��
}CUSTOMVERTEX;
// FVF (Flexible Vertex Format) : ����ڰ� ���� ������ ������ �����ؼ� ����� �� �ֵ��� �ϴ� ���
// D3DFVF_XYZRHW : ���� ������ ��ȯ�� ������ ��ġ ��ǥ�� ����
// D3DFVF_DIFFUSE : ���� ������ Diffuse�� ������ ����
// x,y,z,w ���� diffuse color�� ��Ÿ���� flag ���� ����
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

// ���� ����
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