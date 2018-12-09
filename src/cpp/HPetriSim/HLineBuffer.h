/**************************************************************************
	HLineBuffer.h
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#if !defined(AFX_HLINEBUFFER_H__F761CA62_0452_11D3_BAA5_0000B45E2D1E__INCLUDED_)
#define AFX_HLINEBUFFER_H__F761CA62_0452_11D3_BAA5_0000B45E2D1E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CHLineBuffer
{
public:
	void AddLine(CString& line);
	void Empty();
	void AddLine(LPCTSTR line);
	CHLineBuffer();
	CHLineBuffer(long lines, long linechars);
	LPCTSTR GetBuffer(){return m_pBuffer;};
	virtual ~CHLineBuffer();
private:
	long m_iLines;
	long m_iLineChars;
	long m_iActualLine;
	LPTSTR m_pBuffer;
	size_t m_cchBuffer;
};

#endif // !defined(AFX_HLINEBUFFER_H__F761CA62_0452_11D3_BAA5_0000B45E2D1E__INCLUDED_)
