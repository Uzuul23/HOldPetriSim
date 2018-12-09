#ifndef AFX_HPOSITIONDIALOG_H__54FC4D62_FDE8_11D1_9367_000000000000__INCLUDED_
#define AFX_HPOSITIONDIALOG_H__54FC4D62_FDE8_11D1_9367_000000000000__INCLUDED_

// HPositionDialog.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CHPositionDialog 

class CHPositionDialog : public CDialog
{
// Konstruktion
public:
	int m_iDelay;
	CHPositionDialog(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CHPositionDialog)
	enum { IDD = IDD_POSITION_DIALOG };
	int		m_iDelayOn;
	int		m_iTokenMode;
	int		m_iRandOn;
	BOOL	m_bAuto;
	//}}AFX_DATA


// �berschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
	//{{AFX_VIRTUAL(CHPositionDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst�tzung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CHPositionDialog)
	afx_msg void OnChangeDelayOnEdit();
	afx_msg void OnChangeRandOnEdit();
	afx_msg void OnDelayTokenRadio();
	afx_msg void OnNeverTokenRadio();
	afx_msg void OnRandTokenRadio();
	virtual BOOL OnInitDialog();
	afx_msg void OnTokenCheck();
	afx_msg void OnRestartCheck();
	afx_msg void OnStartTokenRadio();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio f�gt zus�tzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // AFX_HPOSITIONDIALOG_H__54FC4D62_FDE8_11D1_9367_000000000000__INCLUDED_
