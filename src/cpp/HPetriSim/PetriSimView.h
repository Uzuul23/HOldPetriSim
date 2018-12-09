/**************************************************************************
	PetriSimView.h
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#if !defined(AFX_H_PETRI_SYMVIEW_H__1DD387AF_E907_11D1_94E3_9187B43D300C__INCLUDED_)
#define AFX_H_PETRI_SYMVIEW_H__1DD387AF_E907_11D1_94E3_9187B43D300C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "hzoom.h"

class CWheelWnd;
class CPetriSimView : public CHZoom
{
protected:
	CPetriSimView();
	DECLARE_DYNCREATE(CPetriSimView)

public:
	virtual ~CPetriSimView();
	CPetriSimDoc* GetDocument();
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnPrint( CDC* pDC, CPrintInfo* pInfo );
	void StoreScreenToBitmap(bool all);
	void InvalidateDcRect(CRect & crect);
	BOOL IsSelected();
	void DoCutCopyPaste(CArchive &ar, BOOL bCut);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnUpdateEditCopyCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnZoomIn();
	afx_msg void OnUpdateZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnZoomNorm();
	afx_msg void OnUpdateZoomNorm(CCmdUI* pCmdUI);
	afx_msg void OnZoomOut();
	afx_msg void OnUpdateZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnZoomPort();
	afx_msg void OnUpdateZoomPort(CCmdUI* pCmdUI);
	afx_msg void OnContextPopup();
	afx_msg void OnContextPushback();
	afx_msg void OnGroup();
	afx_msg void OnUngroup();
	afx_msg void OnUpdateContextPopup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateContextPushback(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGroup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUngroup(CCmdUI* pCmdUI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnStoreScreen();
	afx_msg void OnStoreClient();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnUpdateIndicatorMousePos(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIndicatorZoomScale(CCmdUI* pCmdUI);
	afx_msg void OnContextCommand( UINT nID );

	DECLARE_MESSAGE_MAP()

private:
	int m_iDrawMode;
	UINT m_nClipboardFormat;
	UINT m_uiItemID;
	CWheelWnd* m_pWheelWnd;
	typedef CTypedPtrList<CObList,CHDrawObject*> CHDrawObjectList;
	CPoint m_cpLine;
	BOOL m_bPolyLineAddPoint;
	CRect m_crDragger;
	void DrawDragger(CDC & dc, CRect & rect,BOOL  xor = TRUE);
	void DrawLine(CDC &dc,POINT & start,POINT & end,BOOL xor = TRUE);
	CRect m_crOldRect;
//	POSITION m_posSel;
	CHDrawObject* object;
	CPoint m_cpFirstPoint;
	CPoint m_cpMousePos;
	CHPoly* m_chplObject;
};

#ifndef _DEBUG
inline CPetriSimDoc* CPetriSimView::GetDocument()
   { return (CPetriSimDoc*)m_pDocument; }
#endif

#endif // !defined(AFX_H_PETRI_SYMVIEW_H__1DD387AF_E907_11D1_94E3_9187B43D300C__INCLUDED_)
