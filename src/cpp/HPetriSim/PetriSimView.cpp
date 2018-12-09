/**************************************************************************
	PetriSimView.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "stdafx.h"
#include "HDrawObject.h"
#include "PetriSim.h"
#include "PetriSimDoc.h"
#include "HSimToolBar.h"
#include "MainFrm.h"
#include "cdib.h"
#include "mfxWhlPan.h"
#include "PetriSimView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

/*************************************************************************/
// CPetriSimView
/*************************************************************************/

IMPLEMENT_DYNCREATE(CPetriSimView, CHZoom)

BEGIN_MESSAGE_MAP(CPetriSimView, CHZoom)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_SETCURSOR()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDBLCLK()
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CPetriSimView::OnUpdateEditCopyCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CPetriSimView::OnUpdateEditCopyCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CPetriSimView::OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_COPY, &CPetriSimView::OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, &CPetriSimView::OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, &CPetriSimView::OnEditPaste)
	ON_COMMAND(ID_ZOOM_IN, &CPetriSimView::OnZoomIn)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_IN, &CPetriSimView::OnUpdateZoomIn)
	ON_COMMAND(ID_ZOOM_NORM, &CPetriSimView::OnZoomNorm)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_NORM, &CPetriSimView::OnUpdateZoomNorm)
	ON_COMMAND(ID_ZOOM_OUT, &CPetriSimView::OnZoomOut)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_OUT, &CPetriSimView::OnUpdateZoomOut)
	ON_COMMAND(ID_ZOOM_PORT, &CPetriSimView::OnZoomPort)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_PORT, &CPetriSimView::OnUpdateZoomPort)
	ON_COMMAND(ID_CONTEXT_POPUP, &CPetriSimView::OnContextPopup)
	ON_COMMAND(ID_CONTEXT_PUSHBACK, &CPetriSimView::OnContextPushback)
	ON_COMMAND(ID_GROUP, &CPetriSimView::OnGroup)
	ON_COMMAND(ID_UNGROUP, &CPetriSimView::OnUngroup)
	ON_UPDATE_COMMAND_UI(ID_CONTEXT_POPUP, &CPetriSimView::OnUpdateContextPopup)
	ON_UPDATE_COMMAND_UI(ID_CONTEXT_PUSHBACK, &CPetriSimView::OnUpdateContextPushback)
	ON_UPDATE_COMMAND_UI(ID_GROUP, &CPetriSimView::OnUpdateGroup)
	ON_UPDATE_COMMAND_UI(ID_UNGROUP, &CPetriSimView::OnUpdateUngroup)
	ON_COMMAND(ID_STORE_SCREEN, &CPetriSimView::OnStoreScreen)
	ON_COMMAND(ID_STORE_CLIENT, &CPetriSimView::OnStoreClient)
	ON_COMMAND_RANGE(ID_CONTEXT_COMMAND1,ID_CONTEXT_COMMAND20, &CPetriSimView::OnContextCommand)
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPetriSimView::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_MOUSEPOS, &CPetriSimView::OnUpdateIndicatorMousePos)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_ZOOMSCALE,&CPetriSimView::OnUpdateIndicatorZoomScale)
END_MESSAGE_MAP()

CPetriSimView::CPetriSimView() : m_cpMousePos(0,0)
{
	m_nClipboardFormat = ::RegisterClipboardFormat(_T("PetriSimFormat"));
	m_pWheelWnd = 0;
}

CPetriSimView::~CPetriSimView()
{
}

BOOL CPetriSimView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

void CPetriSimView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

void CPetriSimView::OnDraw(CDC* pDC)
{
	CPetriSimDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDC dc;
	CDC* pDrawDC = pDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap;
	// nur das Rechteck zeichnen, das neu gezeichnet werden muß
	CRect client;
	pDC->GetClipBox(client);
	//Bei einem Zoomfaktor >> 100% ergeben sich unerklärliche Aussetzer
	//am Rand beim Scrollen. Das Vergrößern der Update Region um 1 Pixel
	//scheint das Problem zu beheben. Rundungsfehler?
	client.InflateRect(1,1);
	CRect rect(client);
	pDC->LPtoDP(rect);
	if (!pDC->IsPrinting())
	{
		// um das Neuzeichnen schnell aussehen zu lassen, wird zunächst in ein Bitmap
		// gezeichnet, welche nicht am Bildschirm angezeigt wird
		if (dc.CreateCompatibleDC(pDC))
		{
			if (bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))
			{
				OnPrepareDC(&dc, NULL);
				pDrawDC = &dc;

				// Ursprung ausgleichen, da das Bitmap ein Teil des gesammten Zeichenvorgangs ist
				dc.OffsetViewportOrg(-rect.left, -rect.top);
				pOldBitmap = dc.SelectObject(&bitmap);
				dc.SetBrushOrg(rect.left % 8, rect.top % 8);

				// kann auch in dasselbe Rechteck fallen
				dc.IntersectClipRect(client);
			}
		}
	}
	// Hintergrund zeichnen
	CBrush brush;
	if (!brush.CreateSolidBrush(pDoc->GetBackColor()))
		return;

	brush.UnrealizeObject();
	pDrawDC->FillRect(client, &brush);
	
	// sind zwischen den Gridpoints weniger als 3 Pixel wird
	// das Grid nicht mehr dargestellt
	CRect gcrect;
	GetClientRect(&gcrect);
	//in die Bitmap alle Objekte zeichnen lasssen
	pDoc->Draw(pDrawDC,client,((float)gcrect.right / 
		((float)pDoc->GetGrid() * GetZoomScale()) > 
		(float)(gcrect.right / 4)) ? false : true);
	//Bitmap in Zeichenkontext kopieren
	if (pDrawDC != pDC)
	{
		pDC->SetViewportOrg(0, 0);
		pDC->SetWindowOrg(0,0);
		pDC->SetMapMode(MM_TEXT);
		dc.SetViewportOrg(0, 0);
		dc.SetWindowOrg(0,0);
		dc.SetMapMode(MM_TEXT);
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
			&dc, 0, 0, SRCCOPY);
		dc.SelectObject(pOldBitmap);
	}
}

void CPetriSimView::OnInitialUpdate()
{
	CHZoom::OnInitialUpdate();

	// ZU ERLEDIGEN: Gesamte Größe dieser Ansicht berechnen
	SetZoomSizes(GetDocument()->GetDocSize());
//	m_posSel = NULL;
	m_iDrawMode = DR_SELECT;
}

BOOL CPetriSimView::OnPreparePrinting(CPrintInfo* pInfo)
{
	GetDocument()->DeselectAll();
	pInfo->m_rectDraw;
	return DoPreparePrinting(pInfo);
}

void CPetriSimView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	
}

void CPetriSimView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	
}

void CPetriSimView::OnPrint( CDC* pDC, CPrintInfo* pInfo )
{
	OnDraw(pDC);
	CView::OnPrint( pDC, pInfo );
}

#ifdef _DEBUG
void CPetriSimView::AssertValid() const
{
	CHZoom::AssertValid();
}

void CPetriSimView::Dump(CDumpContext& dc) const
{
	CHZoom::Dump(dc);
}

CPetriSimDoc* CPetriSimView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPetriSimDoc)));
	return (CPetriSimDoc*)m_pDocument;
}
#endif //_DEBUG

void CPetriSimView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CPetriSimDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	//umrechnen
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	//ersten Punkt merken
	m_cpFirstPoint = point;
	//Startpunkt für Line
	m_cpLine = point;
	//Startpunkt für Dragger 
	m_crDragger.SetRect(point.x, point.y, point.x, point.y);
	SetCapture(); // schnapp die Maus
	switch(pDoc->GetDrawMode())
	{	
	case DR_SELECT:
		pDoc->Select(point);
		//m_iResizePoint
		pDoc->CanMove(point);
		// Cursor setzen
		if(pDoc->m_cplSelected.GetCount() == 1)
			SetCursor(AfxGetApp()->LoadStandardCursor
			(pDoc->m_cplSelected.GetHead()->GetCursor(pDoc->m_iHandle)));
		break;
	case DR_PAN:
		MfxTrackAutoPan(this);
		break;
	case DR_ZOOM_OUT:	
			DoZoomOut(&point);//, 1.1F);
			break;
	case DR_ZOOM_IN: break;
	case DR_MOVE: break;
	case DR_RESIZE:break;
	case DR_CONNECTOR: break;
	default:
		pDoc->DeselectAll();
		pDoc->NewObject(m_cpFirstPoint,point,&dc);
		break;
	}	
	CScrollView::OnLButtonDown(nFlags, point);
}

void CPetriSimView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (GetCapture() != this)return;
	CPetriSimDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	switch(pDoc->GetDrawMode())
	{		
	case DR_SELECT:
		//Box löschen
		DrawDragger(dc,m_crDragger);
		if(m_cpFirstPoint != point)
		{
			// Die Mauspunkte sind unterschiedlich. Es soll also
			// mit der Box ausgewählt werden. Ok, es können
			// also auch mehrere Objekte selektiert werden
			// wenn sie innerhalb der Box sind.
			m_crDragger.NormalizeRect();
			pDoc->Select(m_crDragger);
		}
		break;
	case DR_CONNECTOR:
		DrawLine(dc,m_cpFirstPoint,m_cpLine);
		pDoc->NewObject(m_cpFirstPoint,point,&dc);
		break;
	case DR_ZOOM_IN:
		DrawDragger(dc,m_crDragger);
		if(m_cpFirstPoint != point)
			DoZoomIn(m_crDragger);
		else
			DoZoomIn(&point);//, 1.1F);
		break;
	case DR_MOVE:
	case DR_RESIZE:
		pDoc->UpdateOtherViews(this);
		break;
	}
	// Auswahlmodus wieder einschalten
	if((pDoc->GetDrawMode() == DR_MOVE) ||
		(pDoc->GetDrawMode() == DR_RESIZE))pDoc->SetDrawMode(DR_SELECT);

	ReleaseCapture();   // Freigeben der Maus.
	CScrollView::OnLButtonUp(nFlags, point);
}

void CPetriSimView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CPetriSimDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	m_cpMousePos = point;

	if (GetCapture() != this)
		return;
	switch(pDoc->GetDrawMode())
	{
	case DR_ZOOM_OUT: break;
	case DR_ZOOM_IN:
		{
			// altes Rechteck löschen
			DrawDragger(dc,m_crDragger);
			// neues Rechteck zeichnen
			m_crDragger.BottomRight() = point;
			DrawDragger(dc,m_crDragger);
			break;
		}
	case DR_CONNECTOR:
		{
			DrawLine(dc,m_cpFirstPoint,m_cpLine);
			m_cpLine = point;
			DrawLine(dc,m_cpFirstPoint,m_cpLine);
			break;
		}
	case DR_SELECT:	//Dragger Modus
		{
			// altes Rechteck löschen
			DrawDragger(dc,m_crDragger);
			// neues Rechteck zeichnen
			m_crDragger.BottomRight() = point;
			DrawDragger(dc,m_crDragger);
		}
	case DR_POSITION:
		{
			break;
		}
	case DR_MOVE:
		{
			pDoc->MoveObject(CPoint(point.x - m_cpFirstPoint.x,
				point.y - m_cpFirstPoint.y), this);
			m_cpFirstPoint = point;
			break;
		}
	case DR_RESIZE:
		{
			pDoc->ResizeObject(CPoint(point.x - m_cpFirstPoint.x,
				point.y - m_cpFirstPoint.y), this, false);
			m_cpFirstPoint = point;
			break;
		}
	default:
		{
			pDoc->ResizeObject(CPoint(point.x - m_cpFirstPoint.x,
				point.y - m_cpFirstPoint.y), this,true);
			m_cpFirstPoint = point;
			break;
		}
	}
	CScrollView::OnMouseMove(nFlags, point);
}

void CPetriSimView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_bPolyLineAddPoint = false;
	switch(nChar)
	{
	case 38:// Arrow Up
		GetDocument()->StepMove(STEP_MOVE_UP);
		break;
	case 40:// Arrow Down
		GetDocument()->StepMove(STEP_MOVE_DOWN);
		break;
	case 37:// Arrow Left
		GetDocument()->StepMove(STEP_MOVE_LEFT);
		break;
	case 39:// Arrow Right 
		GetDocument()->StepMove(STEP_MOVE_RIGHT);
		break;
	case 46: //Entf Taste
		GetDocument()->DeleteSelected();
		break;
	case 27: //Esc Taste
		GetDocument()->DeselectAll();
		break;
	}
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

int CPetriSimView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

//	m_posSel = NULL;
	m_iDrawMode = DR_SELECT;
	return 0;
}

void CPetriSimView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CPetriSimDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPoint cpoint(point);
	ScreenToClient(&point);
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);
	pDoc->Select(point);
	
	if(pDoc->m_cplSelected.GetCount() == 1)
	{
		CMenu menu;
		menu.LoadMenu(IDR_HPSTYPE);
		// Das Object kann spezielle Items einfügen
		// Reserviert sind ID_CONTEXT_COMMAND1 bis
		// ID_CONTEXT_COMMAND20
		pDoc->m_cplSelected.GetHead()->InitMenu
			(menu.GetSubMenu(1),point);
		menu.GetSubMenu(1)->TrackPopupMenu(TPM_LEFTALIGN | 
		TPM_RIGHTBUTTON, cpoint.x, cpoint.y, this);
		pDoc->DetachExplObject();
	}
	else if(pDoc->GetSimMode())
	{
		//Menu der Ansicht aufrufen
		CMenu menu;
		menu.LoadMenu(IDR_HPSTYPE);
		menu.GetSubMenu(7)->TrackPopupMenu(TPM_LEFTALIGN | 
		TPM_RIGHTBUTTON, cpoint.x, cpoint.y, this);
	}
	else
	{
		//Menu der Ansicht aufrufen
		CMenu menu;
		menu.LoadMenu(IDR_HPSTYPE);
		menu.GetSubMenu(1)->TrackPopupMenu(TPM_LEFTALIGN | 
		TPM_RIGHTBUTTON, cpoint.x, cpoint.y, this);
	}	
}

void CPetriSimView::DrawDragger(CDC & dc, CRect & rect,BOOL xor)
{
	CPen cpen;
	// alten Gerätekontext sichern
	dc.SaveDC();
	if(xor) 
	{
		dc.SetROP2(R2_NOTXORPEN);
		cpen.CreatePen(PS_DASH,  0, RGB(0, 0, 0));
	} 
	else 
	{
		cpen.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	}
	dc.SelectObject(&cpen);
	// Die Linien zeichnen oder löschen unsere GripperBox
	dc.MoveTo(rect.left,  rect.top);
	dc.LineTo(rect.right, rect.top);
	dc.LineTo(rect.right, rect.bottom);
	dc.LineTo(rect.left,  rect.bottom);
	dc.LineTo(rect.left,  rect.top);

	// Gerätekontext wieder herstellen
	dc.RestoreDC(-1);
}

void CPetriSimView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// Das Dokument hat diese Ansicht informiert,
	// daß sich einige Daten geändert haben.
	CPetriSimDoc* pDoc = GetDocument();
	if (pHint)
	{
		CRect crect;
		CClientDC dc(this);
		OnPrepareDC(&dc);
		CHDrawObject* object;
		POSITION pos;
		CUpdateRectObject* pUpdateRect;
		CTypedPtrList<CPtrList,CHDrawObject*>* ptrUpdateList;
		switch(lHint)
		{
		case 0L:
			// Da ein Objekt geändert wurde
			// wird sein umgebendes Rechteck neu gezeichnet.
			object = dynamic_cast <CHDrawObject*> (pHint); 
			if(!object)return;
			crect = object->GetBoundingRect(&dc);
			dc.LPtoDP(&crect);
			InvalidateRect(&crect, FALSE);
			return;
		case 1L:
			//das aufrufende Document kennt die größe
			//der zu aktuallisierenden Region
			pUpdateRect = dynamic_cast <CUpdateRectObject*> (pHint);
			if(!pUpdateRect)return;
			crect.CopyRect(&pUpdateRect->m_cRect);
			dc.LPtoDP(&crect);
			InvalidateRect(crect, false);
			return;
		case 4L:
			//Alle in einer Liste übergebenen Objekte sollen
			//aktualisiert werden
			ptrUpdateList = dynamic_cast
				<CTypedPtrList<CPtrList,CHDrawObject*>*> (pHint); 
			if(!ptrUpdateList)return;//Downcast gescheitert
			pos = ptrUpdateList->GetHeadPosition();
			while (pos != NULL)
			{
				object = ptrUpdateList->GetNext(pos);
				crect = object->GetBoundingRect();
				dc.LPtoDP(&crect);
				InvalidateRect(&crect, FALSE);			
			}
			return;
		case 6L:
			return;
		default: ASSERT(false);//unbekannter Code
		}
	}
	else
	{
		switch(lHint)
		{
		case 3L:
			SetZoomSizes (GetDocument()->GetDocSize());
			return;
		case 6L:
			return;
		}
	}
	// Da wir nicht feststellen können was sich geändert hat,
	// muß alles neu gezeichnet werden.
	Invalidate(false);
	return;
}

void CPetriSimView::DrawLine (CDC &dc,POINT & start,POINT & end,BOOL xor)
{	
	CPen pen;
	dc.SaveDC();
	if(xor) 
	{
		dc.SetROP2(R2_NOTXORPEN);
		pen.CreatePen(PS_DASH,  0, RGB(0, 0, 0));
	}
	else 
	{
		pen.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	}
	dc.SelectObject(&pen);

	dc.MoveTo(start);
	dc.LineTo(end);

	dc.RestoreDC(-1);
}

void CPetriSimView::OnUpdateEditCopyCut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsSelected());
}

void CPetriSimView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(::IsClipboardFormatAvailable(m_nClipboardFormat));
}

void CPetriSimView::OnEditCopy()
{
	{
		CSharedFile	memFile;
		CArchive ar(&memFile, CArchive::store|CArchive::bNoFlushOnDelete);

		// serialize data to archive object
		DoCutCopyPaste(ar, FALSE);

		ar.Flush();

		HGLOBAL hData = memFile.Detach();
		if (OpenClipboard())
		{
			::SetClipboardData(m_nClipboardFormat, hData);
			CloseClipboard();
		}
		else
			AfxMessageBox(CG_IDS_CANNOT_OPEN_CLIPBOARD);
	}
}

void CPetriSimView::OnEditCut()
{
	{
		CSharedFile	memFile;
		CArchive ar(&memFile, CArchive::store|CArchive::bNoFlushOnDelete);

		// serialize data to archive object
		DoCutCopyPaste(ar, TRUE);

		ar.Flush();

		HGLOBAL hData = memFile.Detach();
		if (OpenClipboard())
		{
			::SetClipboardData(m_nClipboardFormat, hData);
			CloseClipboard();
		}
		else
			AfxMessageBox(CG_IDS_CANNOT_OPEN_CLIPBOARD);
	}
}

void CPetriSimView::OnEditPaste()
{
	{
		if (OpenClipboard())
		{
			HANDLE hData = ::GetClipboardData(m_nClipboardFormat);
			if (hData != NULL)
			{
				CSharedFile memFile;
				memFile.SetHandle(hData,FALSE);
				CArchive ar(&memFile, CArchive::load);
				// serialize data to document
				DoCutCopyPaste(ar, FALSE);
				ar.Close();
				memFile.Detach();
			}
			else
				AfxMessageBox(CG_IDS_CANNOT_GET_CLIPBOARD_DATA);
			CloseClipboard();
		}
		else
			AfxMessageBox(CG_IDS_CANNOT_OPEN_CLIPBOARD);
	}

}

BOOL CPetriSimView::IsSelected()
{
	if(GetDocument()->m_cplSelected.IsEmpty( ))return false;
	return true;
}

void CPetriSimView::DoCutCopyPaste(CArchive &ar, BOOL bCut)
{
	CPetriSimDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	if(ar.IsStoring())
	{
		pDoc->StrippNet(pDoc->m_cplSelected);
		CWaitCursor wait;
		pDoc->m_cplSelected.Serialize(ar);
	}
	else
	{
		CWaitCursor wait;
		CTypedPtrList<CObList,CHDrawObject*> list;
		pDoc->DeselectAll();
		pDoc->SetMoveOnGrid(false);
		list.Serialize(ar);	
		if(!pDoc->InitializeNet(list))//Abbruch
		{
			POSITION pos = list.GetHeadPosition();
			while(pos != NULL)
			{
				delete list.GetNext(pos);			
			}
			return;
		}
		pDoc->RecalcIdent(list);
		pDoc->AddListToMembers(list);
	}
	if (bCut)
	{
		ASSERT(ar.IsStoring());
		GetDocument()->DeleteSelected();
	}
	
	if (!(ar.IsStoring() && !bCut))
	{
		pDoc->UpdateAllViews(NULL);
		pDoc->SetModifiedFlag();
	}
}

void CPetriSimView::OnZoomIn() 
{
	GetDocument()->SetDrawMode(DR_ZOOM_IN);
	GetDocument()->SetCursor(IDC_ZOOMIN);
}

void CPetriSimView::OnUpdateZoomIn(CCmdUI* pCmdUI) 
{
	if(GetDocument()->GetDrawMode() == DR_ZOOM_IN)pCmdUI->SetRadio(TRUE);
	else pCmdUI->SetRadio(FALSE);
//	if(GetDocument()->GetSymMode())pCmdUI->Enable(FALSE);
//	else pCmdUI->Enable(TRUE);
}

void CPetriSimView::OnZoomNorm()
{
	DoZoomNorm();
}

void CPetriSimView::OnUpdateZoomNorm(CCmdUI* pCmdUI) 
{
//	if(GetDocument()->GetSymMode())pCmdUI->Enable(FALSE);
//	else pCmdUI->Enable(TRUE);
}

void CPetriSimView::OnZoomOut() 
{
	GetDocument()->SetDrawMode(DR_ZOOM_OUT);
	GetDocument()->SetCursor(IDC_ZOOMOUT);
}

void CPetriSimView::OnUpdateZoomOut(CCmdUI* pCmdUI) 
{
	if(GetDocument()->GetDrawMode() == DR_ZOOM_OUT)pCmdUI->SetRadio(TRUE);
	else pCmdUI->SetRadio(FALSE);
//	if(GetDocument()->GetSymMode())pCmdUI->Enable(FALSE);
//	else pCmdUI->Enable(TRUE);
}

void CPetriSimView::OnZoomPort() 
{
	DoZoomFull();
}

void CPetriSimView::OnUpdateZoomPort(CCmdUI* pCmdUI) 
{
//	if(GetDocument()->GetSymMode())pCmdUI->Enable(FALSE);
//	else pCmdUI->Enable(TRUE);
}

void CPetriSimView::OnUpdateContextPopup(CCmdUI* pCmdUI) 
{
	if((GetDocument()->GetSimMode())|| 
	(GetDocument()->m_cplSelected.GetCount() == 0))
		pCmdUI->Enable(FALSE);
	else pCmdUI->Enable(TRUE);
}

void CPetriSimView::OnUpdateIndicatorMousePos(CCmdUI* pCmdUI) 
{
	CString cstring;
	cstring.Format(_T("P. %d, %d") ,m_cpMousePos.x, m_cpMousePos.y);
	pCmdUI->SetText(cstring);
	
}

void CPetriSimView::OnUpdateIndicatorZoomScale(CCmdUI* pCmdUI) 
{
	CString cstring;
	cstring.Format(_T("Z. %d%%") , (int)(m_zoomScale * 100));
	pCmdUI->SetText(cstring);
	
}
 
void CPetriSimView::OnUpdateContextPushback(CCmdUI* pCmdUI) 
{
	if((GetDocument()->GetSimMode())|| 
	(GetDocument()->m_cplSelected.GetCount() == 0))
		pCmdUI->Enable(FALSE);
	else pCmdUI->Enable(TRUE);
}

void CPetriSimView::OnUpdateGroup(CCmdUI* pCmdUI) 
{
	if((GetDocument()->GetSimMode())|| 
		(GetDocument()->m_cplSelected.GetCount() < 2))
		pCmdUI->Enable(FALSE);
	else pCmdUI->Enable(TRUE);
}

void CPetriSimView::OnUpdateUngroup(CCmdUI* pCmdUI) 
{
	if((GetDocument()->GetSimMode())|| 
		(GetDocument()->m_cplSelected.GetCount() == 0))
		pCmdUI->Enable(FALSE);
	else if(GetDocument()->m_cplSelected.GetHead()->
	IsKindOf( RUNTIME_CLASS(CHGroup)))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CPetriSimView::OnContextPopup() 
{
	GetDocument()->PopUp();	
}

void CPetriSimView::OnContextPushback() 
{
	GetDocument()->PushBack();
}

void CPetriSimView::OnGroup() 
{
	GetDocument()->Group();
}

void CPetriSimView::OnUngroup() 
{
	GetDocument()->UnGroup();
}

void CPetriSimView::OnContextCommand(UINT nID)
{
	// Die ID in das selektierte Object zur Bearbeitung 
	// Senden
	UINT id = nID - ID_CONTEXT_COMMAND1 + 1;
	//TRACE(_T("ContextCommand->ID: %d\n",nID);
	if(GetDocument()->m_cplSelected.GetCount() == 1)
		GetDocument()->m_cplSelected.GetHead()->DoID(id);
	//Object Neuzeichnen
	GetDocument()->UpdateAllViews(0);
}

BOOL CPetriSimView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	LPCTSTR pszCursor = GetDocument()->GetCursor(m_cpMousePos);
	if(pszCursor)
		::SetCursor(AfxGetApp()->LoadStandardCursor(pszCursor));
	else if (GetDocument()->GetCursor() != NULL)
	{
		::SetCursor(AfxGetApp()->LoadCursor(GetDocument()
		->GetCursor()));
	}
	else return CHZoom::OnSetCursor(pWnd, nHitTest, message);
	return false;
}

void CPetriSimView::InvalidateDcRect(CRect & crect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.LPtoDP(&crect);
	InvalidateRect(&crect, false);
}

void CPetriSimView::OnSetFocus(CWnd* pOldWnd) 
{
	CHZoom::OnSetFocus(pOldWnd);
	
	GetDocument()->DetachExplObject(true);
	CMainFrame* pFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	ASSERT(pFrame);
	CHSimToolbar* pBar = DYNAMIC_DOWNCAST(CHSimToolbar, 
				pFrame->GetControlBar(IDR_SYMBAR));
	if(pBar)
		pBar->GetSliderSpeed()->SetPos(GetDocument()->GetSimSpeed());
}

void CPetriSimView::OnStoreClient() 
{
	StoreScreenToBitmap(false);
}

void CPetriSimView::OnStoreScreen() 
{
	StoreScreenToBitmap(true);
}

void CPetriSimView::StoreScreenToBitmap(bool all)
{
	CPetriSimDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CFileDialog dlg(false, 0, 0, OFN_HIDEREADONLY,
		_T("Bitmap Files (*.bmp)|*.bmp|All Files|*.*||"));
	if(dlg.DoModal()==IDOK)
	{
		CString path(dlg.GetPathName());
		if(dlg.GetFileExt().IsEmpty())
			path += ".bmp";

		CFile cfile(path, CFile::modeCreate|CFile::modeWrite);

		//die Zoom Einstellungen könnten
		//anschliessend wieder Restauriert werden ! :-)
		if(all)DoZoomNorm();

		CClientDC dc(this);
		OnPrepareDC(&dc);
		CDC cdc;
		if(!cdc.CreateCompatibleDC(&dc))return;		
		
		CRect client;
		if(all)
		{
			client.SetRect(0,0,pDoc->GetDocSize().cx,
				pDoc->GetDocSize().cy);
		}
		else
		{
			GetClientRect(&client);
			dc.DPtoLP(&client);
		}
		CSize csize(client.Size());
		if(!all)dc.LPtoDP(&csize);
		CDib cdib(csize, 24);//true color
		//erzeugt eine dibsection kompatibel zum Geräte Kontext
		HBITMAP hbitmap = cdib.CreateSection(&cdc);
		if(!hbitmap)return;

		OnPrepareDC(&cdc);
		cdc.SelectObject(hbitmap);

		CBrush brush;
		if (!brush.CreateSolidBrush(pDoc->GetBackColor()))
			return;
		//in die Bitmap zeichnen
		cdc.FillRect(client, &brush);
		pDoc->Draw(&cdc, client, false);
		//Bitmap speichern
		cdib.Write(&cfile);
	}
}

void CPetriSimView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	
	CHZoom::OnLButtonDblClk(nFlags, point);
}