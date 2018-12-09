/**************************************************************************
	HText.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "stdafx.h"
#include "Resource.h"
#include "HDrawObject.h"
#include "IniSection.h"
#include "HTextDialog.h"
#include "PetriSim.h"

IMPLEMENT_SERIAL(CHText,CObject,DRAW_VERSION)
CHText::CHText()
{
}

CHText::CHText(CPoint& topleft,LOGFONT* stfont)
{
	m_csText = _T("Right Mouse Button to Edit");
	m_crRect.SetRect(topleft.x,topleft.y,topleft.x + 1,topleft.y + 1);

	memcpy(&m_lfFont,stfont,sizeof(m_lfFont));
	m_crColor = RGB(0,0,0);
}

CHText::~CHText()
{
}

void CHText::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << (DWORD)m_crRect.top;
		ar << (DWORD)m_crRect.left;
		ar << (DWORD)m_crRect.bottom;
		ar << (DWORD)m_crRect.right;
		ar << (DWORD)m_crColor;
		ar << (DWORD)m_lfFont.lfHeight;
		ar << (DWORD)m_lfFont.lfWidth;
		ar << (DWORD)m_lfFont.lfEscapement;
		ar << (DWORD)m_lfFont.lfOrientation;
		ar << (DWORD)m_lfFont.lfWeight;
		ar << (BYTE)m_lfFont.lfItalic;
		ar << (BYTE)m_lfFont.lfUnderline;
		ar << (BYTE)m_lfFont.lfStrikeOut;
		ar << (BYTE)m_lfFont.lfCharSet;
		ar << (BYTE)m_lfFont.lfOutPrecision;
		ar << (BYTE)m_lfFont.lfClipPrecision;
		ar << (BYTE)m_lfFont.lfQuality;
		ar << (BYTE)m_lfFont.lfPitchAndFamily;
		for(int i = 0;i < 32;i++)
			ar << (TCHAR)m_lfFont.lfFaceName[i]; 
		ar << m_csText;
	}
	else
	{
		DWORD dw;
		BYTE b;
		TCHAR c;
		ar >> dw;m_crRect.top = dw;
		ar >> dw;m_crRect.left = dw;
		ar >> dw;m_crRect.bottom = dw;
		ar >> dw;m_crRect.right = dw;
		ar >> dw;m_crColor = dw;
		ar >> dw;m_lfFont.lfHeight = dw;
		ar >> dw;m_lfFont.lfWidth = dw;
		ar >> dw;m_lfFont.lfEscapement = dw;
		ar >> dw;m_lfFont.lfOrientation = dw;
		ar >> dw;m_lfFont.lfWeight = dw;
		ar >> b;m_lfFont.lfItalic = b;
		ar >> b;m_lfFont.lfUnderline = b;
		ar >> b;m_lfFont.lfStrikeOut = b;
		ar >> b;m_lfFont.lfCharSet = b;
		ar >> b;m_lfFont.lfOutPrecision = b;
		ar >> b;m_lfFont.lfClipPrecision = b;
		ar >> b;m_lfFont.lfQuality = b;
		ar >> b;m_lfFont.lfPitchAndFamily = b;
		for(int i = 0;i < 32;i++)
		{
			ar >> c;
			m_lfFont.lfFaceName[i] = c;
		}
		ar >> m_csText;
	}	
	
}

#ifdef _DEBUG
void CHText::AssertValid() const
{
	CObject::AssertValid();
}

void CHText::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif //_DEBUG

void CHText::DrawObject(CDC * pDC)
{
	CFont cfont;
	cfont.CreateFontIndirect(&m_lfFont);
	CPetriSimApp* pApp = (CPetriSimApp*)AfxGetApp();
	CHResHolder& rRes = pApp->m_cResHolder;
	CBrush* oldbrush;
	CFont* oldFont;
	COLORREF oldcolor = pDC->SetTextColor(m_crColor);
	oldFont = pDC->SelectObject(&cfont);
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(m_csText,-1,m_crRect,DT_CENTER | DT_CALCRECT);
	pDC->DrawText(m_csText, -1,m_crRect,DT_CENTER);
	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldcolor);
	if(m_blSelected)
	{
		oldbrush = pDC->SelectObject(&rRes.m_cBrushYellow);
		for(int i = 1;i<5;i++)
		{
			pDC->Rectangle(GetGripper(i));
		}
		pDC->SelectObject(oldbrush);		
	}
}

CRect CHText::GetBoundingRect(CDC* pDC,bool mode)
{
	if(mode)return m_crRect;
	if(pDC != NULL)
	{
		CFont cfont;
		cfont.CreateFontIndirect(&m_lfFont);
		CFont* oldFont;
		COLORREF oldcolor = pDC->SetTextColor(m_crColor);
		oldFont = pDC->SelectObject(&cfont);
		pDC->DrawText(m_csText,-1,m_crRect,DT_CENTER|DT_CALCRECT);
		pDC->SelectObject(oldFont);
	}
	CRect crect(m_crRect);
	crect.InflateRect(SL_GRIPPER,SL_GRIPPER);
	return crect;
}

void CHText::InitMenu(CMenu * menu,CPoint & point)
{
	menu->AppendMenu(MF_STRING,ID_CONTEXT_COMMAND1,_T("Color"));
	menu->AppendMenu(MF_STRING,ID_CONTEXT_COMMAND2,_T("Font"));
	menu->AppendMenu(MF_STRING,ID_CONTEXT_COMMAND3,_T("Text"));
}

void CHText::DoID(UINT nID)
{
	CColorDialog* cdialog;
	CFontDialog* fdialog;
	CHTextDialog* tdialog;
	switch(nID)
	{
	case 1:
		cdialog = new CColorDialog(m_crColor);
		if(cdialog->DoModal() == IDOK)
			m_crColor = cdialog->GetColor();
		delete cdialog;
		break;
	case 2:
		fdialog = new CFontDialog(&m_lfFont);
		if(fdialog->DoModal() == IDOK)
			memcpy(&m_lfFont,fdialog->m_cf.lpLogFont,sizeof(m_lfFont));
		delete fdialog;
		break;
	case 3:
		tdialog = new CHTextDialog(m_csText);
		if(tdialog->DoModal() == IDOK)
			m_csText = tdialog->GetText();
		delete tdialog;
		break;
	default:ASSERT(false);
	}
	
}

bool CHText::GetProperty(GV_ITEM* pItem)
{
	ASSERT_VALID(this);
	ASSERT(pItem);
	switch(pItem->lParam)
	{
	case 1:
		pItem->mask = GVIF_PARAM|GVIF_TYPE|GVIF_FONT;
		pItem->type = GVT_FONT;
		memcpy(&pItem->lfFont, &m_lfFont, sizeof(m_lfFont));
		pItem->szText = m_lfFont.lfFaceName;
		return true;
	case 2://border color
		pItem->mask = GVIF_PARAM|GVIF_TYPE|GVIF_BKCLR;
		pItem->type = GVT_COLOR;
		pItem->crBkClr = m_crColor;
		pItem->szText = _T("");
		return true;
	default:return false;
	}
	return false;
}

bool CHText::SetProperty(GV_ITEM* pItem)
{
	ASSERT_VALID(this);
	ASSERT(pItem);

	switch(pItem->lParam)
	{
	case 1:
		memcpy(&m_lfFont, &pItem->lfFont, sizeof(m_lfFont));
	case 2:
		m_crColor = pItem->crBkClr;
		return true;
	default: return false;
	}
	return false;
}

CString CHText::GetPropertyText(UINT par)
{
	ASSERT_VALID(this);
	switch(par)
	{
	case 1:return _T("Font");
	case 2:return _T("Color");
	}
	return _T("");
}

void CHText::WriteExportFormat(CIniSection& ini)
{
	ini.SetValue(_T("RUNTIME_CLASS"), GetRuntimeClass()->m_lpszClassName);
	ini.SetValue(_T("BOUNDING_RECT"), m_crRect);
	ini.SetValue(_T("TEXT"), m_csText);
	ini.SetValue(_T("TEXT_COLOR"), m_crColor);
	ini.SetValue(_T("TEXT_FONT"), m_lfFont);
}

void CHText::ReadExportFormat(CIniSection& ini)
{
	ini.GetValue(_T("BOUNDING_RECT"), m_crRect);
	ini.GetValue(_T("TEXT"), m_csText);
	ini.GetValue(_T("TEXT_COLOR"), m_crColor);
	ini.GetValue(_T("TEXT_FONT"), m_lfFont);
}
