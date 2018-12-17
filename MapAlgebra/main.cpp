#include "MapAlgebraFunc.h"
#include "MapAlgebraTemp.h"

int main() {
	//reverse
	BmpReverse("test256.bmp", "reversed256.bmp");
	//overlay
	BmpOverlay("test256.bmp", "test256-2.bmp", "overlayed256.bmp");
	//format transform
	Bmp256to32b("test256.bmp", "to32bed.bmp");
	//manhattan dis
	//CManHattanTmp *tmp = new CManHattanTmp;
	//CEu5Tmp *tmp = new CEu5Tmp;
	COctagonTmp *tmp = new COctagonTmp;
	DisTransform("adTest.bmp", tmp, "distribution.bmp", "distance.bmp", 0xFF);
	//average
	CAverageTmp *averageTmp = new CAverageTmp;
	BmpFocal("disSource.bmp", averageTmp, "averagedTmp.bmp");
	//boundary
	GenerateBoundary("distribution.bmp", "boundaryed.bmp");
	//buffer
	GenerateBuffer("distance.bmp", 14, "buffered.bmp");
	//AdhesionTransform
	AdhesionTransform("adTest.bmp", 80, 20);
	//AxisTransform
	AxisTransform("axisTest.bmp", "axised.bmp");
	//DelauneyTransform
	DelauneyTransform("delanueyTest.bmp", "delanuryResult.bmp");
	delete tmp;
	delete averageTmp;
	return 0;
}