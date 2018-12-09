#ifndef AFX_HSYMSTATEFORMVIEW_H__5C9963E3_F59E_11D2_BA80_0000B45E2D1E__INCLUDED_
#define AFX_HSYMSTATEFORMVIEW_H__5C9963E3_F59E_11D2_BA80_0000B45E2D1E__INCLUDED_

///////////////////////////////////////////////////////////////////////
// HSymStateFormView.h : Header-Datei                                //
//                                                                   //
// Written by Henryk Anschuetz (s0140382@rz.fhtw-berlin.de)          //
// Copyright (c) 1999                                                //
///////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Formularansicht CHSymStateFormView 

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
class CPetriSymDoc;
class CHSymStateFormView : public CFormView
{
protected:
	CHSymStateFormView();           // Dynamische Erstellung verwendet geschützten Konstruktor
	DECLARE_DYNCREATE(CHSymStateFormView)

// Formulardaten
public:
	//{{AFX_DATA(CHSymStateFormView)
	enum { IDD = IDD_SYMSTATE_VIEW };
	CProgressCtrl	m_ctrProgressLoad;
	CSliderCtrl	m_ctrSliderSpeed;
	//}}AFX_DATA

// Attribute
public:
private:

// Operationen
public:
private:
	inline CPetriSymDoc* GetDocument()
	{
		return (CPetriSymDoc*)CView::GetDocument();
	};

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CHSymStateFormView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementierung
protected:
	virtual ~CHSymStateFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CHSymStateFormView)
	afx_msg long OnUpdateState(UINT wParam,LONG lParam);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // AFX_HSYMSTATEFORMVIEW_H__5C9963E3_F59E_11D2_BA80_0000B45E2D1E__INCLUDED_
