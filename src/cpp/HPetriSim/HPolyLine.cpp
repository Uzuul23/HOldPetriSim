/**************************************************************************
	HPolyLine.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "StdAfx.h"
#include "Resource.h"
#include "IniSection.h"
#include "HDrawObject.h"

IMPLEMENT_SERIAL(CHPoly, CObject, DRAW_VERSION)

CHPoly::CHPoly()
{
	m_iPolyStyle = PM_POLYLINE;
	m_nFillMode = ALTERNATE;
	GetBoundingRect();
}

CHPoly::CHPoly(POINT& start,int PenStyle,
			   int Width,COLORREF crColor)
{
	m_caPoints.Add(start);
	m_caPoints.Add(start);
	m_iPenStyle = PenStyle;
	m_iPenWidth = Width; 
	m_crPenColor = crColor;
	m_crFillColor = RGB(255,255,0);
	m_iPolyStyle = PM_POLYGON;
	m_nFillMode = ALTERNATE;
	m_bEmpty = false;
	GetBoundingRect();
}

CHPoly::CHPoly(POINT& start)
{
	m_caPoints.Add(start);
	m_caPoints.Add(start);	
	m_iPenStyle = PS_SOLID;
	m_iPenWidth = 1; 
	m_crPenColor = RGB(0,0,0);
	m_crFillColor = RGB(255,255,0);
	m_iPolyStyle = PM_POLYGON;
	m_nFillMode = ALTERNATE;
	m_bEmpty = false;
	GetBoundingRect();
}

CHPoly::~CHPoly()
{
	m_caPoints.RemoveAll(); 
}

void CHPoly::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << (DWORD)m_crRect.left;
		ar << (DWORD)m_crRect.top;
		ar << (DWORD)m_crRect.right;
		ar << (DWORD)m_crRect.bottom;
		ar << (DWORD)m_crPenColor;
		ar << (DWORD)m_crFillColor;
		ar << (WORD)m_iPolyStyle;
		ar << (WORD)m_iPenStyle;
		ar << (WORD)m_iPenWidth;
		ar << (BYTE)m_bEmpty;	
	}
	else
	{
		BYTE b;
		WORD w;
		DWORD dw;
		ar >> dw; m_crRect.left = dw;
		ar >> dw; m_crRect.top = dw;
		ar >> dw; m_crRect.right = dw;
		ar >> dw; m_crRect.bottom = dw;
		ar >> dw; m_crPenColor = dw;
		ar >> dw; m_crFillColor = dw;
		ar >> w; m_iPolyStyle = w;
		ar >> w; m_iPenStyle = w;
		ar >> w; m_iPenWidth = w;
		ar >> b; m_bEmpty = (b != 0)? true : false;		
	}
	m_caPoints.Serialize(ar);	
}

#ifdef _DEBUG
void CHPoly::AssertValid() const
{
	CObject::AssertValid();
}

void CHPoly::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif //_DEBUG

void CHPoly::DrawObject(CDC* pDC)
{
	CPen penStroke;
	penStroke.CreatePen(m_iPenStyle,m_iPenWidth,m_crPenColor);
	CPen* oldpen = pDC->SelectObject(&penStroke);

	CBrush brush;
	CBrush* oldbrush;
	int oldmode;
	int i;
	switch(m_iPolyStyle)
	{
	case PM_POLYGON:
		LOGBRUSH logbrush;
		if(m_bEmpty)
		{
			logbrush.lbStyle = BS_NULL;
		}
		else
		{
			logbrush.lbStyle = BS_SOLID;
			logbrush.lbColor = m_crFillColor; 
		}
		brush.CreateBrushIndirect(&logbrush);
		oldbrush = pDC->SelectObject(&brush);
		oldmode = pDC->SetPolyFillMode(ALTERNATE);
		pDC->Polygon(m_caPoints.GetData(), m_caPoints.GetSize());
		pDC->SetPolyFillMode(oldmode);
		pDC->SelectObject(oldbrush);
		pDC->SelectObject(oldpen);
		if(m_blSelected)
		{
			CPoint cpoint(m_caPoints[0]);
			for (i=1; i < m_caPoints.GetSize(); i++)
			{
				int a = max(cpoint.x,m_caPoints[i].x);
				int b = min(cpoint.x,m_caPoints[i].x);
				cpoint.x =  (a - b) / 2 + b; 
				a = max(cpoint.y,m_caPoints[i].y);
				b = min(cpoint.y,m_caPoints[i].y);
				cpoint.y =  (a - b) / 2 + b; 
				pDC->Rectangle(CRect(cpoint.x - SL_GRIPPER / 3,
					cpoint.y - SL_GRIPPER / 3, cpoint.x + 
					SL_GRIPPER / 3,cpoint.y + SL_GRIPPER / 3));
				cpoint = m_caPoints[i];
			}
			int a = max(cpoint.x,m_caPoints[0].x);
			int b = min(cpoint.x,m_caPoints[0].x);
			cpoint.x =  (a - b) / 2 + b; 
			a = max(cpoint.y,m_caPoints[0].y);
			b = min(cpoint.y,m_caPoints[0].y);
			cpoint.y =  (a - b) / 2 + b; 
			pDC->Rectangle(CRect(cpoint.x - SL_GRIPPER / 3,
				cpoint.y - SL_GRIPPER / 3, cpoint.x + 
				SL_GRIPPER / 3,cpoint.y + SL_GRIPPER / 3));

			for (i=0; i < m_caPoints.GetSize(); i++)
			{
				pDC->Rectangle(CRect(m_caPoints[i].x - SL_GRIPPER / 2,
					m_caPoints[i].y - SL_GRIPPER / 2, m_caPoints[i].x + 
					SL_GRIPPER / 2,m_caPoints[i].y + SL_GRIPPER / 2));
			}
		}
		break;
	case PM_POLYLINE:
		pDC->Polyline(m_caPoints.GetData(),m_caPoints.GetSize());
		pDC->SelectObject(oldpen);
		if(m_blSelected)
		{
			CPoint cpoint(m_caPoints[0]);
			for (i=1; i < m_caPoints.GetSize(); i++)
			{
				int a = max(cpoint.x,m_caPoints[i].x);
				int b = min(cpoint.x,m_caPoints[i].x);
				cpoint.x =  (a - b) / 2 + b; 
				a = max(cpoint.y,m_caPoints[i].y);
				b = min(cpoint.y,m_caPoints[i].y);
				cpoint.y =  (a - b) / 2 + b; 
				pDC->Rectangle(CRect(cpoint.x - SL_GRIPPER / 3,
					cpoint.y - SL_GRIPPER / 3, cpoint.x + 
					SL_GRIPPER / 3,cpoint.y + SL_GRIPPER / 3));
				cpoint = m_caPoints[i];
			}
			for (i=0; i < m_caPoints.GetSize(); i++)
			{
				pDC->Rectangle(CRect(m_caPoints[i].x - SL_GRIPPER / 2,
					m_caPoints[i].y - SL_GRIPPER / 2, m_caPoints[i].x + 
					SL_GRIPPER / 2,m_caPoints[i].y + SL_GRIPPER / 2));
			}
		}
		break;
	default: ASSERT(false);
	}
}

bool CHPoly::PtInObject(POINT & point)
{
	//Wenn der Punkt nicht im umschliessenden Rechteck
	//liegt brechen wir gleich hier ab.
	if(!GetBoundingRect(0,true).PtInRect(point))
		return false;
	if(m_iPolyStyle == PM_POLYGON)
	{
		if(m_caPoints.GetSize() > 2)
		{
			if(m_bEmpty)
			{
				double dick = 5.0;
				double ax,ay,hx,hy;
				double dist;
				//ge‰ndert 11.03.01:
				//Fehler gefixt: es wurde das Objekt selektiert obwohl
				//der Cursor auﬂerhalb der Polyline lag
				CRect crect;
				for ( int i = 1; i < m_caPoints.GetSize(); i++)
				{
					crect.SetRect(m_caPoints[i], m_caPoints[i - 1]);
					crect.NormalizeRect();
					crect.InflateRect(2,2,2,2);
					if(crect.PtInRect(point))
					{
						ax = (double)m_caPoints[i].x - m_caPoints[i - 1].x;
						ay = (double)m_caPoints[i].y - m_caPoints[i - 1].y;
						hx = (double)point.x - m_caPoints[i - 1].x;
						hy = (double)point.y - m_caPoints[i - 1].y;
						dist = (ax * hy - ay * hx) / ( sqrt (ax * ax + ay * ay));
						dist = (dist >= 0) ? dist : -dist;
						if(dist < (dick / 2))return true;
					}
				}
				int last = m_caPoints.GetSize() - 1;
				crect.SetRect(m_caPoints[0], m_caPoints[last]);
				crect.NormalizeRect();
				crect.InflateRect(2,2,2,2);
				if(crect.PtInRect(point))
				{
					ax = (double)m_caPoints[0].x - m_caPoints[last].x;
					ay = (double)m_caPoints[0].y - m_caPoints[last].y;
					hx = (double)point.x - m_caPoints[last].x;
					hy = (double)point.y - m_caPoints[last].y;
					dist = (ax * hy - ay * hx) / ( sqrt (ax * ax + ay * ay));
					dist = (dist >= 0) ? dist : -dist;
					if(dist < (dick / 2))return true;
				}
				return false;
			}
			else
			{
				CRgn crgn;
				crgn.CreatePolygonRgn(m_caPoints.GetData(),
					m_caPoints.GetSize(), ALTERNATE);
				return (crgn.PtInRegion(point) == TRUE)? true: false;
			}
		}
		double dick = 5.0;
		double ax,ay,hx,hy;
		double dist;
		ax = (double)m_caPoints[1].x - m_caPoints[0].x;
		ay = (double)m_caPoints[1].y - m_caPoints[0].y;
		hx = (double)point.x - m_caPoints[0].x;
		hy = (double)point.y - m_caPoints[0].y;
		dist = (ax * hy - ay * hx) / ( sqrt (ax * ax + ay * ay));
		dist = (dist >= 0) ? dist : -dist;
		if(dist < (dick / 2))return true;
	}
	else
	{
		//ein kleiner Ausflug in die Vektorrechnung
		// ist der Punkt auf der Linie ??? 
		double dick = 5.0;
		double ax,ay,hx,hy;
		double dist;
		//ge‰ndert 11.03.01:
		//Fehler gefixt: es wurde das Objekt selektiert obwohl
		//der Cursor auﬂerhalb der Polyline lag
		for ( int i = 1; i < m_caPoints.GetSize(); i++)
		{
			CRect crect(m_caPoints[i], m_caPoints[i - 1]);
			crect.NormalizeRect();
			crect.InflateRect(2,2,2,2);
			if(crect.PtInRect(point))
			{
				ax = (double)m_caPoints[i].x - m_caPoints[i - 1].x;
				ay = (double)m_caPoints[i].y - m_caPoints[i - 1].y;
				hx = (double)point.x - m_caPoints[i - 1].x;
				hy = (double)point.y - m_caPoints[i - 1].y;
				dist = (ax * hy - ay * hx) / ( sqrt (ax * ax + ay * ay));
				dist = (dist >= 0) ? dist : -dist;
				if(dist < (dick / 2))return true;
			}
		}
		return false;
	}
	return false;
}

CRect CHPoly::GetBoundingRect(CDC* pDC,bool mode)
{
	if (m_caPoints.GetSize()==0)
	{
		m_crRect.SetRectEmpty();
		return m_crRect;
	}
	if(!mode)
	{
		CPoint pt = m_caPoints[0];
		m_crRect.SetRect(pt.x, pt.y, pt.x, pt.y);
		for (int i=1; i < m_caPoints.GetSize(); i++)
		{
			// Wenn der Punkt auﬂerhalb des Rechtecks liegt, 
			// wird das Rechteck entsprechend vergrˆﬂert.
			pt = m_caPoints[i];
			m_crRect.left     = min(m_crRect.left, pt.x);
			m_crRect.right    = max(m_crRect.right, pt.x);
			m_crRect.top      = max(m_crRect.top, pt.y);
			m_crRect.bottom   = min(m_crRect.bottom, pt.y);
		}
	}
	m_crRect.NormalizeRect();
	CRect crect(m_crRect);
	crect.InflateRect(SL_GRIPPER / 2,SL_GRIPPER / 2);
	return crect;
}

int CHPoly::PtInGripper(POINT & point)
{
	ASSERT(m_blSelected);
	
	CRect crect;
	int i;
	for (i = 0; i < m_caPoints.GetSize(); i++)
	{ 
		crect.SetRect(m_caPoints[i].x - SL_GRIPPER / 2,
			m_caPoints[i].y - SL_GRIPPER / 2,
			m_caPoints[i].x + SL_GRIPPER / 2,
			m_caPoints[i].y + SL_GRIPPER / 2);
		if(crect.PtInRect(point))
			return i * 2 + 1;
		if ((i + 1) < m_caPoints.GetSize())
		{
			CPoint cpoint(m_caPoints[i+1]);
			int a = max(cpoint.x,m_caPoints[i].x);
			int b = min(cpoint.x,m_caPoints[i].x);
			cpoint.x =  (a - b) / 2 + b; 
			a = max(cpoint.y,m_caPoints[i].y);
			b = min(cpoint.y,m_caPoints[i].y);
			cpoint.y =  (a - b) / 2 + b; 
			crect.SetRect(cpoint.x - SL_GRIPPER / 3,
				cpoint.y - SL_GRIPPER / 3, cpoint.x + 
				SL_GRIPPER / 3,cpoint.y + SL_GRIPPER / 3);
			if(crect.PtInRect(point))
			{
				return i * 2 + 2;
			}
		}
	}
	i--;
	CPoint cpoint(m_caPoints[i]);
	int a = max(cpoint.x,m_caPoints[0].x);
	int b = min(cpoint.x,m_caPoints[0].x);
	cpoint.x =  (a - b) / 2 + b; 
	a = max(cpoint.y,m_caPoints[0].y);
	b = min(cpoint.y,m_caPoints[0].y);
	cpoint.y =  (a - b) / 2 + b; 
	crect.SetRect(cpoint.x - SL_GRIPPER / 3,
		cpoint.y - SL_GRIPPER / 3, cpoint.x + 
		SL_GRIPPER / 3,cpoint.y + SL_GRIPPER / 3);
	if(crect.PtInRect(point))
		{
			return i * 2 + 2;
		}
	return 0;
}

void CHPoly::ResizeTo(POINT & to,int handle)
{
	if(handle == NULL)
	{
		m_caPoints[1] += to;
		GetBoundingRect();
		return;
	}
	else
	{
		m_caPoints[handle / 2] += to;
		GetBoundingRect();
		return;
	}
}

void CHPoly::MoveTo(POINT & to)
{
	for (int i=0; i < m_caPoints.GetSize(); i++)
	{
		m_caPoints[i] += to;
	}
	GetBoundingRect();
}

void CHPoly::InitMenu(CMenu * menu,CPoint & point)
{
	m_iHandle = PtInGripper(point);
	
	static CMenu typmenu;
	typmenu.DestroyMenu();
	typmenu.CreateMenu();
	typmenu.AppendMenu(MF_STRING | (m_iPolyStyle == PM_POLYLINE)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND1,_T("Polyline"));
	typmenu.AppendMenu(MF_STRING  | (m_iPolyStyle == PM_POLYGON)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND2,_T("Polygon"));

	static CMenu penmenu;
	penmenu.DestroyMenu();
	penmenu.CreateMenu();
	penmenu.AppendMenu(MF_STRING | (m_iPenStyle == PS_SOLID)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND3,_T("Solid"));
	penmenu.AppendMenu(MF_STRING | (m_iPenStyle == PS_DASH)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND4,_T("Dash"));
	penmenu.AppendMenu(MF_STRING | (m_iPenStyle == PS_DOT)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND5,_T("Dot"));
	penmenu.AppendMenu(MF_STRING | (m_iPenStyle == PS_DASHDOT)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND6,_T("Dashdot"));

	static CMenu widthmenu;
	widthmenu.DestroyMenu();
	widthmenu.CreateMenu();
	widthmenu.AppendMenu(MF_STRING | (m_iPenWidth == 1)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND7,_T("1"));
	widthmenu.AppendMenu(MF_STRING | (m_iPenWidth == 2)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND8,_T("2"));
	widthmenu.AppendMenu(MF_STRING | (m_iPenWidth == 3)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND9,_T("3"));
	widthmenu.AppendMenu(MF_STRING | (m_iPenWidth == 4)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND10,_T("4"));
	widthmenu.AppendMenu(MF_STRING | (m_iPenWidth == 5)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND11,_T("5"));

	menu->AppendMenu( MF_STRING,ID_CONTEXT_COMMAND12,_T("Pen color"));
	menu->AppendMenu(MF_STRING  | (m_bEmpty)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND13,_T("Empty"));
	menu->AppendMenu( MF_STRING | (m_bEmpty)
		? MF_GRAYED : MF_ENABLED,ID_CONTEXT_COMMAND14,_T("&Fillcolor"));
	menu->AppendMenu( MF_POPUP,(UINT)typmenu.m_hMenu ,_T("Typ"));
	menu->AppendMenu( MF_POPUP,(UINT)penmenu.m_hMenu ,_T("Pen style"));
	menu->AppendMenu( MF_POPUP,(UINT)widthmenu.m_hMenu ,_T("Pen width"));
	menu->AppendMenu( MF_STRING | (m_iHandle == NULL)
		? MF_GRAYED : MF_ENABLED,ID_CONTEXT_COMMAND15,_T("Delete Point"));
	menu->AppendMenu( MF_STRING,ID_CONTEXT_COMMAND16,_T("Del. All Points"));
}

void CHPoly::DoID(UINT nID)
{
	CColorDialog* cdialog;
	switch(nID)
	{
	case 1:
		m_iPolyStyle = PM_POLYLINE;
		break;
	case 2:
		m_iPolyStyle = PM_POLYGON;
		break;
	case 3:
		m_iPenStyle = PS_SOLID;
		break;
	case 4:
		m_iPenStyle = PS_DASH;
		m_iPenWidth = 1;
		break;
	case 5:
		m_iPenStyle = PS_DOT;
		m_iPenWidth = 1;
		break;
	case 6:
		m_iPenStyle = PS_DASHDOT;
		m_iPenWidth = 1;
		break;
	case 7:
		m_iPenWidth = 1;
		break;
	case 8:
		m_iPenWidth = 2;
		m_iPenStyle = PS_SOLID;
		break;
	case 9:
		m_iPenWidth = 3;
		m_iPenStyle = PS_SOLID;
		break;
	case 10:
		m_iPenWidth = 4;
		m_iPenStyle = PS_SOLID;
		break;
	case 11:
		m_iPenWidth = 5;
		m_iPenStyle = PS_SOLID;
		break;
	case 12:
		cdialog = new CColorDialog(m_crPenColor);
		if(cdialog->DoModal() == IDOK)
			m_crPenColor = cdialog->GetColor();
		delete cdialog;
		break;
	case 13:
		m_bEmpty = !m_bEmpty;
		break;
	case 14:
		cdialog = new CColorDialog(m_crFillColor);
		if(cdialog->DoModal() == IDOK)
			m_crFillColor = cdialog->GetColor();
		delete cdialog;
		break;
	case 15:
		m_caPoints.RemoveAt(m_iHandle / 2);
		break;
	case 16:
		m_caPoints.RemoveAt(1,m_caPoints.GetSize() - 2);
		break;
	default:ASSERT(false);
	}
	
}

void CHPoly::ToGrid(int grid,int handle)
{
	int h = handle / 2;
	if(h != NULL)
	{
		CPoint point(m_caPoints[h]);
		int r;
		point.x = ((r = point.x % grid) >= grid / 2) ? grid - r : -r;
		point.y = ((r = point.y % grid) >= grid / 2) ? grid - r : -r;
		m_caPoints[h] += point;
	}
	else
	{
		CPoint point;
		int r;
		for (int i=0; i < m_caPoints.GetSize(); i++)
		{
			point = m_caPoints[i];
			point.x = ((r = point.x % grid) >= grid / 2)
			? grid - r : -r;
			point.y = ((r = point.y % grid) >= grid / 2)
			? grid - r : -r;
			m_caPoints[i] += point;
		}
	}
}

LPCTSTR CHPoly::GetCursor(int handle)
{	
	if(handle && (handle % 2) == 0)
		return IDC_CROSS;
	else if(handle)
		return IDC_SIZENWSE;
	else
		return IDC_SIZEALL;
}

bool CHPoly::GetProperty(GV_ITEM* pItem)
{
	ASSERT_VALID(this);
	ASSERT(pItem);

	switch(pItem->lParam)
	{
	case 1:
		pItem->mask = GVIF_PARAM|GVIF_TYPE|GVIF_ITEM;
		pItem->type = GVT_ITEMS;
		pItem->array.Add(_T("Polygon"));
		pItem->array.Add(_T("Polyline"));
		switch(m_iPolyStyle)
		{
		case PM_POLYGON:pItem->szText = _T("Polygon");break;
		case PM_POLYLINE:pItem->szText = _T("Polyline");break;
		}
		return true;
	case 2://border color
		pItem->mask = GVIF_PARAM|GVIF_TYPE|GVIF_BKCLR;
		pItem->type = GVT_COLOR;
		pItem->crBkClr = m_crPenColor;
		pItem->szText = _T(" ");
		return true;
	case 3://inner color
		pItem->mask = GVIF_PARAM|GVIF_TYPE|GVIF_BKCLR;
		pItem->type = GVT_COLOR;
		pItem->crBkClr = m_crFillColor;
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

bool CHPoly::SetProperty(GV_ITEM* pItem)
{
	int i;
	switch(pItem->lParam)
	{
	case 1:
		if(pItem->szText == _T("Polygon"))m_iPolyStyle = PM_POLYGON;
		else if(pItem->szText == _T("Polyline"))m_iPolyStyle = PM_POLYLINE;
		return true;
	case 2:
		m_crPenColor = pItem->crBkClr;
		return true;
	case 3:
		m_crFillColor = pItem->crBkClr;
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
			pItem->szText = _T("5");
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

CString CHPoly::GetPropertyText(UINT par)
{
	switch(par)
	{
	case 1:return _T("Type");
	case 2:return _T("Pen Color");
	case 3:return _T("Fill Color");
	case 4:return _T("Pen Style");
	case 5:return _T("Pen Width");
	case 6:return _T("Empty");
	}
	return _T("");
}

void CHPoly::WriteExportFormat(CIniSection& ini)
{
	ini.SetValue(_T("RUNTIME_CLASS"),GetRuntimeClass()->m_lpszClassName);
	ini.SetValue(_T("BOUNDING_RECT"),m_crRect);
	ini.SetValue(_T("FILL_COLOR"),m_crFillColor);
	ini.SetValue(_T("PEN_COLOR"),m_crPenColor);
	ini.SetValue(_T("PEN_STYLE"), m_iPenStyle);
	ini.SetValue(_T("PEN_WIDTH"),m_iPenWidth);
	ini.SetValue(_T("POLY_STYLE"), m_iPolyStyle);
	ini.SetValue(_T("EMPTY_POLYGON"),m_bEmpty);
	CString buffer;
	for(int i = 0;i < m_caPoints.GetSize();i++)
	{
		buffer.Format(_T("POLY_POINT_%d"), i);
		ini.SetValue(buffer, m_caPoints[i]);
	}
}

void CHPoly::ReadExportFormat(CIniSection& ini)
{
	ini.GetValue(_T("BOUNDING_RECT"),m_crRect);
	ini.GetValue(_T("FILL_COLOR"),m_crFillColor);
	ini.GetValue(_T("PEN_COLOR"),m_crPenColor);
	ini.GetValue(_T("PEN_STYLE"), m_iPenStyle);
	ini.GetValue(_T("PEN_WIDTH"),m_iPenWidth);
	ini.GetValue(_T("POLY_STYLE"), m_iPolyStyle);
	ini.GetValue(_T("EMPTY_POLYGON"),m_bEmpty);
	CString buffer;
	CPoint point;
	for(int i = 0;;i++)
	{
		buffer.Format(_T("POLY_POINT_%d"), i);
		if(!ini.GetValue(buffer, point))
			break;
		m_caPoints.Add(point);
	}
}

void CHPoly::AddPoint(POINT & point)
{
	CRect crect;
	CPoint cpoint(m_caPoints[0]);
	for (int i = 1; i < m_caPoints.GetSize(); i++)
	{
		int a = max(cpoint.x,m_caPoints[i].x);
		int b = min(cpoint.x,m_caPoints[i].x);
		cpoint.x =  (a - b) / 2 + b; 
		a = max(cpoint.y,m_caPoints[i].y);
		b = min(cpoint.y,m_caPoints[i].y);
		cpoint.y =  (a - b) / 2 + b; 
		crect.SetRect(cpoint.x - SL_GRIPPER / 3,
			cpoint.y - SL_GRIPPER / 3, cpoint.x + 
			SL_GRIPPER / 3,cpoint.y + SL_GRIPPER / 3);
		
		if(crect.PtInRect(point))
		{
			m_caPoints.InsertAt(i,point);
			return;
		}
		cpoint = m_caPoints[i];
	}
	int a = max(cpoint.x,m_caPoints[0].x);
	int b = min(cpoint.x,m_caPoints[0].x);
	cpoint.x =  (a - b) / 2 + b; 
	a = max(cpoint.y,m_caPoints[0].y);
	b = min(cpoint.y,m_caPoints[0].y);
	cpoint.y =  (a - b) / 2 + b; 
	crect.SetRect(cpoint.x - SL_GRIPPER / 3,
		cpoint.y - SL_GRIPPER / 3, cpoint.x + 
		SL_GRIPPER / 3,cpoint.y + SL_GRIPPER / 3);
	if(crect.PtInRect(point))
		{
			m_caPoints.Add(point);
			return;
		}
}