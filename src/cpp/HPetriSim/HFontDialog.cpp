/**************************************************************************
	HFontDialog.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "stdafx.h"
#include "HFontDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CHFontDialog, CFontDialog)

CHFontDialog::CHFontDialog(LPLOGFONT lplfInitial, DWORD dwFlags
						   , CDC* pdcPrinter, CWnd* pParentWnd) 
	: CFontDialog(lplfInitial, dwFlags, pdcPrinter, pParentWnd)
{
}

BEGIN_MESSAGE_MAP(CHFontDialog, CFontDialog)
END_MESSAGE_MAP()

