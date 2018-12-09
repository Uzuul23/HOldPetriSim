/**************************************************************************
	HRandom.h
	
	Copyright (C) 1999 - 2010 Henryk Anschuetz
	All rights reserved.
	
	www.winpesim.de
**************************************************************************/

#include "stdafx.h"

//recht akzeptable Gleichverteilung aus Pseudo Zufallszahlen
//im angegebenen Bereich 
class CHRandom
{  
public:
	CHRandom(double min = 0, double max = RAND_MAX):
	  m_dMin(min), m_dMax(max){} 
	
	
	double Rand() 
	{
		return ((double(rand())/RAND_MAX) *
			(m_dMax - m_dMin)) + m_dMin;
	}
	operator double() {return Rand();}
	static double GetRand(double min = 0, double max = RAND_MAX)
	{
		return ((double(rand())/RAND_MAX) *
			(max - min)) + min;
	}
	static int GetIntRand(double min = 0, double max = RAND_MAX)
	{
		double val = ((double(rand())/RAND_MAX) *
			(max - min) + min);
		return (val == max)? int(val) - 1 : int(val);
	}
private:
    double m_dMin, m_dMax;
};
//negativ exponentiale Verteilung um einen Mittelwert
class CHExpRandom : public CHRandom
{
private:
	int m_iMiddle;
public:
	CHExpRandom(int middle) : m_iMiddle(middle) {}
	operator double()
	{
		return m_iMiddle * (1- log((RAND_MAX - Rand())/RAND_MAX + 0.5));
	}
	static double GetRand(int middle)
	{
		return middle * (1- log((RAND_MAX - CHRandom::GetRand())/
			RAND_MAX + 0.5));
	}
};

/*
//alternative Erzeugung von Pseudo Zufallszahlen ohne
//verwendung von rand()
//müsste noch eingehend getestet werden
class CFastRand
{
public:
	CFastRand(unsigned long seed): m_RandSeed(seed) {}
	operator double()
	{
		return (m_RandSeed = 1664525L * m_RandSeed + 1013904223L) >> 5;
	}
private:
	unsigned long m_RandSeed;
};
*/