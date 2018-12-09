/**************************************************************************
	LimitDialog.h
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#if !defined(AFX_LIMITDIALOG_H__0B558B7F_77EB_459D_913C_6886BDEF1D5B__INCLUDED_)
#define AFX_LIMITDIALOG_H__0B558B7F_77EB_459D_913C_6886BDEF1D5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LimitDialog.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CLimitDialog 

class CLimitDialog : public CDialog
{
// Konstruktion
public:
	CLimitDialog(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CLimitDialog)
	enum { IDD = IDD_LIMIT_DIALOG };
	CString	m_strLimitText;
	//CHyperLink	m_HyperLink;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CLimitDialog)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CLimitDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_LIMITDIALOG_H__0B558B7F_77EB_459D_913C_6886BDEF1D5B__INCLUDED_
