/**************************************************************************
	HLabel.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "stdafx.h"
#include "Resource.h"
#include "HDrawObject.h"
#include "IniSection.h"
#include "HLabelDialog.h"
#include "PetriSim.h"

IMPLEMENT_SERIAL(CHLabel,CObject, DRAW_VERSION)
CHLabel::CHLabel()
{
	m_bVisible = true;
	m_iOwner = 0;
	m_bVisible = true;
	m_bBorder = false;
	m_bEmpty = true;
}

CHLabel::CHLabel(int top,int left,CString string,
				 long owner /* = 0 */, LABEL_SUB sub/* = LS_LABEL_A */) :
m_csText(string)
{	
	m_crRect.SetRect(top,left,
		top,left);
	m_iOwner = owner;
	m_iSubIdent = sub;
	m_bVisible = true;
	m_bBorder = false;
	m_bEmpty = true;
}

CHLabel::~CHLabel()
{
}

void CHLabel::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_csText;
		ar << (DWORD)m_crRect.top;
		ar << (DWORD)m_crRect.left;
		ar << (DWORD)m_crRect.bottom;
		ar << (DWORD)m_crRect.right;
		ar << (DWORD)m_iOwner;
		ar << (DWORD)m_iSubIdent;
		ar << (BYTE)m_bVisible;
		ar << (BYTE)m_bBorder;
		ar << (BYTE)m_bEmpty;
	}
	else
	{
		DWORD dw;
		BYTE b;
		ar >> m_csText;
		ar >> dw;m_crRect.top = dw;
		ar >> dw;m_crRect.left = dw;
		ar >> dw;m_crRect.bottom = dw;
		ar >> dw;m_crRect.right = dw;
		ar >> dw;m_iOwner = dw;
		ar >> dw;m_iSubIdent = (LABEL_SUB)dw;
		ar >> b;m_bVisible = (b == 0)? false : true;
		ar >> b;m_bBorder = (b == 0)? false : true;
		ar >> b;m_bEmpty = (b == 0)? false : true;
	}	
}

#ifdef _DEBUG
void CHLabel::AssertValid() const
{
	CObject::AssertValid();
}

void CHLabel::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif //_DEBUG

void CHLabel::DrawObject(CDC * pDC)
{
	if(m_bVisible)
	{
		CPetriSimApp* pApp = (CPetriSimApp*)AfxGetApp();
		CHResHolder& rRes = pApp->m_cResHolder;
		m_crRect.right = m_crRect.left + pDC->GetTextExtent(m_csText).cx;
		m_crRect.bottom = m_crRect.top + pDC->GetTextExtent(m_csText).cy;
		if(m_bEmpty && m_bBorder)
		{
			pDC->MoveTo(m_crRect.TopLeft());
			pDC->LineTo(m_crRect.top, m_crRect.right);
			pDC->LineTo(m_crRect.BottomRight());
			pDC->LineTo(m_crRect.bottom, m_crRect.left);
		}
		if(!m_bEmpty && m_bBorder)
			pDC->Rectangle(m_crRect);
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(m_csText,-1,m_crRect,DT_SINGLELINE);
		if(m_blSelected)
		{
			CBrush* oldbrush = pDC->SelectObject(&rRes.m_cBrushYellow);
			for(int i = 1;i<5;i++)
			{
				pDC->Rectangle(GetGripper(i));
			}
			pDC->SelectObject(oldbrush);
		}
	}
}

bool CHLabel::PtInObject(POINT & point)
{
	if(!m_bVisible)return false;
	if(m_crRect.PtInRect(point))return true;
	return false;	
}

void CHLabel::MoveTo(POINT & to)
{ 
	m_crRect.OffsetRect(to);
}

CRect CHLabel::GetBoundingRect(CDC* pDC,bool mode)
{
	if(mode)return m_crRect;
	if(pDC)
	{
		m_crRect.right = m_crRect.left + pDC->GetTextExtent(m_csText).cx;
		m_crRect.bottom = m_crRect.top + pDC->GetTextExtent(m_csText).cy;
		m_crRect.NormalizeRect();
	}
	if(!m_blSelected)return m_crRect;
	CRect crect(m_crRect);
	crect.InflateRect(SL_GRIPPER,SL_GRIPPER);
	return crect;
}

CRect CHLabel::GetGripper(int nHandle)
{
	int x, y, xCenter, yCenter;
	xCenter = m_crRect.left + m_crRect.Width() / 2;
	yCenter = m_crRect.top + m_crRect.Height() / 2;

	switch (nHandle)
	{
	case 1:
		x = m_crRect.left - SL_GRIPPER / 3;
		y = m_crRect.top - SL_GRIPPER / 3;
		break;
	case 2:
		x = m_crRect.right + SL_GRIPPER / 3;
		y = m_crRect.top - SL_GRIPPER / 3;
		break;
	case 3:
		x = m_crRect.right + SL_GRIPPER / 3;
		y = m_crRect.bottom + SL_GRIPPER / 3;
		break;
	case 4:
		x = m_crRect.left - SL_GRIPPER / 3;
		y = m_crRect.bottom + SL_GRIPPER / 3;
		break;
	default:
		ASSERT(FALSE);//unknown
	}
	return CRect(x - SL_GRIPPER / 3,
		y - SL_GRIPPER / 3,
		x + SL_GRIPPER / 3,
		y + SL_GRIPPER / 3);
}

void CHLabel::ToGrid(int grid,int handle)
{
	CPoint point(m_crRect.CenterPoint());
	int r;
	point.x = ((r = point.x % grid) >= grid / 2) ? grid - r : -r;
	point.y = ((r = point.y % grid) >= grid / 2) ? grid - r : -r;
	m_crRect.OffsetRect(point);
	
}

void CHLabel::InitMenu(CMenu * menu,CPoint & point)
{
}

void CHLabel::DoID(UINT nID)
{
	CHLabelDialog* ldialog;
	switch(nID)
	{
	case 1:
		ldialog = new CHLabelDialog(m_csText);
		if(ldialog->DoModal() == IDOK)
			m_csText = ldialog->GetText();
		delete ldialog;
		break;
	default:ASSERT(false);
	}	
}

void CHLabel::WriteExportFormat(CIniSection& ini)
{
	ini.SetValue(_T("RUNTIME_CLASS"), GetRuntimeClass()->m_lpszClassName);
	ini.SetValue(_T("BOUNDING_RECT"), m_crRect);
	ini.SetValue(_T("OWNER_IDENT"), m_iOwner);
	ini.SetValue(_T("SUB_IDENT"), m_iSubIdent);
	ini.SetValue(_T("LABEL_VISIBLE"), m_bVisible);
	ini.SetValue(_T("TEXT"), m_csText);
}

void CHLabel::ReadExportFormat(CIniSection& ini)
{
	ini.GetValue(_T("BOUNDING_RECT"), m_crRect);
	ini.GetValue(_T("OWNER_IDENT"), m_iOwner);
	int sub = 0;
	ini.GetValue(_T("SUB_IDENT"), sub);
	m_iSubIdent = (LABEL_SUB)sub;
	ini.GetValue(_T("LABEL_VISIBLE"), m_bVisible);
	ini.GetValue(_T("TEXT"), m_csText);
}
