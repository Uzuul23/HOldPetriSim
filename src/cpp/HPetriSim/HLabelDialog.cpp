/**************************************************************************
	HLabelDialog.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "stdafx.h"
#include "Resource.h"
#include "HLabelDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

CHLabelDialog::CHLabelDialog(CString string)
	: CDialog(CHLabelDialog::IDD, NULL)
{
	m_csText = string;
}


void CHLabelDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEXT_EDIT, m_csText);
	DDV_MaxChars(pDX, m_csText, 50);
}


BEGIN_MESSAGE_MAP(CHLabelDialog, CDialog)
END_MESSAGE_MAP()

CString CHLabelDialog::GetText()
{
	return m_csText;
}
