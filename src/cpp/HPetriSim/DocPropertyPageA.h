/**************************************************************************
	DocPropertyPageA.h
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#ifndef __DOCPROPERTYPAGEA_H__
#define __DOCPROPERTYPAGEA_H__

/////////////////////////////////////////////////////////////////////////////
// CDocPropertyPageAA Dialogfeld
#include "resource.h"
class CDocPropertyPageAA : public CPropertyPage
{
	DECLARE_DYNCREATE(CDocPropertyPageAA)

// Konstruktion
public:
	CDocPropertyPageAA();
	~CDocPropertyPageAA();

//members
public:
	COLORREF m_iBackColor;

// Dialogfelddaten
	//{{AFX_DATA(CDocPropertyPageAA)
	enum { IDD = IDD_PROPPAGE1 };
	int		m_iVerExtend;
	int		m_iHorExtend;
	BOOL	m_bPopupExplorer;
	BOOL	m_bPopupMessage;
	//}}AFX_DATA


// Überschreibungen
	// Der Klassen-Assistent generiert Überschreibungen virtueller Funktion
	//{{AFX_VIRTUAL(CDocPropertyPageAA)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CDocPropertyPageAA)
	afx_msg void OnButtonBackcolor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CDocPropertyPageAB Dialogfeld

class CDocPropertyPageAB : public CPropertyPage
{
	DECLARE_DYNCREATE(CDocPropertyPageAB)

// Konstruktion
public:
	CDocPropertyPageAB();
	~CDocPropertyPageAB();

// Dialogfelddaten
	//{{AFX_DATA(CDocPropertyPageAB)
	enum { IDD = IDD_PROPPAGE2 };
	CSpinButtonCtrl	m_cSpin;
	BOOL	m_bAlignDo;
	int		m_iGrid;
	BOOL	m_bGridVisible;
	BOOL	m_bAlignNo;
	//}}AFX_DATA


// Überschreibungen
	// Der Klassen-Assistent generiert Überschreibungen virtueller Funktion
	//{{AFX_VIRTUAL(CDocPropertyPageAB)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CDocPropertyPageAB)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CDocPropertyPageAC 

class CDocPropertyPageAC : public CPropertyPage
{
// Konstruktion
public:
	CDocPropertyPageAC();   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CDocPropertyPageAC)
	enum { IDD = IDD_PROPPAGE4 };
	UINT	m_iSampleTime;
	UINT	m_iStopTime;
	UINT	m_iStopCount;
	CString	m_csOutputFile;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CDocPropertyPageAC)
	protected:
	virtual BOOL OnKillActive();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CDocPropertyPageAC)
	afx_msg void OnButtonFiledialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CDocPropertyPageAD 

class CDocPropertyPageAD : public CPropertyPage
{
// Konstruktion
public:
	CDocPropertyPageAD();   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CDocPropertyPageAD)
	enum { IDD = IDD_PROPPAGE5 };
		// HINWEIS: Der Klassen-Assistent fügt hier Datenelemente ein
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CDocPropertyPageAD)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CDocPropertyPageAD)
		// HINWEIS: Der Klassen-Assistent fügt hier Member-Funktionen ein
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif // __DOCPROPERTYPAGEA_H__
