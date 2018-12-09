/**************************************************************************
	HDrawObject.h
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#ifndef AFX_HDRAWOBJECT_H__84A4D540_EA85_11D1_94E3_BB7F79BF0103__INCLUDED_
#define AFX_HDRAWOBJECT_H__84A4D540_EA85_11D1_94E3_BB7F79BF0103__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// taken from MFC Grid Control header file
enum GV_TYPE{GVT_DEFAULT = 1, GVT_BOOL, GVT_COLOR, GVT_FONT, GVT_ITEMS};

typedef struct _GV_ITEM { 
	int      row,col;     // Row and Column of item
	UINT     mask;        // Mask for use in getting/setting cell data
	UINT     state;       // cell state (focus/highlighted etc)
	UINT     nFormat;     // Format of cell
	CString  szText;      // Text in cell
	int      iImage;      // index of the list view item’s icon
	COLORREF crBkClr;     // Background color (or CLR_DEFAULT)
	COLORREF crFgClr;     // Foreground color (or CLR_DEFAULT)
	LPARAM   lParam;      // 32-bit value to associate with item 
	LOGFONT  lfFont;      // Cell font
	GV_TYPE  type;        // Property to edit
	CStringArray array;   // Items for Edit List
} GV_ITEM;

#define GVIF_TEXT               LVIF_TEXT
#define GVIF_IMAGE              LVIF_IMAGE
#define GVIF_PARAM              LVIF_PARAM
#define GVIF_STATE              LVIF_STATE
#define GVIF_BKCLR              (GVIF_STATE<<1)
#define GVIF_FGCLR              (GVIF_STATE<<2)
#define GVIF_FORMAT             (GVIF_STATE<<3)
#define GVIF_FONT               (GVIF_STATE<<4)
#define GVIF_TYPE               (GVIF_STATE<<5)
#define GVIF_ITEM               (GVIF_STATE<<6)
// <end> taken from MFC Grid Control header file

/*************************************************************************/
// CHDrawObject
/*************************************************************************/

class CIniSection;
class CHDrawObject : public CObject
{

public:
	CHDrawObject();
	virtual ~CHDrawObject();

	virtual void MoveTo(POINT & to) = 0;
	virtual void ResizeTo(POINT & to,int handle) = 0; 
	virtual void DrawObject(CDC* pdc) = 0;
	virtual bool PtInObject(POINT& point) = 0;
	virtual int PtInGripper(POINT& point){return 0;};
	virtual CRect GetBoundingRect(CDC* pDC = NULL,bool mode = false) = 0;
	virtual void Propertys(){ASSERT(false);};
	virtual void ToGrid(int grid,int handle = NULL){};
	virtual bool IsPMember(){return false;};
	virtual void InitMenu(CMenu* menu,CPoint & point){};
	virtual void DoID(UINT nID){};
	virtual LPCTSTR GetCursor(int handle) { return IDC_SIZEALL; };
	virtual LPCTSTR GetCursorEx(CPoint& point);
	virtual void Select(bool sel) {ASSERT_VALID(this);m_blSelected = sel;};
	virtual bool GetProperty(GV_ITEM* pItem) { return false; };
 	virtual bool SetProperty(GV_ITEM* pItem) { return false; };
	virtual CString GetPropertyText(UINT par) {return _T(""); };
	virtual void WriteExportFormat(CIniSection& ini) {};
	virtual void ReadExportFormat(CIniSection& ini) {};
	virtual long GetIdent() { return m_iIdent; };
	virtual void SetIdent(long id) { m_iIdent = id; };
	virtual void AddPoint(POINT & point){};

	bool IfSelected();
	CRect GetRect();
	void SetRect(CRect& rect);
	bool IsInvalid();
	void SetInvalid(bool invalid);

protected:
	CRect m_crRect;
	bool m_blSelected;
	bool m_bInvalid;
	long m_iIdent;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
};

/*************************************************************************/
// CHLabel
/*************************************************************************/

class CHLabel : public CHDrawObject  
{

public:
	CHLabel();
	virtual ~CHLabel();

	CHLabel(int top,int left,CString string
		,long owner = 0,LABEL_SUB sub = LS_LABEL_A);

	virtual void WriteExportFormat(CIniSection& ini);
	virtual void ReadExportFormat(CIniSection& ini);
	virtual void ToGrid(int grid,int handle = NULL);
	virtual void Serialize(CArchive& ar);
	virtual void DoID(UINT nID);
	virtual void InitMenu(CMenu* menu,CPoint & point);
	virtual void ResizeTo(POINT & to,int handle){};
	virtual void MoveTo(POINT & to);
	virtual void DrawObject(CDC* pDC);
	virtual int PtInGripper(POINT& point){return 0;};
	virtual bool PtInObject(POINT& point);
	virtual CRect GetBoundingRect(CDC* pDC = NULL,bool mode = false);
	LABEL_SUB GetSubIdent(){return m_iSubIdent;};
	void SetSubIdent(LABEL_SUB sub){m_iSubIdent = sub;};
	long GetOwner(){return m_iOwner;};
	void SetOwner(long owner){m_iOwner = owner;};
	CString & GetText(){return m_csText;};
	void SetText(CString & text){m_csText = text;};
	void SetText(const TCHAR * text){m_csText = text;};
	bool GetVisible(){return m_bVisible;};
	void SetVisible(bool set){m_bVisible = set;};
	CRect GetGripper(int nHandle);

protected:
	DECLARE_SERIAL(CHLabel)


protected:
	LABEL_SUB m_iSubIdent;
	long m_iOwner;
	bool m_bVisible;
	bool m_bBorder;
	bool m_bEmpty;
	CString m_csText;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

/*************************************************************************/
// CHNetMember
/*************************************************************************/

class CHNetMember
{

public:
	CHNetMember() { m_chlLabelA = 0; m_chlLabelB = 0; };
	virtual ~CHNetMember(){};

	CHLabel* GetLabelA(){return m_chlLabelA;};
	CHLabel* GetLabelB(){return m_chlLabelB;};
	void SetLabelA(CHLabel* label);
	void SetLabelB(CHLabel* label);


protected:
	CHLabel* m_chlLabelA;
	CHLabel* m_chlLabelB;

};

/*************************************************************************/
// CHText
/*************************************************************************/

class CHText : public CHLabel  
{

public:
	CHText(CPoint& topleft,LOGFONT* stfont);
	virtual ~CHText();

	virtual void WriteExportFormat(CIniSection& ini);
	virtual void ReadExportFormat(CIniSection& ini);
	virtual bool GetProperty(GV_ITEM* pItem);
	virtual bool SetProperty(GV_ITEM* pItem);
	virtual CString GetPropertyText(UINT par);
	virtual void Serialize(CArchive& ar);
	virtual void DoID(UINT nID);
	virtual void InitMenu(CMenu* menu,CPoint & point);
	virtual CRect GetBoundingRect(CDC* pDC = NULL,bool mode = false);
	virtual void DrawObject(CDC* pDC);
	CRect GetRect(){return m_crRect;};

protected:
	CHText();
	DECLARE_SERIAL(CHText)

protected:
	LOGFONT m_lfFont;
	COLORREF m_crColor;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

/*************************************************************************/
// CHRect
/*************************************************************************/

enum {RS_RECT, RS_ROUND, RS_ELLIPSE};

class CHRect : public CHDrawObject  
{

public:
	CHRect(CPoint & topleft);
	CHRect(CRect & rect);
	virtual ~CHRect();

	virtual void Select(bool sel);
	virtual void WriteExportFormat(CIniSection& ini);
	virtual void ReadExportFormat(CIniSection& ini);
	virtual bool GetProperty(GV_ITEM* pItem);
	virtual bool SetProperty(GV_ITEM* pItem);
	virtual CString GetPropertyText(UINT par);
	virtual void ToGrid(int grid,int handle = 0);
	virtual LPCTSTR GetCursor(int handle);
	virtual void DoID(UINT nID);
	virtual void InitMenu(CMenu* menu,CPoint & point);
	virtual void Serialize(CArchive& ar);
	virtual void ResizeTo(POINT & to,int handle);
	virtual void MoveTo(POINT & to);
	virtual void DrawObject(CDC* pDC);
	virtual int PtInGripper(POINT & point);
	virtual bool PtInObject(POINT& point);
	virtual CRect GetBoundingRect(CDC* pDC = 0,bool mode = false);
	CRect GetRect(){return m_crRect;};

protected:
	CHRect();
	CRect GetGripper(int nHandle);
	DECLARE_SERIAL(CHRect)

protected:
	bool m_bSetRound;
	bool m_bEmpty;
	CPoint m_cpRound;
	int m_iResizePoint;
	int m_iPenStyle;
	int m_iPenWidth;
	int m_RectStyle;
	COLORREF m_crInnerColor; 
	COLORREF m_crBorderColor;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

/*************************************************************************/
// CHLine
/*************************************************************************/

class CHLine : public CHDrawObject  
{

public:
	CHLine();
	virtual ~CHLine();
	CHLine(CPoint& start);
	CHLine(CPoint& start,CPoint& end,int PenStyle
		,int Width,COLORREF crColor);

	virtual void WriteExportFormat(CIniSection& ini);
	virtual void ReadExportFormat(CIniSection& ini);
	virtual bool GetProperty(GV_ITEM* pItem);
	virtual bool SetProperty(GV_ITEM* pItem);
	virtual CString GetPropertyText(UINT par);
	virtual void ToGrid(int grid,int handle = NULL);
	virtual LPCTSTR GetCursor(int handle);
	virtual void Serialize(CArchive& ar);
	virtual void DrawObject(CDC* pDC);
	virtual void DoID(UINT nID);
	virtual void InitMenu(CMenu* menu,CPoint & point);
	virtual void MoveTo(POINT & to);
	virtual void ResizeTo(POINT & to,int handle);
	virtual int PtInGripper(POINT& point);
	virtual bool PtInObject(POINT & point);
	virtual CRect GetBoundingRect(CDC* pDC = 0,bool mode = false);
	
protected:
	DECLARE_SERIAL(CHLine)


protected:
	int m_iPenStyle;
	int m_iPenWidth; 
	COLORREF m_crColor; 

protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

/*************************************************************************/
// CHPoly
/*************************************************************************/

enum {PM_POLYLINE,PM_POLYGON,PM_POLYRECT};

class CHPoly : public CHDrawObject  
{

public:
	CHPoly();
	CHPoly(POINT& start);
	CHPoly(POINT& start,int PenStyle,int Width,COLORREF crPenColor);
	virtual ~CHPoly();
	
	virtual void AddPoint(POINT & point);
	virtual void WriteExportFormat(CIniSection& ini);
	virtual void ReadExportFormat(CIniSection& ini);
	virtual bool GetProperty(GV_ITEM* pItem);
	virtual bool SetProperty(GV_ITEM* pItem);
	virtual CString GetPropertyText(UINT par);
	virtual void ToGrid(int grid,int handle = NULL);
	virtual LPCTSTR GetCursor(int handle);
	virtual void Serialize(CArchive& ar);
	virtual void DrawObject(CDC* pDC);
	virtual void DoID(UINT nID);
	virtual void InitMenu(CMenu* menu,CPoint & point);
	virtual void MoveTo(POINT & to);
	virtual void ResizeTo(POINT & to,int handle);
	virtual bool PtInObject(POINT & point);
	virtual int PtInGripper(POINT& point);
	virtual CRect GetBoundingRect(CDC* pDC = NULL,bool mode = false);

	CArray<CPoint,CPoint> m_caPoints;

protected:
	DECLARE_SERIAL(CHPoly)

protected:
	int m_nFillMode;
	int m_iHandle;
	bool m_bEmpty;
	int m_iPolyStyle;
	int m_iPenStyle;
	int m_iPenWidth; 
	COLORREF m_crPenColor;
	COLORREF m_crFillColor;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

/*************************************************************************/
// CHConnector
/*************************************************************************/

class CHPetriMember;
class CHConnector : public CHPoly, public CHNetMember
{

public:
	CHConnector();
	CHConnector(CHPetriMember * from,CHPetriMember * to,long ident);
	virtual ~CHConnector();

	bool StepTokenAnim(int nStep, bool bStop);
	void StartTokenAnim(int nSteps);
	void RecalcLabelPos();
	long GetWeight(){return m_iWeight;};
	void SetWeigth(long weight){m_iWeight = weight;};
	virtual LPCTSTR GetCursor(int handle);
	virtual void WriteExportFormat(CIniSection& ini);
	virtual void ReadExportFormat(CIniSection& ini);
	virtual bool GetProperty(GV_ITEM* pItem);
	virtual bool SetProperty(GV_ITEM* pItem);
	virtual CString GetPropertyText(UINT par);
	virtual CRect GetBoundingRect(CDC* pDC = NULL,bool mode = false);
	virtual bool IsPMember(){return true;};
	virtual void ToGrid(int grid,int handle = NULL);
	virtual int PtInGripper(POINT& point);
	virtual void Serialize(CArchive& ar);   
	virtual void DrawObject(CDC* pDC);
	virtual void MoveTo(POINT & to);
	virtual void ResizeTo(POINT & to,int handle);
	virtual void DoID(UINT nID);
	virtual void InitMenu(CMenu* menu,CPoint & point);
	virtual void SetIdent(long ident);
	long GetFrom(){ASSERT_VALID(this);return m_iFrom;};
	long GetTo(){ASSERT_VALID(this);return m_iTo;};
	CHPetriMember* GetPtrFrom(){ASSERT_VALID(this);return m_ptrFrom;};
	CHPetriMember* GetPtrTo(){ASSERT_VALID(this);return m_ptrTo;};
	void RecalcPoint();
	void SetFrom(long from){ASSERT_VALID(this);m_iFrom = from;};
	void SetTo(long to){ASSERT_VALID(this);m_iTo = to;};
	void SetPtrFrom(CHPetriMember* from){ASSERT_VALID(this);m_ptrFrom = from;};
	void SetPtrTo(CHPetriMember* to){ASSERT_VALID(this);m_ptrTo = to;};
	CT_TYP GetTyp(){ASSERT_VALID(this);return m_ctTyp;};
	void SetTyp(CT_TYP typ){ASSERT_VALID(this);m_ctTyp = typ;};

protected:
	DECLARE_SERIAL(CHConnector)

protected:
	CT_TYP m_ctTyp;
	long m_iFrom;
	long m_iTo;
	long m_iWeight;
	CHPetriMember* m_ptrFrom;
	CHPetriMember* m_ptrTo;
	CPoint m_cpArrow[3];
	CPoint m_cpToken; //position of the animated token
	bool m_bToken;
	double m_doAnimLenghtLine;
	int m_nAnimSteps;
	int m_nAnimStepsMax;
	double m_doAnimPieces;
	int m_nAnimIndex;
	double m_doAnimLenght;
	float m_fDist;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

/*************************************************************************/
// CHPetriMember
/*************************************************************************/

class CHPetriMember : public CHLabel, public CHNetMember
{

public:
	CHPetriMember();
	virtual ~CHPetriMember();

	virtual void Reset(){ASSERT(false);};
	virtual void ToGrid(int grid,int handle = NULL);
	virtual void MoveTo(POINT & to);
	virtual CRect GetBoundingRect(CDC* pDC = NULL,bool mode = false);
	void SetIdent(long ident);
	CTypedPtrList<CPtrList, CHConnector*> & GetInList() {return m_cplLineIn;};
	CTypedPtrList<CPtrList, CHConnector*> & GetOutList() {return m_cplLineOut;};

protected:
	void RecalcArcs();

	CTypedPtrList<CPtrList, CHConnector*> m_cplLineIn;
	CTypedPtrList<CPtrList, CHConnector*> m_cplLineOut;

};

/*************************************************************************/
// CHPetriMember
/*************************************************************************/

class CHPosition : public CHPetriMember
{
public:
	CHPosition();
	virtual ~CHPosition();
	CHPosition(POINT & point, long ident);

	void Reset();
	void RecalcLabelPos();
	virtual bool IsPMember(){return true;};
	long GetToken(){return m_iTokens;};
	void SetToken(long tokens){m_iTokens = tokens;};
	long GetTokenCount(){return m_iTokensCount;};
	void SetTokenCount(long tokens){m_iTokensCount = tokens;};
	long GetTokenMax(){return m_iTokensMax;};
	void SetTokenMax(long tokens){m_iTokensMax = tokens;};
	virtual void WriteExportFormat(CIniSection& ini);
	virtual void ReadExportFormat(CIniSection& ini);
	virtual bool GetProperty(GV_ITEM* pItem);
	virtual bool SetProperty(GV_ITEM* pItem);
	virtual CString GetPropertyText(UINT par);
	virtual void DoID(UINT nID);
	virtual void InitMenu(CMenu* menu,CPoint & point);
	virtual void Serialize(CArchive& ar);
	virtual void DrawObject(CDC* pDC);

protected:
	DECLARE_SERIAL(CHPosition)

protected:
	PT_SIZE m_size;
	long m_iTokens;
	long m_iTokensMax;
	long m_iTokensStart;
	long m_iTokensCount;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
};

/*************************************************************************/
// CHTransition
/*************************************************************************/

enum TRAN_TIME_MODE{TTM_IMMIDIATE,TTM_DELAY,
 TTM_EXPONENTIAL, TTM_EQUAL_DISTR};

class CHTransition : public CHPetriMember
{

public:
	CHTransition();
	CHTransition(POINT & point, long ident);
	virtual ~CHTransition();

	void Reset();
	void RecalcLabelPos();
	TRAN_TIME_MODE GetTimeMode(){return m_TimeMode;};
	void SetTimeMode(TRAN_TIME_MODE mode){m_TimeMode = mode;};
	long GetTokensCount(){return m_iTokensCount;};
	void SetTokensCount(long count){m_iTokensCount = count;};
	long GetRangeDelay(){return m_iRangeDelay;};
	long GetStartDelay(){return m_iStartDelay;};
	long GetDelay(){return m_iDelay;};
	void SetRangeDelay(long time){m_iRangeDelay = time;};
	void SetStartDelay(long time){m_iStartDelay = time;};
	void SetDelay(UINT time){m_iDelay = time;};
	virtual void WriteExportFormat(CIniSection& ini);
	virtual void ReadExportFormat(CIniSection& ini);
	virtual bool IsPMember(){return true;};
	virtual bool GetProperty(GV_ITEM* pItem);
	virtual bool SetProperty(GV_ITEM* pItem);
	virtual CString GetPropertyText(UINT par);
	virtual void DoID(UINT nID);
	virtual void InitMenu(CMenu* menu,CPoint & point);
	virtual void Serialize(CArchive& ar);
	virtual void DrawObject(CDC* pDC);
	void SetAktiv(bool aktiv){m_bAktiv = aktiv;};
	bool GetAktiv(){return m_bAktiv;};
	void SetEnabled(bool enabled){m_bEnabled = enabled;};
	bool GetEnabled(){return m_bEnabled;};

protected:
	DECLARE_SERIAL(CHTransition)

protected:
	TRAN_TIME_MODE m_TimeMode;//Zeit model
	PT_SIZE m_size;//Größe
	long m_iDelay;//restliche Verzögerungszeit
	long m_iStartDelay;//Verzögerungszeit
	long m_iRangeDelay;//2. Parameter für eine Gleichverteilung
	long m_iTokensCount;//gefeuerte Transitionen
	bool m_bAktiv;//aktiviert
	bool m_bEnabled;//eingeschaltet

protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/*************************************************************************/
// CHPetriModul
/*************************************************************************/

class CHPetriModul : public CHPetriMember 
{

public:
	CHPetriModul();
	virtual ~CHPetriModul();

	virtual void Serialize(CArchive& ar);

protected:
	CTypedPtrList<CObList, CHDrawObject*> m_colMembers;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

/*************************************************************************/
// CHGroup
/*************************************************************************/

class CHGroup : public CHRect 
{

public:
	CHGroup();
	virtual ~CHGroup();

	virtual bool GetProperty(GV_ITEM* pItem){return false;};
	virtual bool SetProperty(GV_ITEM* pItem){return false;};
	virtual CString GetPropertyText(UINT par){return "NULL";};
	virtual LPCTSTR GetCursor(int handle);
	virtual void Serialize(CArchive& ar);
	virtual void DrawObject(CDC* pDC);
	virtual void DoID(UINT nID);
	virtual void InitMenu(CMenu* menu,CPoint & point);
	virtual void MoveTo(POINT & to);
	virtual void ResizeTo(POINT & to,int handle);
	virtual bool PtInObject(POINT & point);
	virtual int PtInGripper(POINT& point);
	virtual CRect GetBoundingRect(CDC* pDC = NULL,bool mode = false);
	void Add(CHDrawObject * object);
	CHDrawObject* Remove();


protected:
	DECLARE_SERIAL(CHGroup)
	
protected:
	CRect GetGripper(int nHandle);
	CTypedPtrList<CObList, CHDrawObject*> m_colMembers;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

#endif // AFX_HDRAWOBJECT_H__84A4D540_EA85_11D1_94E3_BB7F79BF0103__INCLUDED_

