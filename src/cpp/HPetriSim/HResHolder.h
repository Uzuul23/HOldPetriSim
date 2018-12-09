/**************************************************************************
	HResHolder.h
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#if !defined(AFX_HRESHOLDER_H__0AF20C77_9BD6_405A_B85C_97F4370E9490__INCLUDED_)
#define AFX_HRESHOLDER_H__0AF20C77_9BD6_405A_B85C_97F4370E9490__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHResHolder  
{
public:
	bool LoadRes(HINSTANCE hInstance);
	CHResHolder();
	virtual ~CHResHolder();

public:
	CImageList m_cImgPos[5];
	CBrush m_cBrushBlack;
	CBrush m_cBrushWhite;
	CBrush m_cBrushRed;
	CBrush m_cBrushYellow;
	CBrush m_cBrushGray;
	CBrush m_cBrushGreen;
	CPen m_cPenBlack;
	CPen m_cPenRed;
	CFont m_cfStandart;
};

#endif // !defined(AFX_HRESHOLDER_H__0AF20C77_9BD6_405A_B85C_97F4370E9490__INCLUDED_)
