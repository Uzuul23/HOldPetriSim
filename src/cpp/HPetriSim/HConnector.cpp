/**************************************************************************
	HConnector.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "StdAfx.h"
#include "Resource.h"
#include "IniSection.h"
#include "HDrawObject.h"

IMPLEMENT_SERIAL(CHConnector, CObject, DRAW_VERSION)

CHConnector::CHConnector()
{
	m_iPolyStyle = PM_POLYLINE;
	m_ctTyp = CT_NORMAL;
	m_chlLabelA = 0;
	m_chlLabelB = 0;
	m_iWeight = 1;
	m_nAnimIndex = 0;
	m_bToken = false;
}

CHConnector::CHConnector(CHPetriMember * from, CHPetriMember * to
						 , long ident)
{
	m_caPoints.Add(from->GetBoundingRect(NULL).CenterPoint());
	m_caPoints.Add(to->GetBoundingRect(NULL).CenterPoint());
	m_ptrFrom = from;
	m_ptrTo = to;
	from->GetOutList().AddTail(this);
	to->GetInList().AddTail(this);
	//erforderlich um die Konsistens des 
	//Netzes für das Speichern zu erhalten
	m_iFrom = from->GetIdent();
	m_iTo = to->GetIdent();
	//Standardwerte
	m_iPolyStyle = PM_POLYLINE;
	m_ctTyp = CT_NORMAL;
	m_chlLabelA = 0;
	m_chlLabelB = 0;
	m_iWeight = 1;
	m_iIdent = ident;
	m_bToken = false;
	m_nAnimIndex = 0;
	RecalcPoint();
}

CHConnector::~CHConnector()
{
	m_caPoints.RemoveAll(); 
}

void CHConnector::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);
	if (ar.IsStoring())
	{
		ar << (DWORD)m_crRect.left;
		ar << (DWORD)m_crRect.top;
		ar << (DWORD)m_crRect.right;
		ar << (DWORD)m_crRect.bottom;
		ar << (DWORD)m_cpArrow[1].x;
		ar << (DWORD)m_cpArrow[1].y;
		ar << (DWORD)m_cpArrow[2].x;
		ar << (DWORD)m_cpArrow[2].y;
		ar << (DWORD)m_iIdent;
		ar << (DWORD)m_iFrom;
		ar << (DWORD)m_iTo;
		ar << (DWORD)m_iWeight;
		ar << (DWORD)0;
		ar << (WORD)0;
		ar << (WORD)m_ctTyp;
		ar << (WORD)0;
		ar << (WORD)0;
		ar << (WORD)0;
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
		WORD w;
		DWORD dw;
		double db;
		ar >> dw;m_crRect.left = dw;
		ar >> dw;m_crRect.top = dw;
		ar >> dw;m_crRect.right = dw;
		ar >> dw;m_crRect.bottom = dw;
		ar >> dw;m_cpArrow[1].x = dw;
		ar >> dw;m_cpArrow[1].y = dw;
		ar >> dw;m_cpArrow[2].x = dw;
		ar >> dw;m_cpArrow[2].y = dw;
		ar >> dw;m_iIdent = dw;
		ar >> dw;m_iFrom = dw;
		ar >> dw;m_iTo = dw;
		ar >> dw;m_iWeight = dw;
		ar >> dw;
		ar >> w;
		ar >> w;m_ctTyp = (CT_TYP)w;
		ar >> w;
		ar >> w;
		ar >> w;
		ar >> b;
		ar >> b;
		ar >> b;
		ar >> b;
		ar >> b;
		ar >> db;
	}
	m_caPoints.Serialize(ar);
	m_cpArrow[0] = m_caPoints[m_caPoints.GetSize() - 1];
	m_iPolyStyle = PM_POLYLINE;
}

#ifdef _DEBUG
void CHConnector::AssertValid() const
{
	CObject::AssertValid();
}

void CHConnector::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif //_DEBUG

void CHConnector::DrawObject(CDC* pDC)
{
	ASSERT_VALID(this);
	static CBrush redbrush(RGB(255,0,0));
	static CBrush blackbrush(RGB(0,0,0));
	static CBrush greenbrush(RGB(0,255,0));
	static CPen redpen(PS_SOLID,1,RGB(255,0,0));
	static CPen dashpen(PS_DASH,1,RGB(0,0,0));
	static CPen solidpen(PS_SOLID,1,RGB(0,0,0));
	CPen* oldpen;
	CBrush* oldbrush;
	if(m_blSelected)
	{
		oldpen = pDC->SelectObject(&redpen);
		pDC->Polyline((LPPOINT)m_caPoints.GetData(),m_caPoints.GetSize());
		pDC->SelectObject(oldpen);
	}
	else
	{
		oldpen = pDC->SelectObject
			((m_ctTyp == CT_TEST) ? &dashpen : &solidpen);
		pDC->Polyline((LPPOINT)m_caPoints.GetData(),m_caPoints.GetSize());
		pDC->SelectObject(oldpen);
	}
	if(m_blSelected)
	{
		oldbrush = pDC->SelectObject(&redbrush);
		pDC->SelectObject(&redpen);
		pDC->Polyline((LPPOINT)m_caPoints.GetData(),
		m_caPoints.GetSize());
		if(m_ctTyp == CT_INHIBITOR)
		{
		pDC->Ellipse(CRect((m_caPoints[m_caPoints.GetSize() - 1].x - 3),
			(m_caPoints[m_caPoints.GetSize() - 1].y - 3),
			(m_caPoints[m_caPoints.GetSize() - 1].x + 3),
			(m_caPoints[m_caPoints.GetSize() - 1].y + 3)));
		}
		else
		{
			pDC->Polygon((LPPOINT)m_cpArrow,3);
		}
		pDC->SelectObject(oldpen);
		pDC->SelectObject(oldbrush);
		//Erweiterungsquadrate zeichnen
		CPoint cpoint(m_caPoints[0]);
		for (int i=1; i < m_caPoints.GetSize(); i++)
		{
			int a = max(cpoint.x,m_caPoints[i].x);
			int b = min(cpoint.x,m_caPoints[i].x);
			cpoint.x =  (a - b) / 2 + b; 
			a = max(cpoint.y,m_caPoints[i].y);
			b = min(cpoint.y,m_caPoints[i].y);
			cpoint.y =  (a - b) / 2 + b; 
			pDC->Rectangle(CRect(cpoint.x - SL_GRIPPER / 3,
				cpoint.y - SL_GRIPPER / 3,
				cpoint.x + SL_GRIPPER / 3,
				cpoint.y + SL_GRIPPER / 3));
				cpoint = m_caPoints[i];
		}
		for (int i=1; i < m_caPoints.GetSize() - 1; i++)
		{
			pDC->Rectangle(CRect(m_caPoints[i].x - SL_GRIPPER / 2,
				m_caPoints[i].y - SL_GRIPPER / 2,
				m_caPoints[i].x + SL_GRIPPER / 2,
				m_caPoints[i].y + SL_GRIPPER / 2));
		}
	}
	else
	{
		oldbrush = pDC->SelectObject(&blackbrush);
		if(m_ctTyp == CT_INHIBITOR)
		{
		pDC->Ellipse(CRect((m_caPoints[m_caPoints.GetSize() - 1].x - 3),
			(m_caPoints[m_caPoints.GetSize() - 1].y - 3),
			(m_caPoints[m_caPoints.GetSize() - 1].x + 3),
			(m_caPoints[m_caPoints.GetSize() - 1].y + 3)));
		}
		else
		{
			pDC->Polygon((LPPOINT)m_cpArrow,3);
		}
		
	}
	if(m_bToken) //animated
	{
		pDC->SelectObject(&greenbrush);
		pDC->Ellipse(m_cpToken.x - 5, m_cpToken.y - 5, m_cpToken.x + 5,
			m_cpToken.y + 5);
	}
	pDC->SelectObject(oldbrush);
}

int CHConnector::PtInGripper(POINT & point)
{
	ASSERT_VALID(this);
	ASSERT(m_blSelected);//Falscher Aufruf der Funktion!
	CRect crect;
	int i;
	for (i = 0; i < m_caPoints.GetSize() - 1; i++)
	{ 
		if(i > 0)
		{
			crect.SetRect(m_caPoints[i].x - SL_GRIPPER,
				m_caPoints[i].y - SL_GRIPPER,
				m_caPoints[i].x + SL_GRIPPER,
				m_caPoints[i].y + SL_GRIPPER);
			if(crect.PtInRect(point))
				return i * 2 + 1;
		}
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

void CHConnector::RecalcPoint()
{
	ASSERT_VALID(this);
	// todo: optimize
	//sind Beide Figuren selektiert müssen die Punkte nicht neu
	//berechnet werden 
	if(m_ptrFrom->IfSelected() && m_ptrTo->IfSelected()
		&& m_blSelected)return;
	//"von" Punkt berechnen
	double ax = (double)m_ptrFrom->GetRect().CenterPoint().x;
	double ay = (double)m_ptrFrom->GetRect().CenterPoint().y;
	double bx;
	double by;
	double a = (double)m_ptrFrom->GetRect().Width() / 2;
	double b = (double)m_ptrFrom->GetRect().Height() / 2;
	double gx,gy;
	double alpha,beta;
	double m,ex,ey;
	if(m_caPoints.GetSize() == 2)
	{
		//Kante besteht aus 2 Punkten
		bx = (double)m_ptrTo->GetRect().CenterPoint().x;
		by = (double)m_ptrTo->GetRect().CenterPoint().y;
	}
	else
	{
		//Kante hat mehr als 2 Punkte
		bx = (double)m_caPoints[1].x;
		by =  (double)m_caPoints[1].y;
	}
	gx = bx - ax;
	gy = by - ay;
	if((gx == 0) || (gy == 0))//Objekte liegen im rechten Winkel
	{
		m_caPoints[0] = m_ptrFrom->GetRect().CenterPoint();
		if((gx == 0) && (gy > 0))
		{
			//Mitte Unten
			m_caPoints[0].x = (int)ax;
			m_caPoints[0].y = m_ptrFrom->GetRect().bottom;
		}
		if((gx == 0) && (gy < 0))
		{
			//Mitte Oben
			m_caPoints[0].x = (int)ax;
			m_caPoints[0].y = m_ptrFrom->GetRect().top;
		}
		if((gx > 0) && (gy == 0))
		{
			//Mitte Rechts
			m_caPoints[0].x = m_ptrFrom->GetRect().right;
			m_caPoints[0].y = (int)ay;
		}
		if((gx < 0) && (gy == 0))
		{
			//Mitte Links
			m_caPoints[0].x = m_ptrFrom->GetRect().left;
			m_caPoints[0].y = (int)ay;
		}
	}
	else
	{
		if(m_ptrFrom->IsKindOf( RUNTIME_CLASS(CHPosition)))
		{
			m = gy / gx;
			ex=a*b/(b*b+a*a*m*m)*sqrt(a*a*m*m+b*b);
			ey=b/a*sqrt(a*a-ex*ex);
			if(gy <0)
				ey= -ey;
			if(gx < 0)
				ex= -ex;
			m_caPoints[0].x = (int)(ex+ax);
			m_caPoints[0].y = (int)(ey+ay);
		}
		if(m_ptrFrom->IsKindOf( RUNTIME_CLASS(CHTransition)))
		{
			m = sqrt(gx * gx + gy * gy);
			alpha = 3.14 / 2.0 - atan(b / a);
			beta = acos(((gy / m ) < 0) ? -(gy / m) : (gy / m));
			if(beta < alpha)
			{
				ex=b*tan(beta);
				if(gy < 0)
					m_caPoints[0].y = m_ptrFrom->GetRect().top;
				else
					m_caPoints[0].y = m_ptrFrom->GetRect().bottom;
				if(gx > 0)
					m_caPoints[0].x = 
					int(a + m_ptrFrom->GetRect().left + ex);
				else
					m_caPoints[0].x = 
					int(a + m_ptrFrom->GetRect().left - ex);
			}
			else
			{
				ex=a / tan(beta);
				if(gx < 0)
					m_caPoints[0].x = m_ptrFrom->GetRect().left;
				else
					m_caPoints[0].x = m_ptrFrom->GetRect().right;
				if(gy > 0)
					m_caPoints[0].y = 
					int(b + m_ptrFrom->GetRect().top + ex);
				else
					m_caPoints[0].y = 
					int(b + m_ptrFrom->GetRect().top - ex);
			}
		}
	}
	// "zum" Punkt berechnen
	bx = (double)m_ptrTo->GetRect().CenterPoint().x;
	by = (double)m_ptrTo->GetRect().CenterPoint().y;
			
	a = (double)m_ptrTo->GetRect().Width() / 2;
	b = (double)m_ptrTo->GetRect().Height() / 2;
	if(m_caPoints.GetSize() == 2)
	{
		ax = (double)m_ptrFrom->GetRect().CenterPoint().x;
		ay = (double)m_ptrFrom->GetRect().CenterPoint().y;
	}
	else
	{
		ax = (double)m_caPoints[m_caPoints.GetSize() - 2].x;
		ay =  (double)m_caPoints[m_caPoints.GetSize() - 2].y;
	}
	gx = ax - bx;
	gy = ay - by;
	if((gx == 0) || (gy == 0))//Objekte liegen im rechten Winkel
	{
		//Katheten des Pfeils berechnen, einmal reicht
		const int arrowx = (int)(sin(ARROW_ARC) * ARROW_LENGHT);
		const int arrowy = (int)(cos(ARROW_ARC) * ARROW_LENGHT);
		if((gx == 0) && (gy == 0))
		{
			m_caPoints[m_caPoints.GetSize() - 1]
			= m_ptrTo->GetRect().CenterPoint();
			//Zielpfeil
			for(int i = 0;i < 3;i++)
				m_cpArrow[i] = m_ptrFrom->GetRect().bottom;
		}
		if((gx == 0) && (gy > 0))
		{
			//MU
			m_caPoints[m_caPoints.GetSize() - 1].x = (int)ax;
			m_caPoints[m_caPoints.GetSize() - 1].y
				= m_ptrTo->GetRect().bottom;
			//Zielpfeil
			m_cpArrow[1].x
				= m_caPoints[m_caPoints.GetSize() - 1].x + arrowx;
			m_cpArrow[1].y
				= m_caPoints[m_caPoints.GetSize() - 1].y + arrowy;
			m_cpArrow[2].x
				= m_caPoints[m_caPoints.GetSize() - 1].x - arrowx;
			m_cpArrow[2].y
				= m_caPoints[m_caPoints.GetSize() - 1].y + arrowy;
		}
		if((gx == 0) && (gy < 0))
		{
			//MO
			m_caPoints[m_caPoints.GetSize() - 1].x = (int)ax;
			m_caPoints[m_caPoints.GetSize() - 1].y
				= m_ptrTo->GetRect().top;
			//Zielpfeil
			m_cpArrow[1].x
				= m_caPoints[m_caPoints.GetSize() - 1].x + arrowx;
			m_cpArrow[1].y
				= m_caPoints[m_caPoints.GetSize() - 1].y - arrowy;
			m_cpArrow[2].x
				= m_caPoints[m_caPoints.GetSize() - 1].x - arrowx;
			m_cpArrow[2].y
				= m_caPoints[m_caPoints.GetSize() - 1].y - arrowy;
		}
		if((gx > 0) && (gy == 0))
		{
			//MR
			m_caPoints[m_caPoints.GetSize() - 1].x
				= m_ptrTo->GetRect().right;
			m_caPoints[m_caPoints.GetSize() - 1].y = (int)ay;
			//Zielpfeil
			m_cpArrow[1].x
				= m_caPoints[m_caPoints.GetSize() - 1].x + arrowy;
			m_cpArrow[1].y
				= m_caPoints[m_caPoints.GetSize() - 1].y + arrowx;
			m_cpArrow[2].x
				= m_caPoints[m_caPoints.GetSize() - 1].x + arrowy;
			m_cpArrow[2].y
				= m_caPoints[m_caPoints.GetSize() - 1].y - arrowx;
		}
		if((gx < 0) && (gy == 0))
		{
			m_caPoints[m_caPoints.GetSize() - 1].x
				= m_ptrTo->GetRect().left;
			m_caPoints[m_caPoints.GetSize() - 1].y = (int)ay;
			//Zielpfeil
			m_cpArrow[1].x
				= m_caPoints[m_caPoints.GetSize() - 1].x - arrowy;
			m_cpArrow[1].y
				= m_caPoints[m_caPoints.GetSize() - 1].y - arrowx;
			m_cpArrow[2].x
				= m_caPoints[m_caPoints.GetSize() - 1].x - arrowy;
			m_cpArrow[2].y
				= m_caPoints[m_caPoints.GetSize() - 1].y + arrowx;
		}
	}
	else
	{
		if(m_ptrTo->IsKindOf( RUNTIME_CLASS(CHPosition)))
		{
			//Punkt auf Kreis errechnen
			ASSERT(gx != 0);
			m = gy / gx;
			ex = a * b / (b*b+a*a*m*m) * sqrt(a*a*m*m+b*b);
			ey = b / a * sqrt(a*a-ex*ex);
			if(gy <0)
				ey= - ey;
			if(gx < 0)
				ex= - ex;
			m_caPoints[m_caPoints.GetSize() - 1].x = (int)(ex+bx);
			m_caPoints[m_caPoints.GetSize() - 1].y = (int)(ey+by);
		}
		if(m_ptrTo->IsKindOf( RUNTIME_CLASS(CHTransition)))
		{
			//Punkt auf Rechteck errechnen
			m = sqrt(gx * gx + gy * gy);
			alpha = 3.14 / 2.0 - atan(b / a);
			beta = acos(((gy / m ) < 0) ? -(gy / m) : (gy / m));
			if(beta < alpha)
			{
				ex=b*tan(beta);
				if(gy < 0)
					m_caPoints[m_caPoints.GetSize() - 1].y
					= m_ptrTo->GetRect().top;
				else
					m_caPoints[m_caPoints.GetSize() - 1].y
					= m_ptrTo->GetRect().bottom;
				if(gx > 0)
					m_caPoints[m_caPoints.GetSize() - 1].x = 
					int(a + m_ptrTo->GetRect().left + ex);
				else
					m_caPoints[m_caPoints.GetSize() - 1].x = 
					int(a + m_ptrTo->GetRect().left - ex);
			}
			else
			{
				ex=a / tan(beta);
				if(gx < 0)
					m_caPoints[m_caPoints.GetSize() - 1].x
					= m_ptrTo->GetRect().left;
				else
					m_caPoints[m_caPoints.GetSize() - 1].x
					= m_ptrTo->GetRect().right;
				if(gy > 0)
					m_caPoints[m_caPoints.GetSize() - 1].y = 
					int(b + m_ptrTo->GetRect().top + ex);
				else
					m_caPoints[m_caPoints.GetSize() - 1].y = 
					int(b + m_ptrTo->GetRect().top - ex);
			}
		}
		//Punkte des Zeilpfeils errechnen
		double dxx,dxy,dyx,dyy;
		alpha = atan(((gy / gx ) < 0) ? -(gy / gx) : (gy / gx));
		dxy = cos(alpha - ARROW_ARC*2) * ARROW_LENGHT;
		dxx = sin(alpha - ARROW_ARC*2) * ARROW_LENGHT;
		dyx = sin(alpha + ARROW_ARC*2) * ARROW_LENGHT;
		dyy = cos(alpha + ARROW_ARC*2) * ARROW_LENGHT;
		//TRACE(_T("alpha: %f dxx: %f dxy: %f dyx: %f dyy: %f\n",
		//	alpha,dxx,dxy,dyx,dyy);//Diagnose
		if(gy > 0)
		{
			if(gx > 0)
			{
				m_cpArrow[1].x
				= m_caPoints[m_caPoints.GetSize() - 1].x - (int)dxx;
				m_cpArrow[1].y
				= m_caPoints[m_caPoints.GetSize() - 1].y + (int)dxy;
				m_cpArrow[2].x
				= m_caPoints[m_caPoints.GetSize() - 1].x + (int)dyx;
				m_cpArrow[2].y
				= m_caPoints[m_caPoints.GetSize() - 1].y - (int)dyy;
			}
			else
			{
				m_cpArrow[1].x
				= m_caPoints[m_caPoints.GetSize() - 1].x + (int)dxx;
				m_cpArrow[1].y
				= m_caPoints[m_caPoints.GetSize() - 1].y + (int)dxy;
				m_cpArrow[2].x
				= m_caPoints[m_caPoints.GetSize() - 1].x - (int)dyx;
				m_cpArrow[2].y
				= m_caPoints[m_caPoints.GetSize() - 1].y - (int)dyy;
			}
		}
		else
		{
			if(gx > 0)
			{
				m_cpArrow[1].x
				= m_caPoints[m_caPoints.GetSize() - 1].x - (int)dxx;
				m_cpArrow[1].y
				= m_caPoints[m_caPoints.GetSize() - 1].y - (int)dxy;
				m_cpArrow[2].x
				= m_caPoints[m_caPoints.GetSize() - 1].x + (int)dyx;
				m_cpArrow[2].y
				= m_caPoints[m_caPoints.GetSize() - 1].y + (int)dyy;
			}
			else
			{
				m_cpArrow[1].x
				= m_caPoints[m_caPoints.GetSize() - 1].x + (int)dxx;
				m_cpArrow[1].y
				= m_caPoints[m_caPoints.GetSize() - 1].y - (int)dxy;
				m_cpArrow[2].x
				= m_caPoints[m_caPoints.GetSize() - 1].x - (int)dyx;
				m_cpArrow[2].y
				= m_caPoints[m_caPoints.GetSize() - 1].y + (int)dyy;
			}
		}
	}
	m_cpArrow[0] = m_caPoints[m_caPoints.GetSize() - 1];
	RecalcLabelPos();
}

CRect CHConnector::GetBoundingRect(CDC* pDC,bool mode)
{
	CRect crect(CHPoly::GetBoundingRect(pDC, mode));

	if(m_chlLabelA && m_chlLabelA->GetVisible())
		crect.UnionRect(m_chlLabelA->GetBoundingRect(pDC, mode), crect);
	crect.NormalizeRect();
	if(m_bToken) //animated
	{
		crect.InflateRect(10, 10);
		crect.UnionRect(crect, m_ptrFrom->GetBoundingRect(0, true));
		crect.UnionRect(crect, m_ptrTo->GetBoundingRect(0, true));
		crect.NormalizeRect();
	}
	return crect;
}

void CHConnector::MoveTo(POINT & to)
{
	ASSERT_VALID(this);
	ASSERT(m_blSelected);
	//Kante nur verschieben wenn die Objekte an beiden
	//Enden auch Verschoben werden
	if(m_ptrFrom->IfSelected() && m_ptrTo->IfSelected())
	{
		for (int i=0; i < m_caPoints.GetSize(); i++)
		{
			m_caPoints[i] += to;
		}
	m_cpArrow[0] += to;
	m_cpArrow[1] += to;
	m_cpArrow[2] += to;
	}
}

void CHConnector::InitMenu(CMenu* menu,CPoint & point)
{
	m_iHandle = PtInGripper(point);

	static CMenu typmenu;
	typmenu.DestroyMenu();
	typmenu.CreateMenu();
	typmenu.AppendMenu(MF_STRING | (m_ctTyp == CT_NORMAL)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND1, _T( "Normal"));
	typmenu.AppendMenu(MF_STRING  | (m_ctTyp == CT_INHIBITOR)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND2, _T("Inhibitor"));
	typmenu.AppendMenu(MF_STRING  | (m_ctTyp == CT_TEST)
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND3, _T("Test Arc"));
	static CMenu labelmenu;
	labelmenu.DestroyMenu();
	labelmenu.CreateMenu();
	if(m_chlLabelA)
	labelmenu.AppendMenu(MF_STRING | (GetLabelA()->GetVisible())
		? MF_CHECKED : MF_UNCHECKED,ID_CONTEXT_COMMAND4, _T( "Weight"));
	menu->AppendMenu( MF_POPUP,(UINT)labelmenu.m_hMenu , _T("Show Label"));
	menu->AppendMenu( MF_POPUP,(UINT)typmenu.m_hMenu , _T("Typ"));
	menu->AppendMenu( MF_STRING | (m_iHandle == NULL)
		? MF_GRAYED : MF_ENABLED,ID_CONTEXT_COMMAND5, _T("Delete Point"));
	menu->AppendMenu( MF_STRING,ID_CONTEXT_COMMAND6, _T("Del. All Points"));
}

void CHConnector::DoID(UINT nID)
{
	switch(nID)
	{
	case 1:
		m_ctTyp = CT_NORMAL;
		break;
	case 2:
		m_ctTyp = CT_INHIBITOR;
		break;
	case 3:
		m_ctTyp = CT_TEST;
		break;
	case 4:
		if(GetLabelA())
			GetLabelA()->SetVisible(!GetLabelA()->GetVisible());
		break;
	case 5:
		m_caPoints.RemoveAt(m_iHandle / 2);
		RecalcPoint();
		break;
	case 6:
		m_caPoints.RemoveAt(1,m_caPoints.GetSize() - 2);
		RecalcPoint();
		break;
	default:ASSERT(false);
	}
}

bool CHConnector::GetProperty(GV_ITEM* pItem)
{
	ASSERT_VALID(this);
	ASSERT(pItem);
	switch(pItem->lParam)
	{
	case 1://Weight
		pItem->mask = GVIF_PARAM|GVIF_TYPE|GVIF_ITEM;
		pItem->type = GVT_DEFAULT;
		pItem->szText.Format(_T("%d") , m_iWeight);
		return true;
	case 2://Type
		pItem->mask = GVIF_PARAM|GVIF_TYPE|GVIF_ITEM;
		pItem->type = GVT_ITEMS;
		pItem->array.Add(_T("Normal"));
		pItem->array.Add(_T("Inhibitor"));
		pItem->array.Add(_T("Test"));
		switch(m_ctTyp)
		{
		case CT_NORMAL:pItem->szText = _T("Normal");break;
		case CT_INHIBITOR:pItem->szText = _T("Inhibitor");break;
		case CT_TEST:pItem->szText = _T("Test");break;
		}
		return true;
	case 3://Show Label Weight
		pItem->mask = GVIF_PARAM|GVIF_TYPE;
		pItem->type = GVT_BOOL;
		if(GetLabelA()->GetVisible())pItem->szText = _T("TRUE");
		else pItem->szText = _T("FALSE");
		return true;
	default:return false;
	}
	return false;

}

bool CHConnector::SetProperty(GV_ITEM* pItem)
{
	int i;
	switch(pItem->lParam)
	{
	case 1:
		i = _tstoi(pItem->szText);
		if(i < 1)
		{
			pItem->mask = GVIF_TEXT;
			pItem->szText = _T("1");
			m_iWeight = 1;
			if(m_chlLabelA)
				m_chlLabelA->SetText(pItem->szText);
			return false;
		}
		else if(i > MAX_TOKENS)
		{
			pItem->mask = GVIF_TEXT;
			pItem->szText.Format(_T("%d"), MAX_TOKENS);
			m_iWeight = MAX_TOKENS;
			if(m_chlLabelA)
				m_chlLabelA->SetText(pItem->szText);
			return false;
		}
		else
		{
			m_iWeight = i;
			if(m_chlLabelA)
				m_chlLabelA->SetText(pItem->szText);
			return true;
		}
		return true;
		return true;
	case 2:
		if(pItem->szText == _T("Normal"))DoID(1);
		else if(pItem->szText == _T("Inhibitor"))DoID(2);
		else if(pItem->szText == _T("Test"))DoID(3);
		return true;
	case 3:
		GetLabelA()->SetVisible((pItem->szText == _T("TRUE"))? true : false);
		return true;
	default: return false;
	}
	return true;
}

CString CHConnector::GetPropertyText(UINT par)
{
	switch(par)
	{
	case 1:return _T("Weight");
	case 2:return _T("Type");
	case 3:return _T("Show Weight");
	}
	return _T("");
}

void CHConnector::RecalcLabelPos()
{
	if(GetLabelA())
	{
		ASSERT(m_caPoints.GetSize() >= 2);
		int i = m_caPoints.GetSize() - 1;
		CPoint cpoint(m_caPoints[i]);
		int a = max(cpoint.x,m_caPoints[i - 1].x);
		int b = min(cpoint.x,m_caPoints[i - 1].x);
		cpoint.x =  (a - b) / 2 + b; 
		a = max(cpoint.y,m_caPoints[i - 1].y);
		b = min(cpoint.y,m_caPoints[i - 1].y);
		cpoint.y =  (a - b) / 2 + b; 
		int h = GetLabelA()->GetRect().Height();
		int w = GetLabelA()->GetRect().Width();
		/*
		CRect crect((cpoint.x - LABEL_OFFSET_X),
			(cpoint.y - h - LABEL_OFFSET_Y),
			(cpoint.x - LABEL_OFFSET_X + w),
			(cpoint.y - LABEL_OFFSET_Y));
		GetLabelA()->SetRect(crect);
		*/
		CRect crect((cpoint.x),
			(cpoint.y - h),
			(cpoint.x + w),
			(cpoint.y));
		GetLabelA()->SetRect(crect);
	}
}

void CHConnector::SetIdent(long ident)
{
	m_iIdent = ident;
	if(m_chlLabelA)
		m_chlLabelA->SetOwner(ident);
	if(m_chlLabelB)
		m_chlLabelB->SetOwner(ident);
}

void CHConnector::ResizeTo(POINT & to,int handle)
{	
	ASSERT_VALID(this);
	ASSERT(m_blSelected);
	m_caPoints[handle / 2] += to;
	RecalcPoint();
	return;
}

void CHConnector::ToGrid(int grid,int handle)
{
	if(handle)CHPoly::ToGrid(grid,handle);
}

void CHConnector::WriteExportFormat(CIniSection& ini)
{
	ini.SetValue(_T("RUNTIME_CLASS"), GetRuntimeClass()->m_lpszClassName);
	ini.SetValue(_T("BOUNDING_RECT"),m_crRect);
	ini.SetValue(_T("ARC_ID"),m_iIdent);
	ini.SetValue(_T("TARGET_ID"),m_iTo);
	ini.SetValue(_T("SOURCE_ID"), m_iFrom);
	ini.SetValue(_T("ARC_WEIGHT"), m_iWeight);
	ini.SetValue(_T("ARC_TYP"), m_ctTyp);
	ini.SetValue(_T("ARC_ARROW_1"), m_cpArrow[1]);
	ini.SetValue(_T("ARC_ARROW_2"), m_cpArrow[2]);
	CString buffer;
	for(int i = 0;i < m_caPoints.GetSize();i++)
	{
		buffer.Format(_T("POLY_POINT_%d"), i);
		ini.SetValue(buffer, m_caPoints[i]);
	}
}

void CHConnector::ReadExportFormat(CIniSection& ini)
{
	ini.GetValue(_T("BOUNDING_RECT"), m_crRect);
	ini.GetValue(_T("ARC_ID"),m_iIdent);
	ini.GetValue(_T("TARGET_ID"),m_iTo);
	ini.GetValue(_T("SOURCE_ID"), m_iFrom);
	ini.GetValue(_T("ARC_WEIGHT"), m_iWeight);
	int typ;
	ini.GetValue(_T("ARC_TYP"), typ);
	m_ctTyp = (CT_TYP)typ;
	ini.GetValue(_T("ARC_ARROW_1"), m_cpArrow[1]);
	ini.GetValue(_T("ARC_ARROW_2"), m_cpArrow[2]);
	CString buffer;
	CPoint point;
	for(int i = 0;;i++)
	{
		buffer.Format(_T("POLY_POINT_%d"), i);
		if(!ini.GetValue(buffer, point))
			break;
		m_caPoints.Add(point);
	}
	m_cpArrow[0] = m_caPoints[m_caPoints.GetSize() - 1];
}

void CHConnector::StartTokenAnim(int nSteps)
{
	//Startpunkt
	ASSERT(nSteps);
	m_cpToken = m_caPoints[0];
	m_nAnimIndex = 0;
	m_bToken = true;
	m_doAnimPieces = 0;
	//gesamtlänge der geraden errechnen
	m_doAnimLenght = 0;
	for(int i = 1;i < m_caPoints.GetSize();i++)
	{
		int a = m_caPoints[i].x -  m_caPoints[i - 1].x;
		int b = m_caPoints[i].y -  m_caPoints[i - 1].y;
		double c = (a*a + b*b);
		c = sqrt(c);
		m_doAnimLenght += c;
	}
	//Schritte pro Teilstück
	int a = m_caPoints[1].x -  m_caPoints[0].x;
	int b = m_caPoints[1].y -  m_caPoints[0].y;
	double c = (a*a + b*b);
	m_doAnimLenghtLine = sqrt(c);
}

bool CHConnector::StepTokenAnim(int nStep, bool bStop)
{
	ASSERT_VALID(this);

	//Punkt verschieben
	m_doAnimPieces += m_doAnimLenght / nStep;
	double l = (m_doAnimPieces > m_doAnimLenghtLine)? m_doAnimLenghtLine : m_doAnimPieces;
	int ax = m_caPoints[m_nAnimIndex].x;
	int ay = m_caPoints[m_nAnimIndex].y;
	int bx = m_caPoints[m_nAnimIndex + 1].x;
	int by = m_caPoints[m_nAnimIndex + 1].y;
	double d = sqrt( l * l / (((bx - ax) * (bx - ax)) + ((by-ay)*(by - ay))));
	m_cpToken.x = (int)(ax + 1 * d * (bx - ax));
	m_cpToken.y = (int)(ay + 1 * d * (by - ay));
	
	if(m_doAnimPieces >= m_doAnimLenghtLine
	&& m_nAnimIndex < (m_caPoints.GetSize()-2))
	{
		m_nAnimIndex++;
		m_doAnimPieces = m_doAnimPieces - m_doAnimLenghtLine;
		//Schritte pro Teilstück
		int a = m_caPoints[m_nAnimIndex+1].x -  m_caPoints[m_nAnimIndex].x;
		int b = m_caPoints[m_nAnimIndex+1].y -  m_caPoints[m_nAnimIndex].y;
		double c = (a*a + b*b);
		m_doAnimLenghtLine = sqrt(c);	
	}
	m_bToken = bStop;
	return true;
}

LPCTSTR CHConnector::GetCursor(int handle)
{
	if(handle && (handle % 2) == 0)
		return IDC_CROSS;
	else if(handle)
		return IDC_SIZENWSE;
	else
		return IDC_ARROW;
}