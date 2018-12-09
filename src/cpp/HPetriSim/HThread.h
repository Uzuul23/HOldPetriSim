/**************************************************************************
	HThread.h
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#ifndef AFX_HTHREAD_H__57484B52_553A_11D2_9418_000000000000__INCLUDED_
#define AFX_HTHREAD_H__57484B52_553A_11D2_9418_000000000000__INCLUDED_

#include "HDrawObject.h"

class CPetriSimDoc;
class CHThread : public CWinThread
{
	DECLARE_DYNCREATE(CHThread)
public:
	CHThread();
    virtual ~CHThread();
	virtual void Delete();
	void ResetOutputFile();
	bool SetOutputFile(CString path);
	void SetDoc(CPetriSimDoc* pDoc);
	virtual BOOL InitInstance();

private:
	CStdioFile m_cFileStep;
	CPetriSimDoc* m_pDoc;
	CWnd* m_pWindow;
	HWND m_hOutputWindow;
	CTypedPtrList<CPtrList,CHTransition*> m_cplTranList;
	CTypedPtrList<CPtrList,CHTransition*> m_cplConflTranList;
	CTypedPtrList<CPtrList,CHPosition*> m_cplPosiList;
//	CTypedPtrList<CPtrList,CHConnector*> m_cplConnList;

private:
	void WriteStepToFileHeader();
	void WriteStepToFile();
	void SingleStep();
	void InitalizeSim();

	DECLARE_MESSAGE_MAP()

};

#endif // AFX_HTHREAD_H__57484B52_553A_11D2_9418_000000000000__INCLUDED_
