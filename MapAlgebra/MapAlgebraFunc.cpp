#include "MapAlgebraFunc.h"
#include <stdio.h>
#include <windows.h>

bool BmpReverse(const char * SrcBmpName, const char * DestBmpName)
{
	//open source file
	FILE *file1 = fopen(SrcBmpName, "rb");
	if (file1 == NULL)
	{
		printf("file not found");
		return false;
	}

	//load bmp file
	BITMAPFILEHEADER BmpFileHeader;
	BITMAPINFOHEADER BmpInfoHeader;
	unsigned char ClrTab[256 * 4];
	fread(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file1);
	fread(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file1);
	fread(ClrTab, 1024, 1, file1);
	unsigned MtxWidth, MtxHeight, BmpWidth;
	BmpWidth = BmpInfoHeader.biWidth;
	MtxWidth = (BmpWidth + 3) / 4 * 4;
	MtxHeight = BmpInfoHeader.biHeight;

	//read matrix
	unsigned char * LineBuf = new unsigned char[MtxWidth];//read data line buffer
	unsigned char ** MtxBuf = new unsigned char *[MtxHeight];
	for (int i = 0; i < MtxHeight; i++)
	{
		MtxBuf[i] = new unsigned char[MtxWidth];
		for (int j = 0; j < MtxWidth; j++)
		{
			MtxBuf[i][j] = 0xFF;
		}
	}

	//reverse color
	for (int i = 0; i < MtxHeight; i++)
	{
		fread(LineBuf, sizeof(unsigned char), MtxWidth, file1);
		for (int j = 0; j < MtxWidth; j++)
		{
			if (LineBuf[j] == 0xFF) continue;
			MtxBuf[i][j] = 255 - LineBuf[j];
		}
	}

	//write file
	FILE *file2 = fopen(DestBmpName, "wb");
	if (file2 == NULL)
	{
		printf("file not found");
		return false;
	}
	fwrite(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file2);
	fwrite(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file2);
	fwrite(ClrTab, 1024, 1, file2);
	for (int i = 0; i < MtxHeight; i++)
	{
		fwrite(MtxBuf[i], 1, MtxWidth, file2);
	}
	fclose(file1);
	fclose(file2);

	//free storage
	for (int i = 0; i < MtxHeight; i++)
	{
		delete[] MtxBuf[i];
		MtxBuf[i] = NULL;
	}
	delete[] MtxBuf;
	MtxBuf = NULL;
	delete[] LineBuf;
	LineBuf = NULL;

	return false;
}
