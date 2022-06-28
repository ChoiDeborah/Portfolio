#include "stdafx.h"
#include "WndInfo.h"


CWndInfo::CWndInfo()
{
}


CWndInfo::~CWndInfo()
{
}

bool CWndInfo::IsRectInPoint(D3DXVECTOR3 vPoint)
{
	if (vParentPos.x + vPos.x - vOffset.x <= vPoint.x && vParentPos.x + vPos.x + (vSize.x - vOffset.x) >= vPoint.x &&
		vParentPos.y + vPos.y - vOffset.y <= vPoint.y && vParentPos.y + vPos.y + (vSize.y - vOffset.y) >= vPoint.y)
	{
		iHover = 1;
		return true;
	}
	else
	{
		iHover = 0;
		return false;
	}

	return false;
}
