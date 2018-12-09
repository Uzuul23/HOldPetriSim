/**************************************************************************
	HLabelDialog.h
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#ifndef AFX_HLABELDIALOG_H__0C14F962_3CF8_11D2_93F2_000000000000__INCLUDED_
#define AFX_HLABELDIALOG_H__0C14F962_3CF8_11D2_93F2_000000000000__INCLUDED_

// HLabelDialog.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CHLabelDialog 

class CHLabelDialog : public CDialog
{
// Konstruktion
public:
	CString GetText();
	CHLabelDialog(CString string);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CHLabelDialog)
	enum { IDD = IDD_LABEL_DIALOG };
	CString	m_csText;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CHLabelDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CHLabelDialog)
		// HINWEIS: Der Klassen-Assistent fügt hier Member-Funktionen ein
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // AFX_HLABELDIALOG_H__0C14F962_3CF8_11D2_93F2_000000000000__INCLUDED_
