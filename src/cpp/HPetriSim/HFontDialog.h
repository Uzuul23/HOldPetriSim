/**************************************************************************
	HFontDialog.h
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#ifndef AFX_HFONTDIALOG_H__B0A54092_26EA_11D2_93BC_000000000000__INCLUDED_
#define AFX_HFONTDIALOG_H__B0A54092_26EA_11D2_93BC_000000000000__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CHFontDialog 

class CHFontDialog : public CFontDialog
{
	DECLARE_DYNAMIC(CHFontDialog)

public:
	CHFontDialog(LPLOGFONT lplfInitial = NULL,
		DWORD dwFlags = CF_EFFECTS | CF_SCREENFONTS,
		CDC* pdcPrinter = NULL,
		CWnd* pParentWnd = NULL);
#ifndef _AFX_NO_RICHEDIT_SUPPORT
	CHFontDialog(const CHARFORMAT& charformat,
		DWORD dwFlags = CF_SCREENFONTS,
		CDC* pdcPrinter = NULL,
		CWnd* pParentWnd = NULL);
#endif

protected:
	//{{AFX_MSG(CHFontDialog)
		// HINWEIS - Der Klassen-Assistent fügt hier Member-Funktionen ein und entfernt diese.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // AFX_HFONTDIALOG_H__B0A54092_26EA_11D2_93BC_000000000000__INCLUDED_
