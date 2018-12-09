/**************************************************************************
	PropertiesWnd.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "stdafx.h"
#include "PropertiesWnd.h"
#include "HDrawObject.h"
#include "PetriSimDoc.h"

IMPLEMENT_DYNAMIC(CPropertiesWnd, CDockablePane)

CPropertiesWnd::CPropertiesWnd() : m_pObject(0)
{
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED
	, &CPropertiesWnd::OnPropertyChanged)
END_MESSAGE_MAP()

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndPropList.Create(WS_VISIBLE|WS_CHILD, rectDummy, this, 1))
	{
		TRACE0("failed to create properties list\n");
		return -1;
	}

	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	m_wndPropList.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE
		|SWP_NOACTIVATE|SWP_NOZORDER);
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

LRESULT CPropertiesWnd::OnPropertyChanged(WPARAM wp, LPARAM lp)
{
	CMFCPropertyGridProperty *pProp = (CMFCPropertyGridProperty*)lp;

	ASSERT_VALID(this);
	ASSERT_VALID(pProp);

	if (m_pObject == 0)
		return 0;

	ASSERT_VALID(m_pObject);
	ASSERT_VALID(m_pObject->m_pDoc);
	ASSERT_VALID(m_pObject->m_pObject);

	_variant_t var = pProp->GetValue();
	var.ChangeType(VT_BSTR);
	
	GV_ITEM item;
	item.lParam = pProp->GetData();
	item.szText = var.bstrVal;

	CUpdateRectObject curect;
	curect.m_cRect.UnionRect(curect.m_cRect,
	m_pObject->m_pObject->GetBoundingRect());

	CSingleLock sLock(&m_pObject->m_pDoc->m_cCSection, true);

	if(!m_pObject->m_pObject->SetProperty(&item))
	{
		pProp->SetValue((_variant_t)item.szText);
		m_wndPropList.Invalidate();
	}
	
	sLock.Unlock();

	curect.m_cRect.UnionRect(curect.m_cRect
		, m_pObject->m_pObject->GetBoundingRect());

	m_pObject->m_pDoc->UpdateAllViews(0, 1L, &curect);

	return 0;
}

void CPropertiesWnd::SetObject(CPropertiesObject * pObject/* = 0 */)
{
	if (pObject != 0 && m_pObject == pObject)
	{
		UpdateList();
	}
	else if(pObject)
	{
		ASSERT_VALID(pObject);
		ASSERT_VALID(pObject->m_pDoc);
		ASSERT_VALID(pObject->m_pObject);
		m_pObject = pObject;

		InitList();
	}
	else if(m_pObject)
	{
		m_pObject = 0;
		m_wndPropList.RemoveAll();
		m_wndPropList.Invalidate();
	}
}

void CPropertiesWnd::UpdateList()
{
	if (m_pObject == 0)
		return;

	GV_ITEM item;
	CMFCPropertyGridProperty* pProp = 0;

	for (int loop=1; ; loop++)
	{
		item.lParam = loop;

		if (!m_pObject->m_pObject->GetProperty(&item))
		{
			break;
		}

		pProp = m_wndPropList.FindItemByData(loop);

		if (pProp && (_variant_t)item.szText != pProp->GetValue())
		{
			pProp->SetValue((_variant_t)item.szText);
		}
	}
}

void CPropertiesWnd::InitList()
{
	m_wndPropList.RemoveAll();

	if (m_pObject == 0)
		return;
	

	GV_ITEM item;
	CMFCPropertyGridProperty* pProp = 0;

	for (int loop=1; ; loop++)
	{
		item.lParam = loop;
		item.array.RemoveAll();

		if (!m_pObject->m_pObject->GetProperty(&item))
		{
			break;
		}

		switch (item.type)
		{
		case GVT_DEFAULT:
			{
				pProp = new CMFCPropertyGridProperty(
					m_pObject->m_pObject->GetPropertyText(loop)
					, (_variant_t)item.szText, _T(""), loop);

				m_wndPropList.AddProperty(pProp);
				break;
			}
		case GVT_ITEMS:
			{
				pProp = new CMFCPropertyGridProperty(
					m_pObject->m_pObject->GetPropertyText(loop)
					, (_variant_t)item.szText, _T(""), loop);

				for (INT_PTR loop2=0; loop2<item.array.GetCount(); loop2++)
				{
					pProp->AddOption(item.array[loop2]);
				}

				pProp->AllowEdit(FALSE);
				m_wndPropList.AddProperty(pProp);
				break;
			}
		case GVT_BOOL:
			{
				pProp = new CMFCPropertyGridProperty(
					m_pObject->m_pObject->GetPropertyText(loop)
					, (_variant_t)item.szText, _T(""), loop);

				pProp->AddOption(_T("TRUE"));
				pProp->AddOption(_T("FALSE"));

				pProp->AllowEdit(FALSE);
				m_wndPropList.AddProperty(pProp);
				break;
			}
		}
	}
}