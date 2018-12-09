/**************************************************************************
	HLineBuffer.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "stdafx.h"
#include "HLineBuffer.h"

#ifdef _DEBUG
#undef THIS_FILE
static CHAR THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CHLineBuffer::CHLineBuffer()
{
	m_iLines = 0;
	m_iLineChars = 0;
	m_iActualLine = 0;
	m_pBuffer = 0;
	m_cchBuffer = 0;
}

CHLineBuffer::CHLineBuffer(long lines, long linechars) 
	: m_iLines(lines)
	, m_iLineChars(linechars)
{
	m_iActualLine = 0;
	m_cchBuffer = lines * (linechars + 2);
	m_pBuffer = new TCHAR[m_cchBuffer];
	m_pBuffer[0] = _T('\0');
}

CHLineBuffer::~CHLineBuffer()
{
	delete m_pBuffer;
}

void CHLineBuffer::AddLine(LPCTSTR line)
{
	if(m_iActualLine < m_iLines)
	{
		StringCchCat(m_pBuffer, m_cchBuffer, line);
		StringCchCat(m_pBuffer, m_cchBuffer, _T("\r\n"));
		m_iActualLine++;
	}
	else
	{
		LPCTSTR strSource = _tcschr(m_pBuffer, _T('\n'));
		*strSource++;
		StringCchCopy(m_pBuffer, m_cchBuffer, strSource);
		StringCchCat(m_pBuffer, m_cchBuffer, line);
		StringCchCat(m_pBuffer, m_cchBuffer, _T("\r\n"));
	}
}

void CHLineBuffer::AddLine(CString & line)
{
	AddLine(line.GetBuffer(0));
}

void CHLineBuffer::Empty()
{
	m_pBuffer[0] = _T('\0');
	m_iActualLine = 0;
}


