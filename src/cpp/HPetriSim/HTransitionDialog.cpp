// HTransitionDialog.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "PetriSim.h"
#include "HTransitionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CHTransitionDialog 


CHTransitionDialog::CHTransitionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CHTransitionDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHTransitionDialog)
	m_iDelayOff = 0;
	m_iDelayOn = 0;
	m_iEnabledMode = 0;
	m_iRandOff = 0;
	m_iRandOn = 0;
	m_bAuto = FALSE;
	m_bTimerStop = FALSE;
	//}}AFX_DATA_INIT
}


void CHTransitionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHTransitionDialog)
	DDX_Text(pDX, IDC_DELAY_OFF_EDIT, m_iDelayOff);
	DDV_MinMaxInt(pDX, m_iDelayOff, 0, 16000000);
	DDX_Text(pDX, IDC_DELAY_ON_EDIT, m_iDelayOn);
	DDV_MinMaxInt(pDX, m_iDelayOn, 0, 16000000);
	DDX_Radio(pDX, IDC_INMEDIATE_RADIO, m_iEnabledMode);
	DDX_Text(pDX, IDC_RAND_OFF_EDIT, m_iRandOff);
	DDV_MinMaxInt(pDX, m_iRandOff, 0, 1000);
	DDX_Text(pDX, IDC_RAND_ON_EDIT, m_iRandOn);
	DDV_MinMaxInt(pDX, m_iRandOn, 0, 1000);
	DDX_Check(pDX, IDC_AUTO_CHECK, m_bAuto);
	DDX_Check(pDX, IDC_TIMER_STOP_CHECK, m_bTimerStop);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHTransitionDialog, CDialog)
	//{{AFX_MSG_MAP(CHTransitionDialog)
	ON_BN_CLICKED(IDC_INMEDIATE_RADIO, OnInmediateRadio)
	ON_BN_CLICKED(IDC_DELAY_ON_RADIO, OnDelayOnRadio)
	ON_BN_CLICKED(IDC_DELAY_OFF_RADIO, OnDelayOffRadio)
	ON_BN_CLICKED(IDC_RAND_ON_RADIO, OnRandOnRadio)
	ON_BN_CLICKED(IDC_RAND_OFF_RADIO, OnRandOffRadio)
	ON_EN_CHANGE(IDC_DELAY_OFF_EDIT, OnChangeDelayOffEdit)
	ON_EN_CHANGE(IDC_DELAY_ON_EDIT, OnChangeDelayOnEdit)
	ON_EN_CHANGE(IDC_RAND_OFF_EDIT, OnChangeRandOffEdit)
	ON_EN_CHANGE(IDC_RAND_ON_EDIT, OnChangeRandOnEdit)
	ON_BN_CLICKED(IDC_AUTO_CHECK, OnAutoCheck)
	ON_BN_CLICKED(IDC_TIMER_STOP_CHECK, OnTimerStopCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CHTransitionDialog 

void CHTransitionDialog::OnInmediateRadio() 
{
	m_iDelayOff = 0;
	m_iDelayOn = 0;
	m_iRandOff = 0;
	m_iRandOn = 0;
	m_iEnabledMode = 0;
	UpdateData(FALSE);

	GetDlgItem(IDC_DELAY_ON_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_DELAY_OFF_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RAND_ON_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RAND_OFF_EDIT)->EnableWindow(FALSE);
		
}

void CHTransitionDialog::OnDelayOnRadio() 
{
	GetDlgItem(IDC_DELAY_OFF_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RAND_ON_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RAND_OFF_EDIT)->EnableWindow(FALSE);

	GetDlgItem(IDC_DELAY_ON_EDIT)->EnableWindow(TRUE);

	m_iDelayOff = 0;
	m_iRandOff = 0;
	m_iRandOn = 0;
	m_iDelayOn = m_iDelay;
	m_iEnabledMode = 1;

	UpdateData(FALSE);
}

void CHTransitionDialog::OnDelayOffRadio() 
{
	GetDlgItem(IDC_DELAY_ON_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RAND_ON_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RAND_OFF_EDIT)->EnableWindow(FALSE);

	GetDlgItem(IDC_DELAY_OFF_EDIT)->EnableWindow(TRUE);
	
	
	m_iDelayOn = 0;
	m_iRandOff = 0;
	m_iRandOn = 0;
	m_iDelayOff = m_iDelay;
	m_iEnabledMode = 2;
	UpdateData(FALSE);
}

void CHTransitionDialog::OnRandOnRadio() 
{
	GetDlgItem(IDC_DELAY_ON_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_DELAY_OFF_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RAND_OFF_EDIT)->EnableWindow(FALSE);

	GetDlgItem(IDC_RAND_ON_EDIT)->EnableWindow(TRUE);
	
	m_iDelayOff = 0;
	m_iDelayOn = 0;
	m_iRandOff = 0;
	m_iRandOn = m_iDelay;
	m_iEnabledMode = 3;
	UpdateData(FALSE);
}

void CHTransitionDialog::OnRandOffRadio() 
{
	GetDlgItem(IDC_DELAY_ON_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_DELAY_OFF_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_RAND_ON_EDIT)->EnableWindow(FALSE);

	GetDlgItem(IDC_RAND_OFF_EDIT)->EnableWindow(TRUE);
	
	m_iDelayOn = 0;
	m_iRandOn = 0;
	m_iRandOff = m_iDelay;
	m_iEnabledMode = 4;
	UpdateData(FALSE);
}


BOOL CHTransitionDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	switch(m_iEnabledMode)
	{
	case 1:
		{
			GetDlgItem(IDC_DELAY_ON_EDIT)->EnableWindow(TRUE);
			m_iDelayOn = m_iDelay;
			break;
		}
	case 2:
		{
			GetDlgItem(IDC_DELAY_OFF_EDIT)->EnableWindow(TRUE);
			m_iDelayOff = m_iDelay;
			break;
		}
	case 3:
		{
			GetDlgItem(IDC_RAND_ON_EDIT)->EnableWindow(TRUE);
			m_iRandOn = m_iDelay;
			break;
		}
	case 4:
		{
			GetDlgItem(IDC_RAND_OFF_EDIT)->EnableWindow(TRUE);
			m_iRandOff = m_iDelay;
			break;
		}
	}
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CHTransitionDialog::OnChangeDelayOffEdit() 
{
	UpdateData(TRUE);
	m_iDelay = m_iDelayOff;
}

void CHTransitionDialog::OnChangeDelayOnEdit() 
{
	UpdateData(TRUE);
	m_iDelay = m_iDelayOn;	
}

void CHTransitionDialog::OnChangeRandOffEdit() 
{
	UpdateData(TRUE);
	m_iDelay = m_iRandOff;
}

void CHTransitionDialog::OnChangeRandOnEdit() 
{
	UpdateData(TRUE);
	m_iDelay = m_iRandOn;
}

void CHTransitionDialog::OnAutoCheck() 
{
	UpdateData(TRUE);	
}

void CHTransitionDialog::OnTimerStopCheck() 
{
	UpdateData(TRUE);	
}
