/**************************************************************************
	HLine.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "stdafx.h"
#include "Resource.h"
#include "IniSection.h"
#include "HDrawObject.h"

IMPLEMENT_SERIAL(CHLine, CObject,DRAW_VERSION)
CHLine::CHLine()
{
}

CHLine::CHLine(CPoint& start)
{
	m_crRect.SetRect(start.x,start.y,start.x,start.y);
	m_iPenStyle = PS_SOLID;
	m_iPenWidth = 1; 
	m_crColor = RGB(0,0,0); 

}

CHLine::CHLine(CPoint& start,CPoint& end,int PenStyle,
			   int Width,COLORREF crColor)
{
	m_crRect.SetRect(start.x,start.y,start.x,start.y);
	m_iPenStyle = PenStyle;
	m_iPenWidth = Width; 
	m_crColor = crColor; 
}

CHLine::~CHLine()
{

}

void CHLine::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << (DWORD)m_crRect.left;
		ar << (DWORD)m_crRect.top;
		ar << (DWORD)m_crRect.right;
		ar << (DWORD)m_crRect.bottom;
		ar << (WORD)m_iPenStyle;
		ar << (WORD)m_iPenWidth;
		ar << (DWORD)m_crColor; 
	}
	else
	{
		WORD w;
		DWORD dw;
		ar >> dw; m_crRect.left = dw;
		ar >> dw; m_crRect.top = dw;
		ar >> dw; m_crRect.right = dw;
		ar >> dw; m_crRect.bottom = dw;
		ar >> w; m_iPenStyle = w;
		ar >> w; m_iPenWidth = w;
		ar >> dw; m_crColor = dw;
	}		
}

#ifdef _DEBUG
void CHLine::AssertValid() const
{
	CObject::AssertValid();
}

void CHLine::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif //_DEBUG

void CHLine::DrawObject(CDC* pDC)
{
	CPen penStroke;
	penStroke.CreatePen(m_iPenStyle,m_iPenWidth,m_crColor);
	CPen* pOldPen = pDC->SelectObject(&penStroke);
	
	pDC->MoveTo(m_crRect.left,m_crRect.top);
	pDC->LineTo(m_crRect.right,m_crRect.bottom);
	pDC->SelectObject(pOldPen);

	if(m_blSelected)
	{
		pDC->Rectangle(CRect(m_crRect.left- SL_GRIPPER / 2,
			m_crRect.top - SL_GRIPPER / 2,
			m_crRect.left + SL_GRIPPER / 2,
			m_crRect.top + SL_GRIPPER / 2));

		pDC->Rectangle(CRect(m_crRect.right - SL_GRIPPER / 2,
			m_crRect.bottom - SL_GRIPPER / 2,
			m_crRect.right + SL_GRIPPER / 2,
			m_crRect.bottom + SL_GRIPPER / 2));
	}
}

bool CHLine::PtInObject(POINT & point)
{
	//Wenn der Punkt nicht im umschliessenden Rechteck
	//liegt brechen wir ab.
	if(!GetBoundingRect().PtInRect(point))return false;

	//ein kleiner Ausflug in die Vektorrechnung
	// ist der Punkt auf der Linie ??? 
	int dick = max(m_iPenWidth,5);
	int ax,ay,hx,hy;
	double dist;

	ax = m_crRect.right - m_crRect.left;
	ay = m_crRect.bottom - m_crRect.top;
	hx = point.x - m_crRect.left;
	hy = point.y - m_crRect.top;
	dist = (ax * hy - ay * hx)/( sqrt ((double)(ax * ax + ay * ay)));
	
	if(abs((int)dist) < (dick / 2))return true;

	return false;
}

CRect CHLine::GetBoundingRect(CDC* pDC,bool mode)
{
	CRect crect(m_crRect);
	crect.NormalizeRect();
	crect.InflateRect(SL_GRIPPER,SL_GRIPPER);
	return crect;
}

int CHLine::PtInGripper(POINT & point)
{
	ASSERT(m_blSelected);

	CRect crect(m_crRect.left - SL_GRIPPER,m_crRect.top - SL_GRIPPER,
	m_crRect.left + SL_GRIPPER,m_crRect.top + SL_GRIPPER);
	if(crect.PtInRect(point))return 1;

	crect.SetRect(m_crRect.right - SL_GRIPPER,m_crRect.bottom - SL_GRIPPER,
	m_crRect.right + SL_GRIPPER,m_crRect.bottom + SL_GRIPPER);
	if(crect.PtInRect(point))return 2;

	return 0;
}

void CHLine::ResizeTo(POINT & to,int handle)
{
	ASSERT(m_blSelected);

	switch(handle)
	{
	case NULL:
		m_crRect.right += to.x;
		m_crRect.bottom += to.y;
		return;
	case 1:
		m_crRect.left += to.x;
		m_crRect.top += to.y;
		return;
	case 2:
		m_crRect.right += to.x;
		m_crRect.bottom += to.y;
		return;
	default: ASSERT(false);
	}
}

void CHLine::MoveTo(POINT & to)
{
	m_crRect.left += to.x;
	m_crRect.top += to.y;
	m_crRect.right += to.x;
	m_crRect.bottom += to.y;
}

void CHLine::InitMenu(CMenu * menu,CPoint & point)
{
	static CMenu penmenu;
	penmenu.DestroyMenu();
	penmenu.CreateMenu();
	penmenu.AppendMenu(MF_STRING | (m_iPenStyle == PS_SOLID)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND1,_T("Solid"));
	penmenu.AppendMenu(MF_STRING | (m_iPenStyle == PS_DASH)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND2,_T("Dash"));
	penmenu.AppendMenu(MF_STRING | (m_iPenStyle == PS_DOT)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND3,_T("Dot"));
	penmenu.AppendMenu(MF_STRING | (m_iPenStyle == PS_DASHDOT)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND4,_T("Dashdot"));

	static CMenu widthmenu;
	widthmenu.DestroyMenu();
	widthmenu.CreateMenu();
	widthmenu.AppendMenu(MF_STRING | (m_iPenWidth == 1)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND5,_T("1"));
	widthmenu.AppendMenu(MF_STRING | (m_iPenWidth == 2)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND6,_T("2"));
	widthmenu.AppendMenu(MF_STRING | (m_iPenWidth == 3)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND7,_T("3"));
	widthmenu.AppendMenu(MF_STRING | (m_iPenWidth == 4)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND8,_T("4"));
	widthmenu.AppendMenu(MF_STRING | (m_iPenWidth == 5)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND9,_T("5"));

	menu->AppendMenu( MF_STRING,ID_CONTEXT_COMMAND10,_T("&Color"));
	menu->AppendMenu(MF_STRING,ID_CONTEXT_COMMAND11,_T("Font"));
	menu->AppendMenu( MF_POPUP,(UINT)penmenu.m_hMenu ,_T("Penstyle"));
	menu->AppendMenu( MF_POPUP,(UINT)widthmenu.m_hMenu ,_T("Pen width"));
}

void CHLine::DoID(UINT nID)
{
	CColorDialog* cdialog;
	switch(nID)
	{
	case 1:
		m_iPenStyle = PS_SOLID;
		break;
	case 2:
		m_iPenStyle = PS_DASH;
		m_iPenWidth = 1;
		break;
	case 3:
		m_iPenStyle = PS_DOT;
		m_iPenWidth = 1;
		break;
	case 4:
		m_iPenStyle = PS_DASHDOT;
		m_iPenWidth = 1;
		break;
	case 5:
		m_iPenWidth = 1;
		break;
	case 6:
		m_iPenWidth = 2;
		m_iPenStyle = PS_SOLID;
		break;
	case 7:
		m_iPenWidth = 3;
		m_iPenStyle = PS_SOLID;
		break;
	case 8:
		m_iPenWidth = 4;
		m_iPenStyle = PS_SOLID;
		break;
	case 9:
		m_iPenWidth = 5;
		m_iPenStyle = PS_SOLID;
		break;
	case 10:
		cdialog = new CColorDialog(m_crColor);
		if(cdialog->DoModal() == IDOK)
			m_crColor = cdialog->GetColor();
		delete cdialog;
		break;
	case 11:
		break;
	default:ASSERT(false);
	}
	
}

void CHLine::ToGrid(int grid,int handle)
{
	CPoint point;
	int r;
	switch(handle)
	{
	case NULL:
		point = m_crRect.TopLeft();
		point.x = ((r = point.x % grid) >= grid / 2) ? grid - r : -r;
		point.y = ((r = point.y % grid) >= grid / 2) ? grid - r : -r;
		m_crRect.left += point.x;
		m_crRect.top += point.y;
		point = m_crRect.BottomRight();
		point.x = ((r = point.x % grid) >= grid / 2) ? grid - r : -r;
		point.y = ((r = point.y % grid) >= grid / 2) ? grid - r : -r;
		m_crRect.right += point.x;
		m_crRect.bottom += point.y;
		break;
	case 1:
		point = m_crRect.TopLeft();
		point.x = ((r = point.x % grid) >= grid / 2) ? grid - r : -r;
		point.y = ((r = point.y % grid) >= grid / 2) ? grid - r : -r;
		m_crRect.left += point.x;
		m_crRect.top += point.y;
		break;
	case 2:
		point = m_crRect.BottomRight();
		point.x = ((r = point.x % grid) >= grid / 2) ? grid - r : -r;
		point.y = ((r = point.y % grid) >= grid / 2) ? grid - r : -r;
		m_crRect.right += point.x;
		m_crRect.bottom += point.y;
		break;
	default:
		ASSERT(false);
		break;
	}
}

LPCTSTR CHLine::GetCursor(int handle)
{
	if(handle)
		return IDC_SIZENWSE;
	else
		return IDC_SIZEALL;
}

bool CHLine::GetProperty(GV_ITEM* pItem)
{
	ASSERT_VALID(this);
	ASSERT(pItem);
	switch(pItem->lParam)
	{
	case 1://border color
		pItem->mask = GVIF_PARAM|GVIF_TYPE|GVIF_BKCLR;
		pItem->type = GVT_COLOR;
		pItem->crBkClr = m_crColor;
		pItem->szText = _T(" ");
		return true;
	case 2://pen style
		pItem->mask = GVIF_PARAM|GVIF_TYPE|GVIF_ITEM;
		pItem->type = GVT_ITEMS;
		pItem->array.Add(_T("Solid"));
		pItem->array.Add(_T("Dash"));
		pItem->array.Add(_T("Dot"));
		pItem->array.Add(_T("Dash dot"));
		switch(m_iPenStyle)
		{
		case PS_SOLID:pItem->szText = _T("Solid");break;
		case PS_DASH:pItem->szText = _T("Dash");break;
		case PS_DOT:pItem->szText = _T("Dot");break;
		case PS_DASHDOT:pItem->szText = _T("Dash dot");break;
		}
		return true;
	case 3://pen width
		pItem->mask = GVIF_PARAM|GVIF_TYPE;
		pItem->type = GVT_DEFAULT;
		pItem->szText.Format(_T("%d"),m_iPenWidth);
		return true;
	default:return false;
	}
	return false;
}

bool CHLine::SetProperty(GV_ITEM* pItem)
{
	int i;
	switch(pItem->lParam)
	{
	case 1:
		m_crColor = pItem->crBkClr;
		return true;
	case 2:
		if(pItem->szText == _T("Solid"))m_iPenStyle = PS_SOLID;
		else if(pItem->szText == _T("Dash"))m_iPenStyle = PS_DASH;
		else if(pItem->szText == _T("Dot"))m_iPenStyle = PS_DOT;
		else if(pItem->szText == _T("Dash dot"))m_iPenStyle = PS_DASHDOT;
		return true;
	case 3:
		i = _tstoi(pItem->szText);
		if(i < 1)
		{
			pItem->mask = GVIF_TEXT;
			pItem->szText = _T("1");
			m_iPenWidth = 1;
			return false;
		}
		else if(i > 5)
		{
			pItem->mask = GVIF_TEXT;
			pItem->szText = _T("5");
			m_iPenWidth = 5;
			return false;
		}
		else
		{
			m_iPenWidth = i;
			return true;
		}
	default: return false;
	}
	return false;
}

CString CHLine::GetPropertyText(UINT par)
{
	switch(par)
	{
	case 1:return _T("Color");
	case 2:return _T("Pen Style");
	case 3:return _T("Pen Width");
	}
	return _T("");
}

void CHLine::WriteExportFormat(CIniSection& ini)
{
	ini.SetValue(_T("RUNTIME_CLASS"),GetRuntimeClass()->m_lpszClassName);
	ini.SetValue(_T("BOUNDING_RECT"),GetRect());
	ini.SetValue(_T("PEN_COLOR"),m_crColor);
	ini.SetValue(_T("PEN_STYLE"), m_iPenStyle);
	ini.SetValue(_T("PEN_WIDTH"),m_iPenWidth);
}

void CHLine::ReadExportFormat(CIniSection& ini)
{
	ini.GetValue(_T("BOUNDING_RECT"), m_crRect);
	ini.GetValue(_T("PEN_COLOR"),m_crColor);
	ini.GetValue(_T("PEN_STYLE"), m_iPenStyle);
	ini.GetValue(_T("PEN_WIDTH"),m_iPenWidth);
}

