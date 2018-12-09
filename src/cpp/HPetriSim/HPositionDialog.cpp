// HPositionDialog.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "PetriSim.h"
#include "HPositionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CHPositionDialog 


CHPositionDialog::CHPositionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CHPositionDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHPositionDialog)
	m_iDelayOn = 0;
	m_iTokenMode = 0;
	m_iRandOn = 0;
	m_bAuto = TRUE;
	//}}AFX_DATA_INIT
}


void CHPositionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHPositionDialog)
	DDX_Text(pDX, IDC_DELAY_ON_EDIT, m_iDelayOn);
	DDV_MinMaxInt(pDX, m_iDelayOn, 0, 16000000);
	DDX_Radio(pDX, IDC_NEVER_TOKEN_RADIO, m_iTokenMode);
	DDX_Text(pDX, IDC_RAND_ON_EDIT, m_iRandOn);
	DDV_MinMaxInt(pDX, m_iRandOn, 0, 16000000);
	DDX_Check(pDX, IDC_RESTART_CHECK, m_bAuto);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHPositionDialog, CDialog)
	//{{AFX_MSG_MAP(CHPositionDialog)
	ON_EN_CHANGE(IDC_DELAY_ON_EDIT, OnChangeDelayOnEdit)
	ON_EN_CHANGE(IDC_RAND_ON_EDIT, OnChangeRandOnEdit)
	ON_BN_CLICKED(IDC_DELAY_TOKEN_RADIO, OnDelayTokenRadio)
	ON_BN_CLICKED(IDC_NEVER_TOKEN_RADIO, OnNeverTokenRadio)
	ON_BN_CLICKED(IDC_RAND_TOKEN_RADIO, OnRandTokenRadio)
	ON_BN_CLICKED(IDC_RESTART_CHECK, OnRestartCheck)
	ON_BN_CLICKED(IDC_START_TOKEN_RADIO, OnStartTokenRadio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CHPositionDialog 

void CHPositionDialog::OnChangeDelayOnEdit() 
{
	UpdateData(TRUE);
	m_iDelay = m_iDelayOn;	
}

void CHPositionDialog::OnChangeRandOnEdit() 
{
	UpdateData(TRUE);
	m_iDelay = m_iRandOn;
}

void CHPositionDialog::OnNeverTokenRadio() 
{
	m_iDelayOn = 0;
	m_iRandOn = 0;
	m_iTokenMode = 0;
	UpdateData(FALSE);

	GetDlgItem(IDC_DELAY_ON_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RAND_ON_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RESTART_CHECK)->EnableWindow(FALSE);
	
}


void CHPositionDialog::OnStartTokenRadio() 
{
	m_iDelayOn = 0;
	m_iRandOn = 0;
	m_iTokenMode = 1;
	UpdateData(FALSE);

	GetDlgItem(IDC_DELAY_ON_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RAND_ON_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RESTART_CHECK)->EnableWindow(FALSE);
	
}


void CHPositionDialog::OnDelayTokenRadio() 
{
	m_iDelayOn = 1;
	m_iRandOn = 0;
	m_iTokenMode = 2;
	UpdateData(FALSE);

	GetDlgItem(IDC_DELAY_ON_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_RESTART_CHECK)->EnableWindow(TRUE);
	GetDlgItem(IDC_RAND_ON_EDIT)->EnableWindow(FALSE);
	
}


void CHPositionDialog::OnRandTokenRadio() 
{
	m_iDelayOn = 0;
	m_iRandOn = 1;
	m_iTokenMode = 3;
	UpdateData(FALSE);

	GetDlgItem(IDC_DELAY_ON_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RAND_ON_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_RESTART_CHECK)->EnableWindow(TRUE);
	
}

BOOL CHPositionDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(m_bAuto && (m_iTokenMode > 1))
		GetDlgItem(IDC_RESTART_CHECK)->EnableWindow(TRUE);

	switch(m_iTokenMode)
	{
	case 2:
		{
			GetDlgItem(IDC_DELAY_ON_EDIT)->EnableWindow(TRUE);
			m_iDelayOn = m_iDelay;
			break;
		}
	case 3:
		{
			GetDlgItem(IDC_RAND_ON_EDIT)->EnableWindow(TRUE);
			m_iRandOn = m_iDelay;
			break;
		}
	}
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CHPositionDialog::OnTokenCheck() 
{
	UpdateData(TRUE);	
}

void CHPositionDialog::OnRestartCheck() 
{
	UpdateData(TRUE);	
}


