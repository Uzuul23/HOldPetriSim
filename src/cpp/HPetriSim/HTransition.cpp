/**************************************************************************
	HTransition.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "stdafx.h"
#include "HDrawObject.h"
#include "HRandom.h"
#include "IniSection.h"
#include "PetriSim.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

/*************************************************************************/
// CHTransition
/*************************************************************************/

IMPLEMENT_SERIAL(CHTransition,CObject,DRAW_VERSION)

CHTransition::CHTransition()
{
	m_iDelay = 0;
	m_iStartDelay = 0;
	m_iTokensCount = 0;
	m_iRangeDelay = 0;
	m_bAktiv = false;
	m_bEnabled = false;
}

CHTransition::CHTransition(POINT & point, long ident)
{
	m_TimeMode = TTM_IMMIDIATE;
	m_size = PT_NORMAL;
	m_crRect.SetRect(point.x - m_size / 2,
	point.y - m_size / 6,
	point.x + m_size / 2,
	point.y + m_size / 6);
	m_iDelay = 0;
	m_iStartDelay = 0;
	m_iTokensCount = 0;
	m_iRangeDelay = 0;
	m_bAktiv = false;
	m_bEnabled = false;
	m_iIdent = ident;
}

CHTransition::~CHTransition()
{
}

#ifdef _DEBUG
void CHTransition::AssertValid() const
{
	CObject::AssertValid();
}

void CHTransition::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif //_DEBUG

void CHTransition::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << (DWORD)m_size;
		ar << (DWORD)m_crRect.left;
		ar << (DWORD)m_crRect.top;
		ar << (DWORD)m_crRect.right;
		ar << (DWORD)m_crRect.bottom;
		ar << (DWORD)m_iIdent;
		ar << (DWORD)m_iDelay;
		ar << (DWORD)m_iStartDelay;
		ar << (DWORD)m_iTokensCount;
		ar << (DWORD)m_iRangeDelay;
		ar << (DWORD)0;
		ar << (DWORD)0;
		ar << (BYTE)0;
		ar << (BYTE)0;
		ar << (BYTE)m_TimeMode;
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
		BYTE b;
		DWORD dw;
		double db;
		ar >> dw;m_size = (PT_SIZE)dw;
		ar >> dw;m_crRect.left = dw;
		ar >> dw;m_crRect.top = dw;
		ar >> dw;m_crRect.right = dw;
		ar >> dw;m_crRect.bottom = dw;
		ar >> dw;m_iIdent = dw;
		ar >> dw;m_iDelay = dw;
		ar >> dw;m_iStartDelay = dw;
		ar >> dw;m_iTokensCount = dw;
		ar >> dw;m_iRangeDelay = dw;
		ar >> dw;
		ar >> dw;
		ar >> b;
		ar >> b;
		ar >> b;m_TimeMode = (TRAN_TIME_MODE)b;
		ar >> b;
		ar >> b;
		ar >> b;
		ar >> b;
		ar >> b;
		ar >> b;
		ar >> db;
	}
}

void CHTransition::DrawObject(CDC * pDC)
{
	ASSERT_VALID(this);
	CPetriSimApp* pApp = (CPetriSimApp*)AfxGetApp();
	CHResHolder& rRes = pApp->m_cResHolder;
	CBrush* oldbrush;
	if(m_blSelected)
	{
		if(m_bAktiv)
		{
			oldbrush = pDC->SelectObject(&rRes.m_cBrushGreen);
			pDC->Rectangle(m_crRect);
			oldbrush = pDC->SelectObject(&rRes.m_cBrushYellow);
			for(int i = 1;i<5;i++)
				pDC->Rectangle(GetGripper(i));
		}
		else if(m_bEnabled)
		{
			oldbrush = pDC->SelectObject(&rRes.m_cBrushYellow);
			pDC->Rectangle(m_crRect);
			for(int i = 1;i<5;i++)
				pDC->Rectangle(GetGripper(i));
		}
		else
		{
			oldbrush = pDC->SelectObject(&rRes.m_cBrushRed);
			pDC->Rectangle(m_crRect);
			oldbrush = pDC->SelectObject(&rRes.m_cBrushYellow);
			for(int i = 1;i<5;i++)
				pDC->Rectangle(GetGripper(i));
		}
	}
	else
	{
		if(m_bAktiv)
		{
			oldbrush = pDC->SelectObject(&rRes.m_cBrushGreen);
			pDC->Rectangle(m_crRect);
		}
		else if(m_bEnabled)
		{
			oldbrush = pDC->SelectObject(&rRes.m_cBrushYellow);
			pDC->Rectangle(m_crRect);
		}
		else
		{
			if(m_TimeMode == TTM_IMMIDIATE)
			oldbrush = pDC->SelectObject(&rRes.m_cBrushBlack);
			else
				oldbrush = pDC->SelectObject(&rRes.m_cBrushWhite);
			pDC->Rectangle(m_crRect);
		}	
	}
	pDC->SelectObject(oldbrush);
}

void CHTransition::InitMenu(CMenu* menu,CPoint & point)
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

void CHTransition::DoID(UINT nID)
{
	CPoint cpoint(m_crRect.CenterPoint());
	switch(nID)
	{
	case 1:
		m_size = PT_VERYSMALL;
		m_crRect.SetRect(cpoint.x - m_size / 2,
		cpoint.y - m_size / 6,
		cpoint.x + m_size / 2,
		cpoint.y + m_size / 6);
		RecalcArcs();
		break;
	case 2:
		m_size = PT_SMALL;
		m_crRect.SetRect(cpoint.x - m_size / 2,
		cpoint.y - m_size / 6,
		cpoint.x + m_size / 2,
		cpoint.y + m_size / 6);
		RecalcArcs();
		break;
	case 3:
		m_size = PT_NORMAL;
		m_crRect.SetRect(cpoint.x - m_size / 2,
		cpoint.y - m_size / 6,
		cpoint.x + m_size / 2,
		cpoint.y + m_size / 6);
		RecalcArcs();
		break;
	case 4:
		m_size = PT_LARGE;
		m_crRect.SetRect(cpoint.x - m_size / 2,
		cpoint.y - m_size / 6,
		cpoint.x + m_size / 2,
		cpoint.y + m_size / 6);
		RecalcArcs();
		break;
	case 5:
		m_size = PT_VERYLARGE;
		m_crRect.SetRect(cpoint.x - m_size / 2,
		cpoint.y - m_size / 6,
		cpoint.x + m_size / 2,
		cpoint.y + m_size / 6);
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
	default:ASSERT(false); //unbekannte ID
	}
	RecalcLabelPos();
}

bool CHTransition::GetProperty(GV_ITEM* pItem)
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
		case PT_VERYSMALL:pItem->szText = "Very Small";break;
		case PT_SMALL:pItem->szText = "Small";break;
		case PT_NORMAL:pItem->szText = "Normal";break;
		case PT_LARGE:pItem->szText = "Large";break;
		case PT_VERYLARGE:pItem->szText = "Very Large";break;
		}
		return true;
	case 3://Show Label Name
		pItem->mask = GVIF_PARAM|GVIF_TYPE;
		pItem->type = GVT_BOOL;
		if(GetLabelA()->GetVisible())pItem->szText = "TRUE";
		else pItem->szText = "FALSE";
		return true;
	case 4://Show Label Delay
		pItem->mask = GVIF_PARAM|GVIF_TYPE;
		pItem->type = GVT_BOOL;
		if(GetLabelB()->GetVisible())pItem->szText = "TRUE";
		else pItem->szText = "FALSE";
		return true;
	case 5:
		pItem->mask = GVIF_PARAM|GVIF_TYPE|GVIF_ITEM;
		pItem->type = GVT_ITEMS;
		pItem->array.Add(_T("Immediate"));
		pItem->array.Add(_T("Deterministic"));
		pItem->array.Add(_T("Exponential"));
		pItem->array.Add(_T("Uniform Distr."));
		switch(m_TimeMode)
		{
		case TTM_IMMIDIATE:pItem->szText = _T("Immediate");break;
		case TTM_DELAY:pItem->szText = _T("Deterministic");break;
		case TTM_EXPONENTIAL:pItem->szText = _T("Exponential");break;
		case TTM_EQUAL_DISTR:pItem->szText = _T("Uniform Distr.");break;
		}
		return true;
	case 6://Initial Delay
		pItem->mask = GVIF_PARAM|GVIF_TYPE;
		pItem->type = GVT_DEFAULT;
		pItem->szText.Format(_T("%d"), m_iStartDelay);
		return true;
	case 7://Range Delay
		pItem->mask = GVIF_PARAM|GVIF_TYPE;
		pItem->type = GVT_DEFAULT;
		pItem->szText.Format(_T("%d"), m_iRangeDelay);
		return true;
	case 8://Current Delay
		pItem->mask = GVIF_PARAM|GVIF_TYPE;
		pItem->type = GVT_DEFAULT;
		pItem->szText.Format(_T("%d"), m_iDelay);
		return true;
	case 9://Tokens Fired
		pItem->mask = GVIF_PARAM|GVIF_TYPE;
		pItem->type = GVT_DEFAULT;
		pItem->szText.Format(_T("%d"), m_iTokensCount);
		return true;
	default:return false;
	}
	return false;

}

bool CHTransition::SetProperty(GV_ITEM* pItem)
{
	long i;
	switch(pItem->lParam)
	{
	case 1:
		GetLabelA()->SetText(pItem->szText);
		return true;
	case 2:
		if(pItem->szText == "Very Small")DoID(1);
		else if(pItem->szText == "Small")DoID(2);
		else if(pItem->szText == "Normal")DoID(3);
		else if(pItem->szText == "Large")DoID(4);
		else if(pItem->szText == "Very Large")DoID(5);
		return true;
	case 3:
		GetLabelA()->SetVisible((pItem->szText == _T("TRUE"))? true : false);
		return true;
	case 4:
		GetLabelB()->SetVisible((pItem->szText == _T("TRUE"))? true : false);
		return true;
	case 5:
		if(pItem->szText == "Immediate")
		{
			m_TimeMode = TTM_IMMIDIATE;
			m_iStartDelay = 0;
			m_iDelay = 0;
			m_iRangeDelay = 0;
		}
		else if(pItem->szText == "Deterministic")
		{
			m_TimeMode = TTM_DELAY;
			m_iStartDelay = 1;
			m_iDelay = 1;
			m_iRangeDelay = 0;
		}
		else if(pItem->szText == "Exponential")
		{
			m_TimeMode = TTM_EXPONENTIAL;
			m_iStartDelay = 1;
			m_iDelay = 1;
			m_iRangeDelay = 0;
		}
		else if(pItem->szText == "Uniform Distr.")
		{
			m_TimeMode = TTM_EQUAL_DISTR;
			m_iStartDelay = 1;
			m_iDelay = 1;
			m_iRangeDelay = 2;
		}
		return false;
	case 6://initial delay
		{
			CString cstring;
			i = _tstol(pItem->szText);
			switch(m_TimeMode)
			{
			case TTM_IMMIDIATE:
				m_iStartDelay = 0;
				break;
			case TTM_DELAY:
				m_iStartDelay = min(abs(i), MAX_DELAY_TIME);
				m_iDelay = m_iStartDelay;
				break;
			case TTM_EXPONENTIAL:
				m_iStartDelay = min(abs(i), MAX_DELAY_TIME);
				m_iDelay = (long)CHExpRandom::GetRand(m_iStartDelay);
				break;
			case TTM_EQUAL_DISTR:
				m_iStartDelay = min(abs(i), m_iRangeDelay);
				m_iDelay = 
					(long)CHRandom::GetRand(m_iStartDelay,
						m_iRangeDelay);
				break;
			}
			cstring.Format(_T("%d"), m_iStartDelay);
			GetLabelB()->SetText(cstring);
			return false;
		}
	case 7:
		i = _tstoi(pItem->szText);
		switch(m_TimeMode)
		{
		case TTM_IMMIDIATE:
		case TTM_DELAY:
		case TTM_EXPONENTIAL:
			m_iRangeDelay = 0;
			break;
		case TTM_EQUAL_DISTR:
			m_iRangeDelay = max(abs(i), m_iStartDelay);
			m_iRangeDelay = min(m_iRangeDelay, MAX_DELAY_TIME);
			m_iDelay = 
				(long)CHRandom::GetRand(m_iStartDelay, m_iRangeDelay);
			break;
		}
		return false;
	case 8:
		i = _tstoi(pItem->szText);
		m_iDelay = min(abs(i), MAX_DELAY_TIME);
		return false;
	case 9:
		i = _tstoi(pItem->szText);
		m_iTokensCount = min(abs(i), MAX_TOKENS);
		return false;
	default: return false;
	}
	return true;
}

CString CHTransition::GetPropertyText(UINT par)
{
	switch(par)
	{
	case 1:return "Name";
	case 2:return "Size";
	case 3:return "Show Name";
	case 4:return "Show Delay";
	case 5:return "Time Mode";
	case 6:return "Initial Delay";
	case 7:return "Range Delay";
	case 8:return "Current Delay";
	case 9:return "Tokens Fired";
	}
	return "";
}

void CHTransition::RecalcLabelPos()
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

void CHTransition::Reset()
{
	if(m_TimeMode == TTM_IMMIDIATE)
		m_iDelay = m_iStartDelay;
	m_iTokensCount = 0;
	m_bAktiv = false;
	m_bEnabled = false;
}

void CHTransition::WriteExportFormat(CIniSection& ini)
{
	ini.SetValue(_T("RUNTIME_CLASS"),
		GetRuntimeClass()->m_lpszClassName);
	ini.SetValue(_T("BOUNDING_RECT"), m_crRect);
	ini.SetValue(_T("TRANSITION_ID"), m_iIdent);
	ini.SetValue(_T("TRANSITION_SIZE"), m_size);
	ini.SetValue(_T("FIRE_DELAY"), m_iStartDelay);
	ini.SetValue(_T("FIRE_DELAY_RANGE"), m_iRangeDelay);
	ini.SetValue(_T("TRANSITION_TIME_MODE"), m_TimeMode);
}

void CHTransition::ReadExportFormat(CIniSection& ini)
{
	ini.GetValue(_T("BOUNDING_RECT"), m_crRect);
	ini.GetValue(_T("TRANSITION_ID"), m_iIdent);
	int size = 0;
	ini.GetValue(_T("TRANSITION_SIZE"), size);
	m_size = (PT_SIZE)size;
	ini.GetValue(_T("FIRE_DELAY"), m_iStartDelay);
	ini.GetValue(_T("FIRE_DELAY_RANGE"), m_iRangeDelay);
	int mode = 0;
	ini.GetValue(_T("TRANSITION_TIME_MODE"), mode);
	m_TimeMode = (TRAN_TIME_MODE)mode;
}











