/**************************************************************************
	HZoom.h
**************************************************************************/

#ifndef AFX_HZOOM_H__6BFCBEE1_2572_11D2_93B8_000000000000__INCLUDED_
#define AFX_HZOOM_H__6BFCBEE1_2572_11D2_93B8_000000000000__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// CHZoom view

class CHZoom : public CScrollView
{
	DECLARE_DYNCREATE(CHZoom)
protected:
	CHZoom();

// Operations
public:
	// Overridden CScrollView member functions
	void SetZoomSizes(SIZE sizeTotal, const SIZE& sizePage = sizeDefault
		, const SIZE& sizeLine = sizeDefault);
	void CenterOnLogicalPoint(CPoint ptCenter);
	CPoint GetLogicalCenterPoint(void);

	// Zooming functions
	typedef enum {MODE_ZOOMOFF, MODE_ZOOMIN, MODE_ZOOMOUT} ZoomMode_;
	void      SetZoomMode(ZoomMode_ zoomMode);
	ZoomMode_ GetZoomMode() {return m_zoomMode;};
	int       DoZoomIn    (CRect &rect);
	int       DoZoomIn    (CPoint *point = NULL, float delta = 1.25);
	int       DoZoomOut   (CPoint *point = NULL, float delta = 1.25);
	int       DoZoomFull  ();

	// Override this to get notified of zoom scale change
	virtual void NotifyZoom(void) {};

	// Zooming utility functions
	void      ViewDPtoLP (LPPOINT lpPoints, int nCount = 1);
	void      ViewLPtoDP (LPPOINT lpPoints, int nCount = 1);
	void      ClientToDevice(CPoint &point);
	void      NormalizeRect(CRect &rect);
	void 		 DrawBox(CDC &dc, CRect &rect, BOOL xor = TRUE);
	void 		 DrawLine(CDC &dc, const int &x1, const int &y1, 
							 const int &x2, const int &y2, BOOL xor = TRUE);

// Implementation
protected:
	virtual ~CHZoom();
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif //_DEBUG
	virtual void OnDraw(CDC* pDC) {/*CScrollView::OnDraw(pDC);*/}; // Pure Virtual
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

private:
	// Internally called functions
	void 		 PersistRatio(const CSize &orig, CSize &dest, CPoint &remainder);
	void      CalcBars(void);

	// Private member variables
	ZoomMode_ m_zoomMode;
	BOOL      m_bCaptured;
	CRect     m_ptDragRect;
	CSize     m_origTotalDev;           // Origional total size in device units
	CSize     m_origPageDev;            // Origional per page scroll size in device units
	CSize     m_origLineDev;            // Origional per line scroll size in device units
	
	HCURSOR   m_hZoomCursor;

public:
	float     m_zoomScale;
	float GetZoomScale(){return m_zoomScale;};
	void DoZoomNorm(void);
	// Generated message map functions
	//{{AFX_MSG(CHZoom)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // AFX_HZOOM_H__6BFCBEE1_2572_11D2_93B8_000000000000__INCLUDED_
