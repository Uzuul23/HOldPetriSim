/**************************************************************************
	HPosition.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "stdafx.h"
#include "HDrawObject.h"
#include "IniSection.h"
#include "PetriSim.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CHPosition,CObject, DRAW_VERSION)

CHPosition::CHPosition()
{
	m_iTokens = 0;
	m_iTokensMax = 1;
	m_iTokensStart = 0;
	m_iTokensCount = 0;
}
CHPosition::CHPosition(POINT & point, long ident)
{
	m_size = PT_NORMAL;
	m_crRect.SetRect(point.x - m_size / 2,
		point.y - m_size / 2,
		point.x + m_size / 2,
		point.y + m_size / 2);
	m_iTokens = 0;
	m_iTokensMax = 1;
	m_iTokensStart = 0;
	m_iTokensCount = 0;
	m_iIdent = ident;
}
CHPosition::~CHPosition()
{
}

#ifdef _DEBUG
void CHPosition::AssertValid() const
{
	CObject::AssertValid();
}
void CHPosition::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif //_DEBUG

void CHPosition::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << (DWORD)m_iTokens;
		ar << (DWORD)m_iTokensMax;
		ar << (DWORD)m_iTokensStart;
		ar << (DWORD)m_iTokensCount;
		ar << (DWORD)m_crRect.left;
		ar << (DWORD)m_crRect.top;
		ar << (DWORD)m_crRect.right;
		ar << (DWORD)m_crRect.bottom;
		ar << (DWORD)m_size;
		ar << (DWORD)m_iIdent;
		ar << (DWORD)0;
		ar << (BYTE)0;
		ar << (BYTE)0;
		ar << (BYTE)0;
		ar << (BYTE)0;
		ar << (BYTE)0;
		ar << (BYTE)0;
		ar << (BYTE)0;
		ar << (double)0.0;
	}
	else
	{
		DWORD dw;
		BYTE b;
		double db;
		ar >> dw;m_iTokens = dw;
		ar >> dw;m_iTokensMax = dw;
		ar >> dw;m_iTokensStart = dw;
		ar >> dw;m_iTokensCount = dw;
		ar >> dw;m_crRect.left = dw;
		ar >> dw;m_crRect.top = dw;
		ar >> dw;m_crRect.right = dw;
		ar >> dw;m_crRect.bottom = dw;		
		ar >> dw;m_size = (PT_SIZE)dw;
		ar >> dw;m_iIdent = dw;
		ar >> dw;
		ar >> b;
		ar >> b;
		ar >> b;
		ar >> b;
		ar >> b;
		ar >> b;
		ar >> b;
		ar >> db;
	}
}

void CHPosition::DrawObject(CDC * pDC)
{
	CPetriSimApp* pApp = (CPetriSimApp*)AfxGetApp();
	CHResHolder& rRes = pApp->m_cResHolder;

	int nIndex = 0;
	switch(m_size)
	{
	case PT_VERYSMALL:
		nIndex = 0;
		break;
	case PT_SMALL:
		nIndex = 1;
		break;
	case PT_NORMAL:
		nIndex = 2;
		break;
	case PT_LARGE:
		nIndex = 3;
		break;
	case PT_VERYLARGE:
		nIndex = 4;
		break;
	}
	if(m_iTokens < 5)
	{
		int nImage = 0;
		switch(m_iTokens)
		{
		case 1:
			nImage = 2;
			break;
		case 2:
			nImage = 4;
			break;
		case 3:
			nImage = 6;
			break;
		case 4:
			nImage = 8;
			break;
		}
		if(m_blSelected)
			nImage++;
		rRes.m_cImgPos[nIndex].Draw(pDC, nImage, m_crRect.TopLeft()
			,ILD_TRANSPARENT);
	}
	else
	{
		rRes.m_cImgPos[nIndex].Draw(pDC, (m_blSelected) ? 1 : 0
			, m_crRect.TopLeft(),ILD_TRANSPARENT);
		pDC->SetBkMode(TRANSPARENT);
		TCHAR buffer[10];
		StringCchPrintf(buffer, 10, _T("%d"), m_iTokens);
		pDC->DrawText(buffer,-1,m_crRect
			,DT_SINGLELINE|DT_CENTER|DT_VCENTER);	
	}
	if(m_blSelected)
	{
		CBrush* pOldBrush = pDC->SelectObject(&rRes.m_cBrushYellow);
		for(int i = 1;i<5;i++)
		{
			pDC->Rectangle(GetGripper(i));
		}
		pDC->SelectObject(pOldBrush);
	}
}

void CHPosition::InitMenu(CMenu* menu,CPoint & point)
{
	static CMenu sizemenu;
	sizemenu.DestroyMenu();
	sizemenu.CreateMenu();
	sizemenu.AppendMenu(MF_STRING | (m_size == PT_VERYSMALL)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND1,_T("Very Small"));
	sizemenu.AppendMenu(MF_STRING | (m_size == PT_SMALL)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND2,_T("Small"));
	sizemenu.AppendMenu(MF_STRING | (m_size == PT_NORMAL)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND3,_T("Normal"));
	sizemenu.AppendMenu(MF_STRING | (m_size == PT_LARGE)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND4,_T("Large"));
	sizemenu.AppendMenu(MF_STRING | (m_size == PT_VERYLARGE)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND5,_T("Very Large"));

	static CMenu labelmenu;
	labelmenu.DestroyMenu();
	labelmenu.CreateMenu();
	if(m_chlLabelA)
	labelmenu.AppendMenu(MF_STRING | (GetLabelA()->GetVisible())
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND6,_T("Name"));
	if(m_chlLabelB)
	labelmenu.AppendMenu(MF_STRING | (GetLabelB()->GetVisible())
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND7,_T("Capacity"));
	menu->AppendMenu( MF_POPUP,(UINT)labelmenu.m_hMenu ,_T("Show Label"));
	menu->AppendMenu( MF_POPUP,(UINT)sizemenu.m_hMenu ,_T("Set Size"));
	menu->AppendMenu(MF_STRING, ID_CONTEXT_COMMAND8, _T("Reset Labels"));
}

void CHPosition::DoID(UINT nID)
{
	CPoint cpoint(m_crRect.CenterPoint());
	switch(nID)
	{
	case 1:
		m_size = PT_VERYSMALL;
		m_crRect.SetRect(cpoint.x - m_size / 2,
		cpoint.y - m_size / 2,
		cpoint.x + m_size / 2,
		cpoint.y + m_size / 2);
		RecalcArcs();
		break;
	case 2:
		m_size = PT_SMALL;
		m_crRect.SetRect(cpoint.x - m_size / 2,
		cpoint.y - m_size / 2,
		cpoint.x + m_size / 2,
		cpoint.y + m_size / 2);
		break;
	case 3:
		m_size = PT_NORMAL;
		m_crRect.SetRect(cpoint.x - m_size / 2,
		cpoint.y - m_size / 2,
		cpoint.x + m_size / 2,
		cpoint.y + m_size / 2);
		RecalcArcs();
		break;
	case 4:
		m_size = PT_LARGE;
		m_crRect.SetRect(cpoint.x - m_size / 2,
		cpoint.y - m_size / 2,
		cpoint.x + m_size / 2,
		cpoint.y + m_size / 2);
		RecalcArcs();
		break;
	case 5:
		m_size = PT_VERYLARGE;
		m_crRect.SetRect(cpoint.x - m_size / 2,
		cpoint.y - m_size / 2,
		cpoint.x + m_size / 2,
		cpoint.y + m_size / 2);
		RecalcArcs();
		break;
	case 6:
		if(GetLabelA())
			GetLabelA()->SetVisible(!GetLabelA()->GetVisible());

		break;
	case 7:
		if(GetLabelB())
			GetLabelB()->SetVisible(!GetLabelB()->GetVisible());
		break;
	case 8:
		break;
	default:ASSERT(false);
	}
	RecalcLabelPos();
}

bool CHPosition::GetProperty(GV_ITEM* pItem)
{
	ASSERT_VALID(this);
	ASSERT(pItem);
	switch(pItem->lParam)
	{
	case 1://Name
		pItem->mask = GVIF_PARAM|GVIF_TYPE|GVIF_ITEM;
		pItem->type = GVT_DEFAULT;
		pItem->szText = GetLabelA()->GetText();
		return true;
	case 2://Size
		pItem->mask = GVIF_PARAM|GVIF_TYPE|GVIF_ITEM;
		pItem->type = GVT_ITEMS;
		pItem->array.Add(_T("Very Small"));
		pItem->array.Add(_T("Small"));
		pItem->array.Add(_T("Normal"));
		pItem->array.Add(_T("Large"));
		pItem->array.Add(_T("Very Large"));
		switch(m_size)
		{
		case PT_VERYSMALL:pItem->szText = _T("Very Small");break;
		case PT_SMALL:pItem->szText = _T("Small");break;
		case PT_NORMAL:pItem->szText = _T("Normal");break;
		case PT_LARGE:pItem->szText = _T("Large");break;
		case PT_VERYLARGE:pItem->szText = _T("Very Large");break;
		}
		return true;
	case 3://Show Label Name
		pItem->mask = GVIF_PARAM|GVIF_TYPE;
		pItem->type = GVT_BOOL;
		if(GetLabelA()->GetVisible())pItem->szText = _T("TRUE");
		else pItem->szText = _T("FALSE");
		return true;
	case 4://Show Label Capacity
		pItem->mask = GVIF_PARAM|GVIF_TYPE;
		pItem->type = GVT_BOOL;
		if(GetLabelB()->GetVisible())pItem->szText = _T("TRUE");
		else pItem->szText = _T("FALSE");
		return true;
	case 5://Initial Tokens
		pItem->mask = GVIF_PARAM|GVIF_TYPE;
		pItem->type = GVT_DEFAULT;
		pItem->szText.Format(_T("%d"), m_iTokensStart);
		return true;
	case 6://Tokens now
		pItem->mask = GVIF_PARAM|GVIF_TYPE;
		pItem->type = GVT_DEFAULT;
		pItem->szText.Format(_T("%d"), m_iTokens);
		return true;
	case 7://Max Tokens
		pItem->mask = GVIF_PARAM|GVIF_TYPE;
		pItem->type = GVT_DEFAULT;
		pItem->szText.Format(_T("%d"), m_iTokensMax);
		return true;
	case 8://Tokens Count
		pItem->mask = GVIF_PARAM|GVIF_TYPE;
		pItem->type = GVT_DEFAULT;
		pItem->szText.Format(_T("%d"), m_iTokensCount);
		return true;
	default:return false;
	}
	return false;
}
/////////////////////////////////////////////////////////////////////////////
//
bool CHPosition::SetProperty(GV_ITEM* pItem)
{
	long i;
	switch(pItem->lParam)
	{
	case 1:
		GetLabelA()->SetText(pItem->szText);
		return true;
	case 2:
		if(pItem->szText == _T("Very Small"))DoID(1);
		else if(pItem->szText == _T("Small"))DoID(2);
		else if(pItem->szText == _T("Normal"))DoID(3);
		else if(pItem->szText == _T("Large"))DoID(4);
		else if(pItem->szText == _T("Very Large"))DoID(5);
		return true;
	case 3:
		GetLabelA()
			->SetVisible((pItem->szText == _T("TRUE"))? true : false);
		return true;
	case 4:
		GetLabelB()
			->SetVisible((pItem->szText == _T("TRUE"))? true : false);
		return true;
	case 5:
		i = _tstol(pItem->szText);
		m_iTokensStart = min(abs(i), MAX_TOKENS);
		m_iTokens = m_iTokensStart;
		return false;
	case 6:
		i = _tstol(pItem->szText);
		m_iTokens = min(abs(i), MAX_TOKENS);
		return false;
	case 7:
		{
		i = _tstol(pItem->szText);
		m_iTokensMax = min(abs(i), MAX_TOKENS);
		CString cstring;
		cstring.Format(_T("%d"), m_iTokensMax);
		GetLabelB()->SetText(cstring);
		return false;
		}
	case 8:
		i = _tstol(pItem->szText);
		m_iTokensCount = min(abs(i), MAX_TOKENS);
		return false;
	default: return false;
	}
	return true;
}
/////////////////////////////////////////////////////////////////////////////
//
CString CHPosition::GetPropertyText(UINT par)
{
	switch(par)
	{
	case 1:return _T("Name");
	case 2:return _T("Size");
	case 3:return _T("Show Name");
	case 4:return _T("Show Capacity");
	case 5:return _T("Initial Tokens");
	case 6:return _T("Current Tokens");
	case 7:return _T("Capacity");
	case 8:return _T("Tokens Count");
	}
	return _T("");
}

void CHPosition::RecalcLabelPos()
{
	if(GetLabelA())
	{
		int h = GetLabelA()->GetRect().Height();
		int w = GetLabelA()->GetRect().Width();
		CRect crect((m_crRect.left - LABEL_OFFSET_X),
			(m_crRect.top - h - LABEL_OFFSET_Y),
			(m_crRect.left - LABEL_OFFSET_X + w),
			(m_crRect.top - LABEL_OFFSET_Y));
		GetLabelA()->SetRect(crect);
	}
	if(GetLabelB())
	{
		int h = GetLabelB()->GetRect().Height();
		int w = GetLabelB()->GetRect().Width();
		CRect crect((m_crRect.left - LABEL_OFFSET_X),
			(m_crRect.bottom + LABEL_OFFSET_Y),
			(m_crRect.left - LABEL_OFFSET_X + w),
			(m_crRect.bottom - LABEL_OFFSET_Y + h));
		GetLabelB()->SetRect(crect);
	}
}

void CHPosition::Reset()
{
	m_iTokens = m_iTokensStart;
	m_iTokensCount = 0;
}

void CHPosition::WriteExportFormat(CIniSection& ini)
{
	ini.SetValue(_T("RUNTIME_CLASS"),GetRuntimeClass()->m_lpszClassName);
	ini.SetValue(_T("BOUNDING_RECT"), m_crRect);
	ini.SetValue(_T("POSITION_ID"), m_iIdent);
	ini.SetValue(_T("POSITION_SIZE"), m_size);
	ini.SetValue(_T("TOKENS_CAPAZITY"), m_iTokensMax);
	ini.SetValue(_T("TOKENS_START"), m_iTokensStart);
}

void CHPosition::ReadExportFormat(CIniSection& ini)
{
	ini.GetValue(_T("BOUNDING_RECT"), m_crRect);
	ini.GetValue(_T("POSITION_ID"), m_iIdent);
	int size;
	ini.GetValue(_T("POSITION_SIZE"), size);
	m_size = (PT_SIZE)size;
	ini.GetValue(_T("TOKENS_CAPAZITY"), m_iTokensMax);
	ini.GetValue(_T("TOKENS_START"), m_iTokensStart);
}
