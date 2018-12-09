/**************************************************************************
	ProgDlg1.h
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg Dialogfeld

#ifndef __PROGDLG1_H__
#define __PROGDLG1_H__

class CProgressDlg : public CDialog
{
// Konstruktion / Destruktion
public:
    CProgressDlg(UINT nCaptionID = 0);   // Standard-Konstruktor
    ~CProgressDlg();

    BOOL Create(CWnd *pParent=NULL);

    // Schaltfläche "Abbrechen" wird geprüft
    BOOL CheckCancelButton();
    // Anpassen des Dialogfelds Fortschritt
    void SetStatus(LPCTSTR lpszMessage);
    void SetRange(int nLower,int nUpper);
    int  SetStep(int nStep);
    int  SetPos(int nPos);
    int  OffsetPos(int nPos);
    int  StepIt();
	BOOL IfCancel(){return m_bCancel;};
        
// Dialogfelddaten
    //{{AFX_DATA(CProgressDlg)
    enum { IDD = CG_IDD_PROGRESS };
    CProgressCtrl	m_Progress;
    //}}AFX_DATA

// Überschreibungen
    // Vom Klassen-Assistenten generierte Überschreibungen virtueller Funktionen
    //{{AFX_VIRTUAL(CProgressDlg)
    public:
    virtual BOOL DestroyWindow();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
    //}}AFX_VIRTUAL

// Implementierung
protected:
	UINT m_nCaptionID;
    int m_nLower;
    int m_nUpper;
    int m_nStep;
    
    BOOL m_bCancel;
    BOOL m_bParentDisabled;

    void ReEnableParent();

    virtual void OnCancel();
    virtual void OnOK() {}; 
    void UpdatePercent(int nCurrent);
    void PumpMessages();

    // Generierte Nachrichtenzuordnungsfunktionen
    //{{AFX_MSG(CProgressDlg)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif // __PROGDLG1_H__
