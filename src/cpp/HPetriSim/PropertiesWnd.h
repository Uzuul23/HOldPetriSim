/**************************************************************************
	PropertiesWnd.h
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/
#pragma once

class CPropertiesObject;
class CPropertiesWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CPropertiesWnd)

public:
	CPropertiesWnd();
	virtual ~CPropertiesWnd();
	void SetObject(CPropertiesObject* pObject = 0);

private:
	void UpdateList();
	void InitList();
	CMFCPropertyGridCtrl m_wndPropList;
	CPropertiesObject* m_pObject;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnPropertyChanged(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
};
/*************************************************************************/

