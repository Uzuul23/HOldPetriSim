/**************************************************************************
	HThread.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/
#include "stdafx.h"
#include "PetriSimDoc.h"
#include "HRandom.h"
#include "HLineBuffer.h"
#include "HThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

/*************************************************************************/
// CHThread
/*************************************************************************/

IMPLEMENT_DYNCREATE(CHThread, CWinThread)

CHThread::CHThread()
{
	m_pDoc = 0;
	m_pWindow = 0;
	m_hOutputWindow = 0;
}

CHThread::~CHThread()
{

}

BOOL CHThread::InitInstance()
{
	ASSERT(m_pDoc);
	ASSERT(m_pWindow);
	timeBeginPeriod(1);

	long time_start, time_end, time_wait, time_calc, load = 0;
	CSyncObject* ppObjectArray[CHT_HANDLES_COUNT];
	for(int i = 0;i < CHT_HANDLES_COUNT; i++)
	{
		CSyncObject* pSyncOb = DYNAMIC_DOWNCAST(CSyncObject,
			m_pDoc->GetThreadEvent((CHT_HANDLES)i));
		ppObjectArray[i] = pSyncOb;
	}
	//Timerauflösung auf 1ms stellen
	
	InitalizeSim();
	CHLineBuffer* pBuffer = m_pDoc->GetOutputBuffer();
	CMultiLock mLock(ppObjectArray, CHT_HANDLES_COUNT);
	// Schleife erst verlassen wenn das "Kill" Ereigniss
	// gesetzt wurde
	bool bRun = true;
	while(bRun)
	{
		switch(mLock.Lock(INFINITE, false, 0))//warte auf Ereigniss
		{
		case WAIT_OBJECT_0 + CHT_EVENT_KILL:
			m_pDoc->GetThreadEvent(CHT_EVENT_RUNNORMAL)->ResetEvent();
			m_pDoc->GetThreadEvent(CHT_EVENT_RUNFAST)->ResetEvent();
			m_pDoc->GetThreadEvent(CHT_EVENT_STEP)->ResetEvent();
			bRun = false;
			break;
		case WAIT_OBJECT_0 + CHT_EVENT_STEP:
			m_pDoc->GetThreadEvent(CHT_EVENT_RUNNORMAL)->ResetEvent();
			time_start =  timeGetTime();
			SingleStep();
			VERIFY(m_pWindow->PostMessage(WM_UPDATENET, 0, 
					(LPARAM)m_pDoc));
			time_end =  timeGetTime();
			time_calc = time_end - time_start;
			m_pDoc->SetStepTime(time_calc);
			break;
		case WAIT_OBJECT_0 + CHT_EVENT_RUNNORMAL:
			time_start =  timeGetTime();
			m_pDoc->GetThreadEvent(CHT_EVENT_STEP)->ResetEvent();
			if(m_pDoc->GetSimTime() >= m_pDoc->GetStopTime())
			{
				m_pDoc->GetThreadEvent(CHT_EVENT_RUNNORMAL)->ResetEvent();
				m_pDoc->StopSim();
				m_pDoc->SetCalcTime(0);
				m_pDoc->SetStepTime(0);
				pBuffer->AddLine(_T("Message: Time Limit reached"));
				VERIFY(m_pWindow->PostMessage(WM_UPDATEOUTPUT, 0, 
					(LPARAM)m_pDoc));
				break;
			}
			if(m_pDoc->GetStepCount() >= m_pDoc->GetStopStep())
			{
				m_pDoc->GetThreadEvent(CHT_EVENT_RUNNORMAL)->ResetEvent();
				m_pDoc->StopSim();
				m_pDoc->SetCalcTime(0);
				m_pDoc->SetStepTime(0);
				pBuffer->AddLine(_T("Message: Step Limit reached"));
				VERIFY(m_pWindow->PostMessage(WM_UPDATEOUTPUT, 0, 
					(LPARAM)m_pDoc));
				break;
			}
			SingleStep();
			VERIFY(m_pWindow->PostMessage(WM_UPDATENET, 0, 
					(LPARAM)m_pDoc));
			time_end =  timeGetTime();
			time_calc = time_end - time_start;
			m_pDoc->SetCalcTime(time_calc);
			time_wait = m_pDoc->GetSimSpeed() - time_calc;
			time_wait = (time_wait < 0)? 0: time_wait;
			Sleep(time_wait);
			time_end =  timeGetTime();
			time_calc = time_end - time_start;
			m_pDoc->SetStepTime(time_calc);
			break;
		case WAIT_OBJECT_0 + CHT_EVENT_RUNFAST:
			time_start =  timeGetTime();
			if(m_pDoc->GetSimTime() >= m_pDoc->GetStopTime())
			{
				m_pDoc->GetThreadEvent(CHT_EVENT_RUNFAST)->ResetEvent();
				m_pDoc->StopSim();
				m_pDoc->SetCalcTime(0);
				m_pDoc->SetStepTime(0);
				pBuffer->AddLine(_T("Message: Time Limit reached"));
				VERIFY(m_pWindow->PostMessage(WM_UPDATEOUTPUT, 0, 
					(LPARAM)m_pDoc));
				break;
			}
			if(m_pDoc->GetStepCount() >= m_pDoc->GetStopStep())
			{
				m_pDoc->GetThreadEvent(CHT_EVENT_RUNFAST)->
					ResetEvent();
				m_pDoc->StopSim();
				m_pDoc->SetCalcTime(0);
				m_pDoc->SetStepTime(0);
				pBuffer->AddLine(_T("Message: Step Limit reached"));
				VERIFY(m_pWindow->PostMessage(WM_UPDATEOUTPUT, 0, 
					(LPARAM)m_pDoc));
				break;
			}
			SingleStep();
			time_end =  timeGetTime();
			time_calc = time_end - time_start;
			m_pDoc->SetCalcTime(time_calc);
			m_pDoc->SetStepTime(0);
			break;
		case 0xFFFFFFFF:
			TRACE("ERROR: %d", GetLastError());
			ASSERT(false);
			break;
		default:ASSERT(false);
		}
	}
	//Transitionen bei Thread Ende zurückstzen
	POSITION pos = m_cplTranList.GetHeadPosition();
	while (pos)
	{
		CHTransition* ptrTran = m_cplTranList.GetNext(pos);
		ptrTran->SetAktiv(false);
		ptrTran->SetEnabled(false);
	}
	timeEndPeriod(1);
	AfxEndThread(0); //kill myself
	return false;
}

BEGIN_MESSAGE_MAP(CHThread, CWinThread)
END_MESSAGE_MAP()

void CHThread::Delete()
{
	// acknowledge receipt of kill notification
	VERIFY(m_pDoc->GetThreadEvent(CHT_EVENT_DEAD)->SetEvent());
	CWinThread::Delete();
}

void CHThread::SingleStep()
{
	CSingleLock sLock(&m_pDoc->m_cCSection, true);
	m_pDoc->IncrStepCount();
	
	TCHAR buffer[80];
	CHLineBuffer* pBuffer = m_pDoc->GetOutputBuffer();
	StringCbPrintf(buffer, sizeof(buffer)
		, _T("Step: %d / Time: %d =====>")
		, m_pDoc->GetStepCount()
		, m_pDoc->GetSimTime());
	pBuffer->Empty();
	pBuffer->AddLine(buffer);
	
	bool bTimeless = false;
	bool bDeadlock = true;
	// Schaltbarkeit der Transitionen prüfen
	POSITION pos = m_cplTranList.GetHeadPosition();
	while (pos)
	{
		//in der Hauptschleife werden alle Transitionen geprüft
		CHTransition* ptrTran = m_cplTranList.GetNext(pos);
		//erst Deaktivieren
		bool bResetAktiv = ptrTran->GetAktiv();
		ptrTran->SetAktiv(false);
		bool bReset = ptrTran->GetEnabled();
		ptrTran->SetEnabled(false);
		//Liste aller Connectoren die auf diese Transition
		//zeigen hohlen.
		{
		CONN_LIST& rListIn = ptrTran->GetInList();
		POSITION posA = rListIn.GetHeadPosition();
		while (posA)
		{
			//ist eine Kante nicht aktiviert abbrechen
			CHConnector* ptrConn = rListIn.GetNext(posA);
			if(ptrConn->GetTyp() == CT_INHIBITOR)
			{
				CHPosition* pPosi = static_cast <CHPosition*>
					(ptrConn->GetPtrFrom());
				if(pPosi->GetToken() >= ptrConn->GetWeight())
					goto nextTran;
			}
			else //CT_TEST CT_NORMAL
			{
				CHPosition* pPosi = static_cast <CHPosition*>
					(ptrConn->GetPtrFrom());
				if(pPosi->GetToken() < ptrConn->GetWeight())
					goto nextTran;
			}
		}
		//Liste aller Connectoren die von dieser Transition
		//kommen hohlen.
		CONN_LIST& rListOut = ptrTran->GetOutList();
		posA = rListOut.GetHeadPosition();
		while (posA)
		{
			//ist eine Kante aktiviert abbrechen
			CHConnector* ptrConn = rListOut.GetNext(posA);
			if(ptrConn->GetTyp() == CT_INHIBITOR)
			{
				CHPosition* pPosi = static_cast <CHPosition*>
					(ptrConn->GetPtrTo());
				if((pPosi->GetToken() - pPosi->GetTokenMax())
					>= ptrConn->GetWeight())
						goto nextTran;
			}
			else //CT_TEST CT_NORMAL
			{
				CHPosition* pPosi = static_cast <CHPosition*>
					(ptrConn->GetPtrTo());
				if((pPosi->GetTokenMax() - pPosi->GetToken()) < 
					ptrConn->GetWeight())
						goto nextTran;
			}
		}
		}
		if(ptrTran->GetDelay() > 0)
		{
			ptrTran->SetEnabled(true);
			ptrTran->SetInvalid(true);

			StringCbPrintf(buffer, sizeof(buffer), _T("Start Timer: %s"),
				ptrTran->GetLabelA()->GetText());
			pBuffer->AddLine(buffer);
		}
		else
		{
			ptrTran->SetAktiv(true);
			ptrTran->SetEnabled(false);
			ptrTran->SetInvalid(true);

			StringCbPrintf(buffer, sizeof(buffer), _T("Enable: %s"),
				ptrTran->GetLabelA()->GetText());
			pBuffer->AddLine(buffer);
		}
nextTran:;//nächste Transition bearbeiten
		if(bResetAktiv || bReset)
			//deaktivate transition
			ptrTran->SetInvalid(true);
		if(bReset && !ptrTran->GetEnabled() &&
			(ptrTran->GetTimeMode() == TTM_DELAY))
				ptrTran->SetDelay(ptrTran->GetStartDelay());

	}
	
	//Konflikte erkennen und lösen
	POSITION posT = m_cplConflTranList.GetHeadPosition();
	while (posT)
	{
		CHTransition* pTran = m_cplConflTranList.GetNext(posT);
		if(pTran->GetAktiv() && pTran->IsInvalid())
		{
			int iConCount = 0;
			CONN_LIST& rListIn = pTran->GetInList();
			POSITION posL = rListIn.GetHeadPosition();
			while(posL)
			{
				CHConnector* pConn = rListIn.GetNext(posL);
				CONN_LIST& rListOutA = pConn->GetPtrFrom()->GetOutList();
				if(rListOutA.GetCount() > 1)
				{
					iConCount = 0;
					POSITION posA = rListOutA.GetHeadPosition();
					while(posA)
					{
						CHConnector* pConnA = rListOutA.GetNext(posA);
						if(pConnA->GetTyp() == CT_NORMAL)
						{
							CHTransition* pTranA = static_cast <CHTransition*>
								(pConnA->GetPtrTo());
							if(pTranA->GetAktiv())
							{
								iConCount++;
								//pTran->SetAktiv(false);
							}
						}
					}
					if(iConCount > 1)
					{
						pBuffer->AddLine(_T("Warning: branch conflict !"));
						pBuffer->AddLine(_T("list: "));
						posA = rListOutA.GetHeadPosition();
						while(posA)
						{
							CHConnector* pConnA = rListOutA.GetNext(posA);
							if(pConnA->GetTyp() == CT_NORMAL)
							{
								CHTransition* pTranA = static_cast <CHTransition*>
									(pConnA->GetPtrTo());
								if(pTranA->GetAktiv())
								{
									StringCbPrintf(buffer, sizeof(buffer)
										, _T("Transition: %s ")
										, pTranA->GetLabelA()->GetText());
									pBuffer->AddLine(buffer);
								}
							}
						}
						pBuffer->AddLine(_T("end list: "));

						iConCount = CHRandom::GetIntRand(0, iConCount);
						POSITION posA = rListOutA.GetHeadPosition();
						int i = 0;
						while(posA)
						{
							CHConnector* pConnA = rListOutA.GetNext(posA);
							CHTransition* pTranA = static_cast <CHTransition*>
								(pConnA->GetPtrTo());
							pTranA->SetInvalid(false);
							if(pTranA->GetAktiv())
							{
								if(i != iConCount)
								{
									pTranA->SetAktiv(false);

									StringCbPrintf(buffer, sizeof(buffer)
										, _T("Disable: %s")
										, pTranA->GetLabelA()->GetText());
									pBuffer->AddLine(buffer);
								}
								i++;
							}
						}
					}
				}
			}
			/////////////////////////////////////////////////////////////
			iConCount = 0;
			CONN_LIST& rListOut = pTran->GetOutList();
			posL = rListOut.GetHeadPosition();
			while(posL)
			{
				CHConnector* pConn = rListOut.GetNext(posL);
				CONN_LIST& rListInA = pConn->GetPtrTo()->GetInList();
				if(rListInA.GetCount() > 1)
				{
					iConCount = 0;
					POSITION posA = rListInA.GetHeadPosition();
					while(posA)
					{
						CHConnector* pConnA = rListInA.GetNext(posA);
						if(pConnA->GetTyp() == CT_NORMAL)
						{
							CHTransition* pTranA = static_cast <CHTransition*>
								(pConnA->GetPtrFrom());
							if(pTranA->GetAktiv())
							{
								iConCount++;
								//pTran->SetAktiv(false);
							}
						}
					}
					if(iConCount > 1)
					{
						pBuffer->AddLine(_T("Warning: meet conflict !"));
						pBuffer->AddLine(_T("list: "));
						posA = rListInA.GetHeadPosition();
						while(posA)
						{
							CHConnector* pConnA = rListInA.GetNext(posA);
							if(pConnA->GetTyp() == CT_NORMAL)
							{
								CHTransition* pTranA = static_cast <CHTransition*>
									(pConnA->GetPtrFrom());
								if(pTranA->GetAktiv())
								{
									StringCbPrintf(buffer, sizeof(buffer)
										, _T("Transition: %s ")
										, pTranA->GetLabelA()->GetText());
									pBuffer->AddLine(buffer);
								}
							}
						}
						pBuffer->AddLine(_T("end list: "));

						iConCount = CHRandom::GetIntRand(0, iConCount);
						POSITION posA = rListInA.GetHeadPosition();
						int i = 0;
						while(posA)
						{
							CHConnector* pConnA = rListInA.GetNext(posA);
							CHTransition* pTranA = static_cast <CHTransition*>
								(pConnA->GetPtrFrom());
							pTranA->SetInvalid(false);
							if(pTranA->GetAktiv())
							{
								if(i != iConCount)
								{
									pTranA->SetAktiv(false);

									StringCbPrintf(buffer, sizeof(buffer)
										, _T("Disable: %s")
										, pTranA->GetLabelA()->GetText());
									pBuffer->AddLine(buffer);
								}
								i++;
							}
						}
					}
				}
			}
		}
	}
	//Transitionen Input schalten
	//Liste für Animationen bilden
	posT = m_cplTranList.GetHeadPosition();
	while (posT)
	{
		CHTransition* ptrTran = m_cplTranList.GetNext(posT);
		int fired = 0;//Zähler für gefeuerte token
		if(ptrTran->GetAktiv())
		{
			bDeadlock = false;
			//Liste aller Connectoren die auf diese Transition
			//zeigen hohlen.
			CONN_LIST& rInList = ptrTran->GetInList();		
			POSITION posA = rInList.GetHeadPosition();
			while (posA)
			{
				CHConnector* ptrConn = rInList.GetNext(posA);
				if(!m_pDoc->m_bRunFast)
					m_pDoc->m_colAnimInput.AddTail(ptrConn);
				CHPosition* pPosi = static_cast <CHPosition*> 
					(ptrConn->GetPtrFrom());
				if(ptrConn->GetTyp() == CT_NORMAL)
				{
					StringCbPrintf(buffer, sizeof(buffer)
						, _T("Position: %s decr. %d token(s)")
						, pPosi->GetLabelA()->GetText()
						, ptrConn->GetWeight());
					pBuffer->AddLine(buffer);

					pPosi->SetToken(pPosi->GetToken() - 
						ptrConn->GetWeight());
					pPosi->SetInvalid(true);
					fired += ptrConn->GetWeight();
					bTimeless = true;
				}
			}
			ptrTran->SetTokensCount(ptrTran->GetTokensCount() + fired);
			if(!m_pDoc->m_bRunFast)
			{
				CONN_LIST& rOutList = ptrTran->GetOutList();
				posA = rOutList.GetHeadPosition();
				while (posA)
				{
					CHConnector* ptrConn = rOutList.GetNext(posA);
					m_pDoc->m_colAnimOutput.AddTail(ptrConn);
				}
			}
		}
	}
	//Warten bis Animation Abgeschlossen
	if(!m_pDoc->m_bRunFast)
	{
		VERIFY(m_pWindow->PostMessage(WM_UPDATENET, 0, (LPARAM)m_pDoc));
		m_pDoc->m_pAnimEvent->ResetEvent();
		sLock.Unlock();
		CSingleLock cLockA(m_pDoc->m_pAnimEvent, true);
		sLock.Lock();
	}
	//Transitionen output schalten
	posT = m_cplTranList.GetHeadPosition();
	while (posT)
	{
		CHTransition* ptrTran = m_cplTranList.GetNext(posT);
		int fired = 0;//Zähler für gefeuerte token
		if(ptrTran->GetAktiv())
		{
			bDeadlock = false;
			//Neustart der Verzögerungszeit
			switch(ptrTran->GetTimeMode())
			{
			case TTM_IMMIDIATE:break;
			case TTM_DELAY:
				ptrTran->SetDelay(ptrTran->GetStartDelay());
				break;
			case TTM_EXPONENTIAL:
				ptrTran->SetDelay((int)CHExpRandom::GetRand(
					ptrTran->GetStartDelay()));
				break;
			case TTM_EQUAL_DISTR:
				ptrTran->SetDelay(CHRandom::GetIntRand(
					ptrTran->GetStartDelay(),
					ptrTran->GetRangeDelay()));
				break;
			}
			//Liste aller Connectoren die von dieser Transition
			//kommen hohlen.
			CONN_LIST& rOutList = ptrTran->GetOutList();
			POSITION posA = rOutList.GetHeadPosition();
			while (posA)
			{
				CHConnector* ptrConn = rOutList.GetNext(posA);
				CHPosition* pPosi = static_cast <CHPosition*> 
					(ptrConn->GetPtrTo());
				if(ptrConn->GetTyp() == CT_NORMAL)
				{
					StringCbPrintf(buffer, sizeof(buffer)
						, _T( "Position: %s incr. %d token(s)")
						, pPosi->GetLabelA()->GetText()
						, ptrConn->GetWeight());
					pBuffer->AddLine(buffer);

					pPosi->SetToken(pPosi->GetToken() + 
						ptrConn->GetWeight());
					pPosi->SetTokenCount(pPosi->GetTokenCount() + 
						ptrConn->GetWeight());
					pPosi->SetInvalid(true);
					bTimeless = true;
				}
			}
		}
	}
	if(!bTimeless)
	{
		posT = m_cplTranList.GetHeadPosition();
		while (posT)
		{
			CHTransition* ptrTran = m_cplTranList.GetNext(posT);
			if((ptrTran->GetTimeMode() != TTM_IMMIDIATE) &&
				ptrTran->GetEnabled())
			{
				bDeadlock = false;
				//dekrementiere Verzögerungszeit
				ptrTran->SetDelay(ptrTran->GetDelay() - 
					m_pDoc->GetSimSampleTime());
			}
		}
		if(!bDeadlock)
			m_pDoc->IncrSimTime();
	}
	if(bDeadlock)
	{
		TCHAR buffer[80];
		CHLineBuffer* pBuffer = m_pDoc->GetOutputBuffer();
		StringCbPrintf(buffer, sizeof(buffer)
			, _T("Error: Deadlock at Time: %d ms, Step: %d")
			, m_pDoc->GetSimTime(), m_pDoc->GetStepCount());
		pBuffer->AddLine(buffer);
		m_pDoc->GetThreadEvent(CHT_EVENT_RUNFAST)->ResetEvent();
		VERIFY(m_pWindow->PostMessage(WM_UPDATEOUTPUT, 0, 
		(LPARAM)m_pDoc));
		VERIFY(m_pWindow->PostMessage(WM_UPDATENET, 0, 
		(LPARAM)m_pDoc));
		m_pDoc->SetThreadError(true);
	}
	if(m_pDoc->IsEnabledOutputFile())
		WriteStepToFile();
	//update output window
	if(!m_pDoc->IfRunFast())
		VERIFY(m_pWindow->PostMessage(WM_UPDATEOUTPUT, 0, 
			(LPARAM)m_pDoc));
}

void CHThread::SetDoc(CPetriSimDoc* pDoc)
{
	ASSERT_VALID(pDoc);
	m_pDoc = pDoc;
	m_pWindow = AfxGetMainWnd();
}

bool CHThread::SetOutputFile(CString path)
{ 
	if(!m_cFileStep.Open(path, CFile::modeCreate|CFile::modeWrite
		|CFile::typeText))
	{
		m_pDoc->GetOutputBuffer()
			->AddLine(_T("Error: Can't open Output File"));
		VERIFY(m_pWindow->PostMessage(WM_UPDATEOUTPUT, 0, 
		(LPARAM)m_pDoc));
		return false;
	}
	TCHAR buffer[80];
	StringCbPrintf(buffer, sizeof(buffer)
		, _T("Message: logging enabled at: %d ms, Step: %d")
		, m_pDoc->GetSimTime(), m_pDoc->GetStepCount());
	m_pDoc->GetOutputBuffer()->AddLine(buffer);
	VERIFY(m_pWindow->PostMessage(WM_UPDATEOUTPUT, 0, 
		(LPARAM)m_pDoc));
	WriteStepToFileHeader();
	return true;
}

void CHThread::ResetOutputFile()
{
	m_cFileStep.Close();
	TCHAR buffer[80];
	StringCbPrintf(buffer, sizeof(buffer)
		, _T("Message: logging disabled at: %d ms, Step: %d")
		, m_pDoc->GetSimTime(), m_pDoc->GetStepCount());
	m_pDoc->GetOutputBuffer()->AddLine(buffer);
	VERIFY(m_pWindow->PostMessage(WM_UPDATEOUTPUT, 0, 
		(LPARAM)m_pDoc));
}

void CHThread::InitalizeSim()
{
	m_cplTranList.RemoveAll();
	m_cplPosiList.RemoveAll();

	POSITION pos = m_pDoc->GetDrawObjectList().GetHeadPosition();
	while (pos)
	{
		CHDrawObject* pObject = 
			m_pDoc->GetDrawObjectList().GetNext(pos);
		if(pObject->IsKindOf( RUNTIME_CLASS(CHTransition)))
		{
			m_cplTranList.
				AddTail(static_cast <CHTransition*> (pObject));
		}
		if(pObject->IsKindOf( RUNTIME_CLASS(CHPosition)))
		{
			m_cplPosiList.
				AddTail(static_cast <CHPosition*> (pObject));
		}
	}
	//suchen aller potentiellen Konflikte
	CSingleLock sLock(&(m_pDoc->m_cCSection), true);
	POSITION posT = m_cplTranList.GetHeadPosition();
	while (posT)
	{
		bool bConf = false;
		CHTransition* pTran = m_cplTranList.GetNext(posT);
		CONN_LIST& rInList = pTran->GetInList();
		POSITION posL = rInList.GetHeadPosition();
		while(posL)
		{
			CHConnector* pConn = rInList.GetNext(posL);
			if(pConn->GetPtrFrom()->GetOutList().GetCount() > 1)
			{
				//Eingangskonflikt gefunden
				pTran->SetEnabled(true);
				pTran->SetInvalid(true);
				bConf = true;
				break;
			}
		}
		if(!bConf)
		{
			CONN_LIST& rOutList = pTran->GetOutList();
			posL = rOutList.GetHeadPosition();
			while(posL)
			{
				CHConnector* pConn = rOutList.GetNext(posL);
				if(pConn->GetPtrTo()->GetInList().GetCount() > 1)
				{
					//Ausgangskonflikt gefunden
					pTran->SetEnabled(true);
					pTran->SetInvalid(true);
					bConf = true;
					break;
				}
			}
		}
		if(bConf)
			m_cplConflTranList.AddTail(pTran);
	}
	//schreibe Meldungen 
	TCHAR buffer[80];
	CHLineBuffer* pBuffer = m_pDoc->GetOutputBuffer();
	pBuffer->Empty();
	pBuffer->AddLine(_T("Petri Net Simulation V0.92"));
	pBuffer->AddLine(_T("========================="));
	StringCbPrintf(buffer, sizeof(buffer), _T("Load: %d Position(s)")
		, m_cplPosiList.GetCount());
	pBuffer->AddLine(buffer);
	StringCbPrintf(buffer, sizeof(buffer),  _T("Load: %d Transition(s)")
		, m_cplTranList.GetCount());
	pBuffer->AddLine(buffer);
	StringCbPrintf(buffer, sizeof(buffer),  _T("Sample Time: %d ms")
		, m_pDoc->GetSimSampleTime());
	pBuffer->AddLine(buffer);
	StringCbPrintf(buffer, sizeof(buffer),  _T("Limits: %d ms or %d step(s)")
		, m_pDoc->GetStopTime(), m_pDoc->GetStopStep());
	pBuffer->AddLine(buffer);
	if(!m_cplConflTranList.IsEmpty())
	{
		StringCbPrintf(buffer, sizeof(buffer)
			, _T("Warning: %d potentialy Conflicts found")
			, m_cplConflTranList.GetCount());
		pBuffer->AddLine(buffer);
		pBuffer->AddLine(_T("list:"));
		POSITION posT = m_cplConflTranList.GetHeadPosition();
		while (posT)
		{
			CHTransition* pTran = m_cplConflTranList.GetNext(posT);
				StringCbPrintf(buffer, sizeof(buffer)
					,  _T( "Transition: %s ")
					, pTran->GetLabelA()->GetText());
					pBuffer->AddLine(buffer);
		}
		pBuffer->AddLine(_T("end list"));
	}
	//Transitionen bei Thread Ende zurückstzen
	pos = m_cplTranList.GetHeadPosition();
	while (pos)
	{
		CHTransition* ptrTran = m_cplTranList.GetNext(pos);
		ptrTran->SetAktiv(false);
		ptrTran->SetEnabled(false);
	}
	VERIFY(m_pWindow->PostMessage(WM_UPDATEOUTPUT, 0, 
		(LPARAM)m_pDoc));
}

void CHThread::WriteStepToFile()
{
	TCHAR buffer[20];
	TRY
	{
		StringCbPrintf(buffer, sizeof(buffer), _T("%d")
			, m_pDoc->GetStepCount());
		m_cFileStep.WriteString(buffer);
		m_cFileStep.WriteString(_T(";"));
		StringCbPrintf(buffer, sizeof(buffer), _T("%d")
			, m_pDoc->GetSimTime());
		m_cFileStep.WriteString(buffer);
		m_cFileStep.WriteString(_T(";"));
		POSITION pos = m_cplPosiList.GetHeadPosition();
		while (pos)
		{
			CHPosition* pPosi = m_cplPosiList.GetNext(pos);
			StringCbPrintf(buffer, sizeof(buffer), _T("%d")
				, pPosi->GetToken());
			m_cFileStep.WriteString(buffer);
			m_cFileStep.WriteString(_T(";"));
		}
		m_cFileStep.WriteString(_T("\n"));
	}CATCH (CFileException, e)
	{
			m_pDoc->GetOutputBuffer()
				->AddLine(_T("Error: writing output file"));
			m_pDoc->SetEnabledOutputFile(false);
			m_cFileStep.Abort();
			TCHAR buffer[80];
			StringCbPrintf(buffer, sizeof(buffer)
				,  _T("Message: logging disabled at: %d ms, Step: %d"),
				m_pDoc->GetSimTime(), m_pDoc->GetStepCount());
			m_pDoc->GetOutputBuffer()->AddLine(buffer);
			VERIFY(m_pWindow->PostMessage(WM_UPDATEOUTPUT, 0, 
				(LPARAM)m_pDoc));
			return;
	}
	END_CATCH
}

void CHThread::WriteStepToFileHeader()
{	
	CTime ctime = CTime::GetCurrentTime();
	m_cFileStep.WriteString(_T("Simulation Data generated by HPSim "));
	m_cFileStep.WriteString(ctime.Format(_T("%b-%d-%Y %H:%M:%S;\n")));
	m_cFileStep.WriteString(_T("Count/ Steps;"));
	m_cFileStep.WriteString(_T("Time/ ms;"));
	POSITION pos = m_cplPosiList.GetHeadPosition();
	while (pos)
	{
		CHPosition* pPosi = m_cplPosiList.GetNext(pos);
		m_cFileStep.WriteString(pPosi->GetLabelA()->GetText());
		m_cFileStep.WriteString(_T(";"));
	}
	m_cFileStep.WriteString(_T("\n"));
}



