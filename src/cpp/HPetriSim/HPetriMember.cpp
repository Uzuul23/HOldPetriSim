/**************************************************************************
	HPetriMember.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "stdafx.h"
#include "HDrawObject.h"

CHPetriMember::CHPetriMember()
{
	m_chlLabelA = NULL;
	m_chlLabelB = NULL;
}

CHPetriMember::~CHPetriMember()
{
	m_cplLineIn.RemoveAll();
	m_cplLineOut.RemoveAll();
}

void CHPetriMember::MoveTo(POINT & to)
{
	ASSERT_VALID(this);
	m_crRect.OffsetRect(to);
	RecalcArcs();
	
	if(m_chlLabelA != NULL)
	{
		if(!m_chlLabelA->IfSelected())m_chlLabelA->MoveTo(to);
	}
	if(m_chlLabelB != NULL)
	{
		if(!m_chlLabelB->IfSelected())m_chlLabelB->MoveTo(to);
	}
	
}

CRect CHPetriMember::GetBoundingRect(CDC* pDC,bool mode)
{
	ASSERT_VALID(this);
	CRect crect(m_crRect);
	if(m_blSelected)
	{
		CPoint pt;
		POSITION pos = m_cplLineIn.GetHeadPosition();
		while (pos != NULL)
		{
			pt = m_cplLineIn.GetNext(pos)->m_caPoints
				[m_cplLineIn.GetAt(pos)->m_caPoints.GetSize() - 2];
			if(pt.x < crect.left)crect.left = pt.x;
			if(pt.x > crect.right)crect.right = pt.x;
			if(pt.y < crect.top)crect.top = pt.y;
			if(pt.y > crect.bottom)crect.bottom = pt.y;
		}
		pos = m_cplLineOut.GetHeadPosition();
		while (pos != NULL)
		{
			pt = m_cplLineOut.GetNext(pos)->m_caPoints[1];
			if(pt.x < crect.left)crect.left = pt.x;
			if(pt.x > crect.right)crect.right = pt.x;
			if(pt.y < crect.top)crect.top = pt.y;
			if(pt.y > crect.bottom)crect.bottom = pt.y;
		}		
		if(m_chlLabelA != NULL)
		{
			if(!m_chlLabelA->IfSelected())
				crect.UnionRect(crect,m_chlLabelA->GetBoundingRect());
		}
		if(m_chlLabelB != NULL)
		{
			if(!m_chlLabelB->IfSelected())
				crect.UnionRect(crect,m_chlLabelB->GetBoundingRect());
		}
	crect.NormalizeRect();
	crect.InflateRect(SL_GRIPPER,SL_GRIPPER);
	}
	return crect;
}

void CHPetriMember::SetIdent(long ident)
{
	ASSERT_VALID(this);
	m_iIdent = ident;
	POSITION pos = m_cplLineIn.GetHeadPosition();
	while (pos != NULL)
	{
		m_cplLineIn.GetNext(pos)->SetTo(ident);
	}
	pos = m_cplLineOut.GetHeadPosition();
	while (pos != NULL)
	{
		m_cplLineOut.GetNext(pos)->SetFrom(ident);
	}
	if(m_chlLabelA != NULL)
	{
		m_chlLabelA->SetOwner(ident);
	}
	if(m_chlLabelB != NULL)
	{
		m_chlLabelB->SetOwner(ident);
	}

}

void CHPetriMember::ToGrid(int grid,int handle)
{
	CPoint point(m_crRect.CenterPoint());
	int r;
	point.x = ((r = point.x % grid) >= grid / 2) ? grid - r : -r;
	point.y = ((r = point.y % grid) >= grid / 2) ? grid - r : -r;
	m_crRect.OffsetRect(point);
	RecalcArcs();
}

void CHPetriMember::RecalcArcs()
{
	POSITION pos = m_cplLineIn.GetHeadPosition();
	while (pos != NULL)
	{
		m_cplLineIn.GetNext(pos)->RecalcPoint();
	}
	pos = m_cplLineOut.GetHeadPosition();
	while (pos != NULL)
	{
		m_cplLineOut.GetNext(pos)->RecalcPoint();
	}
}