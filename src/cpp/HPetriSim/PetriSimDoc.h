/**************************************************************************
	PetriSimDoc.h
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#if !defined(AFX_H_PETRI_SYMDOC_H__1DD387AD_E907_11D1_94E3_9187B43D300C__INCLUDED_)
#define AFX_H_PETRI_SYMDOC_H__1DD387AD_E907_11D1_94E3_9187B43D300C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "HDrawObject.h"

class CPetriSimDoc;
class CHThread;
class CPetriSimView;
class CMainFrame;
class CHLineBuffer;

class CPropertiesObject : public CObject
{
public:
	CPetriSimDoc *m_pDoc;
	CHDrawObject *m_pObject;
};

class CUpdateRectObject : public CObject
{
public:
	CRect m_cRect;
};

//Hints for UpdateAllViews
enum UPDATE_HINTS{UH_RECT = 1};
enum STEP_MOVE_MODE{STEP_MOVE_DOWN, STEP_MOVE_RIGHT, STEP_MOVE_UP, 
	STEP_MOVE_LEFT};

const int CHT_HANDLES_COUNT = 5;
enum CHT_HANDLES{CHT_EVENT_KILL = 0,CHT_EVENT_STEP,CHT_EVENT_RUNNORMAL,
	CHT_EVENT_RUNFAST, CHT_EVENT_DEAD};

const int OUT_BUFFER_LENGHT = 2000;

typedef CTypedPtrList<CObList,CHDrawObject*> CHDOBJECT_LIST;
typedef CTypedPtrList<CObList,CHConnector*> CH_ANIMATOR_LIST;

class CPetriSimDoc : public CDocument
{

public:
	DECLARE_DYNCREATE(CPetriSimDoc)

	CPetriSimDoc();
	virtual ~CPetriSimDoc();
	
	int m_iHandle;
	CMap< LPCSTR, LPCSTR, CHDrawObject*, CHDrawObject* > myMap2;
	CH_ANIMATOR_LIST m_colAnimInput;
	CH_ANIMATOR_LIST m_colAnimOutput;
	CHDOBJECT_LIST m_colMembers;
	CHDOBJECT_LIST m_cplSelected;
	CMap<CHDrawObject*,CHDrawObject*, POSITION, POSITION&> m_mapMembers;
	CCriticalSection m_cCSection;
	CEvent* m_cThreadEvents[CHT_HANDLES_COUNT];
	CEvent* m_pAnimEvent;

private:
	int m_nAnimSteps;
	bool m_bPopupMessage;
	bool m_bPopupExplorer;
	bool m_bEnableOutputFile;
	bool m_bThreadError;
	bool m_bStopNotif;
	bool m_bMoveOnGrid;
	bool m_bAlignDo;
	bool m_bAlignNo;
	bool m_bGridVisible;
	bool m_bShowLabel;
	bool m_bSimMode;
	bool m_bSingleStep;
	bool m_bRunNormal;
	bool m_bSimStop;
	bool m_bThreadCalc;
	int m_iDrawMode;
	int m_iGrid;
	long m_iStepCount;
	long m_iStopTime;
	long m_iStopStep;
	long m_iTimerIdent;
	long m_iStepTime;
	long m_iCalcTime;
	long m_iSimSpeed;
	long m_iSimTime;
	long m_iSimSampleTime;
	long m_uCursor;
	long m_iNextFreePositionIdent;
	long m_iNextFreeTransitionIdent;
	long m_iNextFreeConnectorIdent;
	COLORREF m_iBackColor;
	CHLineBuffer* m_pOutputBuffer;
	CFont *m_font;
	CHDrawObject* object;
	CSize m_sizeDoc;
	CHThread* m_pSimThread;
	CString m_csOutputFilePath;
	CHDrawObject* m_ExplObject;
	CPropertiesObject m_SenderObject;
	CUpdateRectObject m_UpdateRect;

private:
	int m_nAnimCurSteps;
	bool HandleInitError(const LPCTSTR cause = 0);
	CMainFrame& GetMainFrame();
public:
	LPCTSTR GetCursor(CPoint & cpoint);
	bool m_bRunFast;
	int m_nAnimPhase;
	UINT m_nIDEvent;
	bool AnimTokens();
	bool IfRunFast(){return m_bRunFast;};
	bool AddListToMembers(CHDOBJECT_LIST & list);
	bool InitMap(CHDOBJECT_LIST & list);
	bool PeakAndPump();
	COLORREF GetBackColor(){return m_iBackColor;};
	CHDOBJECT_LIST& GetDrawObjectList(){return m_colMembers;};
	CHLineBuffer* GetOutputBuffer(){return m_pOutputBuffer;};
	void UpdateOutputWindow();
	void UpdateExplObject();
	void SetEnabledOutputFile(bool enabled){m_bEnableOutputFile = enabled;};
	bool IsEnabledOutputFile(){return m_bEnableOutputFile;};
	CString GetOutputFilePath(){return m_csOutputFilePath;};
	void SetThreadError(bool error){m_bThreadError = error;};
	void StopSim();
	CEvent* GetThreadEvent(CHT_HANDLES event){return m_cThreadEvents[event];};
	void SetCalcTime(int time){m_iCalcTime = time;};
	void IncrSimTime(){m_iSimTime += m_iSimSampleTime;};
	void IncrStepCount(){m_iStepCount++;};
	void SetSimSampleTime(UINT time){m_iSimSampleTime = time;};
	UINT GetSimSampleTime(){return m_iSimSampleTime;};
	UINT GetStopTime(){return m_iStopTime;};
	UINT GetStopStep(){return m_iStopStep;};
	UINT GetStepCount(){return m_iStepCount;};
	void SetSimTime(UINT time){m_iSimTime = time;};
	UINT GetSimTime(){return m_iSimTime;};
	void SetStepTime(int time){m_iStepTime = time;};
	void StepMove(int direction);
	void SetSimSpeed(int speed){m_iSimSpeed = speed; SetModifiedFlag(true);};
	int GetSimSpeed(){return m_iSimSpeed;};
	void UpdateOtherViews(CView * pView);
	void KillSimThread();
	void UpdateInvalidObjects();
	void DetachExplObject(bool test = false);
	void AttachExplObject(CHDrawObject* pObject);
	void SetSimMode(bool set){m_bSimMode = set;};
	bool GetSimMode(){return m_bSimMode;};
	void SetMoveOnGrid(bool set){m_bMoveOnGrid = set;};
	bool GetMoveOnGrid(){return m_bMoveOnGrid;};
	void SetAlignDo(bool set){m_bAlignDo = set;};
	bool GetAlignDo(){return m_bAlignDo;};
	void SetAlignNo(bool set){m_bAlignNo = set;};
	bool GetAlignNo(){return m_bAlignNo;};
	void Draw(CDC* pDC,CRect & client,bool grid);
	int GetGrid(){return m_iGrid;};
	void PointToGrid(CPoint* point);
	void DeselectAll();
	void Select(CRect & rect);
	void Select(CPoint & point);
	void DeleteAll();
	bool DeleteSelected();
	void ResizeObject(CPoint & to, CPetriSimView * pView, 
		bool init = false);
	void MoveObject(CPoint & to,  CPetriSimView * pView);
	bool CanMove(CPoint & point);
	CHDrawObject* NewObject(CPoint & start,CPoint & end,CDC * pDC);
	void RecalcIdent(CHDOBJECT_LIST & list);
	bool InitializeNet(CHDOBJECT_LIST & list);
	void StrippNet(CHDOBJECT_LIST & list);
	UINT GetCursor(){return m_uCursor;};
	void SetCursor(UINT cursor){m_uCursor = cursor;};
	void OnProperties();
	void UnGroup();
	void Group();
	void PushBack();
	void PopUp();
	LOGFONT m_lfDefaultFont;
	void ToGrid();
	CSize GetDocSize() { return m_sizeDoc; }
	int GetDrawMode(){return m_iDrawMode;};
	void SetDrawMode(int mode){m_iDrawMode = mode;};
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	virtual void OnChangedViewList();

protected:
	afx_msg void OnUpdateMultiLineMode(CCmdUI* pCmdUI);
	afx_msg void OnMultiLineMode();
	afx_msg void OnPositionMode();
	afx_msg void OnUpdatePositionMode(CCmdUI* pCmdUI);
	afx_msg void OnRectMode();
	afx_msg void OnUpdateRectMode(CCmdUI* pCmdUI);
	afx_msg void OnSelectMode();
	afx_msg void OnUpdateSelectMode(CCmdUI* pCmdUI);
	afx_msg void OnTransitionMode();
	afx_msg void OnUpdateTransitionMode(CCmdUI* pCmdUI);
	afx_msg void OnLineMode();
	afx_msg void OnUpdateLineMode(CCmdUI* pCmdUI);
	afx_msg void OnConnectorMode();
	afx_msg void OnUpdateConnectorMode(CCmdUI* pCmdUI);
	afx_msg void OnTextMode();
	afx_msg void OnUpdateTextMode(CCmdUI* pCmdUI);
	afx_msg void OnStepMode();
	afx_msg void OnUpdateStepMode(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUpdatePropertys(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePropertys();
	afx_msg void OnObjectPropertys();
	afx_msg void OnEditorProperties();
	afx_msg void OnEditorShowlabel();
	afx_msg void OnUpdateEditorShowlabel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIndicatorsStepTime(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIndicatorsSimTime(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIndicatorsSimStatus(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIndicatorsCount(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSliderSimspeed(CCmdUI* pCmdUI);
	afx_msg void OnSymReset();
	afx_msg void OnSymRunfast();
	afx_msg void OnUpdateSymRunfast(CCmdUI* pCmdUI);
	afx_msg void OnSymRunnormal();
	afx_msg void OnUpdateSymRunnormal(CCmdUI* pCmdUI);
	afx_msg void OnSymSinglestep();
	afx_msg void OnUpdateSymSinglestep(CCmdUI* pCmdUI);
	afx_msg void OnSymStop();
	afx_msg void OnUpdateSymStop(CCmdUI* pCmdUI);
	afx_msg void OnPauseMode();
	afx_msg void OnUpdatePauseMode(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSymReset(CCmdUI* pCmdUI);
	afx_msg void OnSymDecreasespeed();
	afx_msg void OnUpdateSymDecreasespeed(CCmdUI* pCmdUI);
	afx_msg void OnSymIncreasespeed();
	afx_msg void OnUpdateSymIncreasespeed(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIndicatorsStepCount(CCmdUI* pCmdUI);
	afx_msg void OnEnableWriteoutputfile();
	afx_msg void OnUpdateEnableWriteoutputfile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditorProperties(CCmdUI* pCmdUI);
	afx_msg void OnFileExportNet();
	afx_msg void OnFileExportInterchange();
	afx_msg void OnFileImportInterchange();
	afx_msg void OnRemoveSelected();
	afx_msg void OnUpdateRemoveSelected(CCmdUI* pCmdUI);
	afx_msg void OnPanMode();
	afx_msg void OnUpdatePanMode(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_H_PETRI_SYMDOC_H__1DD387AD_E907_11D1_94E3_9187B43D300C__INCLUDED_)
