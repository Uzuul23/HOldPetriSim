// HSymDialog.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "HSymDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CHSymDialog 


CHSymDialog::CHSymDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CHSymDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHSymDialog)
	m_iConMod = 0;
	//}}AFX_DATA_INIT
}


void CHSymDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHSymDialog)
	DDX_Radio(pDX, IDC_IGNORE_RADIO, m_iConMod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHSymDialog, CDialog)
	//{{AFX_MSG_MAP(CHSymDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CHSymDialog 
