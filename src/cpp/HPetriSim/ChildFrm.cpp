/**************************************************************************
	ChildFrm.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "stdafx.h"
#include "PetriSim.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_COMMAND_EX(CG_ID_VIEW_MYDIALOGBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(CG_ID_VIEW_MYDIALOGBAR, OnUpdateControlBarMenu)
	ON_WM_CREATE()
END_MESSAGE_MAP()

CChildFrame::CChildFrame()
{

}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	return CMDIChildWndEx::PreCreateWindow(cs);
}

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}

#endif //_DEBUG

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs
								 , CCreateContext* pContext)
{
	//return	CMDIChildWndEx::OnCreateClient( lpcs, pContext);	
		
	if (!m_wndSplitter.Create(this,2, 2, CSize(50, 50), pContext))
	{
			TRACE0("Fehler beim Erstellen des Fensterteilers ");
			return FALSE;
	}
	return true;
	
}



