#pragma once
#include <math.h>

//root class for dis template
class CDisTmp
{
public:
	virtual char GetOffX(unsigned i) = 0;
	virtual char GetOffY(unsigned i) = 0;
	virtual float GetDis(unsigned i) = 0;
	virtual unsigned GetSize() = 0;
};

static char            ManHattanTemOffX[13] = { 0, -1, 0, 1,  -2,-1, 0, 1, 2,  -1,0,1,0 };
static char            ManHattanTemOffY[13] = { -2, -1,-1,-1,   0, 0, 0, 0, 0,   1,1,1,2 };
static unsigned char    ManHattanTemDis[13] = { 2,  2, 1, 2,   2, 1, 0, 1, 2,   2,1,2,2 };

class CManHattanTmp : public CDisTmp
{
private:
	char *m_OffX;
	char *m_OffY;
	unsigned char *m_Dis;
public:
	CManHattanTmp()
	{
		m_OffX = ManHattanTemOffX;
		m_OffY = ManHattanTemOffY;
		m_Dis = ManHattanTemDis;
	};

	virtual char GetOffX(unsigned i) { return m_OffX[i]; };
	virtual char GetOffY(unsigned i) { return m_OffY[i]; };
	virtual float GetDis(unsigned i) { return m_Dis[i]; };
	virtual unsigned GetSize() { return 13; };
};

class CEuTmp13 : public CDisTmp
{
private:
	float m_Dis[169];
public:
	CEuTmp13()
	{
		for (unsigned i = 0; i < 169; i++)
		{
			float x = GetOffX(i);
			float y = GetOffY(i);
			m_Dis[i] = sqrt(x*x + y*y);
		}
	};

	virtual char GetOffX(unsigned i) { return i % 13 - 6; };
	virtual char GetOffY(unsigned i) { return i / 13 - 6; };
	virtual float GetDis(unsigned i) { return m_Dis[i]; };
	virtual unsigned GetSize() { return 169; };
};


class CFocalTmp
{
public:
	virtual char GetOffX(unsigned i) = 0;
	virtual char GetOffY(unsigned i) = 0;
	virtual float GetPower(unsigned i) = 0;
	virtual unsigned GetSize() = 0;
};

static char             AverageTemOffX[9] =  { 1, 0,-1,-1, 0, 1, 1, 0,-1 };
static char             AverageTemOffY[9] =  {-1,-1,-1, 0, 0, 0, 1, 1, 1 };
static char             AverageTemPower[9] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };

class CAverageTmp : public CFocalTmp
{
private:
	char *m_OffX;
	char *m_OffY;
	char *m_Power;
public:
	CAverageTmp()
	{
		m_OffX = AverageTemOffX;
		m_OffY = AverageTemOffY;
		m_Power = AverageTemPower;
	};

	virtual char GetOffX(unsigned i) { return m_OffX[i]; };
	virtual char GetOffY(unsigned i) { return m_OffY[i]; };
	virtual float GetPower(unsigned i) { return m_Power[i]; };
	virtual unsigned GetSize() { return 9; };
};