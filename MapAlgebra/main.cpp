#include "MapAlgebraFunc.h"
#include "MapAlgebraTemp.h"

int main() {
	BmpReverse("test256.bmp","reversed256.bmp");
	BmpOverlay("test256.bmp", "test256-2.bmp",  "overlayed256.bmp");
	Bmp256to32b("test256.bmp", "to32bed.bmp");
	CManHattanTmp *manHattanTmp = new CManHattanTmp;
	DisTransform("disSource.bmp", manHattanTmp);
	CAverageTmp *averageTmp = new CAverageTmp;
	BmpFocal("disSource.bmp", averageTmp);
	delete manHattanTmp;
	delete averageTmp;
	return 0;
}