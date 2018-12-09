/**************************************************************************
	PetriSimDoc.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "stdafx.h"
#include "HZoom.h"
#include "DocPropertySheetA.h"
#include "ProgDlg1.h"
#include "HThread.h"
#include "PetriSimView.h"
#include "MainFrm.h"
#include "HSimToolBar.h"
#include "HLineBuffer.h"
#include "IniSection.h"
#include "ProgressBar.h"
#include "HErrorFile.h"
#include "LimitDialog.h"
#include "PetriSimDoc.h"
#include "PetriSim.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

/*************************************************************************/
// CPetriSimDoc
/*************************************************************************/

IMPLEMENT_DYNCREATE(CPetriSimDoc, CDocument)

BEGIN_MESSAGE_MAP(CPetriSimDoc, CDocument)
	//{{AFX_MSG_MAP(CPetriSimDoc)
	ON_UPDATE_COMMAND_UI(ID_MULTI_LINE_MODE, OnUpdateMultiLineMode)
	ON_COMMAND(ID_MULTI_LINE_MODE, OnMultiLineMode)
	ON_COMMAND(ID_POSITION_MODE, OnPositionMode)
	ON_UPDATE_COMMAND_UI(ID_POSITION_MODE, OnUpdatePositionMode)
	ON_COMMAND(ID_RECT_MODE, OnRectMode)
	ON_UPDATE_COMMAND_UI(ID_RECT_MODE, OnUpdateRectMode)
	ON_COMMAND(ID_SELECT_MODE, OnSelectMode)
	ON_UPDATE_COMMAND_UI(ID_SELECT_MODE, OnUpdateSelectMode)
	ON_COMMAND(ID_TRANSITION_MODE, OnTransitionMode)
	ON_UPDATE_COMMAND_UI(ID_TRANSITION_MODE, OnUpdateTransitionMode)
	ON_COMMAND(ID_LINE_MODE, OnLineMode)
	ON_UPDATE_COMMAND_UI(ID_LINE_MODE, OnUpdateLineMode)
	ON_COMMAND(ID_CONNECTOR_MODE, OnConnectorMode)
	ON_UPDATE_COMMAND_UI(ID_CONNECTOR_MODE, OnUpdateConnectorMode)
	ON_COMMAND(ID_TEXT_MODE, OnTextMode)
	ON_UPDATE_COMMAND_UI(ID_TEXT_MODE, OnUpdateTextMode)
	ON_COMMAND(ID_STEP_MODE, OnStepMode)
	ON_UPDATE_COMMAND_UI(ID_STEP_MODE, OnUpdateStepMode)
	ON_UPDATE_COMMAND_UI(ID_UPDATE_PROPERTYS, OnUpdateUpdatePropertys)
	ON_COMMAND(ID_UPDATE_PROPERTYS, OnUpdatePropertys)
	ON_COMMAND(ID_OBJECT_PROPERTYS, OnObjectPropertys)
	ON_COMMAND(ID_EDITOR_PROPERTIES, OnEditorProperties)
	ON_COMMAND(ID_EDITOR_SHOWLABEL, OnEditorShowlabel)
	ON_UPDATE_COMMAND_UI(ID_EDITOR_SHOWLABEL, OnUpdateEditorShowlabel)
	ON_COMMAND(ID_SYM_RESET, OnSymReset)
	ON_COMMAND(ID_SYM_RUNFAST, OnSymRunfast)
	ON_UPDATE_COMMAND_UI(ID_SYM_RUNFAST, OnUpdateSymRunfast)
	ON_COMMAND(ID_SYM_RUNNORMAL, OnSymRunnormal)
	ON_UPDATE_COMMAND_UI(ID_SYM_RUNNORMAL, OnUpdateSymRunnormal)
	ON_COMMAND(ID_SYM_SINGLESTEP, OnSymSinglestep)
	ON_UPDATE_COMMAND_UI(ID_SYM_SINGLESTEP, OnUpdateSymSinglestep)
	ON_COMMAND(ID_SYM_STOP, OnSymStop)
	ON_UPDATE_COMMAND_UI(ID_SYM_STOP, OnUpdateSymStop)
	ON_COMMAND(ID_PAUSE_MODE, OnPauseMode)
	ON_UPDATE_COMMAND_UI(ID_PAUSE_MODE, OnUpdatePauseMode)
	ON_UPDATE_COMMAND_UI(ID_SYM_RESET, OnUpdateSymReset)
	ON_COMMAND(ID_SYM_DECREASESPEED, OnSymDecreasespeed)
	ON_UPDATE_COMMAND_UI(ID_SYM_DECREASESPEED, OnUpdateSymDecreasespeed)
	ON_COMMAND(ID_SYM_INCREASESPEED, OnSymIncreasespeed)
	ON_UPDATE_COMMAND_UI(ID_SYM_INCREASESPEED, OnUpdateSymIncreasespeed)
	ON_COMMAND(ID_ENABLE_WRITEOUTPUTFILE, OnEnableWriteoutputfile)
	ON_UPDATE_COMMAND_UI(ID_ENABLE_WRITEOUTPUTFILE, OnUpdateEnableWriteoutputfile)
	ON_UPDATE_COMMAND_UI(ID_EDITOR_PROPERTIES, OnUpdateEditorProperties)
	ON_COMMAND(ID_FILE_EXPORT_NET, OnFileExportNet)
	ON_COMMAND(ID_FILE_EXPORT_INTERCHANGE, OnFileExportInterchange)
	ON_COMMAND(ID_FILE_IMPORT_INTERCHANGE, OnFileImportInterchange)
	ON_COMMAND(ID_REMOVE_SELECTED, OnRemoveSelected)
	ON_UPDATE_COMMAND_UI(ID_REMOVE_SELECTED, OnUpdateRemoveSelected)
	ON_COMMAND(ID_PAN_MODE, OnPanMode)
	ON_UPDATE_COMMAND_UI(ID_PAN_MODE, OnUpdatePanMode)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STEPTIME, OnUpdateIndicatorsStepTime)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_SIMSTATUS, OnUpdateIndicatorsSimStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_SIMTIME, OnUpdateIndicatorsSimTime)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_COUNT, OnUpdateIndicatorsCount)
	ON_UPDATE_COMMAND_UI(ID_SLIDER_SIMSPEED, OnUpdateSliderSimspeed)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STEPCOUNT, OnUpdateIndicatorsStepCount)
	END_MESSAGE_MAP()


CPetriSimDoc::CPetriSimDoc() : m_mapMembers(100)
{
	//Default Font
	m_lfDefaultFont.lfHeight = -11;
	m_lfDefaultFont.lfWidth = 0;
	m_lfDefaultFont.lfEscapement = 0;
	m_lfDefaultFont.lfOrientation = FW_NORMAL;
	m_lfDefaultFont.lfWeight = false;
	m_lfDefaultFont.lfItalic = false;
	m_lfDefaultFont.lfUnderline = false;
	m_lfDefaultFont.lfStrikeOut = NULL;
	m_lfDefaultFont.lfCharSet = ANSI_CHARSET;
	m_lfDefaultFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_lfDefaultFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_lfDefaultFont.lfQuality = DEFAULT_QUALITY;
	m_lfDefaultFont.lfPitchAndFamily = VARIABLE_PITCH | FF_DONTCARE;
	StringCchCopy(m_lfDefaultFont.lfFaceName
		, sizeof(m_lfDefaultFont.lfFaceName), _T("Arial"));
	// Create the LOGICAL font to draw with (only once!)
	m_font = new CFont();
	m_font->CreateFontIndirect(&m_lfDefaultFont);

	m_cThreadEvents[CHT_EVENT_KILL] = new CEvent();
	m_cThreadEvents[CHT_EVENT_STEP] = new CEvent();
	m_cThreadEvents[CHT_EVENT_RUNNORMAL] = new CEvent();
	m_cThreadEvents[CHT_EVENT_RUNFAST] = new CEvent(false, true, 0, 0);
	m_cThreadEvents[CHT_EVENT_DEAD] = new CEvent();
	m_pAnimEvent = new CEvent();

	m_iBackColor = RGB(255,255,255);
	m_pOutputBuffer = new CHLineBuffer(250,60);
	m_iStepCount = 0;
	m_iStopTime = 1000;
	m_iStopStep = 1000;
	m_iTimerIdent = 0;
	m_bPopupMessage = true;
	m_bPopupExplorer = true;
	m_bEnableOutputFile = false;
	m_bThreadError = false;
	m_bStopNotif = false;
	m_bSingleStep = false;
	m_bRunNormal = false;
	m_bRunFast = false;
	m_bSimMode = false;
	m_uCursor = 0;
	m_bShowLabel = true;
	m_ExplObject = 0;
	m_iStepTime = 0;
	m_pSimThread = 0;
	m_iSimSpeed = 500;
	m_iSimTime = 0;
	m_iSimSampleTime = 1;
	m_iCalcTime = 0;
	m_iDrawMode = DR_SELECT;
	m_sizeDoc = CSize(1000,1500);//Layout
	m_iGrid = DRAW_GRID;//Raster
	m_bSimMode = false;
	m_bGridVisible = true;
	m_bAlignNo = true;
	m_bAlignDo = false;
	m_nIDEvent = 0;
	m_iNextFreePositionIdent = FIRST_POSITION;
	m_iNextFreeTransitionIdent = FIRST_TRANSITION;
	m_iNextFreeConnectorIdent = FIRST_CONNECTOR;
	m_nAnimPhase = 0;
}

CPetriSimDoc::~CPetriSimDoc()
{
	//clean up
	for(int i = 0;i < CHT_HANDLES_COUNT; i++)
	{
		delete m_cThreadEvents[i];
		m_cThreadEvents[i] = 0;
	}
	DeleteAll();
	delete m_pAnimEvent;
	delete m_font;
	delete m_pOutputBuffer;
}

BOOL CPetriSimDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return false;
	return true;
}

void CPetriSimDoc::Serialize(CArchive& ar)
{
	//enter critical section
	CSingleLock sLock(&m_cCSection, true);
	//m_colMembers.Serialize(ar);
	CObject::Serialize(ar);
	if (ar.IsStoring())
	{
		CProgressBar bar(_T("Write Data..."), 70, 
			m_colMembers.GetCount(), false);
		bar.SetStepRate(m_colMembers.GetCount() / 100);
		ar.WriteCount(m_colMembers.GetCount());
		POSITION pos = m_colMembers.GetHeadPosition();
		while(pos)
		{
			CObject* pObject = m_colMembers.GetNext(pos);
			ar << pObject;
			bar.StepIt();
		}
		ar << (DWORD)m_sizeDoc.cx;
		ar << (DWORD)m_sizeDoc.cy;
		ar << (DWORD)m_iNextFreeConnectorIdent;
		ar << (DWORD)m_iNextFreePositionIdent;
		ar << (DWORD)m_iNextFreeTransitionIdent;
		ar << (DWORD)m_iGrid;
		ar << (DWORD)m_iStepCount;
		ar << (DWORD)m_iStopTime;
		ar << (DWORD)m_iStopStep;
		ar << (DWORD)m_iSimSpeed;
		ar << (DWORD)m_iSimTime;
		ar << (DWORD)m_iSimSampleTime;
		ar << (DWORD)m_iBackColor;
		ar << (DWORD)0;
		ar << (DWORD)0;
		ar << (DWORD)0;
		ar << (DWORD)0;
		ar << (DWORD)0;
		ar << (DWORD)0;
		ar << (DWORD)0;
		ar << (BYTE)m_bShowLabel;
		ar << (BYTE)m_bGridVisible;
		ar << (BYTE)m_bAlignNo;
		ar << (BYTE)m_bAlignDo;
		ar << (BYTE)m_bPopupMessage;
		ar << (BYTE)m_bPopupExplorer;
		ar << (BYTE)0;
		ar << (BYTE)0;
		ar << (BYTE)0;
		ar << (BYTE)0;
	}
	else
	{
		DWORD dw;
		BYTE b;
		dw = ar.ReadCount();
		CObject* newData;
		CProgressBar bar(_T("Read Data..."), 70, dw, false);
		bar.SetStepRate(m_colMembers.GetCount() / 100);
		while (dw--)
		{
			ar >> newData;
			m_colMembers.AddTail((CHDrawObject*)newData);
			bar.StepIt();
		}
		ar >> dw;m_sizeDoc.cx = dw;
		ar >> dw;m_sizeDoc.cy = dw;
		ar >> dw;m_iNextFreeConnectorIdent = dw;
		ar >> dw;m_iNextFreePositionIdent = dw;
		ar >> dw;m_iNextFreeTransitionIdent = dw;
		ar >> dw;m_iGrid = dw;
		ar >> dw;m_iStepCount = dw;
		ar >> dw;m_iStopTime = dw;
		ar >> dw;m_iStopStep = dw;
		ar >> dw;m_iSimSpeed = dw;
		ar >> dw;m_iSimTime = dw;
		ar >> dw;m_iSimSampleTime = dw;
		ar >> dw;m_iBackColor = dw;
		ar >> dw;
		ar >> dw;
		ar >> dw;
		ar >> dw;
		ar >> dw;
		ar >> dw;
		ar >> dw;
		ar >> b;m_bShowLabel = (b != 0)? true : false;
		ar >> b;m_bGridVisible = (b != 0)? true : false;
		ar >> b;m_bAlignNo = (b != 0)? true : false;
		ar >> b;m_bAlignDo = (b != 0)? true : false;
		ar >> b;m_bPopupMessage = (b != 0)? true : false;
		ar >> b;m_bPopupExplorer = (b != 0)? true : false;
		ar >> b;
		ar >> b;
		ar >> b;
		ar >> b;
	}
}

#ifdef _DEBUG

void CPetriSimDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPetriSimDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

#endif //_DEBUG

BOOL CPetriSimDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return false;

	CString cstring(lpszPathName);
	int point = cstring.ReverseFind('.');
	m_csOutputFilePath = cstring.Left(point);
	m_csOutputFilePath += "_OUT.csv";
	m_iDrawMode = DR_SELECT;
	// Initialisierung
	if(!InitializeNet(m_colMembers))return false;
	InitMap(m_colMembers);
	UpdateAllViews(NULL,3L,NULL);
	return true;
}

void CPetriSimDoc::Draw(CDC * pDC,CRect & client,bool grid)
{
	CPetriSimApp* pApp = (CPetriSimApp*)AfxGetApp();
	CHResHolder& rRes = pApp->m_cResHolder;
	// Default Einstellungen vornehmen
	CBrush* oldbrush = pDC->SelectObject(&rRes.m_cBrushGray);
	CPen* oldpen = pDC->SelectObject(&rRes.m_cPenBlack);
	CFont* oldFont = pDC->SelectObject(&rRes.m_cfStandart);

	COLORREF oldcolor = pDC->SetTextColor(RGB(0,0,0));

	pDC->MoveTo(0,0);
	pDC->LineTo(m_sizeDoc.cx - 1,0);
	pDC->LineTo(m_sizeDoc.cx - 1,m_sizeDoc.cy - 1);
	pDC->LineTo(0,m_sizeDoc.cy - 1);
	pDC->LineTo(0,0);

	if(m_bGridVisible && !pDC->IsPrinting() && !m_bSimMode && grid)
	{
		//Raster zeichnen
		int x1 = (client.left - client.left % m_iGrid) / m_iGrid;
		int	x2 = (client.right + (m_iGrid - client.right % m_iGrid)) / m_iGrid;
		int	y1 = (client.top - client.top % m_iGrid) / m_iGrid;
		int y2 = (client.bottom + (m_iGrid - client.bottom % m_iGrid)) / m_iGrid;
		COLORREF col = RGB(0,0,0);
		for(int i = x1;i < x2;i++)
			for(int f = y1;f < y2;f++)
				pDC->SetPixelV(m_iGrid * i,m_iGrid * f,col);
	}
	//Alle Objekte zeichnen
	POSITION pos = m_colMembers.GetHeadPosition();
	while (pos != NULL)
	{
		object = m_colMembers.GetNext(pos);
		if(object->GetBoundingRect(NULL,true).IntersectRect
		(object->GetBoundingRect(NULL,true),client)
		&& ( ((object->IsKindOf( RUNTIME_CLASS(CHLabel))
		&& m_bShowLabel)) 
		|| !object->IsKindOf( RUNTIME_CLASS(CHLabel))))
		{
			object->DrawObject(pDC);
		}
	}	
	pDC->SelectObject(oldbrush);
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldcolor);
}

bool CPetriSimDoc::InitMap(CHDOBJECT_LIST & list)
{
	CProgressBar bar(_T("Init Map..."), 70,
			m_colMembers.GetCount(), false);
	bar.SetStepRate(m_colMembers.GetCount() / 100);
	if(&m_colMembers == &list)
	{
		m_mapMembers.RemoveAll();
		long hashsize = m_colMembers.GetCount();
		if(hashsize > 0)
			m_mapMembers.InitHashTable(hashsize * 4 /3);
		else
			m_mapMembers.InitHashTable(1);
	}
	POSITION pos = m_colMembers.GetHeadPosition();
	while(pos)
	{
		bar.StepIt();
		POSITION posA = pos;
		CHDrawObject* pObject = m_colMembers.GetNext(pos);
		m_mapMembers.SetAt(pObject, posA);
	}
	return true;
}

bool CPetriSimDoc::AddListToMembers(CHDOBJECT_LIST & list)
{
	CProgressBar bar(_T("Copy Objects..."), 70,
			list.GetCount(), false);
	bar.SetStepRate(list.GetCount() / 100);
	POSITION pos = list.GetHeadPosition();
	while(pos)
	{
		CHDrawObject* ptrObject = list.GetNext(pos);
		ASSERT_VALID(ptrObject);
		if(ptrObject->IsPMember() && GetAlignNo())
			SetMoveOnGrid(true);
		if(!ptrObject->IsPMember() && GetAlignDo())
			SetMoveOnGrid(true);
		ptrObject->Select(true);
		POSITION posA = m_colMembers.AddTail(ptrObject);
		m_mapMembers.SetAt(ptrObject, posA);
		m_cplSelected.AddTail(ptrObject);
		bar.StepIt();
	}
	return true;
}

bool CPetriSimDoc::DeleteSelected()
{
	CSingleLock sLock(&m_cCSection, true);
	if(m_cplSelected.IsEmpty())
		return false;
	DetachExplObject();
	SetModifiedFlag(true);
	CWaitCursor wait;
	CUpdateRectObject curect;
	CProgressBar bar(_T("Remove Objects..."), 70,
			m_cplSelected.GetCount(), false);
	bar.SetStepRate(m_cplSelected.GetCount() / 100);
	POSITION pos = m_cplSelected.GetHeadPosition();
	CMap<CHDrawObject*,CHDrawObject*, POSITION, POSITION&> map(10);
	map.InitHashTable(m_cplSelected.GetCount() * 4 / 3);
	while(pos)
	{
		POSITION posA = pos;
		CHDrawObject* pObject = m_cplSelected.GetNext(pos);
		//Labels dürfen nur mit dem Eigentümer gelöscht werden
		if(pObject->IsKindOf( RUNTIME_CLASS(CHLabel)))
		{
			m_cplSelected.RemoveAt(posA);
			curect.m_cRect.UnionRect(curect.m_cRect,
				pObject->GetBoundingRect());
			pObject->Select(false);
		}
		else
			map.SetAt(pObject, posA);
	}
	while(!m_cplSelected.IsEmpty())
	{
		CHDrawObject* pObject = m_cplSelected.RemoveHead();
		ASSERT_VALID(pObject);
		if(pObject->IsKindOf( RUNTIME_CLASS(CHPosition)) ||
			pObject->IsKindOf( RUNTIME_CLASS(CHTransition)))
		{
			curect.m_cRect.UnionRect(curect.m_cRect,
					pObject->GetBoundingRect());
			CHPetriMember* pMem = (CHPetriMember*)pObject;
			//abhängige Labels zerstören
			CHLabel* pLabel;
			POSITION findpos;
			if((pLabel = pMem->GetLabelA()))
			{
				if(m_mapMembers.Lookup(pLabel, findpos))
				{
					m_colMembers.RemoveAt(findpos);
					m_mapMembers.RemoveKey(pLabel);
					curect.m_cRect.UnionRect(curect.m_cRect,
						pLabel->GetBoundingRect());
					pMem->SetLabelA(0);
					delete pLabel;
				}
				else
					return HandleInitError();
			}
			if((pLabel = pMem->GetLabelB()))
			{
				if(m_mapMembers.Lookup(pLabel, findpos))
				{
					m_colMembers.RemoveAt(findpos);
					m_mapMembers.RemoveKey(pLabel);
					curect.m_cRect.UnionRect(curect.m_cRect,
						pLabel->GetBoundingRect());
					pMem->SetLabelB(0);
					delete pLabel;
				}
				else
					return HandleInitError();
			}
			//abhängige Connectoren zerstören
			CONN_LIST& rInList = pMem->GetInList();
			POSITION posy = rInList.GetHeadPosition();
			while (posy)
			{
				CHConnector* pConn = rInList.GetNext(posy);
				pConn->GetPtrFrom()->GetOutList().RemoveAt
					(pConn->GetPtrFrom()->GetOutList().Find(pConn));
				CHLabel* pLabel;
				if((pLabel = pConn->GetLabelA()))
				{
					if(m_mapMembers.Lookup(pLabel, findpos))
					{
						m_colMembers.RemoveAt(findpos);
						m_mapMembers.RemoveKey(pLabel);
						curect.m_cRect.UnionRect(curect.m_cRect,
							pLabel->GetBoundingRect());
						pConn->SetLabelA(0);
						delete pLabel;
					}
				}
				else
					return HandleInitError();
				if(m_mapMembers.Lookup(pConn, findpos))
				{
					m_colMembers.RemoveAt(findpos);
					m_mapMembers.RemoveKey(pConn);
					curect.m_cRect.UnionRect(curect.m_cRect,
						pConn->GetBoundingRect());
					delete pConn;
				}
				else
					return HandleInitError();
				if(map.Lookup(pConn, findpos))
				{
					m_cplSelected.RemoveAt(findpos);
				}
			}
			CONN_LIST& rOutList = pMem->GetOutList();
			posy = rOutList.GetHeadPosition();
			while (posy)
			{
				CHConnector* pConn = rOutList.GetNext(posy);
				pConn->GetPtrTo()->GetInList().RemoveAt
					(pConn->GetPtrTo()->GetInList().Find(pConn));
				CHLabel* pLabel;
				if((pLabel = pConn->GetLabelA()))
				{
					if(m_mapMembers.Lookup(pLabel, findpos))
					{
						m_colMembers.RemoveAt(findpos);
						m_mapMembers.RemoveKey(pLabel);
						curect.m_cRect.UnionRect(curect.m_cRect,
							pLabel->GetBoundingRect());
						pConn->SetLabelA(0);
						delete pLabel;
					}
				}
				else
					return HandleInitError();
				if(m_mapMembers.Lookup(pConn, findpos))
				{
					m_colMembers.RemoveAt(findpos);
					m_mapMembers.RemoveKey(pConn);
					curect.m_cRect.UnionRect(curect.m_cRect,
						pConn->GetBoundingRect());
					delete pConn;
				}
				else
					return HandleInitError();
				if(map.Lookup(pConn, findpos))
				{
					m_cplSelected.RemoveAt(findpos);
				}
			}
			if(m_mapMembers.Lookup(pObject, findpos))
			{
				m_colMembers.RemoveAt(findpos);
				m_mapMembers.RemoveKey(pObject);
				delete pObject;
			}
			else
				return HandleInitError();
		}
		else if(pObject->IsKindOf( RUNTIME_CLASS(CHConnector)))
		{
			CHConnector* pConn = static_cast <CHConnector*> (pObject);
			pConn->GetPtrTo()->GetInList().RemoveAt
				(pConn->GetPtrTo()->GetInList().Find(pConn));
			pConn->GetPtrFrom()->GetOutList().RemoveAt
				(pConn->GetPtrFrom()->GetOutList().Find(pConn));
				
			CHLabel* pLabel;
			POSITION findpos;
			if((pLabel = pConn->GetLabelA()))
			{
				if(m_mapMembers.Lookup(pLabel, findpos))
				{
					m_colMembers.RemoveAt(findpos);
					m_mapMembers.RemoveKey(pLabel);
					curect.m_cRect.UnionRect(curect.m_cRect,
						pLabel->GetBoundingRect());
					pConn->SetLabelA(0);
					delete pLabel;
				}
			}
			else
				return HandleInitError();
			if(m_mapMembers.Lookup(pConn, findpos))
			{
				m_colMembers.RemoveAt(findpos);
				m_mapMembers.RemoveKey(pConn);
				curect.m_cRect.UnionRect(curect.m_cRect,
					pConn->GetBoundingRect());
				delete pObject;
			}
			else
				return HandleInitError();
		}
		else
		{
			POSITION findpos;
			if(m_mapMembers.Lookup(pObject, findpos))
			{
				m_colMembers.RemoveAt(findpos);
				m_mapMembers.RemoveKey(pObject);
				curect.m_cRect.UnionRect(curect.m_cRect,
					pObject->GetBoundingRect());
				delete pObject;
			}
			else
				return HandleInitError();
		}
		bar.StepIt();
	}
	UpdateAllViews(NULL,1L,&curect);
	return true;
}

void CPetriSimDoc::DeleteAll()
{
	DetachExplObject();
	while (!m_colMembers.IsEmpty())
	{
		delete m_colMembers.RemoveHead();
	}
	m_cplSelected.RemoveAll();
	m_mapMembers.RemoveAll();
	SetModifiedFlag(true);
}

void CPetriSimDoc::Select(CPoint & point)
{
	CHDrawObject* pObject;
	if(m_bSimMode && !m_bRunFast)	
	{
		DetachExplObject();
		POSITION pos = m_colMembers.GetTailPosition();
		while (pos)
		{
			pObject = m_colMembers.GetPrev(pos);
			if((pObject->IsKindOf( RUNTIME_CLASS(CHTransition)) ||
				pObject->IsKindOf( RUNTIME_CLASS(CHPosition)))
				&& pObject->PtInObject(point))
			{
				// Treffer, der Punkt liegt in diesem Object.
				AttachExplObject( pObject );
				pObject->Select(true);
				UpdateAllViews(NULL,0L,pObject);
				break;
			}
		}
	}
	else if(!m_bSimMode)
	{
		if(m_cplSelected.GetCount() == 1)
		{
			CHDrawObject* pObject = m_cplSelected.GetHead();
			if (pObject->PtInObject(point) || pObject->PtInGripper(point))
				return;
			DeselectAll();
		}
		CHDrawObject* lastobject = 0;
		POSITION pos = m_colMembers.GetTailPosition();
		while (pos)
		{
			object = m_colMembers.GetPrev(pos);
			if(object->PtInObject(point) 
			&& ( ((object->IsKindOf( RUNTIME_CLASS(CHLabel))
			&& m_bShowLabel)) 
			|| !object->IsKindOf( RUNTIME_CLASS(CHLabel))))
			{
				// Treffer, der Punkt liegt in diesem Object.
				// Wir  überschreiben wieder mit dem Aktuellen.
				lastobject = object;
				break;
			}
		}
		if(lastobject != NULL)
		{
			//Weil wir das Obenliegende auf der Zeichenfläche wollen
			//nehmen wir das letzte Objekt und selektieren es,
			//wenn es noch nicht ist
			if(m_cplSelected.Find(lastobject) == NULL)
			{
				m_cplSelected.AddTail(lastobject);
				lastobject->Select(true);
				m_bMoveOnGrid = false;
				if(lastobject->IsPMember() && m_bAlignNo)
				{
					m_bMoveOnGrid = true;
				}
				if(!lastobject->IsPMember() && m_bAlignDo)
				{
					m_bMoveOnGrid = true;
				}
			}
			//Der Sicht mitteilen, daß wir etwas geändert haben
			UpdateAllViews(NULL,0L,lastobject);
		}
		else DeselectAll();
		if(m_cplSelected.GetCount() == 1)
			AttachExplObject(m_cplSelected.GetHead());
	}
}

void CPetriSimDoc::Select(CRect & rect)
{
	CHDrawObject* pObject;
	CUpdateRectObject curect;
	m_bMoveOnGrid = false;
	if(m_bSimMode)return;
	DeselectAll();
	CWaitCursor wait;
	POSITION pos = m_colMembers.GetHeadPosition();
	while (pos)
	{
		pObject = m_colMembers.GetNext(pos);
		if(rect.PtInRect(pObject->GetRect().TopLeft()) 
		&& rect.PtInRect(pObject->GetRect().BottomRight())
		&& ( ((pObject->IsKindOf( RUNTIME_CLASS(CHLabel))
		&& m_bShowLabel)) 
		|| !pObject->IsKindOf( RUNTIME_CLASS(CHLabel))))						
		{
			m_cplSelected.AddTail(pObject);
			pObject->Select(true);
			curect.m_cRect.UnionRect(curect.m_cRect,
				pObject->GetBoundingRect());
			if(pObject->IsPMember() && m_bAlignNo)
			{
				m_bMoveOnGrid = true;	
			}
			if(!pObject->IsPMember() && m_bAlignDo)
			{
				m_bMoveOnGrid = true;
			}
		}
	}
	UpdateAllViews(NULL,1L,&curect);
	if(m_cplSelected.GetCount() == 1)
		AttachExplObject(m_cplSelected.GetHead());
}

void CPetriSimDoc::DeselectAll()
{
	//Es sollen alle Listenelemente Deselektiert werden
	DetachExplObject();
	m_bMoveOnGrid = false;
	CUpdateRectObject curect;
	CWaitCursor wait;
	while (!m_cplSelected.IsEmpty())
	{
		CHDrawObject* pObject = m_cplSelected.RemoveHead();
		curect.m_cRect.UnionRect(curect.m_cRect, pObject->GetBoundingRect());
		pObject->Select(false);
	}
	UpdateAllViews(NULL,1L,&curect);
}

CHDrawObject* CPetriSimDoc::NewObject(CPoint & start,CPoint & end,CDC * pDC)
{
	DeselectAll();
	if(m_colMembers.GetCount() >= MAX_DRAW_OBJECTS)
	{
		m_uCursor = NULL;
		m_iDrawMode = DR_SELECT;
		CLimitDialog dlg;
		dlg.DoModal();
		return 0;
	}

	CUpdateRectObject curect;	
	PointToGrid(&start);
	switch(m_iDrawMode)
	{
	case DR_TEXT:
		{
			// Es wird ein neues Textfeld hinzugefügt.
			CHDrawObject* pObject = new CHText(start,&m_lfDefaultFont);
			pObject->Select(true);
			m_cplSelected.AddTail(pObject);
			POSITION pos = m_colMembers.AddTail(pObject);
			m_mapMembers.SetAt(pObject, pos);
			SetModifiedFlag(true);
			//Neues Objekt zeichnen
			UpdateAllViews(NULL,0L,pObject);
			AttachExplObject(pObject);
			return pObject;
		}
	case DR_LINE:
		{
			// Es wird eine neue Linie hinzugefügt.
			CHDrawObject* pObject = new CHLine(start);
			pObject->Select(true);
			m_cplSelected.AddTail(pObject);
			POSITION pos = m_colMembers.AddTail(pObject);
			m_mapMembers.SetAt(pObject, pos);
			SetModifiedFlag(true);
			//Neues Objekt zeichnen
			UpdateAllViews(NULL,0L,pObject);
			AttachExplObject(pObject);
			return pObject;
		}
	case DR_RECT:
		{
			// Es wird eine neues Rechteck hinzugefügt.
			CHDrawObject* pObject = new CHRect(start);
			pObject->Select(true);
			m_cplSelected.AddTail(pObject);
			POSITION pos = m_colMembers.AddTail(pObject);
			m_mapMembers.SetAt(pObject, pos);
			SetModifiedFlag(true);
			//Neues Objekt zeichnen
			UpdateAllViews(NULL,0L,pObject);
			AttachExplObject(pObject);
			return pObject;
		}
	case DR_POLYLINE:
		{
			// Es wird eine neue Linie hinzugefügt.
			CHDrawObject* pObject = new CHPoly(start);
			pObject->Select(true);
			m_cplSelected.AddTail(pObject);
			POSITION pos = m_colMembers.AddTail(pObject);
			m_mapMembers.SetAt(pObject, pos);
			SetModifiedFlag(true);
			//Neues Objekt zeichnen
			UpdateAllViews(NULL,0L,pObject);
			AttachExplObject(pObject);
			return pObject;
		}
	case DR_TRANSITION:
	{
		// Es wird eine neue Transition hinzugefügt.
		CHTransition* pObject = new CHTransition(start,
			m_iNextFreeTransitionIdent++);
		POSITION pos = m_colMembers.AddTail(pObject);
		m_mapMembers.SetAt(pObject, pos);
		pObject->Select(true);
		m_cplSelected.AddTail(pObject);
		CString cstring;
		cstring.Format(_T("%s%d"), TRANSITION_NAME,
			pObject->GetIdent() - FIRST_TRANSITION);
		CHLabel* pLabelA = new CHLabel(0,0,
			cstring, pObject->GetIdent(), LS_LABEL_A);
		pos = m_colMembers.AddTail(pLabelA);
		m_mapMembers.SetAt(pLabelA, pos);
		pLabelA->GetBoundingRect(pDC, false);
		CHLabel* pLabelB = new CHLabel(0,0,
			"0", pObject->GetIdent(), LS_LABEL_B);
		pos = m_colMembers.AddTail(pLabelB);
		m_mapMembers.SetAt(pLabelB, pos);
		pLabelB->GetBoundingRect(pDC, false);
		pObject->SetLabelA(pLabelA);
		pObject->SetLabelB(pLabelB);
		pObject->RecalcLabelPos();
		UpdateAllViews(0, 0L, pObject);
		SetModifiedFlag(true);
		AttachExplObject(pObject);
		return pObject;
	}
	case DR_POSITION:
	{
		// Es wird eine neue Position hinzugefügt.
		CHPosition* pObject = new CHPosition(start,
			m_iNextFreePositionIdent++);
		POSITION pos = m_colMembers.AddTail(pObject);
		m_mapMembers.SetAt(pObject, pos);
		pObject->Select(true);
		m_cplSelected.AddTail(pObject);
		CString cstring;
		cstring.Format(_T("%s%d"), POSITION_NAME, 
			pObject->GetIdent() - FIRST_POSITION);
		CHLabel* pLabelA = new CHLabel(0,0,
			cstring, pObject->GetIdent(), LS_LABEL_A);
		pos = m_colMembers.AddTail(pLabelA);
		m_mapMembers.SetAt(pLabelA, pos);
		pLabelA->GetBoundingRect(pDC, false);
		CHLabel* pLabelB = new CHLabel(0,0,
			"1", pObject->GetIdent(), LS_LABEL_B);
		pos = m_colMembers.AddTail(pLabelB);
		m_mapMembers.SetAt(pLabelB, pos);
		pLabelB->GetBoundingRect(pDC, false);
		pObject->SetLabelA(pLabelA);
		pObject->SetLabelB(pLabelB);
		pObject->RecalcLabelPos();
		UpdateAllViews(0, 0L, pObject);
		SetModifiedFlag(true);
		AttachExplObject(pObject);
		return pObject;
	}
	case DR_CONNECTOR:
	{
		CPoint cpoints,cpointe;
		bool startok = false;
		bool endok = false;
		bool stype;
		bool etype;
		CHPetriMember* from = 0;
		CHPetriMember* to = 0;

		POSITION pos = m_colMembers.GetHeadPosition();
		while (pos != NULL)
		{
			object = m_colMembers.GetNext(pos);
			if((object->IsKindOf( RUNTIME_CLASS(CHPosition))) &&
				(object->PtInObject(start)))
			{
				from = (CHPetriMember*)object;
				stype = true;
				startok = true;
			}
		}

		pos = m_colMembers.GetHeadPosition();
		while (pos != NULL)
		{
			object = m_colMembers.GetNext(pos);
			if((object->IsKindOf( RUNTIME_CLASS(CHTransition))) &&
				(object->PtInObject(start)))
			{
				from = (CHPetriMember*)object;
				stype = false;
				startok = true;
			}
		}

		pos = m_colMembers.GetHeadPosition();
		while (pos != NULL)
		{
			object = m_colMembers.GetNext(pos);
			if((object->IsKindOf( RUNTIME_CLASS(CHPosition))) &&
				(object->PtInObject(end)))
			{
				to = (CHPetriMember*)object;
				etype = true;
				endok = true;
			}
		}

		pos = m_colMembers.GetHeadPosition();
		while (pos != NULL)
		{
			object = m_colMembers.GetNext(pos);
			if((object->IsKindOf( RUNTIME_CLASS(CHTransition))) &&
				(object->PtInObject(end)))
			{
				to = (CHPetriMember*)object;
				etype = false;
				endok = true;
			}
		}
		if(!startok)
		{
			AfxMessageBox(IDS_ERROR_CONN_A,MB_OK,0);;
			endok = false;
			return NULL;
		}
		if(!endok)
		{
			AfxMessageBox(IDS_ERROR_CONN_B,MB_OK,0);;
			startok = false;
			return NULL;
		}
		if(etype == stype)
		{
			AfxMessageBox(IDS_ERROR_CONN_C,MB_OK,0);;
			startok = false;
			endok = false;
			return NULL;
		}
		//testen ob schon eine Verbindung besteht
		startok = false;
		endok = false;
		pos = m_colMembers.GetHeadPosition();
		while (pos != NULL)
		{
			object = m_colMembers.GetNext(pos);
			CHConnector* ptrConn;
			if(ptrConn = dynamic_cast <CHConnector*> (object))
			{
				startok = ptrConn->GetPtrFrom() == from;
				endok = ptrConn->GetPtrTo() == to;
				if(startok && endok)break;
			}
		}
		if(startok && endok)
		{
			AfxMessageBox(IDS_ERROR_CONN_D,MB_OK,0);;
			return NULL;
		}
		// Es wird eine neue Kante hinzugefügt.
		CHConnector* pObject = new CHConnector(from,to,m_iNextFreeConnectorIdent++);
		pos = m_colMembers.AddTail(pObject);
		m_mapMembers.SetAt(pObject, pos);
		pObject->Select(true);
		m_cplSelected.AddTail(pObject);
		SetModifiedFlag(true);
		CHLabel* pLabelA = new CHLabel(0,0,
			"1", pObject->GetIdent(), LS_LABEL_A);
		pos = m_colMembers.AddTail(pLabelA);
		m_mapMembers.SetAt(pLabelA, pos);
		pLabelA->GetBoundingRect(pDC, false);
		pObject->SetLabelA(pLabelA);
		pObject->RecalcLabelPos();
		UpdateAllViews(0, 0L, pObject);
		AttachExplObject(pObject);
		return pObject;
	}
	default:
		{
			ASSERT(false);
			return NULL;
		}
	}
	return 0;
}

bool CPetriSimDoc::CanMove(CPoint & point)
{
	if((m_cplSelected.GetCount() == 1))
	{
		object = m_cplSelected.GetHead();
		m_iHandle = object->PtInGripper(point);
		object->AddPoint(point);
		//Objekt am Raster ausrichten
		if(object->IsPMember() && m_bAlignNo)
		{
			UpdateAllViews(NULL,0L,object);
			object->ToGrid(m_iGrid,m_iHandle);
			UpdateAllViews(NULL,0L,object);
		}
		if(!object->IsPMember() && m_bAlignDo)
		{
			UpdateAllViews(NULL,0L,object);
			object->ToGrid(m_iGrid,m_iHandle);
			UpdateAllViews(NULL,0L,object);
		}
		if(m_iHandle != NULL)
		{
		// Es ist nur ein Object selektiert und die
		// Maus zeigt in ein Drag Rechteck. Es kann
		// die größe geändert werden
			m_iDrawMode = DR_RESIZE;
			return true;
		}
	}
	//Wir prüfen ob die Maus auf ein Objekt zeigt
	POSITION pos = m_cplSelected.GetHeadPosition();
	while (pos != NULL)
	{
		object = m_cplSelected.GetNext(pos);
		if(object->PtInObject(point) && !(object->PtInGripper(point)))
		{
			// Die Maus zeigt auf ein Objekt. Wir
			// wollen verschieben
			m_iDrawMode = DR_MOVE;
			return NULL;
		}	
	}
	// Es ist kein Objekt selektiert oder die
	// Maus zeigt auf kein Objekt.
	return false;
}

void CPetriSimDoc::ResizeObject(CPoint & to, CPetriSimView * pView
								, bool init)
{
	static CPoint bpoint;
	CHDrawObject* pObject;
	CRect crect(0,0,0,0);
	pObject = m_cplSelected.GetHead();
	ASSERT(pObject);
	if(m_bMoveOnGrid)
	{
		bpoint += to;
		if((abs(bpoint.x) >= m_iGrid)||(abs(bpoint.y) >= m_iGrid))
		{
			CPoint point(bpoint);
			
			if(!(abs(point.x) >= m_iGrid))point.x = 0;
			if(!(abs(point.y) >= m_iGrid))point.y = 0;
			
			UpdateAllViews(NULL,0L,pObject);
			PointToGrid(&point);
			if(init)
				pObject->ResizeTo(point,NULL);
			else
				pObject->ResizeTo(point,m_iHandle);
			UpdateAllViews(NULL,0L,pObject);

			if((abs(point.x) >= m_iGrid))bpoint.x = bpoint.x - point.x;
			if((abs(point.y) >= m_iGrid))bpoint.y = bpoint.y - point.y;
		}
	}
	else
	{
		crect.UnionRect(crect,pObject->GetBoundingRect());
		if(init)
			pObject->ResizeTo(to,0);
		else
			pObject->ResizeTo(to,m_iHandle);
		crect.UnionRect(crect,pObject->GetBoundingRect());
	}
	m_UpdateRect.m_cRect.UnionRect(m_UpdateRect.m_cRect, crect);
	pView->InvalidateDcRect(crect);
	SetModifiedFlag(true);
}

void CPetriSimDoc::MoveObject(CPoint & to, CPetriSimView * pView)
{
	ASSERT(m_iDrawMode == DR_MOVE);
	static CPoint bpoint;
	CHDrawObject* pObject;
	CRect crect(0,0,0,0);
	if(m_bMoveOnGrid)
	{
		bpoint += to;
		if((abs(bpoint.x) >= m_iGrid)||(abs(bpoint.y) >= m_iGrid))
		{
			CPoint point(bpoint);
			
			if(!(abs(point.x) >= m_iGrid))point.x = 0;
			if(!(abs(point.y) >= m_iGrid))point.y = 0;
			POSITION pos = m_cplSelected.GetHeadPosition();
			while (pos != NULL)
			{
				pObject = m_cplSelected.GetNext(pos);
				crect.UnionRect(crect,pObject->GetBoundingRect());
				PointToGrid(&point);
				pObject->MoveTo(point);
				crect.UnionRect(crect,pObject->GetBoundingRect());
			}
			if((abs(point.x) >= m_iGrid))bpoint.x = bpoint.x - point.x;
			if((abs(point.y) >= m_iGrid))bpoint.y = bpoint.y - point.y;
		}
	}
	else
	{
		POSITION pos = m_cplSelected.GetHeadPosition();
		while (pos)
		{
			pObject = m_cplSelected.GetNext(pos);
			crect.UnionRect(crect,pObject->GetBoundingRect());
			pObject->MoveTo(to);
			crect.UnionRect(crect,pObject->GetBoundingRect());
		}
	}
	m_UpdateRect.m_cRect.UnionRect(m_UpdateRect.m_cRect, crect);
	pView->InvalidateDcRect(crect);
	SetModifiedFlag(true);
}

void CPetriSimDoc::PopUp()
{
	POSITION pos = m_cplSelected.GetHeadPosition();
	while (pos != NULL)
	{
		object = m_cplSelected.GetNext(pos);
		m_colMembers.RemoveAt(m_colMembers.Find(object));
		m_colMembers.AddTail(object);
		UpdateAllViews(NULL,0L,object);
	}
	SetModifiedFlag(true);
}

void CPetriSimDoc::OnObjectPropertys() 
{
	if(m_cplSelected.GetCount() == 1)
	{
		object = m_cplSelected.GetHead();
		object->Select(false);
		object->Propertys();
		UpdateAllViews(NULL,0L,object);
	}
}

void CPetriSimDoc::OnEditorProperties() 
{
	CDocPropertySheetA propSheet;
	//Variablen Inititialisieren
	propSheet.m_Page2.m_iGrid = m_iGrid;
	propSheet.m_Page2.m_bGridVisible = m_bGridVisible;
	propSheet.m_Page2.m_bAlignNo = m_bAlignNo;
	propSheet.m_Page2.m_bAlignDo = m_bAlignDo;

	propSheet.m_Page1.m_iVerExtend = m_sizeDoc.cy;
	propSheet.m_Page1.m_iHorExtend = m_sizeDoc.cx;
	propSheet.m_Page1.m_iBackColor = m_iBackColor;
	propSheet.m_Page1.m_bPopupMessage = m_bPopupMessage;
	propSheet.m_Page1.m_bPopupExplorer = m_bPopupExplorer;

	propSheet.m_Page3.m_csOutputFile = m_csOutputFilePath;
	propSheet.m_Page3.m_iStopCount = m_iStopStep;
	propSheet.m_Page3.m_iStopTime = m_iStopTime;
	propSheet.m_Page3.m_iSampleTime = m_iSimSampleTime;

	if(propSheet.DoModal() == IDOK)
	{
		//Bei Ok Variablen zurückschreiben
		m_iGrid = propSheet.m_Page2.m_iGrid;
		m_bGridVisible = (propSheet.m_Page2.m_bGridVisible != 0)? true : false;
		m_bAlignNo = (propSheet.m_Page2.m_bAlignNo != 0)? true : false;
		m_bAlignDo = (propSheet.m_Page2.m_bAlignDo != 0)? true : false;

		m_sizeDoc.cy = propSheet.m_Page1.m_iVerExtend;
		m_sizeDoc.cx = propSheet.m_Page1.m_iHorExtend;
		m_iBackColor = propSheet.m_Page1.m_iBackColor;
		m_bPopupMessage = (propSheet.m_Page1.m_bPopupMessage != 0)? true : false;
		m_bPopupExplorer = (propSheet.m_Page1.m_bPopupExplorer != 0)? true : false;

		m_csOutputFilePath = propSheet.m_Page3.m_csOutputFile;
		m_iStopStep = propSheet.m_Page3.m_iStopCount;
		m_iStopTime = propSheet.m_Page3.m_iStopTime; 
		m_iSimSampleTime = propSheet.m_Page3.m_iSampleTime;

		UpdateAllViews(NULL,3L,NULL);
	}
}

void CPetriSimDoc::PointToGrid(CPoint * point)
{
	int r;
	if(point->x > 0)
	{
		r = point->x % m_iGrid;
		if(r < m_iGrid / 2)
			point->x -= r;
		if(r >= m_iGrid / 2)
			point->x += (m_iGrid - r);
	}
	else
	{
		r = -(point->x % m_iGrid);
		if(r < m_iGrid / 2)
			point->x += r;
		if(r >=	m_iGrid / 2)
			point->x -= (m_iGrid - r);
	}
	if(point->y > 0)
	{
	r = point->y % m_iGrid;
	if(r < m_iGrid / 2)
		point->y -= r;
	if(r >= m_iGrid / 2)
		point->y += (m_iGrid - r);
	}
	else
	{
		r = -(point->y % m_iGrid);

		if(r < m_iGrid / 2)
			point->y += r;
		if(r >= m_iGrid / 2)
			point->y -= (m_iGrid - r);
	}
}

// Stellt aus den logischen Verbindungen (Idents) die erforderlichen
// Pointer wieder her
// zB. Widerherstellung nach Serialisierung
bool CPetriSimDoc::InitializeNet(CHDOBJECT_LIST & list)
{
	CMap<long,long,CHDrawObject*,CHDrawObject*&> map(100);
	CWaitCursor wait;
	CProgressBar bar(_T("Init Net..."), 70, list.GetCount(), false);
	bar.SetStepRate(list.GetCount() / 100);
	POSITION pos = list.GetHeadPosition();
	while (pos)
	{
		CHDrawObject* pObject = list.GetNext(pos);
		long id = pObject->GetIdent();
		if(id >= 0)
		{
			CHDrawObject* pTemp;
			if(!map.Lookup(id, pTemp))
				map.SetAt(id, pObject);
			else
			{
				CString ccause;
				ccause.Format(_T("ID: %d Exist Runtime Class: %s"),
					pObject->GetIdent(), 
					pObject->GetRuntimeClass()->m_lpszClassName);
				CHErrorFile::WriteError(_T("InitializeNet"),
				ccause, this);
				return HandleInitError(ccause);
			}
		}
	}
	pos = list.GetHeadPosition();
	while (pos)
	{
		bar.StepIt();
		CHDrawObject* pObject = list.GetNext(pos);
		CHDrawObject* pTempObject;
		if(pObject->IsKindOf( RUNTIME_CLASS(CHConnector)))
		{
			CHConnector* pConn = static_cast <CHConnector*> (pObject);
			if(map.Lookup(pConn->GetFrom(), pTempObject))
			{
				if(pTempObject->IsPMember())
				{
					CHPetriMember* pMember = 
						static_cast <CHPetriMember*> (pTempObject);
					pMember->GetOutList().AddTail(pConn);
					pConn->SetPtrFrom(pMember);
				}
				else 
					return HandleInitError();
			}
			else 
					return HandleInitError(_T("Arc source not found"));
			if(map.Lookup(pConn->GetTo(), pTempObject))
			{
				if(pTempObject->IsPMember())
				{
					CHPetriMember* pMember = 
						static_cast <CHPetriMember*> (pTempObject);
					pMember->GetInList().AddTail(pConn);
					pConn->SetPtrTo(pMember);
				}
				else 
					return HandleInitError();
			}
			else 
			{
				CHErrorFile::WriteError(_T("InitializeNet"),
				_T("Arc target not found"), this);
				return HandleInitError(_T("Arc target not found"));
			}
		}
		if(pObject->IsKindOf( RUNTIME_CLASS(CHLabel)))
		{
			CHLabel* pLabel = static_cast <CHLabel*> (pObject);
			if(map.Lookup(pLabel->GetOwner(), pTempObject))
			{
				if(pTempObject->IsKindOf( RUNTIME_CLASS(CHConnector)))
				{
					CHConnector* ptrConn = (CHConnector*)pTempObject;
					//erforderlichen Pointer gefunden
					ptrConn->SetLabelA(pLabel);
				}
				if(pTempObject->IsKindOf( RUNTIME_CLASS(CHTransition)))
				{
					CHTransition* ptrTran = (CHTransition*)pTempObject;
					//erforderlichen Pointer gefunden
					if(pLabel->GetSubIdent() == LS_LABEL_A)
						ptrTran->SetLabelA(pLabel);
					else ptrTran->SetLabelB(pLabel);
				}
				if(pTempObject->IsKindOf( RUNTIME_CLASS(CHPosition)))
				{
					CHPosition* ptrPos = (CHPosition*)pTempObject;
					//erforderlichen Pointer gefunden
					if(pLabel->GetSubIdent() == LS_LABEL_A)
						ptrPos->SetLabelA(pLabel);
					else ptrPos->SetLabelB(pLabel);
				}
			}
			else
			{
				CHErrorFile::WriteError(_T("InitializeNet"),
				_T("Label owner not found"), this);
				return HandleInitError(_T("Label owner not found"));
			}
		}
	}	
	return true;
}

//
bool CPetriSimDoc::HandleInitError(const LPCTSTR cause/* = 0*/)
{
	if(cause)
		TRACE(cause);
	AfxMessageBox(IDS_ERROR_INIT_NET);
	DeleteContents();
	UpdateAllViews(0);
	return false;
}

// RecalcIdent(CTypedPtrList<CObList,CHDrawObject*> * list)
// Passt die Idents an den aktuellen Stand dieses Documentes an.
// Das ist erforderlich um eine Doppelvergabe der Idents zu vermeiden
// zB. Zwischenablageoperationen, einfügen von Modulen
void CPetriSimDoc::RecalcIdent(CHDOBJECT_LIST & list)
{
	TCHAR strBuffer[100];
	CWaitCursor wait;
	POSITION pos = list.GetHeadPosition();
	while (pos)
	{
		CHDrawObject* ptrObject = list.GetNext(pos);
		
		if(ptrObject->IsKindOf( RUNTIME_CLASS(CHConnector)))
		{
			ptrObject->SetIdent(m_iNextFreeConnectorIdent++);	
		}
		if(ptrObject->IsKindOf( RUNTIME_CLASS(CHPosition)))
		{
			CHPosition* pPosi = static_cast <CHPosition*> (ptrObject);

			StringCbPrintf(strBuffer, sizeof(strBuffer)
				, _T("%s%d"), POSITION_NAME
				, m_iNextFreePositionIdent - FIRST_POSITION);

			pPosi->GetLabelA()->SetText(strBuffer);
			pPosi->SetIdent(m_iNextFreePositionIdent++);
		}
		if(ptrObject->IsKindOf( RUNTIME_CLASS(CHTransition)))
		{
			CHTransition* pTran = static_cast <CHTransition*> (ptrObject);

			StringCbPrintf(strBuffer, sizeof(strBuffer)
				, _T( "%s%d"), TRANSITION_NAME
				, m_iNextFreeTransitionIdent- FIRST_TRANSITION);
			pTran->GetLabelA()->SetText(strBuffer);
			pTran->SetIdent(m_iNextFreeTransitionIdent++);
		}
		
	}
}

// StrippNet(CTypedPtrList<CObList,CHDrawObject*> * list)
// Beim Kopieren von Netz Elementen muss Sicher sein dass,
// die Label mitkopiert werden und nur die Kanten kopiert werden,
// bei denen Beide verbundenen Elemente, ebenfalls kopiert werden. 
void CPetriSimDoc::StrippNet(CHDOBJECT_LIST & list)
{
	CTypedPtrList<CPtrList,CHDrawObject*>addlist;
	CWaitCursor wait;
	CHDrawObject* ptrObject;
	POSITION pos = list.GetHeadPosition();
	while (pos)
	{
		POSITION posA = pos;
		ptrObject = list.GetNext(pos);
		if(ptrObject->IsKindOf( RUNTIME_CLASS(CHLabel)))
		{
			CHLabel* ptrLab = (CHLabel*)ptrObject;
			ptrLab->Select(false);
			UpdateAllViews(NULL,0L,ptrLab);
			list.RemoveAt(posA);
		}
		if(ptrObject->IsKindOf( RUNTIME_CLASS(CHConnector)))
		{
			CHConnector* ptrConn = (CHConnector*)ptrObject;
			if(!list.Find(ptrConn->GetPtrFrom()) || 
			!list.Find(ptrConn->GetPtrTo()))
			{
				ptrConn->Select(false);
				UpdateAllViews(NULL,0L,ptrConn);
				list.RemoveAt(posA);
			}
			else
			{
				if(ptrConn->GetLabelA())
					addlist.AddTail(ptrConn->GetLabelA());
				if(ptrConn->GetLabelB())
					addlist.AddTail(ptrConn->GetLabelB());
			}
		}
		if(ptrObject->IsKindOf( RUNTIME_CLASS(CHPosition)))
		{
			CHPosition* ptrPos = (CHPosition*)ptrObject;
			if(ptrPos->GetLabelA())addlist.AddTail(ptrPos->GetLabelA());
			if(ptrPos->GetLabelB())addlist.AddTail(ptrPos->GetLabelB());
		}
		if(ptrObject->IsKindOf( RUNTIME_CLASS(CHTransition)))
		{
			CHTransition* ptrTran = (CHTransition*)ptrObject;
			if(ptrTran->GetLabelA())addlist.AddTail(ptrTran->GetLabelA());
			if(ptrTran->GetLabelB())addlist.AddTail(ptrTran->GetLabelB());
		}
	}
	pos = addlist.GetHeadPosition();
	while (pos)
	{
		list.AddTail(ptrObject = addlist.GetNext(pos));
	}
}

void CPetriSimDoc::AttachExplObject(CHDrawObject * pObject)
{
	ASSERT_VALID(pObject);
	ASSERT_VALID(this);

	m_SenderObject.m_pObject = pObject;
	m_SenderObject.m_pDoc = this;
	m_ExplObject = pObject;

	GetMainFrame().GetPropertiesWnd().SetObject(&m_SenderObject);
	
	// TODO:
	/*if(m_bPopupExplorer)
	{
		CControlBar* pBar = GetMainFrame().GetControlBar(CG_IDD_MYDIALOGBAR);
		if(pBar && pBar->IsFloating())
			GetMainFrame().ShowControlBar(pBar, true, false);
	}*/
}

void CPetriSimDoc::UpdateExplObject()
{
	if(m_ExplObject)
	{
		m_SenderObject.m_pObject = m_ExplObject;
		m_SenderObject.m_pDoc = this;
		
		GetMainFrame().GetPropertiesWnd().SetObject(&m_SenderObject);
	}
}

void CPetriSimDoc::DetachExplObject(bool test /* = false */)
{
	if(m_bSimMode && m_ExplObject)
	{
		CUpdateRectObject curect;
		curect.m_cRect = m_ExplObject->GetBoundingRect();
		m_ExplObject->Select(false);
		UpdateAllViews(NULL,1L,&curect);
	}
	if(!test || !m_ExplObject)
	{
		m_ExplObject = 0;
		GetMainFrame().GetPropertiesWnd().SetObject(0);
	
		// TODO:
		/*GetMainFrame().GetProjExplorer()->SetObject(0);
		if(m_bPopupExplorer)
		{
			CControlBar* pBar = 
					GetMainFrame().GetControlBar(CG_IDD_MYDIALOGBAR);
			if(pBar && pBar->IsFloating())
				GetMainFrame().ShowControlBar(pBar, false, false);
		}*/
	}
}

void CPetriSimDoc::DeleteContents() 
{
	KillSimThread();
	DetachExplObject();
	DeleteAll();
	m_iBackColor = RGB(255,255,255);
	m_iStepCount = 0;
	m_iStopTime = 1000;
	m_iStopStep = 1000;
	m_iTimerIdent = 0;
	m_bPopupMessage = true;
	m_bPopupExplorer = true;
	m_bEnableOutputFile = false;
	m_bThreadError = false;
	m_bStopNotif = false;
	m_bSingleStep = false;
	m_bRunNormal = false;
	m_bRunFast = false;
	m_bSimMode = false;
	m_uCursor = 0;
	m_bShowLabel = true;
	m_ExplObject = 0;
	m_iStepTime = 0;
	m_pSimThread = 0;
	m_iSimSpeed = 500;
	m_iSimTime = 0;
	m_iSimSampleTime = 1;
	m_iCalcTime = 0;
	m_iDrawMode = DR_SELECT;
	m_sizeDoc = CSize(1000,1500);//Layout
	m_iGrid = DRAW_GRID;//Raster
	m_bSimMode = false;
	m_bGridVisible = true;
	m_bAlignNo = true;
	m_bAlignDo = false;

	CDocument::DeleteContents();
}

void CPetriSimDoc::UpdateOutputWindow()
{
	GetMainFrame().GetOutputWnd().SetText(m_pOutputBuffer->GetBuffer());
}

void CPetriSimDoc::KillSimThread()
{
	if(m_pSimThread)
	{
		CWaitCursor wait;
		GetMainFrame().StopAnimTimer();
		m_pAnimEvent->SetEvent();
		m_nAnimPhase = 5;
		AnimTokens();
		VERIFY(m_cThreadEvents[CHT_EVENT_KILL]->SetEvent());
		m_bSimMode = false;
		m_bRunFast = false;
		m_bRunNormal = false;
		m_bSingleStep = false;
		m_cThreadEvents[CHT_EVENT_RUNFAST]->ResetEvent();
		// warten auf thread Ende
		CSingleLock sLock(m_cThreadEvents[CHT_EVENT_DEAD], true);
		Sleep(100);
		m_pSimThread = 0;
	}
}

void CPetriSimDoc::PushBack()
{
	POSITION pos = m_cplSelected.GetHeadPosition();
	while (pos != NULL)
	{
		object = m_cplSelected.GetNext(pos);
		m_colMembers.RemoveAt(m_colMembers.Find(object));
		m_colMembers.AddHead(object);
		UpdateAllViews(NULL,0L,object);
	}
	SetModifiedFlag(true);
}

void CPetriSimDoc::Group()
{
	POSITION pos = m_cplSelected.GetHeadPosition();
	while (pos)
	{
		POSITION posA = pos;
		CHDrawObject* pObject = m_cplSelected.GetNext(pos);
		if(pObject->IsPMember() || 
		pObject->IsKindOf( RUNTIME_CLASS(CHLabel)))
		{
			pObject->Select(false);
			UpdateAllViews(NULL,0L,pObject);
			m_cplSelected.RemoveAt(posA);
		}
	}
	if(m_cplSelected.GetCount() >= 2)
	{
		CHGroup* pGroup = new CHGroup();
		pGroup->Select(true);
		POSITION newpos = m_colMembers.AddTail(pGroup);
		m_mapMembers.SetAt(pGroup, newpos);
		pos = m_cplSelected.GetHeadPosition();
		while (pos)
		{
			CHDrawObject* pObject = m_cplSelected.GetNext(pos);
			pObject->Select(false);
			UpdateAllViews(NULL,0L,pObject);
			pGroup->Add(pObject);
			POSITION findpos;
			if(m_mapMembers.Lookup(pObject, findpos))
			{
				m_colMembers.RemoveAt(findpos);
				m_mapMembers.RemoveKey(pObject);
			}
			else
				HandleInitError();
		}
		m_cplSelected.RemoveAll();
		m_cplSelected.AddTail(pGroup);
		UpdateAllViews(NULL,0L,pGroup);
		SetModifiedFlag(true);
	}
}

void CPetriSimDoc::UnGroup()
{
	if(m_cplSelected.GetCount() == 1)
	{
		CHDrawObject* pObject = m_cplSelected.GetHead();
		if(pObject->IsKindOf( RUNTIME_CLASS(CHGroup)))
		{
			CHGroup* pGroup = static_cast <CHGroup*> (pObject);
			POSITION findpos;
			if(m_mapMembers.Lookup(pObject, findpos))
			{
				m_colMembers.RemoveAt(findpos);
				m_mapMembers.RemoveKey(pObject);
			}
			else
				HandleInitError();
			m_cplSelected.RemoveAll();
			while((pObject = pGroup->Remove()))
			{
				pObject->Select(true);
				m_cplSelected.AddTail(pObject);
				POSITION newpos;
				newpos = m_colMembers.AddTail(pObject);
				m_mapMembers.SetAt(pObject, newpos);
			}
		UpdateAllViews(NULL,0L,pGroup);
		delete pGroup;
		SetModifiedFlag(true);
		}
	}
}

void CPetriSimDoc::ToGrid()
{
	POSITION pos = m_cplSelected.GetHeadPosition();
	while (pos)
	{
		CHDrawObject* pObject = m_cplSelected.GetNext(pos);
		UpdateAllViews(NULL,0L,pObject);
		pObject->ToGrid(false);
		UpdateAllViews(NULL,0L,pObject);
	}
	SetModifiedFlag(true);
}

void CPetriSimDoc::UpdateOtherViews(CView * pView)
{
	UpdateAllViews(pView, 1L, &m_UpdateRect);
	m_UpdateRect.m_cRect.SetRect(0,0,0,0);
}

void CPetriSimDoc::StepMove(int direction)
{
	if(m_cplSelected.IsEmpty())return;
	CUpdateRectObject ucrect;
	CHDrawObject* pObject;
	CPoint cpoint(0,0);
	if(m_bMoveOnGrid)
	{
		switch(direction)
		{
		case STEP_MOVE_DOWN:
			cpoint.Offset(0, m_iGrid);
			break;
		case STEP_MOVE_RIGHT:
			cpoint.Offset(m_iGrid, 0);
			break;
		case STEP_MOVE_UP:
			cpoint.Offset(0, -m_iGrid);
			break;
		case STEP_MOVE_LEFT:
			cpoint.Offset(-m_iGrid, 0);
			break;
		}
	}
	else
	{
		switch(direction)
		{
		case STEP_MOVE_DOWN:
			cpoint.Offset(0, 1);
			break;
		case STEP_MOVE_RIGHT:
			cpoint.Offset(1, 0);
			break;
		case STEP_MOVE_UP:
			cpoint.Offset(0, -1);
			break;
		case STEP_MOVE_LEFT:
			cpoint.Offset(-1, 0);
			break;
		}
	}
		
	POSITION pos = m_cplSelected.GetHeadPosition();
	while (pos != NULL)
	{
		pObject = m_cplSelected.GetNext(pos);
		ucrect.m_cRect.UnionRect(ucrect.m_cRect,pObject->GetBoundingRect());
		pObject->MoveTo(cpoint);
		ucrect.m_cRect.UnionRect(ucrect.m_cRect,pObject->GetBoundingRect());
	}
	UpdateAllViews(0, 1L, &ucrect);
	SetModifiedFlag(true);

}

CMainFrame& CPetriSimDoc::GetMainFrame()
{
	CMainFrame* pFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	ASSERT_VALID(pFrame);

	return (*pFrame);
}

void CPetriSimDoc::OnUpdateMultiLineMode(CCmdUI* pCmdUI) 
{
	if(m_iDrawMode == DR_POLYLINE)pCmdUI->SetRadio(true);
	else pCmdUI->SetRadio(false);
	if(m_bSimMode)pCmdUI->Enable(false);
	else pCmdUI->Enable(true);
}

void CPetriSimDoc::OnMultiLineMode() 
{
	m_iDrawMode = DR_POLYLINE;
	m_uCursor = IDC_POLYGON;
	DeselectAll();
}

void CPetriSimDoc::OnUpdatePositionMode(CCmdUI* pCmdUI) 
{
	if(m_iDrawMode == DR_POSITION)pCmdUI->SetRadio(true);
	else pCmdUI->SetRadio(false);
	if(m_bSimMode)pCmdUI->Enable(false);
	else pCmdUI->Enable(true);	
}

void CPetriSimDoc::OnPositionMode() 
{
	m_iDrawMode = DR_POSITION;
	m_uCursor = IDC_POSITION;
	DeselectAll();
}

void CPetriSimDoc::OnUpdateRectMode(CCmdUI* pCmdUI) 
{
	if(m_iDrawMode == DR_RECT)pCmdUI->SetRadio(true);
	else pCmdUI->SetRadio(false);
	if(m_bSimMode)pCmdUI->Enable(false);
	else pCmdUI->Enable(true);
}

void CPetriSimDoc::OnRectMode() 
{
	m_iDrawMode = DR_RECT;
	m_uCursor = IDC_RECT;
	DeselectAll();
}

void CPetriSimDoc::OnUpdateSelectMode(CCmdUI* pCmdUI) 
{
	if(m_iDrawMode == DR_SELECT)pCmdUI->SetRadio(true);
	else pCmdUI->SetRadio(false);
}

void CPetriSimDoc::OnSelectMode() 
{
	m_iDrawMode = DR_SELECT;
	m_uCursor = NULL;
}

void CPetriSimDoc::OnUpdateTransitionMode(CCmdUI* pCmdUI) 
{
	if(m_iDrawMode == DR_TRANSITION)pCmdUI->SetRadio(true);
	else pCmdUI->SetRadio(false);
	if(m_bSimMode)pCmdUI->Enable(false);
	else pCmdUI->Enable(true);
}

void CPetriSimDoc::OnTransitionMode() 
{
	m_iDrawMode = DR_TRANSITION;
	m_uCursor = IDC_TRANSITION;
	DeselectAll();
	
}

void CPetriSimDoc::OnUpdateLineMode(CCmdUI* pCmdUI) 
{
	if(m_iDrawMode == DR_LINE)pCmdUI->SetRadio(true);
	else pCmdUI->SetRadio(false);
	if(m_bSimMode)pCmdUI->Enable(false);
	else pCmdUI->Enable(true);
}

void CPetriSimDoc::OnLineMode() 
{
	m_iDrawMode = DR_LINE;
	m_uCursor = IDC_LINE;
	DeselectAll();	
}

void CPetriSimDoc::OnUpdateConnectorMode(CCmdUI* pCmdUI) 
{
	if(m_iDrawMode == DR_CONNECTOR)pCmdUI->SetRadio(true);
	else pCmdUI->SetRadio(false);
	if(m_bSimMode)pCmdUI->Enable(false);
	else pCmdUI->Enable(true);
}

void CPetriSimDoc::OnConnectorMode() 
{
	m_iDrawMode = DR_CONNECTOR;
	m_uCursor = IDC_CONNECTOR;
	DeselectAll();	
}

void CPetriSimDoc::OnUpdateTextMode(CCmdUI* pCmdUI) 
{
	if(m_iDrawMode == DR_TEXT)pCmdUI->SetRadio(true);
	else pCmdUI->SetRadio(false);	
	if(m_bSimMode)pCmdUI->Enable(false);
	else pCmdUI->Enable(true);
	
}

void CPetriSimDoc::OnTextMode()
{
	m_iDrawMode = DR_TEXT;
	m_uCursor = IDC_TEXT;
	DeselectAll();
}

void CPetriSimDoc::OnStepMode() 
{
//	VERIFY(SetEvent(m_pSimThread->m_hEvent[CHT_EVENT_STEP]));
}

void CPetriSimDoc::OnUpdateStepMode(CCmdUI* pCmdUI) 
{
	if(m_bSimMode)pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
	
}

void CPetriSimDoc::OnUpdateIndicatorsStepTime(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(true);
	if(m_bSimMode)
	{
		CString strPage;
		strPage.Format(_T("S. %d/%dms"), m_iStepTime, m_iCalcTime);
		pCmdUI->SetText( strPage ); 
	}
	else pCmdUI->SetText(_T("Step/Calc Time"));
}

void CPetriSimDoc::OnUpdateIndicatorsStepCount(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(true);
	CString strPage;
	strPage.Format(_T("SC. %d"), m_iStepCount);
	pCmdUI->SetText( strPage );
}

void CPetriSimDoc::OnUpdateIndicatorsCount(CCmdUI* pCmdUI) 
{	
	pCmdUI->Enable(true);
	CString strPage;
	strPage.Format(_T("C.%d/%d"), m_colMembers.GetCount(), 
		m_cplSelected.GetCount());
	pCmdUI->SetText(strPage);
}

void CPetriSimDoc::OnUpdateSliderSimspeed(CCmdUI* pCmdUI) 
{	
	if(m_bSimMode)
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);
}

void CPetriSimDoc::OnUpdateIndicatorsSimTime(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(true);
	
	CString strPage;
	strPage.Format( _T("T. %dms"), m_iSimTime);
	pCmdUI->SetText( strPage ); 
}

void CPetriSimDoc::OnUpdateIndicatorsSimStatus(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(true);
	if(m_pSimThread)
	{
		if(m_bSingleStep)
			pCmdUI->SetText(_T("Step..."));
		else if (m_bRunNormal)
			pCmdUI->SetText(_T("Running..."));
		else if (m_bRunFast)
			pCmdUI->SetText(_T("Fast..."));
		else
			pCmdUI->SetText(_T("Online"));
	}
	else
		pCmdUI->SetText(_T("Offline"));
}

void CPetriSimDoc::OnUpdateUpdatePropertys(CCmdUI* pCmdUI) 
{
	if(!m_cplSelected.IsEmpty())pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}

void CPetriSimDoc::OnUpdatePropertys() 
{
	CHDrawObject* ptrObject = m_cplSelected.GetHead();
	ptrObject->Propertys();
	UpdateAllViews(NULL,0L,ptrObject);
	
}

void CPetriSimDoc::OnEditorShowlabel() 
{
	m_bShowLabel = !m_bShowLabel;
	UpdateAllViews(0);
}

void CPetriSimDoc::OnUpdateEditorShowlabel(CCmdUI* pCmdUI) 
{
	if(m_bShowLabel)pCmdUI->SetCheck(true);
	else pCmdUI->SetCheck(false);		
}

void CPetriSimDoc::OnChangedViewList() 
{
	/*
	if(m_pSimThread)
	{
		POSITION pos = GetFirstViewPosition();
		while (pos)
		{
			CView* pView = GetNextView(pos);
			if(pView->IsKindOf(RUNTIME_CLASS(CHSymStateFormView)))
			{
				m_pSimThread->SetOutputWindow(pView->GetSafeHwnd());
			}
		} 
	}
	*/
	CDocument::OnChangedViewList();
}

void CPetriSimDoc::OnSymReset() 
{
	//Reset Animation
	m_nAnimPhase = 5;
	AnimTokens();
	m_iSimTime = 0;
	m_iStepCount = 0;
	CHDrawObject* pObject;
	POSITION pos = m_colMembers.GetHeadPosition();
	while (pos)
	{
		pObject = m_colMembers.GetNext(pos);
		if(pObject->IsKindOf(RUNTIME_CLASS(CHTransition)))
		{
			CHTransition* pTran = static_cast <CHTransition*>
				(pObject);
			pTran->Reset();
			UpdateAllViews(NULL,0L,pTran);

		}
		if(pObject->IsKindOf(RUNTIME_CLASS(CHPosition)))
		{
			CHPosition* pPosi = static_cast <CHPosition*>
				(pObject);
			pPosi->Reset();
			UpdateAllViews(NULL, 0L, pPosi);
		}
	}
	m_pOutputBuffer->Empty();
	m_pOutputBuffer->AddLine(_T("Warning: Reset!"));
	AfxGetMainWnd()->PostMessage(WM_UPDATEOUTPUT, 0, (LPARAM)this);
	UpdateExplObject();
}

void CPetriSimDoc::OnSymRunfast() 
{
	if(!m_bRunFast)
	{
		DetachExplObject();
		m_bRunFast = true;
		m_bRunNormal = false;
		m_bSingleStep = false;
		VERIFY(m_cThreadEvents[CHT_EVENT_RUNFAST]->SetEvent());
		m_iTimerIdent = GetMainFrame().SetTimer(1, 100, 0);
	}
}

void CPetriSimDoc::OnUpdateSymRunfast(CCmdUI* pCmdUI) 
{
	if(m_bSimMode)
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);
	if(m_bRunFast)
		pCmdUI->SetCheck(true);
	else 
		pCmdUI->SetCheck(false);
}

void CPetriSimDoc::OnSymRunnormal() 
{
	if(!m_bRunNormal)
	{
		if(m_iTimerIdent)
			GetMainFrame().KillTimer(m_iTimerIdent);
		m_bRunFast = false;
		m_bRunNormal = true;
		VERIFY(m_cThreadEvents[CHT_EVENT_RUNNORMAL]->SetEvent());
	}
}

void CPetriSimDoc::OnUpdateSymRunnormal(CCmdUI* pCmdUI) 
{
	if(m_bSimMode)
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);
	if(m_bRunNormal)
		pCmdUI->SetCheck(true);
	else 
		pCmdUI->SetCheck(false);
}

void CPetriSimDoc::OnSymSinglestep() 
{
	if(!m_bSingleStep)
	{
		if(m_iTimerIdent)
			GetMainFrame().KillTimer(m_iTimerIdent);
		m_bRunNormal = false;
		m_bRunFast = false;
		m_bSingleStep = true;
		m_cThreadEvents[CHT_EVENT_RUNFAST]->ResetEvent();
		m_cThreadEvents[CHT_EVENT_STEP]->SetEvent();
	}	
}

void CPetriSimDoc::OnUpdateSymSinglestep(CCmdUI* pCmdUI) 
{
	if(m_bSimMode)
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);
	if(m_bSingleStep)
		pCmdUI->SetCheck(true);
	else 
		pCmdUI->SetCheck(false);
}

void CPetriSimDoc::OnSymStop() 
{
	m_uCursor = 0;
	if(m_bSimMode)
	{
		ASSERT(m_pSimThread);
		DetachExplObject();
		if(m_iTimerIdent)
			GetMainFrame().KillTimer(m_iTimerIdent);
		if(m_bRunFast)
			UpdateInvalidObjects();
		KillSimThread();
		m_bEnableOutputFile = false;
	}
	else
	{
		ASSERT(!m_pSimThread);
		DeselectAll();
		m_bSimStop = false;
		m_bSimMode = true;
		SaveModified();
		CWinThread* thread = AfxBeginThread(RUNTIME_CLASS(CHThread),
							THREAD_PRIORITY_BELOW_NORMAL,0,
							CREATE_SUSPENDED, NULL);
		m_pSimThread = static_cast <CHThread*> (thread);
		m_pSimThread->SetDoc(this);
		POSITION pos = GetFirstViewPosition();  

		// TODO:
		/*if(m_bPopupMessage)
		{
			CMDIMVDocTemplate* pDocTpl = (CMDIMVDocTemplate*)GetDocTemplate();
			pDocTpl->GetAssociatedDocFrameMgr(this)
				->ShowOrCreateFrame(ID_VIEW_SYM_OUTPUT);
		}*/
		// Now the thread can run wild
		m_pSimThread->ResumeThread();
		//Start Timer
		GetMainFrame().StartAnimTimer();
	}
	m_iDrawMode = DR_SELECT;
	UpdateAllViews(0);
}

void CPetriSimDoc::OnUpdateSymStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(true);
	if(m_bSimMode)
		pCmdUI->SetCheck(true);
	else 
		pCmdUI->SetCheck(false);
}

void CPetriSimDoc::OnPauseMode() 
{
	if(m_iTimerIdent)
		GetMainFrame().KillTimer(m_iTimerIdent);
	if(m_bRunFast)
			UpdateInvalidObjects();
	m_bRunNormal = false;
	m_bRunFast = false;
	m_bSingleStep = false;
	m_bStopNotif = false;
	m_cThreadEvents[CHT_EVENT_RUNFAST]->ResetEvent();
	m_cThreadEvents[CHT_EVENT_RUNNORMAL]->ResetEvent();
}

void CPetriSimDoc::OnUpdatePauseMode(CCmdUI* pCmdUI) 
{
	if(m_bStopNotif)OnPauseMode();
	if(m_bRunNormal || m_bRunFast)
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);
}

void CPetriSimDoc::OnUpdateSymReset(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(true);
	if(!m_bRunFast && !m_bSingleStep && !m_bRunNormal)
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);
}

void CPetriSimDoc::OnSymDecreasespeed() 
{
	int r = m_iSimSpeed % INCR_DECR_TIME;
	if(r < INCR_DECR_TIME / 2)m_iSimSpeed -= r;
	if(r >= INCR_DECR_TIME / 2)m_iSimSpeed += (INCR_DECR_TIME - r);
	if(m_iSimSpeed > MIN_TIME)
	{
		m_iSimSpeed -= INCR_DECR_TIME;
		CHSimToolbar* pBar = DYNAMIC_DOWNCAST(CHSimToolbar, 
					GetMainFrame().GetControlBar(IDR_SYMBAR));
		if(pBar)
			pBar->GetSliderSpeed()->SetPos(m_iSimSpeed);
	}
}

void CPetriSimDoc::OnUpdateSymDecreasespeed(CCmdUI* pCmdUI) 
{
	if(m_bSimMode && (m_iSimSpeed > MIN_TIME))
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);
}

void CPetriSimDoc::OnSymIncreasespeed() 
{
	int r = m_iSimSpeed % INCR_DECR_TIME;
	if(r < INCR_DECR_TIME / 2)m_iSimSpeed -= r;
	if(r >= INCR_DECR_TIME / 2)m_iSimSpeed += (INCR_DECR_TIME - r);
	if(m_iSimSpeed < MAX_ANIM_TIME)
	{
		m_iSimSpeed += INCR_DECR_TIME;
		CHSimToolbar* pBar = DYNAMIC_DOWNCAST(CHSimToolbar, 
					GetMainFrame().GetControlBar(IDR_SYMBAR));
		if(pBar)
			pBar->GetSliderSpeed()->SetPos(m_iSimSpeed);
	}
}

void CPetriSimDoc::OnUpdateSymIncreasespeed(CCmdUI* pCmdUI) 
{
	if(m_bSimMode && (m_iSimSpeed < MAX_ANIM_TIME))
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);
}

void CPetriSimDoc::StopSim()
{
	m_bStopNotif = true;
}

void CPetriSimDoc::OnEnableWriteoutputfile() 
{
	if(m_pSimThread)
	{
		if(m_bEnableOutputFile)
		{
			m_pSimThread->ResetOutputFile();
			m_bEnableOutputFile = false;
		}
		else
		{
			if(m_pSimThread->SetOutputFile(m_csOutputFilePath))
				m_bEnableOutputFile = true;
		}
	}
}

void CPetriSimDoc::OnUpdateEnableWriteoutputfile(CCmdUI* pCmdUI) 
{
	if(!m_bRunFast && !m_bSingleStep && !m_bRunNormal && m_bSimMode)
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);
	if(m_bEnableOutputFile)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CPetriSimDoc::OnUpdateEditorProperties(CCmdUI* pCmdUI) 
{
	if(!m_bSimMode)
		pCmdUI->Enable(true);
	else 
		pCmdUI->Enable(false);
}

void CPetriSimDoc::OnFileExportNet() 
{
	CFileDialog dlg(false, 0, 0, OFN_HIDEREADONLY,
		_T("Text Files (*.txt)|*.txt|All Files|*.*||"));
	if(dlg.DoModal()==IDOK)
	{
		CString path(dlg.GetPathName());
		if(dlg.GetFileExt().IsEmpty())
			path += ".txt";

		CStdioFile cfile(path, CFile::modeCreate|CFile::modeWrite);

		CWaitCursor wait;

		TCHAR buffer[80];
		CTypedPtrList<CPtrList,CHTransition*> TranList;
		CTypedPtrList<CPtrList,CHPosition*> PosiList;
		//aus der gesamten Liste die Transitionen
		//und Plätze aussortieren
		POSITION pos = m_colMembers.GetHeadPosition();
		while (pos)
		{
			CHDrawObject* pObject = m_colMembers.GetNext(pos);
			if(pObject->IsKindOf( RUNTIME_CLASS(CHTransition)))
			{
				TranList.
					AddTail(static_cast <CHTransition*> (pObject));
			}
			if(pObject->IsKindOf( RUNTIME_CLASS(CHPosition)))
			{
				PosiList.
					AddTail(static_cast <CHPosition*> (pObject));
			}
		}

	try{
			//Transition List
			cfile.WriteString(_T("// Transition Name Vector:\n"));
			pos = TranList.GetHeadPosition();
			while (pos)
			{
				CHTransition* pTran = TranList.GetNext(pos);
				cfile.WriteString(pTran->GetLabelA()->GetText());
				cfile.WriteString(_T(" ;"));
			}
			cfile.WriteString(_T(")\n"));
			//Position List
			cfile.WriteString(_T("// Position Name Vector:\n(_T("));
			pos = PosiList.GetHeadPosition();
			while (pos)
			{
				CHPosition* pPosi = PosiList.GetNext(pos);
				cfile.WriteString(pPosi->GetLabelA()->GetText());
				cfile.WriteString(_T(";"));
			}
			cfile.WriteString(_T(")\n"));
			//Inzidenzmatrix
			int maxwight = 0;
			POSITION pos = m_colMembers.GetHeadPosition();
			while (pos)
			{
				CHDrawObject* pObject = m_colMembers.GetNext(pos);
				if(pObject->IsKindOf( RUNTIME_CLASS(CHConnector)))
				{
					CHConnector* pConn = static_cast <CHConnector*> (pObject);
					maxwight = max(maxwight, (int)pConn->GetWeight());
				}
			}
			//Erzeugt aus der Liste die entsprechende Inzidenz 
			//Matrix.
			//Diese Form könnte für weitere Berechnungen
			//nützlich sein
			cfile.WriteString(_T("// Inzidenz Matrix:\n{\n"));
			pos = PosiList.GetHeadPosition();
			while (pos)
			{
				cfile.WriteString(_T("(_T("));
				CHPosition* pPosi = PosiList.GetNext(pos);
				POSITION posA = TranList.GetHeadPosition();
				while (posA)
				{
					long connection = 0;
					CHTransition* pTran = TranList.GetNext(posA);
					CONN_LIST& rInList = pTran->GetInList();		
					POSITION posB = rInList.GetHeadPosition();
					while (posB)
					{
						CHConnector* pConn = rInList.GetNext(posB);
						CHPosition* pPosiA = static_cast <CHPosition*> 
							(pConn->GetPtrFrom());
						if((pConn->GetTyp() == CT_NORMAL) &&
							(pPosiA == pPosi))
						{
							connection += pConn->GetWeight();
							//connection = -connection;
						}
					}
					CONN_LIST& rOutList = pTran->GetOutList();		
					posB = rOutList.GetHeadPosition();
					while (posB)
					{
						CHConnector* pConn = rOutList.GetNext(posB);
						CHPosition* pPosiA = static_cast<CHPosition*> 
							(pConn->GetPtrTo());
						if((pConn->GetTyp() == CT_NORMAL) &&
							(pPosiA == pPosi))
						{
							connection = pConn->GetWeight();
							connection = -connection;
						}
					}
					TCHAR format[10];
					StringCbPrintf(format, sizeof(format)
						, _T("%%%dd "), maxwight + 1);
					StringCbPrintf(buffer, sizeof(buffer)
						, format, connection);
					cfile.WriteString(buffer);
				}
				cfile.WriteString(_T(")\n"));
			}
			cfile.WriteString(_T("}\n"));
			//Marking Vector
			cfile.WriteString(_T("// Marking Vector:\n(_T("));
			pos = PosiList.GetHeadPosition();
			while (pos)
			{
				CHPosition* pPosi = PosiList.GetNext(pos);
				StringCbPrintf(buffer, sizeof(buffer), _T("%d ")
					, pPosi->GetToken());

				cfile.WriteString(buffer);
			}
			cfile.WriteString(_T(")\n"));
			//Arc Type Matrix
			cfile.WriteString(_T("// Arc Type Matrix:\n"));
			cfile.WriteString(_T("// Code:0 = None; 1 = Normal; 2 = Inhibitor; 3 = Test \n{\n"));
			pos = PosiList.GetHeadPosition();
			while (pos)
			{
				cfile.WriteString(_T("(_T("));
				CHPosition* pPosi = PosiList.GetNext(pos);
				POSITION posA = TranList.GetHeadPosition();
				while (posA)
				{
					CT_TYP connectortyp = CT_NONE;
					CHTransition* pTran = TranList.GetNext(posA);
					CONN_LIST& rInList = pTran->GetInList();		
					POSITION posB = rInList.GetHeadPosition();
					while (posB)
					{
						CHConnector* pConn = rInList.GetNext(posB);
						CHPosition* pPosiA = static_cast <CHPosition*> 
							(pConn->GetPtrFrom());
						if(pPosiA == pPosi)
						{
							connectortyp = pConn->GetTyp();
						}
					}
					CONN_LIST& rOutList = pTran->GetOutList();		
					posB = rOutList.GetHeadPosition();
					while (posB)
					{
						CHConnector* pConn = rOutList.GetNext(posB);
						CHPosition* pPosiA = static_cast <CHPosition*> 
							(pConn->GetPtrTo());
						if(pPosiA == pPosi)
						{
							connectortyp = pConn->GetTyp();
						}
					}
					switch(connectortyp)
					{
					case CT_NONE:
						cfile.WriteString(_T("0 "));
						break;
					case CT_NORMAL:
						cfile.WriteString(_T("1 "));
						break;
					case CT_INHIBITOR:
						cfile.WriteString(_T("2 "));
						break;
					case CT_TEST:
						cfile.WriteString(_T("3 "));
						break;
					}
				}
				cfile.WriteString(_T(")\n"));
			}
			cfile.WriteString(_T("}\n"));
			//Transition Time Model Vektor
			cfile.WriteString(_T("// Transition Time Model Vektor:\n"));
			cfile.WriteString(_T("// Code:1 = Immidiate; 2= Delay;3 = Exponential; 4 = Equal Distibution;\n(_T("));
			pos = TranList.GetHeadPosition();
			while (pos)
			{
				CHTransition* pTran = TranList.GetNext(pos);
				switch(pTran->GetTimeMode())
				{
				case TTM_IMMIDIATE:
					cfile.WriteString(_T("1 "));
					break;
				case TTM_DELAY:
					cfile.WriteString(_T("2 "));
					break;
				case TTM_EXPONENTIAL:
					cfile.WriteString(_T("3 "));
					break;
				case TTM_EQUAL_DISTR:
					cfile.WriteString(_T("4 "));
					break;
				}
				cfile.WriteString(_T(";"));
			}

		}catch(...)
		{
			return;
		}
	}	
}

void CPetriSimDoc::OnFileExportInterchange() 
{

	CFileDialog dlg(false, 0, 0, OFN_HIDEREADONLY,
		_T("HPSim Export Files (*.hpx)|*.hpx|All Files|*.*||"));

	if(dlg.DoModal()==IDOK)
	{
		CString path(dlg.GetPathName());
		if(dlg.GetFileExt().IsEmpty())
			path += ".hpx";
		CIniSection cini;
		cini.OpenFile(path, true);
		CWaitCursor wait;
		cini.SetSection(_T("DOCUMENT"));
		cini.SetValue(_T("APPLICATION"), "HPSim");
		cini.SetValue(_T("VERSION"), "0.9");
		cini.SetValue(_T("LAST_CONN_ID"), m_iNextFreeConnectorIdent);
		cini.SetValue(_T("LAST_POSI_ID"), m_iNextFreePositionIdent);
		cini.SetValue(_T("LAST_TRAN_ID"), m_iNextFreeTransitionIdent);
		cini.SetValue(_T("DOC_SIZE"), m_sizeDoc);
		cini.WriteSection();
		CString buffer;
		int isectioncount = 0;
		POSITION pos = m_colMembers.GetHeadPosition();
		CProgressBar bar(_T("Write File..."), 70, 
			m_colMembers.GetCount(), false);
		bar.SetStepRate(m_colMembers.GetCount() / 100);
		while (pos)
		{
			CHDrawObject* pObject = m_colMembers.GetNext(pos);
			isectioncount++;
			buffer.Format(_T("OBJECT %d"), isectioncount);
			cini.SetSection(buffer);
			pObject->WriteExportFormat(cini);
			cini.WriteSection();
			bar.StepIt();
		}
	}	
}

void CPetriSimDoc::OnFileImportInterchange() 
{
	CFileDialog dlg(true, 0, 0, OFN_HIDEREADONLY,
		_T("HPSim Export Files (*.hpx)|*.hpx|All Files|*.*||"));

	if((dlg.DoModal()==IDOK) &&
		(AfxMessageBox(_T("All data in this Document will be erased!")
		, MB_OKCANCEL) == IDOK))
	{
		CString path(dlg.GetPathName());
		if(dlg.GetFileExt().IsEmpty())
			path += _T(".hpx");
		CIniSection ini;
		if(!ini.OpenFile(path))
		{
			AfxMessageBox(IDS_ERROR_FILE_OPEN);
			return;
		}
		CWaitCursor wait;
		CString classname;
		ini.FindSection(_T("DOCUMENT"));
		ini.GetValue(_T("DOC_SIZE"), m_sizeDoc);
		ini.GetValue(_T("APPLICATION"), classname);
		if(classname != "HPSim")
		{
			CHErrorFile::WriteError(_T("OnFileImportInterchange")
				, _T("Unknown App Name"), this);

			AfxMessageBox(_T("Error: Unknown App Name"));
			return;
		}
		ini.GetValue(_T("VERSION"), classname);
		if(classname != "0.9")
		{
			CHErrorFile::WriteError(_T("OnFileImportInterchange"),
				_T("Can not import this Version"), this);
			AfxMessageBox(_T("Error: Can not import this Version"));
			return;
		}
		DeleteContents();
		ini.GetValue(_T("LAST_CONN_ID"), m_iNextFreeConnectorIdent);
		ini.GetValue(_T("LAST_POSI_ID"), m_iNextFreePositionIdent);
		ini.GetValue(_T("LAST_TRAN_ID"), m_iNextFreeTransitionIdent);
		CHDrawObject* pObject = 0;

		CProgressBar* pbar = new CProgressBar(_T("Read File..."), 70,
			static_cast<int>(ini.GetFile().GetLength()), false);

		pbar->SetStepRate(static_cast<int>(ini.GetFile().GetLength()) / 100);
		while(ini.ReadNextSection())
		{
			if(ini.GetSection().Find(_T("OBJECT")) == -1)
				continue;
			ini.GetValue(_T("RUNTIME_CLASS"), classname);
			if(classname == "CHRect")
				pObject = new CHRect(CPoint(0,0));
			else if(classname == "CHText")
				pObject = new CHText(CPoint(0,0), &m_lfDefaultFont);
			else if(classname == "CHLabel")
				pObject = new CHLabel();
			else if(classname == "CHPoly")
				pObject = new CHPoly();
			else if(classname == "CHLine")
				pObject = new CHLine();
			else if(classname == "CHConnector")
				pObject = new CHConnector();
			else if(classname == "CHPosition")
				pObject = new CHPosition();
			else if(classname == "CHTransition")
				pObject = new CHTransition();
			if(pObject)
			{
				pObject->ReadExportFormat(ini);
				m_colMembers.AddTail(pObject);
			}
			else
			{
				CHErrorFile::WriteError(_T("OnFileImportInterchange")
					, _T("Memory"), this);
				AfxMessageBox(IDS_ERROR_MEMORY);
				DeleteContents();
				UpdateAllViews(NULL,3L,NULL);
				return;
			}
			pbar->SetPos(static_cast<int>(ini.GetFile().GetPosition()));
		}
		delete pbar;
		if(!InitializeNet(m_colMembers))
		{
			DeleteContents();
		}
		InitMap(m_colMembers);
		UpdateAllViews(NULL,3L,NULL);
		PeakAndPump();
	}
}

void CPetriSimDoc::OnRemoveSelected() 
{
	DeleteSelected();	
}

void CPetriSimDoc::OnUpdateRemoveSelected(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_cplSelected.IsEmpty());
}

//
void CPetriSimDoc::OnPanMode() 
{
	m_iDrawMode = DR_PAN;
	m_uCursor = NULL;
}

void CPetriSimDoc::OnUpdatePanMode(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(m_iDrawMode == DR_PAN);
}

bool CPetriSimDoc::PeakAndPump()
{
	static MSG msg;

	while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)) {
		if (!AfxGetApp()->PumpMessage()) {
			::PostQuitMessage(0);
			return false;
		}	
	}
	return true;
}

bool CPetriSimDoc::AnimTokens()
{
	switch(m_nAnimPhase)
	{
	case 0:
		{
			return true;
		}
	case 1:
		{
			POSITION pos = m_colAnimInput.GetHeadPosition();
			while (pos)
			{
				CHConnector* pCon = m_colAnimInput.GetNext(pos);
				pCon->StepTokenAnim(m_nAnimSteps, true);
				UpdateAllViews(0, 0, pCon);
			}
			m_nAnimCurSteps--;
			if(m_nAnimCurSteps == 0)
				m_nAnimPhase = 2;
			return true;
		}
	case 2: 
		{
			POSITION pos = m_colAnimInput.GetHeadPosition();
			while (pos)
			{
				CHConnector* pCon = m_colAnimInput.GetNext(pos);
				m_UpdateRect.m_cRect = pCon->GetBoundingRect(0, true);
				pCon->StepTokenAnim(m_nAnimSteps, false);
				UpdateAllViews(0, 1L, &m_UpdateRect);
			}
			m_nAnimCurSteps = m_nAnimSteps;
			pos = m_colAnimOutput.GetHeadPosition();
			while (pos)
			{
				CHConnector* pCon = m_colAnimOutput.GetNext(pos);
				pCon->StartTokenAnim(m_nAnimSteps);
				UpdateAllViews(0, 0, pCon);
			}
			m_nAnimPhase = 3;
			return true;
		}
	case 3:
		{
			POSITION pos = m_colAnimOutput.GetHeadPosition();
			while (pos)
			{
				CHConnector* pCon = m_colAnimOutput.GetNext(pos);
				pCon->StepTokenAnim(m_nAnimSteps, true);
				UpdateAllViews(0, 0, pCon);
			}
			m_nAnimCurSteps--;
			if(m_nAnimCurSteps == 0)
				m_nAnimPhase = 4;
			return true;
		}
	case 4:
		{
			POSITION pos = m_colAnimOutput.GetHeadPosition();
			while (pos)
			{
				CHConnector* pCon = m_colAnimOutput.GetNext(pos);
				m_UpdateRect.m_cRect = pCon->GetBoundingRect(0, true);
				pCon->StepTokenAnim(m_nAnimSteps, false);
				UpdateAllViews(0, 1L, &m_UpdateRect);
				
			}
			m_colAnimInput.RemoveAll();
			m_colAnimOutput.RemoveAll();
			m_nAnimPhase = 0;
			m_pAnimEvent->SetEvent();
			return false;
		}
	case 5://Reset
		{
			POSITION pos = m_colAnimInput.GetHeadPosition();
			while (pos)
			{
				CHConnector* pCon = m_colAnimInput.GetNext(pos);
				pCon->StepTokenAnim(m_nAnimSteps, false);
				UpdateAllViews(0, 0, pCon);
			}
			pos = m_colAnimOutput.GetHeadPosition();
			while (pos)
			{
				CHConnector* pCon = m_colAnimOutput.GetNext(pos);
				pCon->StepTokenAnim(m_nAnimSteps, false);
				UpdateAllViews(0, 0, pCon);
			}
			m_colAnimInput.RemoveAll();
			m_colAnimOutput.RemoveAll();
			m_nAnimPhase = 0;
			m_bSingleStep = false;
			return false;
		}
	}
	return false;	
}

void CPetriSimDoc::UpdateInvalidObjects()
{
	CSingleLock sLock(&m_cCSection, true);
	if(m_bThreadError)
	{
		m_bThreadError = false;
		OnPauseMode();
	}
	else
	{
		CHDrawObject* pObject;
		POSITION pos = m_colMembers.GetHeadPosition();
		while (pos)
		{
			pObject = m_colMembers.GetNext(pos);
			if(pObject->IsInvalid())
			{
				UpdateAllViews(0, 0, pObject);
			}
			pObject->SetInvalid(false);
		}
		//start animation
		if(m_bRunNormal || m_bSingleStep)
		{
			if(m_iSimSpeed >= MIN_ANIM_TIME && m_nAnimPhase == 0 
			&& (m_colAnimInput.GetCount() || m_colAnimOutput.GetCount()))
			{
				
				m_nAnimPhase = 1;
				m_nAnimSteps = m_iSimSpeed / 50;
				m_nAnimCurSteps = m_nAnimSteps;
				POSITION pos = m_colAnimInput.GetHeadPosition();
				while (pos)
				{
					CHConnector* pCon = m_colAnimInput.GetNext(pos);
					pCon->StartTokenAnim(m_nAnimSteps);
					UpdateAllViews(0, 0, pCon);
				}
			}
			else
			{
				m_pAnimEvent->SetEvent();
				m_nAnimPhase = 5;
				AnimTokens();
				m_bSingleStep = false;
			}
		}
		UpdateExplObject();
		if(m_pSimThread)
		{
			if(m_bRunNormal)
			{
				m_bRunFast = false;
				VERIFY(m_cThreadEvents[CHT_EVENT_RUNNORMAL]->SetEvent());
			}
		}
	}
}

LPCTSTR CPetriSimDoc::GetCursor(CPoint &cpoint)
{
	if(m_iDrawMode == DR_SELECT && m_cplSelected.GetCount() == 1)
	{
		CHDrawObject* pObject = m_cplSelected.GetHead();
		LPCTSTR cursor = pObject->GetCursorEx(cpoint);
		return (cursor)? cursor : IDC_ARROW;
	}
	else if(m_iDrawMode == DR_SELECT && m_cplSelected.GetCount() > 1)
	{
		POSITION pos = m_cplSelected.GetHeadPosition();
		while (pos != NULL)
		{
			CHDrawObject* pObject = m_cplSelected.GetNext(pos);
			if(pObject->PtInObject(cpoint))
				return IDC_SIZEALL;
		}
	}
	return NULL;
}
