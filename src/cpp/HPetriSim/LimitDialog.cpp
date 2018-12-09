// LimitDialog.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "petrisim.h"
#include "LimitDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CLimitDialog 


CLimitDialog::CLimitDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLimitDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLimitDialog)
	m_strLimitText = _T("");
	//}}AFX_DATA_INIT
}


void CLimitDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLimitDialog)
	DDX_Text(pDX, IDC_EDIT_LIMITTEXT, m_strLimitText);
	//DDX_Control(pDX, IDC_HYPERLINK, m_HyperLink);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLimitDialog, CDialog)
	//{{AFX_MSG_MAP(CLimitDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CLimitDialog 

BOOL CLimitDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString strMail;
	strMail.LoadString(IDS_EMAIL);
	/*m_HyperLink.SetWindowText(strMail);
	m_HyperLink.SetURL(strMail);
	m_HyperLink.SetUnderline(TRUE);*/
	m_strLimitText.LoadString(IDS_LIMIT_TEXT);
	UpdateData(false);
	return TRUE;
}