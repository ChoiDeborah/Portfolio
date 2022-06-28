#include "stdafx.h"
#include "CollisionMgr.h"
#include "GameObject.h"

#include "AttackEffect.h"

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

bool CCollisionMgr::CheckOneToN(const OBB& dst, const OBJLIST & srcLst, CGameObject** pObject)
{
	for (auto& pSource : srcLst)
	{
		if (ACTIVE_ON != pSource->GetActive())
			continue;

		if (CheckOneToOne(dst, pSource->GetOBB(), pObject)) {
			*pObject = pSource;

			/*if (nullptr != pObject) {

				switch (pSource->GetObjID()) {
				case OBJ_PLAYER_ATTACK:
				case OBJ_ATTACK:
					pt->x = dynamic_cast<CAttackEffect*>(pSource)->GetDamage();
					pt->y = dynamic_cast<CAttackEffect*>(pSource)->GetAttackCount();
					break;
				}
			}*/

			return true;
		}
	}

	return false;
}

bool CCollisionMgr::CheckOneToOne(const OBB& tDst, const OBB& tSrc, CGameObject** pObject)
{
	// 방향 벡터
	D3DXVECTOR3 vAxisAx = tDst.vRight;
	D3DXVECTOR3 vAxisAy = tDst.vUp;

	D3DXVECTOR3 vAxisBx = tSrc.vRight;
	D3DXVECTOR3 vAxisBy = tSrc.vUp;


	// 가로 세로 크기 절반 구함. (오프셋 적용을 위함)
	D3DXVECTOR3 vDstDif = {
		(tDst.vVertex[0].x + tDst.vVertex[1].x) * 0.5f,
		(tDst.vVertex[1].y + tDst.vVertex[2].y) * 0.5f,
		0.f
	};
	D3DXVECTOR3 vSrcDif = {
		(tSrc.vVertex[0].x + tSrc.vVertex[1].x) * 0.5f,
		(tSrc.vVertex[1].y + tSrc.vVertex[2].y) * 0.5f,
		0.f
	};
	D3DXVec3TransformCoord(&vDstDif, &vDstDif, &tDst.matWorld);
	D3DXVec3TransformCoord(&vSrcDif, &vSrcDif, &tSrc.matWorld);

	// 각 중심점에서 한 점을 바라보는 벡터 (아직 오프셋 적용 X)
	D3DXVECTOR3 vPointA, vPointB;
	D3DXVec3TransformCoord(&vPointA, &tDst.vVertex[0], &tDst.matWorld);
	D3DXVec3TransformCoord(&vPointB, &tSrc.vVertex[0], &tSrc.matWorld);

	// 오프셋을 빼주어 정중앙에 위치
	vPointA -= vDstDif;
	vPointB -= vSrcDif;


	// 중점 사이 거리
	D3DXVECTOR3 vDst = (tDst.vPos + vDstDif) - (tSrc.vPos + vSrcDif);

	float fBLengthx = 0.f;
	float fBLengthy = 0.f;
	float fALengthx = 0.f;
	float fALengthy = 0.f;
	float fRad = 0.f;
	float fDst = 0.f;

	{
		D3DXVECTOR3 vBx = vAxisBx * fabs(D3DXVec3Dot(&vPointB, &vAxisBx));
		D3DXVECTOR3 vBy = vAxisBy * fabs(D3DXVec3Dot(&vPointB, &vAxisBy));

		{
			// AxisAx 기준
			fALengthx	= fabs(D3DXVec3Dot(&vPointA, &vAxisAx));	// A점 Ax축에 투영
			fBLengthx	= fabs(D3DXVec3Dot(&vBx, &vAxisAx));		// Bx축에 투영한 B점 Ax축에 투영
			fBLengthy	= fabs(D3DXVec3Dot(&vBy, &vAxisAx));		// By축에 투영한 B점 Ax축에 투영
			fRad		= fALengthx + fBLengthx + fBLengthy;		// 길이
			fDst		= fabs(D3DXVec3Dot(&vDst, &vAxisAx));		// 거리

			if (fRad < fDst)
				return false;
		}

		{
			// AxisAy 기준
			fALengthy	= fabs(D3DXVec3Dot(&vPointA, &vAxisAy));	// A점 Ay축에 투영
			fBLengthx	= fabs(D3DXVec3Dot(&vBx, &vAxisAy));		// Bx축에 투영한 B점 Ay축에 투영
			fBLengthy	= fabs(D3DXVec3Dot(&vBy, &vAxisAy));		// By축에 투영한 B점 Ay축에 투영
			fRad		= fALengthy + fBLengthx + fBLengthy;		// 길이
			fDst		= fabs(D3DXVec3Dot(&vDst, &vAxisAx));		// 거리

			if (fRad < fDst)
				return false;
		}
	}

	{
		D3DXVECTOR3 vAx = vAxisAx * fabs(D3DXVec3Dot(&vPointA, &vAxisAx));
		D3DXVECTOR3 vAy = vAxisAy * fabs(D3DXVec3Dot(&vPointA, &vAxisAy));

		{
			// AxisBx 기준
			fBLengthx	= fabs(D3DXVec3Dot(&vPointB, &vAxisBx));	// B점 Bx축에 투영
			fALengthx	= fabs(D3DXVec3Dot(&vAx, &vAxisBx));		// Ax축에 투영한 A점 Bx축에 투영
			fALengthy	= fabs(D3DXVec3Dot(&vAy, &vAxisBx));		// Ay축에 투영한 A점 Bx축에 투영
			fRad		= fBLengthx + fALengthx + fALengthy;		// 길이
			fDst		= fabs(D3DXVec3Dot(&vDst, &vAxisBx));		// 거리

			if (fRad < fDst)
				return false;
		}

		{
			// AxisBy 기준
			fBLengthy	= fabs(D3DXVec3Dot(&vPointB, &vAxisBy));	// B점 By축에 투영
			fALengthx	= fabs(D3DXVec3Dot(&vAx, &vAxisBy));		// Ax축에 투영한 A점 By축에 투영
			fALengthy	= fabs(D3DXVec3Dot(&vAy, &vAxisBy));		// Ay축에 투영한 A점 By축에 투영
			fRad		= fBLengthy + fALengthx + fALengthy;		// 길이
			fDst		= fabs(D3DXVec3Dot(&vDst, &vAxisBy));		// 거리

			if (fRad < fDst) 
				return false;
		}
	}

	return true;
}
