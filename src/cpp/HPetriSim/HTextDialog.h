/**************************************************************************
	HTextDialog.h
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#ifndef AFX_HTEXTDIALOG_H__05C6C251_2703_11D2_93BC_000000000000__INCLUDED_
#define AFX_HTEXTDIALOG_H__05C6C251_2703_11D2_93BC_000000000000__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CHTextDialog 
#include "Resource.h"
class CHTextDialog : public CDialog
{
// Konstruktion
public:
	CString GetText();
	//CHTextDialog(CWnd* pParent = NULL);   // Standardkonstruktor
	CHTextDialog(CString text);
// Dialogfelddaten
	//{{AFX_DATA(CHTextDialog)
	enum { IDD = IDD_TEXT_DIALOG };
	CString	m_csText;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CHTextDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CHTextDialog)
	afx_msg void OnChangeTextEdit();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // AFX_HTEXTDIALOG_H__05C6C251_2703_11D2_93BC_000000000000__INCLUDED_
