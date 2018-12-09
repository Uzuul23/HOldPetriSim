/**************************************************************************
	MainFrm.h
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#if !defined(AFX_MAINFRM_H__1DD387A9_E907_11D1_94E3_9187B43D300C__INCLUDED_)
#define AFX_MAINFRM_H__1DD387A9_E907_11D1_94E3_9187B43D300C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "HSimToolbar.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame();
	virtual ~CMainFrame();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CPropertiesWnd& GetPropertiesWnd() { return m_wndProperties; };
	COutputWnd& GetOutputWnd() { return m_wndOutput; };

	void StopAnimTimer();
	static void CALLBACK TimeProc(UINT uID, UINT uMsg, DWORD dwUser
		, DWORD dw1, DWORD dw2);
	static void CALLBACK EXPORT TimerProc(HWND hWnd, UINT nMsg
		,UINT nIDEvent, DWORD dwTime);
	void StartAnimTimer();

private:
	CMFCMenuBar m_wndMenuBar;
	CMFCStatusBar m_wndStatusBar;
	CMFCToolBar m_wndToolBar;
	CMFCToolBar m_wndEditBar;
	CHSimToolbar m_wndSimBar;
	COutputWnd m_wndOutput;
	CPropertiesWnd m_wndProperties;
	CImageList m_cBtnImageList;
	MMRESULT m_mmTimer;
	int m_nTimerRef;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg void OnTimer( UINT nIDEvent );
	afx_msg long OnUpdateNet(UINT wParam,LONG lParam);
	afx_msg long OnUpdateOutput(UINT wParam,LONG lParam);
	afx_msg long OnAnimStep(UINT wParam,LONG lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
	
};

#endif // !defined(AFX_MAINFRM_H__1DD387A9_E907_11D1_94E3_9187B43D300C__INCLUDED_)
