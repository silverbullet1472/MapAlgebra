#pragma once
#include "MapAlgebraTemp.h"

//1 reverse 256-color img
bool BmpReverse(const char* SrcBmpName, const char* DestBmpName);

//2 overlay
bool BmpOverlay(const char* SrcBmpName1, const char* SrcBmpName2, const char* DestBmpName);

//3 focal function
bool BmpFocal(const char* SrcBmpName, const char* DestBmpName);

//4 format changer
int Bmp256to32b(const char* SourceFileName, const char* IdxFileName);

//5 distance transform
//read a 256-color bmp, generate distance field and distribution field
//input: 256-color output:distance field 32 ,distribution 256-color(8bit)
bool DisTransform(const char* SrcBmpName, CDisTmp *pDisTmp);

