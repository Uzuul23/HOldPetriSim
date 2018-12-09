/**************************************************************************
	HRect.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "stdafx.h"
#include "Resource.h"
#include "IniSection.h"
#include "HDrawObject.h"

IMPLEMENT_SERIAL(CHRect,CObject, DRAW_VERSION)

CHRect::CHRect()
{
}

CHRect::CHRect(CPoint& topleft)
{
	m_crRect.SetRect(topleft.x,topleft.y,topleft.x,topleft.y);
	m_crInnerColor = RGB(255,255,0); 
	m_crBorderColor = RGB(0,0,0);
	m_iPenStyle = PS_SOLID;
	m_iPenWidth = 1;
	m_RectStyle = RS_ROUND;
	m_cpRound.x = 10;
	m_cpRound.y = 10;
	m_bEmpty = false;
	m_bSetRound = false;
}

CHRect::CHRect(CRect & rect)
{
	m_crRect = rect;
	m_crInnerColor = RGB(255,255,0); 
	m_crBorderColor = RGB(0,0,0);
	m_iPenStyle = PS_SOLID;
	m_iPenWidth = 1;
	m_RectStyle = RS_ROUND;
	m_cpRound.x = 10;
	m_cpRound.y = 10;
	m_bEmpty = false;
	m_bSetRound = false;
}

CHRect::~CHRect()
{

}

void CHRect::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);
	m_crRect.NormalizeRect();
	m_bSetRound = false;
	if (ar.IsStoring())
	{
		ar << (DWORD)m_crRect.left;
		ar << (DWORD)m_crRect.top;
		ar << (DWORD)m_crRect.right;
		ar << (DWORD)m_crRect.bottom;
		ar << (DWORD)m_crInnerColor; 
		ar << (DWORD)m_crBorderColor; 
		ar << (WORD)m_iPenStyle;
		ar << (WORD)m_iPenWidth;
		ar << (WORD)m_RectStyle;
		ar << (WORD)m_cpRound.x;
		ar << (WORD)m_cpRound.y;
		ar << (BYTE)m_bEmpty;
	}
	else
	{
		DWORD dw;
		WORD w;
		BYTE b;
		ar >> dw; m_crRect.left = dw;
		ar >> dw; m_crRect.top = dw;
		ar >> dw; m_crRect.right = dw;
		ar >> dw; m_crRect.bottom = dw;
		ar >> dw; m_crInnerColor = dw;
		ar >> dw; m_crBorderColor = dw;
		ar >> w; m_iPenStyle = w;
		ar >> w; m_iPenWidth = w;
		ar >> w; m_RectStyle = w;
		ar >> w; m_cpRound.x = w;
		ar >> w; m_cpRound.y = w;
		ar >> b; m_bEmpty = (b == 0)? false : true;
	}	
}

#ifdef _DEBUG
void CHRect::AssertValid() const
{
	CObject::AssertValid();
}

void CHRect::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif //_DEBUG

void CHRect::DrawObject(CDC * pDC)
{
	ASSERT_VALID(this);
	LOGBRUSH logbrush;

	if(m_bEmpty)
	{
		logbrush.lbStyle = BS_NULL;
	}
	else
	{
		logbrush.lbStyle = BS_SOLID;
		logbrush.lbColor = m_crInnerColor; 
	}
	CBrush pInnerBrush;
	pInnerBrush.CreateBrushIndirect(&logbrush);
	CBrush* oldbrush = pDC->SelectObject(&pInnerBrush);
	CPen cpen;
	cpen.CreatePen(m_iPenStyle,m_iPenWidth,m_crBorderColor);
	CPen* oldpen = pDC->SelectObject(&cpen);
	switch(m_RectStyle)
	{
	case RS_RECT:
		pDC->Rectangle(m_crRect);
		break;
	case RS_ROUND:
		pDC->RoundRect(m_crRect,m_cpRound);
		break;
	case RS_ELLIPSE:
		pDC->Ellipse(m_crRect);
		break;
	}
	pDC->SelectObject(oldbrush);
	pDC->SelectObject(oldpen);
	if(m_blSelected)
	{
		for(int i = 1;i<9;i++)
		{
			pDC->Rectangle(GetGripper(i));
		}
		if((m_RectStyle == RS_ROUND) && (m_bSetRound))
		pDC->Rectangle(GetGripper(9));
	}
}

bool CHRect::PtInObject(POINT & point)
{
	ASSERT_VALID(this);
	switch (m_RectStyle)
	{
	case RS_RECT:
		{
			return (m_crRect.PtInRect(point)) ? true : false;
		}
	case RS_ROUND:
		{
			CRgn crgn;
			crgn.CreateRoundRectRgn(m_crRect.left, m_crRect.top,
				m_crRect.right, m_crRect.bottom, m_cpRound.x, m_cpRound.y);
			return (crgn.PtInRegion(point)) ? true : false;
		}
	case RS_ELLIPSE:
		{
			CRgn crgn;
			crgn.CreateEllipticRgn(m_crRect.left, m_crRect.top,
				m_crRect.right, m_crRect.bottom);
			return (crgn.PtInRegion(point))? true : false;
		}
	}
	return false;
}

void CHRect::MoveTo(POINT & to)
{
	ASSERT_VALID(this);
	m_crRect.OffsetRect(to);
	m_bSetRound = false;
}

CRect CHRect::GetBoundingRect(CDC* pDC,bool mode)
{
	ASSERT_VALID(this);
	
	CRect crect(m_crRect);
	crect.NormalizeRect();
	crect.InflateRect(SL_GRIPPER,SL_GRIPPER);
	return crect;
}

int CHRect::PtInGripper(POINT & point)
{
	ASSERT_VALID(this);
	ASSERT(m_blSelected);

	for(int i = 1;i<9;i++)
	{
		if(GetGripper(i).PtInRect(point))return i;
	}
	if((m_bSetRound) && (m_RectStyle == RS_ROUND) && 
		(GetGripper(9).PtInRect(point)))
			return 9;
	return NULL;
}

void CHRect::ResizeTo(POINT & to,int handle)
{
	ASSERT_VALID(this);
	ASSERT(m_blSelected);

	if(handle == NULL)
	{
		m_crRect.right += to.x;
		m_crRect.bottom += to.y;
		return;
	}
	if(handle < 9)m_bSetRound = false;
	switch(handle)
	{
	case 1:
		m_crRect.left += to.x;
		m_crRect.top += to.y;			
		break;
	case 2:
		m_crRect.top += to.y;
		break;
	case 3:
		m_crRect.right += to.x;
		m_crRect.top += to.y;
		break;
	case 4:
		m_crRect.right += to.x;
		break;
	case 5:
		m_crRect.right += to.x;
		m_crRect.bottom += to.y;
		break;
	case 6:
		m_crRect.bottom += to.y;
		break;
	case 7:
		m_crRect.left += to.x;
		m_crRect.bottom += to.y;
		break;
	case 8:
		m_crRect.left += to.x;
		break;
	case 9:
		{
			m_cpRound.x += to.x;
			m_cpRound.y += to.y;
			CRect crect(m_crRect);
			crect.NormalizeRect();
			crect.SetRect(0,0, crect.Width(), crect.Height());
			if(!crect.PtInRect(m_cpRound))
				m_cpRound = crect.CenterPoint();
			break;
		}
	}
}

CRect CHRect::GetGripper(int nHandle)
{
	ASSERT_VALID(this);
	int x, y, xCenter, yCenter;
	xCenter = m_crRect.left + m_crRect.Width() / 2;
	yCenter = m_crRect.top + m_crRect.Height() / 2;
	switch (nHandle)
	{
	default:
		ASSERT(FALSE);
	case 1:
		x = m_crRect.left;
		y = m_crRect.top;
		break;
	case 2:
		x = xCenter;
		y = m_crRect.top;
		break;
	case 3:
		x = m_crRect.right;
		y = m_crRect.top;
		break;
	case 4:
		x = m_crRect.right;
		y = yCenter;
		break;
	case 5:
		x = m_crRect.right;
		y = m_crRect.bottom;
		break;
	case 6:
		x = xCenter;
		y = m_crRect.bottom;
		break;
	case 7:
		x = m_crRect.left;
		y = m_crRect.bottom;
		break;
	case 8:
		x = m_crRect.left;
		y = yCenter;
		break;
	case 9:
		x = m_crRect.TopLeft().x + m_cpRound.x;
		y = m_crRect.TopLeft().y + m_cpRound.y;
		break;
	}
	return CRect(x - SL_GRIPPER / 2,y - SL_GRIPPER / 2,
		x + SL_GRIPPER / 2,y + SL_GRIPPER / 2);
}

void CHRect::InitMenu(CMenu * menu,CPoint & point)
{
	ASSERT_VALID(this);
	
	static CMenu typmenu;
	typmenu.DestroyMenu();
	typmenu.CreateMenu();
	typmenu.AppendMenu(MF_STRING | (m_RectStyle == RS_RECT)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND1,_T("Rect"));
	typmenu.AppendMenu(MF_STRING  | (m_RectStyle == RS_ROUND)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND2,_T("Rounded"));
	typmenu.AppendMenu(MF_STRING  | (m_RectStyle == RS_ELLIPSE)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND3,_T("Ellipse"));

	static CMenu penmenu;
	penmenu.DestroyMenu();
	penmenu.CreateMenu();
	penmenu.AppendMenu(MF_STRING | (m_iPenStyle == PS_SOLID)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND4,_T("Solid"));
	penmenu.AppendMenu(MF_STRING | (m_iPenStyle == PS_DASH)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND5,_T("Dash"));
	penmenu.AppendMenu(MF_STRING | (m_iPenStyle == PS_DOT)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND6,_T("Dot"));
	penmenu.AppendMenu(MF_STRING | (m_iPenStyle == PS_DASHDOT)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND7,_T("Dashdot"));

	static CMenu widthmenu;
	widthmenu.DestroyMenu();
	widthmenu.CreateMenu();
	widthmenu.AppendMenu(MF_STRING | (m_iPenWidth == 1)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND8,_T("1"));
	widthmenu.AppendMenu(MF_STRING | (m_iPenWidth == 2)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND9,_T("2"));
	widthmenu.AppendMenu(MF_STRING | (m_iPenWidth == 3)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND10,_T("3"));
	widthmenu.AppendMenu(MF_STRING | (m_iPenWidth == 4)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND11, _T("4"));
	widthmenu.AppendMenu(MF_STRING | (m_iPenWidth == 5)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND12,_T("5"));

	menu->AppendMenu( MF_STRING,ID_CONTEXT_COMMAND13,_T("&Border color"));
	menu->AppendMenu(MF_STRING  | (m_bEmpty)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND14,_T("Empty"));
	menu->AppendMenu( MF_STRING | (m_bEmpty)
		? MF_GRAYED : MF_ENABLED,ID_CONTEXT_COMMAND15,_T("&Fill color"));
	menu->AppendMenu( MF_POPUP,(UINT)typmenu.m_hMenu ,_T("Typ"));
	menu->AppendMenu( MF_POPUP,(UINT)penmenu.m_hMenu ,_T("Pen style"));
	menu->AppendMenu( MF_POPUP,(UINT)widthmenu.m_hMenu ,_T("Pen width"));
	menu->AppendMenu(MF_STRING | (m_RectStyle == RS_ROUND)
		? MF_ENABLED : MF_GRAYED,ID_CONTEXT_COMMAND16,_T("Set Round"));
}

void CHRect::DoID(UINT nID)
{
	ASSERT_VALID(this);
	CColorDialog* cdialog;
	switch(nID)
	{
	case 1:
		m_RectStyle = RS_RECT;
		break;
	case 2:
		m_RectStyle = RS_ROUND;
		break;
	case 3:
		m_RectStyle = RS_ELLIPSE;
		break;
	case 4:
		m_iPenStyle = PS_SOLID;
		break;
	case 5:
		m_iPenStyle = PS_DASH;
		m_iPenWidth = 1;
		break;
	case 6:
		m_iPenStyle = PS_DOT;
		m_iPenWidth = 1;
		break;
	case 7:
		m_iPenStyle = PS_DASHDOT;
		m_iPenWidth = 1;
		break;
	case 8:
		m_iPenWidth = 1;
		break;
	case 9:
		m_iPenWidth = 2;
		m_iPenStyle = PS_SOLID;
		break;
	case 10:
		m_iPenWidth = 3;
		m_iPenStyle = PS_SOLID;
		break;
	case 11:
		m_iPenWidth = 4;
		m_iPenStyle = PS_SOLID;
		break;
	case 12:
		m_iPenWidth = 5;
		m_iPenStyle = PS_SOLID;
		break;
	case 13:
		cdialog = new CColorDialog(m_crBorderColor);
		if(cdialog->DoModal() == IDOK)
			m_crBorderColor = cdialog->GetColor();
		delete cdialog;
		break;
	case 14:
		m_bEmpty = !m_bEmpty;
		break;
	case 15:
		cdialog = new CColorDialog(m_crInnerColor);
		if(cdialog->DoModal() == IDOK)
			m_crInnerColor = cdialog->GetColor();
		delete cdialog;
		break;
	case 16:
		{
			m_bSetRound = true;
			m_crRect.NormalizeRect();
			CRect crect(m_crRect);
			crect.SetRect(0,0, crect.Width(), crect.Height());
			if(!crect.PtInRect(m_cpRound))
				m_cpRound = crect.CenterPoint();
			break;
		}
	default:ASSERT(false); //unbekannte ID
	}
	
}

LPCTSTR CHRect::GetCursor(int handle)
{
	ASSERT_VALID(this);
	if((handle == 2) || (handle == 6))return IDC_SIZENS;
	if((handle == 4) || (handle == 8))return IDC_SIZEWE;
	if((handle == 1) || (handle == 5))return IDC_SIZENWSE;
	if((handle == 3) || (handle == 7))return IDC_SIZENESW;
	if(handle == 9)return IDC_CROSS;
	return IDC_SIZEALL;
}

void CHRect::ToGrid(int grid,int handle)
{
	ASSERT_VALID(this);
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
		point.y = m_crRect.top;
		point.y = ((r = point.y % grid) >= grid / 2) ? grid - r : -r;
		m_crRect.top += point.y;
		break;
	case 3:
		point.x = m_crRect.right;
		point.y = m_crRect.top;
		point.x = ((r = point.x % grid) >= grid / 2) ? grid - r : -r;
		point.y = ((r = point.y % grid) >= grid / 2) ? grid - r : -r;
		m_crRect.right += point.x;
		m_crRect.top += point.y;
		break;
	case 4:
		point.x = m_crRect.right;
		point.x = ((r = point.x % grid) >= grid / 2) ? grid - r : -r;
		m_crRect.right += point.x;
		break;
	case 5:
		point = m_crRect.BottomRight();
		point.x = ((r = point.x % grid) >= grid / 2) ? grid - r : -r;
		point.y = ((r = point.y % grid) >= grid / 2) ? grid - r : -r;
		m_crRect.right += point.x;
		m_crRect.bottom += point.y;
		break;
	case 6:
		point.y = m_crRect.bottom;
		point.y = ((r = point.y % grid) >= grid / 2) ? grid - r : -r;
		m_crRect.bottom += point.y;
		break;
	case 7:
		point.x = m_crRect.left;
		point.y = m_crRect.bottom;
		point.x = ((r = point.y % grid) >= grid / 2) ? grid - r : -r;
		point.y = ((r = point.y % grid) >= grid / 2) ? grid - r : -r;
		m_crRect.left += point.x;
		m_crRect.bottom += point.y;
		break;
	case 8:
		point.x = m_crRect.left;
		point.x = ((r = point.x % grid) >= grid / 2) ? grid - r : -r;
		m_crRect.left += point.x;
		break;
	case 9:
		point = m_cpRound;
		point.x = ((r = point.x % grid) >= grid / 2) ? grid - r : -r;
		point.y = ((r = point.y % grid) >= grid / 2) ? grid - r : -r;
		m_cpRound += point;
		break;
	default:
		ASSERT(false);
		break;
	}
}

bool CHRect::GetProperty(GV_ITEM* pItem)
{
	ASSERT_VALID(this);
	ASSERT(pItem);
	switch(pItem->lParam)
	{
	case 1:
		pItem->mask = GVIF_PARAM|GVIF_TYPE|GVIF_ITEM;
		pItem->type = GVT_ITEMS;
		pItem->array.Add(_T("Rect"));
		pItem->array.Add(_T("Roundrect"));
		pItem->array.Add(_T("Ellipse"));
		switch(m_RectStyle)
		{
		case RS_RECT:pItem->szText = _T("Rect");break;
		case RS_ROUND:pItem->szText = _T("Roundrect");break;
		case RS_ELLIPSE:pItem->szText = _T("Ellipse");break;
		}
		return true;
	case 2://border color
		pItem->mask = GVIF_PARAM|GVIF_TYPE|GVIF_BKCLR;
		pItem->type = GVT_COLOR;
		pItem->crBkClr = m_crBorderColor;
		pItem->szText = " ";
		return true;
	case 3://inner color
		pItem->mask = GVIF_PARAM|GVIF_TYPE|GVIF_BKCLR;
		pItem->type = GVT_COLOR;
		pItem->crBkClr = m_crInnerColor;
		pItem->szText = " ";
		return true;
	case 4://pen style
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
	case 5://pen width
		pItem->mask = GVIF_PARAM|GVIF_TYPE;
		pItem->type = GVT_DEFAULT;
		pItem->szText.Format(_T("%d"),m_iPenWidth);
		return true;
	case 6://empty
		pItem->mask = GVIF_PARAM|GVIF_TYPE;
		pItem->type = GVT_BOOL;
		if(m_bEmpty)pItem->szText = _T("TRUE");
		else pItem->szText = _T("FALSE");
		return true;
	default:return false;
	}
	return false;
}

bool CHRect::SetProperty(GV_ITEM* pItem)
{
	int i;
	switch(pItem->lParam)
	{
	case 1:
		if(pItem->szText == _T("Rect"))m_RectStyle = RS_RECT;
		else if(pItem->szText == _T("Roundrect"))m_RectStyle = RS_ROUND;
		else if(pItem->szText == _T("Ellipse"))m_RectStyle = RS_ELLIPSE;
		return true;
	case 2:
		m_crBorderColor = pItem->crBkClr;
		return true;
	case 3:
		m_crInnerColor = pItem->crBkClr;
		return true;
	case 4:
		if(pItem->szText == _T("Solid"))m_iPenStyle = PS_SOLID;
		else if(pItem->szText == _T("Dash"))m_iPenStyle = PS_DASH;
		else if(pItem->szText == _T("Dot"))m_iPenStyle = PS_DOT;
		else if(pItem->szText == _T("Dash dot"))m_iPenStyle = PS_DASHDOT;
		return true;
	case 5:
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
			pItem->szText = _T("5";)
			m_iPenWidth = 5;
			return false;
		}
		else
		{
			m_iPenWidth = i;
			return true;
		}
	case 6:
		if(pItem->szText == _T("TRUE"))m_bEmpty = true;
		else m_bEmpty = false;
		return true;
	default: return false;
	}

	return false;
}

CString CHRect::GetPropertyText(UINT par)
{
	switch(par)
	{
	case 1:return "Type";
	case 2:return "Border Color";
	case 3:return "Fill Color";
	case 4:return "Pen Style";
	case 5:return "Pen Width";
	case 6:return "Empty";
	}
	return "";
}

void CHRect::WriteExportFormat(CIniSection& ini)
{
	ini.SetValue(_T("RUNTIME_CLASS"),GetRuntimeClass()->m_lpszClassName);
	ini.SetValue(_T("BOUNDING_RECT"),GetRect());
	ini.SetValue(_T("INNER_COLOR"),m_crInnerColor);
	ini.SetValue(_T("BORDER_COLOR"),m_crBorderColor);
	ini.SetValue(_T("PEN_STYLE"), m_iPenStyle);
	ini.SetValue(_T("PEN_WIDTH"),m_iPenWidth);
	ini.SetValue(_T("RECT_STYLE"),m_RectStyle);
	ini.SetValue(_T("ROUND_POINT"),m_cpRound);
	ini.SetValue(_T("EMPTY_RECT"),m_bEmpty);
}

void CHRect::Select(bool sel)
{
	ASSERT_VALID(this);
	m_blSelected = sel;
	if(!sel)
	{
		m_crRect.NormalizeRect();
		m_bSetRound = false;
	}
};

void CHRect::ReadExportFormat(CIniSection& ini)
{
	ini.GetValue(_T("BOUNDING_RECT"),m_crRect);
	ini.GetValue(_T("INNER_COLOR"),m_crInnerColor);
	ini.GetValue(_T("BORDER_COLOR"),m_crBorderColor);
	ini.GetValue(_T("PEN_STYLE"),m_iPenStyle);
	ini.GetValue(_T("PEN_WIDTH"),m_iPenWidth);
	ini.GetValue(_T("RECT_STYLE"),m_RectStyle);
	ini.GetValue(_T("ROUND_POINT"),m_cpRound);
	ini.GetValue(_T("EMPTY_RECT"),m_bEmpty);
}


