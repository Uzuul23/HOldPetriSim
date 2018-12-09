/**************************************************************************
	HDrawObject.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "stdafx.h"
#include "PetriSim.h"
#include "HDrawObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

/*************************************************************************/
// CHDrawObject
/*************************************************************************/

//IMPLEMENT_SERIAL(CHDrawObject,CObject, DRAW_VERSION)

CHDrawObject::CHDrawObject()
{
	m_blSelected = false;
	m_bInvalid = false;
	m_iIdent = -1;
}

CHDrawObject::~CHDrawObject()
{
}

LPCTSTR CHDrawObject::GetCursorEx(CPoint& point)
{	
	ASSERT_VALID(this);
	int  nGripp = PtInGripper(point);
	if(nGripp)
		return GetCursor(nGripp);
	else if(PtInObject(point))
		return IDC_SIZEALL;
	else
		return NULL;
}

#ifdef _DEBUG
void CHDrawObject::AssertValid() const
{
//	CObject::AssertValid();
}

void CHDrawObject::Dump(CDumpContext& dc) const
{
//	CObject::Dump(dc);
}
#endif //_DEBUG

bool CHDrawObject::IfSelected()
{
	ASSERT_VALID(this);
	return m_blSelected;
}

CRect CHDrawObject::GetRect()
{
	ASSERT_VALID(this);
	return m_crRect;
}
	
void CHDrawObject::SetRect(CRect& rect)
{
	ASSERT_VALID(this);
	m_crRect = rect;
}

bool CHDrawObject::IsInvalid()
{
	ASSERT_VALID(this);
	return m_bInvalid;
}

void CHDrawObject::SetInvalid(bool invalid)
{
	ASSERT_VALID(this);
	m_bInvalid = invalid; 
}