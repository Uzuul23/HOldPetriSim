///////////////////////////////////////////////////////////////////////
// DocPropertySheetA.cpp : Implementierungsdatei                     //
//                                                                   //
// Written by Henryk Anschuetz (s0140382@rz.fhtw-berlin.de)          //
// Copyright (c) 1999                                                //
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "DocPropertySheetA.h"

#ifdef _DEBUG
#undef THIS_FILE
static CHAR BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDocPropertySheetA

IMPLEMENT_DYNAMIC(CDocPropertySheetA, CPropertySheet)

CDocPropertySheetA::CDocPropertySheetA(CWnd* pWndParent)
	 : CPropertySheet(IDS_PROPSHT_CAPTION1, pWndParent)
{
	// Fügen Sie alle Eigenschaftenseiten hier ein. Beachten Sie, dass
	// die Reihenfolge, in der diese hier erscheinen, der Reihenfolge auf dem
	// Bildschirm entspricht.  Standardmäßig ist die
	// erste Seite der Menge die aktive Seite.
	// Um eine andere Eigenschaftenseite als aktive Seite
	// festzulegen, rufen Sie SetActivePage() auf.

	AddPage(&m_Page1);
	AddPage(&m_Page2);
	AddPage(&m_Page3);
	AddPage(&m_Page4);
}

CDocPropertySheetA::~CDocPropertySheetA()
{
}


BEGIN_MESSAGE_MAP(CDocPropertySheetA, CPropertySheet)
	//{{AFX_MSG_MAP(CDocPropertySheetA)
		// HINWEIS - Der Klassen-Assistent fügt hier Zuordnungsmakros ein oder entfernt sie.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDocPropertySheetA -Behandlungsroutinen


