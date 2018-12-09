///////////////////////////////////////////////////////////////////////
// HSymStateFormView.cpp: Implementierungsdatei                      //
//                                                                   //
// Written by Henryk Anschuetz (s0140382@rz.fhtw-berlin.de)          //
// Copyright (c) 1999                                                //
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "PetriSimDoc.h"
#include "HSymStateFormView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHSymStateFormView

IMPLEMENT_DYNCREATE(CHSymStateFormView, CFormView)

CHSymStateFormView::CHSymStateFormView()
	: CFormView(CHSymStateFormView::IDD)
{
	//{{AFX_DATA_INIT(CHSymStateFormView)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT
}

CHSymStateFormView::~CHSymStateFormView()
{
}

void CHSymStateFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHSymStateFormView)
	DDX_Control(pDX, IDC_PROGRESS_LOAD, m_ctrProgressLoad);
	DDX_Control(pDX, IDC_SLIDER_SPEED, m_ctrSliderSpeed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHSymStateFormView, CFormView)
	//{{AFX_MSG_MAP(CHSymStateFormView)
	ON_MESSAGE(WM_USER + 100,OnUpdateState)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Diagnose CHSymStateFormView

#ifdef _DEBUG
void CHSymStateFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CHSymStateFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CHSymStateFormView 
long CHSymStateFormView::OnUpdateState(UINT wParam,LONG lParam)
{
	m_ctrProgressLoad.SetPos(lParam);
	return 0L;
}
/////////////////////////////////////////////////////////////////////////////
//
void CHSymStateFormView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	
	
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void CHSymStateFormView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CHSymStateFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

}
