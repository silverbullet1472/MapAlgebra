#pragma once
#include "MapAlgebraTemp.h"
#include <vector>

//1 reverse 256-color img
bool BmpReverse(const char* SrcBmpName, const char* DestBmpName);

//2 overlay
bool BmpOverlay(const char* SrcBmpName1, const char* SrcBmpName2, const char* DestBmpName);

//3 focal function
bool BmpFocal(const char* SrcBmpName, CTmp *pFocalTmp, const char* DestBmpName);

//4 format changer
int Bmp256to32b(const char* SourceFileName, const char* IdxFileName);

//5 distance transform
//read a 256-color bmp, generate distance field and distribution field
//input: 256-color output:distance field 32 ,distribution 256-color(8bit)
bool DisTransform(const char* SrcBmpName, CTmp *pDisTmp, const char* DestDistribution, const char* DestDistance,unsigned char MaxColor);

//6 generate boundary
bool GenerateBoundary(const char* SrcBmpName, const char* DestBmpName);

//7 generate buffer
bool GenerateBuffer(const char* SrcBmpName, float radius, const char* DestBmpName);

//8 AdhesionTransform
bool AdhesionTransform(const char* SrcBmpName, float outRadius, float inRadius);

//9 AdhesionTransform
bool AxisTransform(const char* SrcBmpName, const char* DestBmpName);

//10 DelauneyTransform
bool DelauneyTransform(const char* SrcBmpName, const char* DestBmpName);
//delauneyTransUltities
bool GetPtCoors(const char* SrcBmpName, const char* outPtTxt);
bool GeneratePointPairs(const char* SrcBmpName, const char* outPairsTxt, const char* DestBoundaryBmp);
class PtPair 
{
public:
	int sId;
	int eId;
	PtPair(int s,int e) 
	{
		this->sId = s;
		this->eId = e;
	}
};
bool addPair(std::vector<PtPair> *pairs,int s,int e);
class Pt
{
public:
	int color;
	int x;
	int y;
	Pt(int color,int x, int y)
	{
		this->color = color;
		this->x = x;
		this->y = y;
	}
};
bool LinkPts(const char* SrcBmpName, const char* outPtTxt, const char* outPairsTxt, const char* DestBmp);
bool ddaLine(std::vector<Pt> *pts, unsigned char ** NewMtxBuf, int sId,int eId);