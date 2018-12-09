#include "stdafx.h"
#include "resource.h"
#include "mfxWhlPan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

void MfxTrackAutoPan(CWnd* pParentWnd)
{
	CWheelWnd* pWP=new CWheelWnd;
	pWP->Create(pParentWnd);
}

CWheelWnd::CWheelWnd()
{
	m_pParentWnd=NULL;
	m_bmpOrigin.LoadBitmap(IDB_AUTOPAN);
	m_bNoVertScroll=m_bNoHorzScroll=TRUE;
}

CWheelWnd::~CWheelWnd()
{
}

BOOL CWheelWnd::Create(CWnd* pParentWnd)
{
	m_pParentWnd=pParentWnd;
	CScrollBar* pVBar;
	CScrollBar* pSBar;
	DWORD dwStyle = m_pParentWnd->GetStyle();
	pVBar = m_pParentWnd->GetScrollBarCtrl(SB_VERT);
	pSBar = m_pParentWnd->GetScrollBarCtrl(SB_HORZ);
	m_bNoVertScroll=((pVBar != NULL && !pVBar->IsWindowEnabled()) 
		|| (pVBar == NULL && !(dwStyle & WS_VSCROLL)));
	m_bNoHorzScroll=((pSBar != NULL && !pSBar->IsWindowEnabled())
		|| (pSBar == NULL && !(dwStyle & WS_HSCROLL)));

	if(m_bNoVertScroll && m_bNoHorzScroll)
	{
		delete this;
		return FALSE;
	}

	if (!CreateEx(
			WS_EX_TOOLWINDOW,
			AfxRegisterWndClass(CS_SAVEBITS), NULL, WS_CLIPSIBLINGS|WS_POPUP,
			0, 0, 1, 1,
			NULL, NULL))
	{
		TRACE0("Failed to create window in CreateEx\n");
		ASSERT(0);
		return FALSE;
	}

	CRgn rgn;
	rgn.CreateEllipticRgn(2, 2, 29, 29);
 	SetWindowRgn(rgn, FALSE);

	CPoint ptOrigin;
	GetCursorPos(&ptOrigin);
	ptOrigin -= CPoint(15,15);
	SetCursorPos(ptOrigin.x, ptOrigin.y);
	m_pParentWnd->ScreenToClient(&ptOrigin);

	m_ptWheelOrg=ptOrigin;
	CRect rcWndDrag(ptOrigin.x, ptOrigin.y, ptOrigin.x+30, ptOrigin.y+30);
	m_pParentWnd->ClientToScreen(&rcWndDrag);
	MoveWindow(rcWndDrag);
	ShowWindow(SW_SHOW);
	SetCapture();
	m_nWheelTimer = SetTimer(1968, 10, NULL);

	m_nOriginBmpIndex=26;
	if(!m_bNoVertScroll)
	{
		m_nOriginBmpIndex=52;
		if(!m_bNoHorzScroll)
			m_nOriginBmpIndex=0;
	}
	return TRUE;
}

BEGIN_MESSAGE_MAP(CWheelWnd, CWnd)
	//{{AFX_MSG_MAP(CWheelWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_MBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CWheelWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(rect);

	// Bitmap size
	CPoint     ptSize;
	ptSize.x = 26;           // Get width of bitmap
	ptSize.y = 26;           // Get height of bitmap

	// Draw bitmap
	if (rect.Width() >= ptSize.x && rect.Height() >= ptSize.y)
	{
		// must have at least the first bitmap loaded before calling DrawItem
		ASSERT(m_bmpOrigin.m_hObject != NULL);     // required

		int x = rect.left + max(1, (rect.Width()-ptSize.x)/2);
		int y = rect.top + max(1, (rect.Height()-ptSize.y)/2);

		CDC dcMem;
		dcMem.CreateCompatibleDC(NULL);
		CBitmap* pOldBmp=dcMem.SelectObject(&m_bmpOrigin);
		dc.BitBlt(x, y, ptSize.x, ptSize.y, &dcMem, m_nOriginBmpIndex, 0, SRCCOPY);
		dcMem.SelectObject(pOldBmp);
	}
}

void CWheelWnd::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == m_nWheelTimer)
	{
		CPoint ptCur;
		GetCursorPos(&ptCur);
		m_pParentWnd->ScreenToClient(&ptCur);

		CPoint pt = ptCur - m_ptWheelOrg;

		int direction;
		int nScroll;

		if (abs(pt.x) > abs(pt.y))
		{
			direction = pt.x > 0 ? MFX_RIGHT : MFX_LEFT;
			nScroll = abs(pt.x) /8;
		}
		else
		{
			direction = pt.y > 0 ? MFX_DOWN : MFX_UP;
			nScroll = abs(pt.y) /8;
		}

		if (nScroll > 0)
		{
			DoScroll(direction, nScroll);
		}

		if (nScroll == 0 && abs(pt.x) < 5 && abs(pt.y) < 5)
			direction = -1;
		
		int nCursor;
		switch (direction)
		{
			case MFX_RIGHT: nCursor = IDC_PAN_RIGHT; break;
			case MFX_LEFT:	nCursor = IDC_PAN_LEFT; break;
			case MFX_UP:	nCursor = IDC_PAN_UP; break;
			case MFX_DOWN:	nCursor = IDC_PAN_DOWN; break;
			default:		nCursor = IDC_PAN_ALL; break;
		}
	
		SetCursor(nCursor);

	}
	else
		CWnd::OnTimer(nIDEvent);
}

void CWheelWnd::OnDestroy() 
{
	ReleaseCapture();
	KillTimer(m_nWheelTimer);
	::SetCursor(NULL);

	CPoint point;
	GetCursorPos(&point);
	point += CPoint(15,15);
	SetCursorPos(point.x, point.y);
	ScreenToClient(&point);

	CWnd::OnDestroy();
	TRACE0("Ignore this message: ");
	delete this;
}

void CWheelWnd::OnMButtonDown(UINT nFlags, CPoint point) 
{
	DestroyWindow();
}

void CWheelWnd::SetCursor(int nCursor)
{
	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(nCursor),RT_GROUP_CURSOR);
	HICON hCursor = ::LoadCursor(hInst,MAKEINTRESOURCE(nCursor));
	::SetCursor(hCursor);
}

BOOL CWheelWnd::DoScroll(int nDirection, int nSize)
{
    int xOrig, x, xMax;
    int yOrig, y, yMax;

    //If no scroll bars, don't do anything
    // e.g. the user has a small scale in a zoom, or a large
    //     window and there are no scroll bars.  Everything is
    //     in view, so no panning needed.
    
	if(nDirection==MFX_DOWN || nDirection==MFX_UP)
	{
		if(m_bNoVertScroll)	nSize = 0;
	}
	    
	if(nDirection==MFX_RIGHT || nDirection==MFX_LEFT)
	{
		if(m_bNoHorzScroll)	nSize = 0;
	}
	    
    // Adjust current x position based on scroll bar constraints.
    xOrig = x = m_pParentWnd->GetScrollPos(SB_HORZ);
    
    // CWnd scrolling is very different in Win32/Win16, call
    // GetScrollRange on Win16 and GetScrollLimit on Win32 to
    // make sure the correct adjusted max is retrieved not something
    // bogus.  We do this for both x and y here.
    
    xMax = m_pParentWnd->GetScrollLimit(SB_HORZ);
	if (nDirection == MFX_LEFT || nDirection == MFX_RIGHT)
	{
		if (nDirection == MFX_LEFT)
			x -= nSize;
		else 
			x += nSize;

		//Sanity checks.
		if (x < 0)	x = 0;
		else if (x > xMax)	x = xMax;
	}
    
    // Adjust current y position based on scroll bar constraints.
    yOrig = y = m_pParentWnd->GetScrollPos(SB_VERT);
    yMax = m_pParentWnd->GetScrollLimit(SB_VERT);
   
	if (nDirection == MFX_UP ||nDirection == MFX_DOWN)
	{
		if (nDirection == MFX_UP)
			y -= nSize;
		else
			y += nSize;	

		//Sanity checks.
		if (y < 0)	y = 0;
	    else if (y > yMax)	y = yMax;
    }

    // If nothing changed, just return, no work to do.
    if (x == xOrig && y == yOrig) return FALSE;
    

	// Do the Scroll!	
	if (x != xOrig) m_pParentWnd->SetScrollPos(SB_HORZ, x);
	if (y != yOrig)	m_pParentWnd->SetScrollPos(SB_VERT, y);
	m_pParentWnd->ScrollWindow(-(x-xOrig), -(y-yOrig));
	m_pParentWnd->UpdateWindow();
    return TRUE;
}

void CWheelWnd::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	DestroyWindow();	
}

void CWheelWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonUp(nFlags, point);
	DestroyWindow();
}
