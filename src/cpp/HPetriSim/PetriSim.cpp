/**************************************************************************
	PetriSim.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "stdafx.h"
#include "PetriSim.h"
#include "ChildFrm.h"
#include "PetriSimDoc.h"
#include "PetriSimView.h"
#include "MainFrm.h"
#include "Splash.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

/*************************************************************************/
// CPetriSimApp
/*************************************************************************/

BEGIN_MESSAGE_MAP(CPetriSimApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CPetriSimApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()

CPetriSimApp::CPetriSimApp()
{
	m_bHiColorIcons = TRUE;
	m_pDocTemplate = 0;
}

CPetriSimApp theApp;

BOOL CPetriSimApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	SetRegistryKey(IDS_FIRMA);
	LoadStdProfileSettings();
	InitContextMenuManager();
	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	m_pDocTemplate = new CMultiDocTemplate(
		IDR_HPSTYPE,
		RUNTIME_CLASS(CPetriSimDoc),
		RUNTIME_CLASS(CChildFrame), 
		RUNTIME_CLASS(CPetriSimView));

	if (!m_pDocTemplate)
		return FALSE;

	AddDocTemplate(m_pDocTemplate);

	// initialize randomizer
	srand(static_cast<int>(time(0)));

	if(!m_cResHolder.LoadRes(m_hInstance))
	{
		AfxMessageBox(_T("Error: Create GDI-Objects"));
		PostQuitMessage(0);
		return FALSE;
	}

	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		return FALSE;
	}

	m_pMainWnd = pMainFrame;
	m_pMainWnd->DragAcceptFiles();

	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

BOOL CPetriSimApp::PreTranslateMessage(MSG* pMsg)
{
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;  

	return CWinAppEx::PreTranslateMessage(pMsg);
}

/*************************************************************************/
// CAboutDlg
/*************************************************************************/

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };
	// TODO: CHyperLink	m_HyperLink;
	CString	m_strAboutText;
	CString m_strMemText;
	CString	m_strSerial;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	m_strSerial = _T("");
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

//	DDX_Control(pDX, IDC_HYPERLINK, m_HyperLink);
	DDX_Text(pDX, IDC_EDIT_ABOUT, m_strAboutText);
	DDX_Text(pDX, IDC_EDIT_MEM, m_strMemText);
	DDX_Text(pDX, IDC_STATIC_SERIAL, m_strSerial);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

void CPetriSimApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strMail;
	strMail.LoadString(IDS_EMAIL);
	//m_HyperLink.SetWindowText(strMail);
	//m_HyperLink.SetURL(strMail);
	//m_HyperLink.SetUnderline(TRUE);
	
	m_strAboutText.LoadString(IDS_ABOUT_TEXT);
	//CString strLimits;
	//strLimits.LoadString(IDS_ABOUT_LIMIT);
	//m_strAboutText += strLimits;
	//m_strAboutText += theApp.m_strOwner;
	m_strSerial = "Free Version";
	
	{
		CString strFreeDiskSpace;
		CString strFmt;

		MEMORYSTATUS MemStat;
		MemStat.dwLength = sizeof(MEMORYSTATUS);
		GlobalMemoryStatus(&MemStat);

		CString strMemFormat;
		strMemFormat.LoadString(IDS_MEM_TEXT);
		m_strMemText.Format(strMemFormat,
			MemStat.dwTotalPhys / 1024L,
			MemStat.dwMemoryLoad,
			MemStat.dwAvailPhys / 1024L,
			MemStat.dwTotalPageFile / 1024L,
			MemStat.dwAvailPageFile / 1024L,
			MemStat.dwTotalVirtual / 1024L,
			MemStat.dwAvailVirtual / 1024L);

	}
	UpdateData(false);
	return TRUE;
}

