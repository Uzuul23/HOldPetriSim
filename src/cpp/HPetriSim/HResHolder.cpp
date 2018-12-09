// HResHolder.cpp: Implementierung der Klasse CHResHolder.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Resource.h"
#include "HResHolder.h"

#ifdef _DEBUG
#undef THIS_FILE
static CHAR THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CHResHolder::CHResHolder() :
m_cBrushBlack(RGB(0,0,0)),
m_cBrushWhite(RGB(255,255,255)),
m_cBrushRed(RGB(255,0,0)),
m_cBrushYellow(RGB(255,255,0)),
m_cBrushGreen(RGB(0,255,0)),
m_cBrushGray(RGB(180,180,180)),
m_cPenRed(PS_SOLID,1,RGB(255,0,0)),
m_cPenBlack(PS_SOLID,1,RGB(0,0,0))
{
}

CHResHolder::~CHResHolder()
{
}

bool CHResHolder::LoadRes(HINSTANCE hInstance)
{
	//create standard font
	while(true)
	{
		LOGFONT lfFont;
		lfFont.lfHeight = -11;
		lfFont.lfWidth = 0;
		lfFont.lfEscapement = 0;
		lfFont.lfOrientation = FW_NORMAL;
		lfFont.lfWeight = false;
		lfFont.lfItalic = false;
		lfFont.lfUnderline = false;
		lfFont.lfStrikeOut = NULL;
		lfFont.lfCharSet = ANSI_CHARSET;
		lfFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
		lfFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		lfFont.lfQuality = DEFAULT_QUALITY;
		lfFont.lfPitchAndFamily = VARIABLE_PITCH | FF_DONTCARE;
		StringCchCopy(lfFont.lfFaceName, sizeof(lfFont.lfFaceName)
			, _T("Arial"));
		if(!m_cfStandart.CreateFontIndirect(&lfFont))
			break;

		//load place images
		if(!m_cImgPos[0].Create(IDB_PLACE_15, 15, 10, RGB(255,0,255) ))
			break;
		if(!m_cImgPos[1].Create(IDB_PLACE_20, 20, 10, RGB(255,0,255) ))
			break;
		if(!m_cImgPos[2].Create(IDB_PLACE_25, 25, 10, RGB(255,0,255) ))
			break;
		if(!m_cImgPos[3].Create(IDB_PLACE_35, 35, 10, RGB(255,0,255) ))
			break;
		if(!m_cImgPos[4].Create(IDB_PLACE_50, 50, 10, RGB(255,0,255) ))
			break;

		return true;
	}
	return false;
}
