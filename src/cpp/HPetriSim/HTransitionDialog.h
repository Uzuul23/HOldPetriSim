#ifndef AFX_HTRANSITIONDIALOG_H__50B5AAA1_FD3A_11D1_9366_000000000000__INCLUDED_
#define AFX_HTRANSITIONDIALOG_H__50B5AAA1_FD3A_11D1_9366_000000000000__INCLUDED_

// HTransitionDialog.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CHTransitionDialog 

class CHTransitionDialog : public CDialog
{
// Konstruktion
public:
	int m_iDelay;
	CHTransitionDialog(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CHTransitionDialog)
	enum { IDD = IDD_TRANSITION_DIALOG };
	int		m_iDelayOff;
	int		m_iDelayOn;
	int		m_iEnabledMode;
	int		m_iRandOff;
	int		m_iRandOn;
	BOOL	m_bAuto;
	BOOL	m_bTimerStop;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CHTransitionDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CHTransitionDialog)
	afx_msg void OnInmediateRadio();
	afx_msg void OnDelayOnRadio();
	afx_msg void OnDelayOffRadio();
	afx_msg void OnRandOnRadio();
	afx_msg void OnRandOffRadio();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeDelayOffEdit();
	afx_msg void OnChangeDelayOnEdit();
	afx_msg void OnChangeRandOffEdit();
	afx_msg void OnChangeRandOnEdit();
	afx_msg void OnAutoCheck();
	afx_msg void OnTimerStopCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // AFX_HTRANSITIONDIALOG_H__50B5AAA1_FD3A_11D1_9366_000000000000__INCLUDED_
