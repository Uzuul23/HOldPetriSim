/**************************************************************************
	DocPropertySheetA.h
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

// Diese Klasse definiert ein benutzerdefiniertes modales Eigenschaftsblatt 
// CDocPropertySheetA.
 
#ifndef __DOCPROPERTYSHEETA_H__
#define __DOCPROPERTYSHEETA_H__

#include "DocPropertyPageA.h"

/////////////////////////////////////////////////////////////////////////////
// CDocPropertySheetA

class CDocPropertySheetA : public CPropertySheet
{
	DECLARE_DYNAMIC(CDocPropertySheetA)

// Konstruktion
public:
	CDocPropertySheetA(CWnd* pWndParent = NULL);

// Attribute
public:
	CDocPropertyPageAA m_Page1;
	CDocPropertyPageAB m_Page2;
	CDocPropertyPageAC m_Page3;
	CDocPropertyPageAD m_Page4;

// Operationen
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte Überschreibungen virtueller Funktionen
	//{{AFX_VIRTUAL(CDocPropertySheetA)
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CDocPropertySheetA();

// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CDocPropertySheetA)
		// HINWEIS - Der Klassen-Assistent fügt hier Member-Funktionen ein oder entfernt sie.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __DOCPROPERTYSHEETA_H__
