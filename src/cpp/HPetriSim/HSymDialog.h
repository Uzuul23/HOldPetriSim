#ifndef AFX_HSYMDIALOG_H__50B5AAA3_FD3A_11D1_9366_000000000000__INCLUDED_
#define AFX_HSYMDIALOG_H__50B5AAA3_FD3A_11D1_9366_000000000000__INCLUDED_

// HSymDialog.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CHSymDialog 
#include "Resource.h"
class CHSymDialog : public CDialog
{
// Konstruktion
public:
	CHSymDialog(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CHSymDialog)
	enum { IDD = IDD_SYM_DIALOG };
	int		m_iConMod;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CHSymDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CHSymDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // AFX_HSYMDIALOG_H__50B5AAA3_FD3A_11D1_9366_000000000000__INCLUDED_
