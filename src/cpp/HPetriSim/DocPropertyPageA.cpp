/**************************************************************************
	DocPropertyPageA.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "stdafx.h"
#include "resource.h"
#include "DocPropertyPageA.h"

#ifdef _DEBUG
#undef THIS_FILE
static CHAR BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CDocPropertyPageAA, CPropertyPage)
IMPLEMENT_DYNCREATE(CDocPropertyPageAB, CPropertyPage)

CDocPropertyPageAA::CDocPropertyPageAA() 
	: CPropertyPage(CDocPropertyPageAA::IDD)
{
	m_iVerExtend = 500;
	m_iHorExtend = 500;
	m_bPopupExplorer = FALSE;
	m_bPopupMessage = FALSE;
}

CDocPropertyPageAA::~CDocPropertyPageAA()
{
}

void CDocPropertyPageAA::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HOR_EDIT, m_iVerExtend);
	DDV_MinMaxInt(pDX, m_iVerExtend, 100, 15000);
	DDX_Text(pDX, IDC_VER_EDIT, m_iHorExtend);
	DDV_MinMaxInt(pDX, m_iHorExtend, 100, 15000);
	DDX_Check(pDX, IDC_CHECK_POPEXPLORER, m_bPopupExplorer);
	DDX_Check(pDX, IDC_CHECK_POPMESSAGE, m_bPopupMessage);
}

BEGIN_MESSAGE_MAP(CDocPropertyPageAA, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_BACKCOLOR
		, &CDocPropertyPageAA::OnButtonBackcolor)
END_MESSAGE_MAP()

CDocPropertyPageAB::CDocPropertyPageAB()
	: CPropertyPage(CDocPropertyPageAB::IDD)
{
	m_bAlignDo = FALSE;
	m_iGrid = 10;
	m_bGridVisible = FALSE;
	m_bAlignNo = FALSE;
}

CDocPropertyPageAB::~CDocPropertyPageAB()
{
}

void CDocPropertyPageAB::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_GRID_SPIN, m_cSpin);
	DDX_Check(pDX, IDC_DRAWOBJECTS_CHECK, m_bAlignDo);
	DDX_Text(pDX, IDC_GRID_EDIT, m_iGrid);
	DDV_MinMaxInt(pDX, m_iGrid, 5, 50);
	DDX_Check(pDX, IDC_GRIDVISIBLE_CHECK, m_bGridVisible);
	DDX_Check(pDX, IDC_NETOBJECTS_CHECK, m_bAlignNo);
	
}

BEGIN_MESSAGE_MAP(CDocPropertyPageAB, CPropertyPage)
	
END_MESSAGE_MAP()

CDocPropertyPageAC::CDocPropertyPageAC() 
	: CPropertyPage(CDocPropertyPageAC::IDD)
{
	m_iSampleTime = 1;
	m_iStopTime = 1;
	m_iStopCount = 1;
	m_csOutputFile = _T("");
}


void CDocPropertyPageAC::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SAMPLETIME, m_iSampleTime);
	DDV_MinMaxUInt(pDX, m_iSampleTime, 1, 10000);
	DDX_Text(pDX, IDC_EDIT_STOP_SIMULATIONTIME, m_iStopTime);
	DDV_MinMaxUInt(pDX, m_iStopTime, 1, 160000000);
	DDX_Text(pDX, IDC_EDIT_STOP_SIMULATIONTIME2, m_iStopCount);
	DDV_MinMaxUInt(pDX, m_iStopCount, 1, 160000000);
	DDX_Text(pDX, IDC_EDIT_OUTPUT_FILE, m_csOutputFile);
}

BEGIN_MESSAGE_MAP(CDocPropertyPageAC, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FILEDIALOG
		, &CDocPropertyPageAC::OnButtonFiledialog)
END_MESSAGE_MAP()

CDocPropertyPageAD::CDocPropertyPageAD() 
	: CPropertyPage(CDocPropertyPageAD::IDD)
{

}

void CDocPropertyPageAD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDocPropertyPageAD, CDialog)
END_MESSAGE_MAP()

BOOL CDocPropertyPageAA::OnKillActive() 
{
	UpdateData(TRUE);
	return CPropertyPage::OnKillActive();
}

BOOL CDocPropertyPageAB::OnKillActive() 
{
	UpdateData(TRUE);
	return CPropertyPage::OnKillActive();
}

BOOL CDocPropertyPageAC::OnKillActive() 
{
	UpdateData(TRUE);
	return CPropertyPage::OnKillActive();
}

BOOL CDocPropertyPageAB::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	m_cSpin.SetRange(5,50);	
	return TRUE; 
}

void CDocPropertyPageAC::OnButtonFiledialog() 
{
	CFileDialog dlg(false, 0, m_csOutputFile, OFN_HIDEREADONLY
		, _T("Text Files (*.csv)|*.csv|All Files|*.*||"));

	if(dlg.DoModal()==IDOK)
	{
		CString path(dlg.GetPathName());
		if(dlg.GetFileExt().IsEmpty())
			path += ".csv";
		m_csOutputFile = path;
		UpdateData(false);
	}
}

void CDocPropertyPageAA::OnButtonBackcolor() 
{
	CColorDialog dlg(m_iBackColor);
	if(dlg.DoModal() == IDOK)
		m_iBackColor = dlg.GetColor();
}


