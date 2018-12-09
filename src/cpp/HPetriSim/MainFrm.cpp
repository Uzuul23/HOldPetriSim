/**************************************************************************
	MainFrm.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "stdafx.h"
#include "PetriSim.h"
#include "PetriSimDoc.h"
#include "HSimToolbar.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

/*************************************************************************/
// CMainFrame
/*************************************************************************/

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_MESSAGE(WM_UPDATENET, &CMainFrame::OnUpdateNet)
	ON_MESSAGE(WM_ANIMSTEP, &CMainFrame::OnAnimStep)
	ON_MESSAGE(WM_UPDATEOUTPUT, &CMainFrame::OnUpdateOutput)
	ON_COMMAND(ID_HELP_FINDER, &CMDIFrameWndEx::OnHelpFinder)
	ON_COMMAND(ID_HELP, &CMDIFrameWndEx::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, &CMDIFrameWndEx::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, &CMDIFrameWndEx::OnHelpFinder)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_SIMSTATUS,
	ID_INDICATOR_STEPCOUNT,
	ID_INDICATOR_SIMTIME,
	ID_INDICATOR_STEPTIME,
	ID_INDICATOR_MOUSEPOS,
	ID_INDICATOR_ZOOMSCALE,
	ID_INDICATOR_COUNT,
};

CMainFrame::CMainFrame()
{
	m_mmTimer = 0;
	m_nTimerRef = 0;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CTabbedPane::m_StyleTabWnd = CMFCTabCtrl::STYLE_3D_ONENOTE;
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS
	(CMFCVisualManagerWindows7));

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE;
	mdiTabParams.m_bActiveTabCloseButton = TRUE;
	mdiTabParams.m_bTabIcons = FALSE;
	mdiTabParams.m_bAutoColor = TRUE;
	mdiTabParams.m_bDocumentMenu = TRUE;
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menu bar\n");
		return -1; 
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() 
		|CBRS_SIZE_DYNAMIC|CBRS_TOOLTIPS|CBRS_FLYBY);

	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE 
		|CBRS_TOP|CBRS_GRIPPER|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC)||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons 
		? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Failed to create menu bar\n");
		return -1;
	}

	CString strToolBarName;
	BOOL bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	if (!m_wndEditBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE 
		|CBRS_TOP|CBRS_GRIPPER|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC
		, CRect(1, 1, 1, 1), IDR_EDITBAR) ||
		!m_wndEditBar.LoadToolBar(theApp.m_bHiColorIcons 
		? IDR_EDITBAR_256 : IDR_EDITBAR))
	{
		TRACE0("Failed to create menu bar\n");
		return -1;
	}

	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_EDIT);
	ASSERT(bNameValid);
	m_wndEditBar.SetWindowText(strToolBarName);

	if (!m_wndSimBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE 
		|CBRS_TOP|CBRS_GRIPPER|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC
		, CRect(1, 1, 1, 1), IDR_SIMBAR) ||
		!m_wndSimBar.LoadToolBar(theApp.m_bHiColorIcons 
		? IDR_SIMBAR : IDR_SIMBAR))
	{
		TRACE0("Failed to create menu bar\n");
		return -1;
	}

	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_SIM);
	ASSERT(bNameValid);
	m_wndSimBar.SetWindowText(strToolBarName);

	// create slider in the simulation bar
	m_wndSimBar.SetButtonInfo(1, ID_SLIDER_SIMSPEED, TBBS_SEPARATOR, 100);
	m_wndSimBar.RecalcLayout();

	CRect rect;
	m_wndSimBar.GetItemRect(1, &rect);
	
	if (!m_wndSimBar.m_cSliderSpeed.Create(WS_VISIBLE|WS_TABSTOP, rect
		, &m_wndSimBar, ID_SLIDER_SIMSPEED))
	{
		TRACE0("Failed to create Slider\n");
		return FALSE;
	}

	m_wndSimBar.m_cSliderSpeed.SetRange(MIN_TIME, MAX_ANIM_TIME);

	if (!m_wndStatusBar.Create(this) 
		|| !m_wndStatusBar.SetIndicators(indicators
		, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);

	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100)
		, TRUE, ID_VIEW_OUTPUTWND, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS
		|WS_CLIPCHILDREN|CBRS_BOTTOM|CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create output window\n");
		return FALSE;
	}

	CString strPropWnd;
	bNameValid = strPropWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);

	if (!m_wndProperties.Create(strPropWnd, this, CRect(0, 0, 200, 200)
		, TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS
		|WS_CLIPCHILDREN|CBRS_RIGHT|CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create properties window\n");
		return FALSE;
	}

	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndEditBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndSimBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndSimBar);
	DockPaneLeftOf(&m_wndEditBar, &m_wndSimBar);
	DockPaneLeftOf(&m_wndToolBar, &m_wndEditBar);
	DockPane(&m_wndOutput);
	DockPane(&m_wndProperties);

	EnableWindowsDialog(ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	CDockingManager::SetDockingMode(DT_SMART);
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	//CSplashWnd::ShowSplashScreen(this);
	return 0;
}

#ifdef _DEBUG

void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}

#endif //_DEBUG

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust 
		= new CMFCToolBarsCustomizeDialog(this, TRUE);

	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_RBUTTONDOWN)
	{
		CWnd* pWnd = CWnd::FromHandlePermanent(pMsg->hwnd);
		CControlBar* pBar = DYNAMIC_DOWNCAST(CControlBar, pWnd);

		if (pBar != NULL)
		 {
			CMenu Menu;
			CPoint pt;

			pt.x = LOWORD(pMsg->lParam);
			pt.y = HIWORD(pMsg->lParam);
			pBar->ClientToScreen(&pt);

			if (Menu.LoadMenu(IDR_HPSTYPE))
			{
				CMenu* pSubMenu = Menu.GetSubMenu(2);

				if (pSubMenu!=NULL)
				{
					pSubMenu->TrackPopupMenu(TPM_LEFTALIGN
						|TPM_RIGHTBUTTON,pt.x,pt.y,this);
				}
			}
		}
	}
	return CMDIFrameWndEx::PreTranslateMessage(pMsg);
}

long CMainFrame::OnUpdateNet(UINT wParam,LONG lParam)
{
	if(lParam)
	{
		CPetriSimDoc* pDoc = (CPetriSimDoc*)lParam;
		CWinApp* pApp = AfxGetApp();
		POSITION pos = pApp->GetFirstDocTemplatePosition();
		while(pos)
		{
			CDocTemplate* pTem = pApp->GetNextDocTemplate(pos);
			POSITION posA = pTem->GetFirstDocPosition();
			while(posA)
			{
				CDocument* pDocA = pTem->GetNextDoc(posA);
				if(pDocA == pDoc)
				{
					pDoc->UpdateInvalidObjects();
				}
			}
		}
	}
	return 0L;
}

void CMainFrame::OnTimer( UINT nIDEvent )
{

}

long CMainFrame::OnUpdateOutput(UINT wParam,LONG lParam)
{
	if(lParam)
	{
		CPetriSimDoc* pDoc = (CPetriSimDoc*)lParam;
		CWinApp* pApp = AfxGetApp();
		POSITION pos = pApp->GetFirstDocTemplatePosition();
		while(pos)
		{
			CDocTemplate* pTem = pApp->GetNextDocTemplate(pos);
			POSITION posA = pTem->GetFirstDocPosition();
			while(posA)
			{
				CDocument* pDocA = pTem->GetNextDoc(posA);
				if(pDocA == pDoc)
				{
					pDoc->UpdateOutputWindow();
				}
			}
		}
	}	
	return 0L;
}

void CMainFrame::StopAnimTimer()
{
	if(m_mmTimer && !(--m_nTimerRef))
	{
		::timeKillEvent(m_mmTimer);
		m_mmTimer = 0;
	}
}

void CMainFrame::StartAnimTimer()
{
	if(m_mmTimer == 0)
		m_mmTimer = timeSetEvent(20, 100, TimeProc, (DWORD)m_hWnd 
		, TIME_PERIODIC);
	if(m_mmTimer == 0)
		AfxMessageBox(_T("Error : timeSetEvent"));
	else
		m_nTimerRef++;
}

void CALLBACK CMainFrame::TimeProc(UINT uID, UINT uMsg, DWORD dwUser
								   , DWORD dw1, DWORD dw2)
{
	HWND hWnd = (HWND)dwUser;
	::PostMessage(hWnd, WM_ANIMSTEP, 0, 0);
}

long CMainFrame::OnAnimStep(UINT wParam,LONG lParam)
{
	CWinApp* pApp = AfxGetApp();
	POSITION pos = pApp->GetFirstDocTemplatePosition();
	while(pos)
	{
		CDocTemplate* pTem = pApp->GetNextDocTemplate(pos);
		POSITION posA = pTem->GetFirstDocPosition();
		while(posA)
		{
			CPetriSimDoc* pDoc = 
				(CPetriSimDoc*)pTem->GetNextDoc(posA);
			pDoc->AnimTokens();
		}
	}
	return 0;
}
