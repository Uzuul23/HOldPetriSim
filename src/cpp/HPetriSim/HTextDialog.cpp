/**************************************************************************
	HTextDialog.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/
#include "stdafx.h"
#include "Resource.h"
#include "HTextDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

CHTextDialog::CHTextDialog(CString text)
	: CDialog(CHTextDialog::IDD, NULL)
{
	m_csText = text;
}


void CHTextDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEXT_EDIT, m_csText);
	DDV_MaxChars(pDX, m_csText, 1000);
}


BEGIN_MESSAGE_MAP(CHTextDialog, CDialog)
	ON_EN_CHANGE(IDC_TEXT_EDIT, OnChangeTextEdit)
END_MESSAGE_MAP()

void CHTextDialog::OnChangeTextEdit() 
{
	UpdateData(true);
}

BOOL CHTextDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	return TRUE; 
}

CString CHTextDialog::GetText()
{
	return m_csText;
}
