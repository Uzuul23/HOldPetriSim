/**************************************************************************
	HPetriModul.cpp
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "stdafx.h"
#include "HDrawObject.h"

CHPetriModul::CHPetriModul()
{
}

CHPetriModul::~CHPetriModul()
{

}

void CHPetriModul::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		
	}
	else
	{

	}	
	
}

#ifdef _DEBUG
void CHPetriModul::AssertValid() const
{
	CObject::AssertValid();
}

void CHPetriModul::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif //_DEBUG


