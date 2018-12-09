/**************************************************************************
	OutputWnd.h
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#pragma once

// COutputList

class COutputList : public CEdit
{

public:
	COutputList();


public:
	virtual ~COutputList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};


// COutputWnd

class COutputWnd : public CDockablePane
{
	DECLARE_DYNAMIC(COutputWnd)

public:
	COutputWnd();
	virtual ~COutputWnd();

	void SetText(LPCTSTR pszText);

private:
	CFont m_Font;
	COutputList m_wndOutput;
	void AdjustHorzScroll(CListBox& wndListBox);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};
/*************************************************************************/
