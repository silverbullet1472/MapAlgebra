#pragma once
#include <math.h>

//root class for dis template
class CTmp
{
public:
	virtual char GetOffX(unsigned i) = 0;
	virtual char GetOffY(unsigned i) = 0;
	virtual float GetPower(unsigned i) = 0;
	virtual unsigned GetSize() = 0;
};

static char            ManHattanTemOffX[13] = { 0, -1, 0, 1,  -2,-1, 0, 1, 2,  -1,0,1,0 };
static char            ManHattanTemOffY[13] = { -2, -1,-1,-1,   0, 0, 0, 0, 0,   1,1,1,2 };
static unsigned char    ManHattanTemDis[13] = { 2,  2, 1, 2,   2, 1, 0, 1, 2,   2,1,2,2 };
class CManHattanTmp : public CTmp
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
	virtual float GetPower(unsigned i) { return m_Dis[i]; };
	virtual unsigned GetSize() { return 13; };
};

static char            OctagonTemOffX[37] = { 1,  0,-1, 2,  1,0, -1, -2, 3, 2,  1, 0,-1,-2,-3,-3,-2,-1,0,1,2,3,3,2,1,0,-1,-2,-3,2,1,0,-1,-2,1,0,-1 };
static char            OctagonTemOffY[37] = { -3, -3,-3,-2, -2, -2, -2, -2, -1, -1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,2,2,3,3,3 };
static unsigned char    OctagonTemDis[37] = {3,3,3,3,2,2,2,3,3,2,2,1,2,2,3,3,2,1,0,1,2,3,3,2,2,1,2,2,3,3,2,2,2,3,3,3,3};
class COctagonTmp : public CTmp
{
private:
	char *m_OffX;
	char *m_OffY;
	unsigned char *m_Dis;
public:
	COctagonTmp()
	{
		m_OffX = OctagonTemOffX;
		m_OffY = OctagonTemOffY;
		m_Dis = OctagonTemDis;
	};

	virtual char GetOffX(unsigned i) { return m_OffX[i]; };
	virtual char GetOffY(unsigned i) { return m_OffY[i]; };
	virtual float GetPower(unsigned i) { return m_Dis[i]; };
	virtual unsigned GetSize() { return 37; };
};

class CEu5Tmp : public CTmp
{
private:
	float m_DisTmp[25];
public:
	CEu5Tmp()
	{
		for (unsigned i = 0; i<25; i++)
		{
			float x = GetOffX(i);
			float y = GetOffY(i);
			m_DisTmp[i] = sqrt(x*x + y*y);
		}
	};
	virtual unsigned GetSize() { return 25; };
	virtual char GetOffX(unsigned i) { return i % 5 - 2; };
	virtual char GetOffY(unsigned i) { return i / 5 - 2; };
	virtual float GetPower(unsigned i) { return m_DisTmp[i]; };
};

static char             AverageTemOffX[9] = { 1, 0,-1,-1, 0, 1, 1, 0,-1 };
static char             AverageTemOffY[9] = { -1,-1,-1, 0, 0, 0, 1, 1, 1 };
static char             AverageTemPower[9] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
class CAverageTmp : public CTmp
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

static char             BoundaryTemOffX[5] = { -1, 1, 0,-1, 1 };
static char             BoundaryTemOffY[5] = { -1, 0, 0, 0, 1 };
static char             BoundaryTemPower[5] = { -1, 1, 0,-1, 1 };
class CBoundaryTmp : public CTmp
{
private:
	char *m_OffX;
	char *m_OffY;
	char *m_Power;
public:
	CBoundaryTmp()
	{
		m_OffX = BoundaryTemOffX;
		m_OffY = BoundaryTemOffY;
		m_Power = BoundaryTemPower;
	};

	virtual char GetOffX(unsigned i) { return m_OffX[i]; };
	virtual char GetOffY(unsigned i) { return m_OffY[i]; };
	virtual float GetPower(unsigned i) { return m_Power[i]; };
	virtual unsigned GetSize() { return 5; };
};
