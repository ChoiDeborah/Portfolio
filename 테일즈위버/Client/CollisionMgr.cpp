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
	// ���� ����
	D3DXVECTOR3 vAxisAx = tDst.vRight;
	D3DXVECTOR3 vAxisAy = tDst.vUp;

	D3DXVECTOR3 vAxisBx = tSrc.vRight;
	D3DXVECTOR3 vAxisBy = tSrc.vUp;


	// ���� ���� ũ�� ���� ����. (������ ������ ����)
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

	// �� �߽������� �� ���� �ٶ󺸴� ���� (���� ������ ���� X)
	D3DXVECTOR3 vPointA, vPointB;
	D3DXVec3TransformCoord(&vPointA, &tDst.vVertex[0], &tDst.matWorld);
	D3DXVec3TransformCoord(&vPointB, &tSrc.vVertex[0], &tSrc.matWorld);

	// �������� ���־� ���߾ӿ� ��ġ
	vPointA -= vDstDif;
	vPointB -= vSrcDif;


	// ���� ���� �Ÿ�
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
			// AxisAx ����
			fALengthx	= fabs(D3DXVec3Dot(&vPointA, &vAxisAx));	// A�� Ax�࿡ ����
			fBLengthx	= fabs(D3DXVec3Dot(&vBx, &vAxisAx));		// Bx�࿡ ������ B�� Ax�࿡ ����
			fBLengthy	= fabs(D3DXVec3Dot(&vBy, &vAxisAx));		// By�࿡ ������ B�� Ax�࿡ ����
			fRad		= fALengthx + fBLengthx + fBLengthy;		// ����
			fDst		= fabs(D3DXVec3Dot(&vDst, &vAxisAx));		// �Ÿ�

			if (fRad < fDst)
				return false;
		}

		{
			// AxisAy ����
			fALengthy	= fabs(D3DXVec3Dot(&vPointA, &vAxisAy));	// A�� Ay�࿡ ����
			fBLengthx	= fabs(D3DXVec3Dot(&vBx, &vAxisAy));		// Bx�࿡ ������ B�� Ay�࿡ ����
			fBLengthy	= fabs(D3DXVec3Dot(&vBy, &vAxisAy));		// By�࿡ ������ B�� Ay�࿡ ����
			fRad		= fALengthy + fBLengthx + fBLengthy;		// ����
			fDst		= fabs(D3DXVec3Dot(&vDst, &vAxisAx));		// �Ÿ�

			if (fRad < fDst)
				return false;
		}
	}

	{
		D3DXVECTOR3 vAx = vAxisAx * fabs(D3DXVec3Dot(&vPointA, &vAxisAx));
		D3DXVECTOR3 vAy = vAxisAy * fabs(D3DXVec3Dot(&vPointA, &vAxisAy));

		{
			// AxisBx ����
			fBLengthx	= fabs(D3DXVec3Dot(&vPointB, &vAxisBx));	// B�� Bx�࿡ ����
			fALengthx	= fabs(D3DXVec3Dot(&vAx, &vAxisBx));		// Ax�࿡ ������ A�� Bx�࿡ ����
			fALengthy	= fabs(D3DXVec3Dot(&vAy, &vAxisBx));		// Ay�࿡ ������ A�� Bx�࿡ ����
			fRad		= fBLengthx + fALengthx + fALengthy;		// ����
			fDst		= fabs(D3DXVec3Dot(&vDst, &vAxisBx));		// �Ÿ�

			if (fRad < fDst)
				return false;
		}

		{
			// AxisBy ����
			fBLengthy	= fabs(D3DXVec3Dot(&vPointB, &vAxisBy));	// B�� By�࿡ ����
			fALengthx	= fabs(D3DXVec3Dot(&vAx, &vAxisBy));		// Ax�࿡ ������ A�� By�࿡ ����
			fALengthy	= fabs(D3DXVec3Dot(&vAy, &vAxisBy));		// Ay�࿡ ������ A�� By�࿡ ����
			fRad		= fBLengthy + fALengthx + fALengthy;		// ����
			fDst		= fabs(D3DXVec3Dot(&vDst, &vAxisBy));		// �Ÿ�

			if (fRad < fDst) 
				return false;
		}
	}

	return true;
}
