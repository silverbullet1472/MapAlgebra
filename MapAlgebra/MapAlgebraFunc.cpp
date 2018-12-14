#include "MapAlgebraFunc.h"
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <vector>
#define MaxFloat 9999999999.999999
#define MinFloat 0.000001

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
	unsigned char ClrTab[256 * 4];// color table : r + b + g + alpha
	fread(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file1);
	fread(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file1);
	fread(ClrTab, 1024, 1, file1);
	unsigned BmpWidth, MtxHeight, MtxWidth;
	MtxWidth = BmpInfoHeader.biWidth;
	BmpWidth = (MtxWidth + 3) / 4 * 4;
	// Windows规定一个扫描行所占的字节数必须是4 的倍数 ( 即以 long 为单位 ), 不足的以 0 填充，具体数据举例：
	// 也就是说，写入图片一行的像素信息的时候，每一行的字节数都必须是4的倍数，不足的在后面补0，然后才又开始写入下一行的像素信息
	// 在本函数中，每个像素用 1 byte = 8 bit来表示颜色信息
	MtxHeight = BmpInfoHeader.biHeight;

	//read matrix
	unsigned char * LineBuf = new unsigned char[BmpWidth];//read data line buffer
	unsigned char ** MtxBuf = new unsigned char *[MtxHeight];
	for (int i = 0; i < MtxHeight; i++)
	{
		MtxBuf[i] = new unsigned char[BmpWidth];
		for (int j = 0; j < BmpWidth; j++)
		{
			MtxBuf[i][j] = 0xFF; // 0xFF = 1111 1111 = 255
		}
	}

	//reverse color
	for (int i = 0; i < MtxHeight; i++)
	{
		fread(LineBuf, sizeof(unsigned char), BmpWidth, file1);
		for (int j = 0; j < BmpWidth; j++)
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
		fwrite(MtxBuf[i], 1, BmpWidth, file2);
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

	return true;
}

bool BmpOverlay(const char* SrcBmpName1, const char* SrcBmpName2, const char* DestBmpName)
{
	//open source file
	FILE *file1 = fopen(SrcBmpName1, "rb");
	FILE *file2 = fopen(SrcBmpName2, "rb");

	//load bmp file
	BITMAPFILEHEADER BmpFileHeader;
	BITMAPINFOHEADER BmpInfoHeader;
	unsigned char ClrTab[256 * 4];// color table : r + b + g + alpha

	fread(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file1);
	fread(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file1);
	fread(ClrTab, 1024, 1, file1);

	fread(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file2);
	fread(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file2);
	fread(ClrTab, 1024, 1, file2);

	unsigned BmpWidth, MtxHeight, MtxWidth;
	MtxWidth = BmpInfoHeader.biWidth;
	BmpWidth = (MtxWidth + 3) / 4 * 4;
	// Windows规定一个扫描行所占的字节数必须是4 的倍数 ( 即以 long 为单位 ), 不足的以 0 填充，具体数据举例：
	// 也就是说，写入图片一行的像素信息的时候，每一行的字节数都必须是4的倍数，不足的在后面补0，然后才又开始写入下一行的像素信息
	// 在本函数中，每个像素用 1 byte = 8 bit来表示颜色信息
	MtxHeight = BmpInfoHeader.biHeight;

	//read matrix
	unsigned char * LineBuf1 = new unsigned char[BmpWidth];//read data line buffer
	unsigned char * LineBuf2 = new unsigned char[BmpWidth];//read data line buffer
	unsigned char ** MtxBuf = new unsigned char *[MtxHeight];
	for (int i = 0; i < MtxHeight; i++)
	{
		MtxBuf[i] = new unsigned char[BmpWidth];
		for (int j = 0; j < BmpWidth; j++)
		{
			MtxBuf[i][j] = 0xFF; // 0xFF = 1111 1111 = 255
		}
	}

	//reverse color
	for (int i = 0; i < MtxHeight; i++)
	{
		fread(LineBuf1, sizeof(unsigned char), BmpWidth, file1);
		fread(LineBuf2, sizeof(unsigned char), BmpWidth, file2);
		for (int j = 0; j < BmpWidth; j++)
		{
			if (LineBuf1[j] == 0xFF && LineBuf2[j] == 0xFF) continue;
			MtxBuf[i][j] = LineBuf1[j] + LineBuf2[j];
		}
	}
	fclose(file1);
	fclose(file2);

	//write file
	FILE *file3 = fopen(DestBmpName, "wb");

	fwrite(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file3);
	fwrite(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file3);
	fwrite(ClrTab, 1024, 1, file3);
	for (int i = 0; i < MtxHeight; i++)
	{
		fwrite(MtxBuf[i], 1, BmpWidth, file3);
	}

	fclose(file3);
	//free storage
	for (int i = 0; i < MtxHeight; i++)
	{
		delete[] MtxBuf[i];
		MtxBuf[i] = NULL;
	}
	delete[] MtxBuf;
	MtxBuf = NULL;
	delete[] LineBuf1;
	LineBuf1 = NULL;
	delete[] LineBuf2;
	LineBuf2 = NULL;
	return true;
}

bool BmpFocal(const char * SrcBmpName, CTmp *pFocalTmp, const char* DestBmpName)
{
	// 1 read bmp info, prepare storage
	FILE *file1 = fopen(SrcBmpName, "rb");

	//load bmp file
	BITMAPFILEHEADER BmpFileHeader;
	BITMAPINFOHEADER BmpInfoHeader;
	unsigned char ClrTab[256 * 4];
	fread(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file1);
	fread(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file1);
	fread(ClrTab, 1024, 1, file1);

	unsigned BmpWidth, MtxHeight, MtxWidth;
	MtxWidth = BmpInfoHeader.biWidth;
	BmpWidth = (MtxWidth + 3) / 4 * 4;
	MtxHeight = BmpInfoHeader.biHeight;

	//read matrix
	unsigned char * LineBuf = new unsigned char[BmpWidth];//read data line buffer
	unsigned char ** MtxBuf = new unsigned char *[MtxHeight];
	for (int i = 0; i < MtxHeight; i++)
	{
		MtxBuf[i] = new unsigned char[BmpWidth];
		fread(LineBuf, sizeof(unsigned char), BmpWidth, file1);
		for (int j = 0; j < BmpWidth; j++)
		{
			MtxBuf[i][j] = LineBuf[j];
		}
	}

	unsigned char ** NewMtxBuf = new unsigned char *[MtxHeight];
	for (int i = 0; i < MtxHeight; i++)
	{
		NewMtxBuf[i] = new unsigned char[BmpWidth];
		for (int j = 0; j < BmpWidth; j++)
		{
			NewMtxBuf[i][j] = 0xff;
		}
	}

	//calculate
	for (int y = 0; y < MtxHeight; y++)
	{
		for (int x = 0; x < BmpWidth; x++)
		{
			if (MtxBuf[y][x] == 0xff) continue;
			unsigned char tmp = 0;
			for (int k = 0; k < pFocalTmp->GetSize(); k++)
			{
				char OffX = pFocalTmp->GetOffX(k);
				char OffY = pFocalTmp->GetOffY(k);
				float tmpPower = pFocalTmp->GetPower(k);
				int CurrX = x + OffX;
				int CurrY = y + OffY;
				if ((CurrX < 0) || (CurrX > BmpWidth - 1)) continue;
				if ((CurrY < 0) || (CurrY > MtxHeight - 1)) continue;
				unsigned char MtxValue = MtxBuf[CurrY][CurrX];
				tmp += MtxValue*tmpPower;
			}
			NewMtxBuf[y][x] = tmp / pFocalTmp->GetSize();
		}
	}

	//write file
	FILE *file2 = fopen(DestBmpName, "wb");
	fwrite(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file2);
	fwrite(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file2);
	fwrite(ClrTab, 1024, 1, file2);
	for (int i = 0; i < MtxHeight; i++)
	{
		fwrite(NewMtxBuf[i], 1, BmpWidth, file2);
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

	for (int i = 0; i < MtxHeight; i++)
	{
		delete[] NewMtxBuf[i];
		NewMtxBuf[i] = NULL;
	}
	delete[] NewMtxBuf;
	NewMtxBuf = NULL;

	return true;
}

int Bmp256to32b(const char * SourceFileName, const char * IdxFileName)
{
	unsigned i, j; // 循环变量
	// 1 检验源文件
	assert(SourceFileName != NULL);

	FILE* SrcFileStream = fopen(SourceFileName, "rb");
	if (SrcFileStream == NULL) return -1; // 出错代码-1，文件不存在

	BITMAPFILEHEADER SrcFileHead;
	BITMAPINFOHEADER SrcFileInfo;
	unsigned char ColorIdx[1024];

	if (fread(&SrcFileHead, sizeof(BITMAPFILEHEADER), 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return -1;
	}
	if (fread(&SrcFileInfo, sizeof(BITMAPINFOHEADER), 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return -1;
	}
	if (fread(ColorIdx, 1024, 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return -1;
	}

	if (SrcFileInfo.biBitCount != 8) // 256色图处理
	{
		fclose(SrcFileStream);
		return -2; // 出错代码2，文件格式不合要求
	}


	// 2 编制实体索引
	BITMAPFILEHEADER IdxFileHead;
	BITMAPINFOHEADER IdxFileInfo;
	int LineBytes = (SrcFileInfo.biWidth + 3) / 4 * 4;
	int LineBytes2 = SrcFileInfo.biWidth;

	IdxFileHead.bfType = SrcFileHead.bfType;
	IdxFileHead.bfSize = 54 + LineBytes2 * 4 * SrcFileInfo.biHeight;
	IdxFileHead.bfReserved1 = 0;
	IdxFileHead.bfReserved2 = 0;
	IdxFileHead.bfOffBits = 54;

	/* 14 bytes
	int bfType; // 位图文件的类型，必须为 ' B '' M '两个字母 (0-1字节 ) 　
	int bfSize; // 位图文件的大小，以字节为单位 (2-5 字节 ) 　
	int bfReserved1; // 位图文件保留字，必须为 0(6-7 字节 ) 　　
	int bfReserved2; // 位图文件保留字，必须为 0(8-9 字节 ) 　
	int bfOffBits; // 位图数据的起始位置，以相对于位图 (10-13 字节 ) 　
	*/

	IdxFileInfo.biSize = 40;
	IdxFileInfo.biWidth = SrcFileInfo.biWidth;
	IdxFileInfo.biHeight = SrcFileInfo.biHeight;
	IdxFileInfo.biPlanes = 1;
	IdxFileInfo.biBitCount = 32;
	IdxFileInfo.biCompression = 0;
	IdxFileInfo.biSizeImage = 0;
	IdxFileInfo.biXPelsPerMeter = 0;
	IdxFileInfo.biYPelsPerMeter = 0;
	IdxFileInfo.biClrUsed = 0;
	IdxFileInfo.biClrImportant = 0;

	/* 40 bytes
	int Size; // 本结构所占用字节数 (14-17 字节 ) 　
	int image_width; // 位图的宽度，以像素为单位 (18-21 字节 ) 　　
	int image_heigh; // 位图的高度，以像素为单位 (22-25 字节 ) 　
	int Planes; // 目标设备的级别，必须为 1(26-27 字节 ) 　　
	int biBitCount;// 每个像素所需的位数，必须是或1,4,8 24(// 真彩色 ) 之一　　(28-29 字节)
	int biCompression; // 位图压缩类型，必须是 0( 不压缩 ), 1(BI_RLE8 压缩类型 ) 或// 2(BI_RLE4 压缩类型 ) 之一　　(30-33 字节 )
	int SizeImage; // 位图数据的大小，以字节为单位 (34-37 字节 ) 　　
	int biXPelsPerMeter; // 位图水平分辨率，每米像素数 (38-41 字节 ) 　　
	int biYPelsPerMeter; // 位图垂直分辨率，每米像素数 (42-45 字节 ) 　　
	int biClrUsed;// 位图实际使用的颜色表中的颜色数 (46-49 字节 ) 　　
	int biClrImportant;// 位图显示过程中重要的颜色数 (50-53 字节 )
	*/


	// 2.1 构造原始矩阵
	unsigned char * pSrcCellLineBuf = new unsigned char[LineBytes];   // 源文件行缓冲
	unsigned **pIdxCellMatixBuf = new unsigned*[IdxFileInfo.biHeight]; //索引矩阵
	for (i = 0; i < IdxFileInfo.biHeight; i++)
	{
		fseek(SrcFileStream, SrcFileHead.bfOffBits + i * sizeof(char)*LineBytes, 0);
		fread(pSrcCellLineBuf, sizeof(char), LineBytes, SrcFileStream);
		pIdxCellMatixBuf[i] = new unsigned[LineBytes2];
		for (j = 0; j < LineBytes2; j++)
			if (pSrcCellLineBuf[j] == 0xFF) pIdxCellMatixBuf[i][j] = 0xFFFFFFFF;// 4 bytes for 255
			else
			{
				int k = pSrcCellLineBuf[j]; // 0 = [0]
				unsigned uintTmp = k; // uintTmp = 0 = 0000 0000 + 0000 0000 + 0000 0000 + 0000 0000
				//  取对应颜色

				uintTmp = ColorIdx[k * 4]; // color[] 1111 1111 + 1111 1111 + 1111 1111 + 1111 1111 uintTmp = 0000 0000 + 0000 0000 + 0000 0000 + 1111 1111
				uintTmp <<= 8;// uintTmp = 0000 0000 + 0000 0000 + 1111 1111 + 0000 0000 
				uintTmp += ColorIdx[k * 4 + 1];// uintTmp = 0000 0000 + 0000 0000 + 1111 1111 + 1111 1111 
				uintTmp <<= 8;// uintTmp = 0000 0000 + 1111 1111 + 1111 1111 + 0000 0000
				uintTmp += ColorIdx[k * 4 + 2];// uintTmp = 0000 0000 + 1111 1111 + 1111 1111 + 1111 1111
				uintTmp <<= 8;// uintTmp = 1111 1111 + 1111 1111 + 1111 1111 + 0000 0000 
				uintTmp += ColorIdx[k * 4 + 3];// uintTmp = 1111 1111 + 1111 1111 + 1111 1111 + 1111 1111

				pIdxCellMatixBuf[i][j] = uintTmp;
			}
	}

	FILE *ObjIdxFile = fopen(IdxFileName, "wb");
	if (ObjIdxFile == NULL) return -3; // 出错代码3，实体索引矩阵文件无法创建
	fseek(ObjIdxFile, 0, 0);
	fwrite(&IdxFileHead, sizeof(BITMAPFILEHEADER), 1, ObjIdxFile);
	fwrite(&IdxFileInfo, sizeof(BITMAPINFOHEADER), 1, ObjIdxFile);
	for (i = 0; i < IdxFileInfo.biHeight; i++)
	{
		fseek(ObjIdxFile, 54 + i * sizeof(unsigned)*LineBytes2, 0);
		fwrite(pIdxCellMatixBuf[i], sizeof(unsigned), LineBytes2, ObjIdxFile);
		delete[] pIdxCellMatixBuf[i];
		pIdxCellMatixBuf[i] = NULL;
	}

	delete[] pIdxCellMatixBuf;
	delete[] pSrcCellLineBuf;

	fclose(ObjIdxFile);
	fclose(SrcFileStream);
	return 1; // 返回成功代码1
}

bool DisTransform(const char* SrcBmpName, CTmp *pDisTmp, const char* DestDistribution, const char* DestDistance, unsigned char Maxcolor)
{
	// 1 read bmp info, prepare storage
	FILE *file1 = fopen(SrcBmpName, "rb");

	//load bmp file
	BITMAPFILEHEADER BmpFileHeader;
	BITMAPINFOHEADER BmpInfoHeader;
	unsigned char ClrTab[256 * 4];
	fread(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file1);
	fread(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file1);
	fread(ClrTab, 1024, 1, file1);

	unsigned MtxWidth, MtxHeight, Bmp256Width;
	MtxWidth = BmpInfoHeader.biWidth;
	MtxHeight = BmpInfoHeader.biHeight;
	Bmp256Width = (MtxWidth + 3) / 4 * 4;

	// 2 init distribution & distance field
	// 2.1 distribution buffer
	unsigned char ** LocMtx = new unsigned char *[MtxHeight];
	float  ** DisMtx = new float *[MtxHeight];
	for (int i = 0; i < MtxHeight; i++)
	{
		LocMtx[i] = new unsigned char[Bmp256Width];
		fread(LocMtx[i], sizeof(unsigned char), Bmp256Width, file1);
		DisMtx[i] = new float[MtxWidth];
		for (int j = 0; j < MtxWidth; j++)
		{
			if (LocMtx[i][j] == Maxcolor)
			{
				DisMtx[i][j] = MaxFloat;
			}
			else
				DisMtx[i][j] = 0;
		}
	}
	fclose(file1);

	// 3 transform by traversing temp: down to up, left to right
	// traverse temp element, find relative distance minimum, refresh dist & distri field
	// 3.1 ensure field coordinate for each temp element, escape illegal one
	// 3.2 minimum
	// 3.3 refresh
	for (int y = 0; y < MtxHeight; y++)
	{
		for (int x = 0; x < MtxWidth; x++)
		{
			float tmpMin = DisMtx[y][x];
			if (fabs(tmpMin) < MinFloat) continue;
			for (int k = 0; k < pDisTmp->GetSize() / 2; k++)
			{
				char OffX = pDisTmp->GetOffX(k);
				char OffY = pDisTmp->GetOffY(k);
				float tempDis = pDisTmp->GetPower(k);
				int CurrX = x + OffX;
				int CurrY = y + OffY;
				if ((CurrX < 0) || (CurrX > MtxWidth - 1)) continue;
				if ((CurrY < 0) || (CurrY > MtxHeight - 1)) continue;
				float MtxDis = DisMtx[CurrY][CurrX];

				if (fabs(MtxDis - MaxFloat) < 0.00001) continue;

				if (MtxDis + tempDis < tmpMin)
				{
					tmpMin = tempDis + MtxDis;
					DisMtx[y][x] = tmpMin;
					LocMtx[y][x] = LocMtx[CurrY][CurrX];
				}
			}
		}
	}

	// 4 transform by traversing temp: up to down, right to left
	for (int y = MtxHeight - 1; y > 0; y--)
	{
		for (int x = MtxWidth - 1; x > 0; x--)
		{
			float tmpMin = DisMtx[y][x] + 0;
			if (fabs(tmpMin) < MinFloat) continue;
			for (int k = pDisTmp->GetSize(); k > pDisTmp->GetSize() / 2; k--)
			{
				char OffX = pDisTmp->GetOffX(k);
				char OffY = pDisTmp->GetOffY(k);
				float tempDis = pDisTmp->GetPower(k);
				int CurrX = x + OffX;
				int CurrY = y + OffY;
				if ((CurrX < 0) || (CurrX > MtxWidth - 1)) continue;
				if ((CurrY < 0) || (CurrY > MtxHeight - 1)) continue;
				float MtxDis = DisMtx[CurrY][CurrX];
				if (fabs(MtxDis - MaxFloat) < 0.00001) continue;
				if (MtxDis + tempDis < tmpMin)
				{
					tmpMin = tempDis + MtxDis;
					DisMtx[y][x] = tmpMin;
					LocMtx[y][x] = LocMtx[CurrY][CurrX];
				}
			}
		}
	}

	// 5 write file
	BITMAPFILEHEADER IdxFileHead;
	BITMAPINFOHEADER IdxFileInfo;

	IdxFileHead.bfType = BmpFileHeader.bfType;
	IdxFileHead.bfSize = 54 + BmpInfoHeader.biWidth * 4 * BmpInfoHeader.biHeight;
	IdxFileHead.bfReserved1 = 0;
	IdxFileHead.bfReserved2 = 0;
	IdxFileHead.bfOffBits = 54;

	IdxFileInfo.biSize = 40;
	IdxFileInfo.biWidth = BmpInfoHeader.biWidth;
	IdxFileInfo.biHeight = BmpInfoHeader.biHeight;
	IdxFileInfo.biPlanes = 1;
	IdxFileInfo.biBitCount = 32;
	IdxFileInfo.biCompression = 0;
	IdxFileInfo.biSizeImage = 0;
	IdxFileInfo.biXPelsPerMeter = 0;
	IdxFileInfo.biYPelsPerMeter = 0;
	IdxFileInfo.biClrUsed = 0;
	IdxFileInfo.biClrImportant = 0;

	// 5.1 write distance file
	FILE * bmpWrite1 = fopen(DestDistance, "wb");
	fwrite(&IdxFileHead, sizeof(BITMAPFILEHEADER), 1, bmpWrite1);
	fwrite(&IdxFileInfo, sizeof(BITMAPINFOHEADER), 1, bmpWrite1);
	for (int y = 0; y < MtxHeight; y++)
	{
		fwrite(DisMtx[y], sizeof(float), MtxWidth, bmpWrite1);
	}
	fclose(bmpWrite1);

	// 5.2 write distribution file 8位unsigned char型
	FILE * bmpWrite2 = fopen(DestDistribution, "wb");
	fwrite(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, bmpWrite2);
	fwrite(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, bmpWrite2);
	fwrite(ClrTab, 1024, 1, bmpWrite2);
	for (int i = 0; i < MtxHeight; i++)
	{
		fwrite(LocMtx[i], 1, Bmp256Width, bmpWrite2);
	}
	fclose(bmpWrite2);

	// 6 free storage
	for (unsigned j = 0; j < MtxHeight; j++)
	{
		delete[]LocMtx[j];
		delete[]DisMtx[j];
	}
	delete[]DisMtx;
	delete[]LocMtx;
	return true;
}

bool GenerateBoundary(const char * SrcBmpName, const char* DestBmpName)
{
	//open source file
	FILE *file1 = fopen(SrcBmpName, "rb");

	//load bmp file
	BITMAPFILEHEADER BmpFileHeader;
	BITMAPINFOHEADER BmpInfoHeader;
	unsigned char ClrTab[256 * 4];// color table : r + b + g + alpha
	fread(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file1);
	fread(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file1);
	fread(ClrTab, 1024, 1, file1);
	unsigned BmpWidth, MtxHeight, MtxWidth;
	MtxWidth = BmpInfoHeader.biWidth;
	BmpWidth = (MtxWidth + 3) / 4 * 4;
	MtxHeight = BmpInfoHeader.biHeight;

	//read matrix
	unsigned char * LineBuf = new unsigned char[BmpWidth];//read data line buffer
	unsigned char ** MtxBuf = new unsigned char *[MtxHeight];
	for (int i = 0; i < MtxHeight; i++)
	{
		MtxBuf[i] = new unsigned char[MtxWidth];
		fread(LineBuf, sizeof(unsigned char), BmpWidth, file1);
		for (int j = 0; j < MtxWidth; j++)
		{
			MtxBuf[i][j] = LineBuf[j];
		}
	}

	//initialize
	unsigned char ** NewMtxBuf = new unsigned char *[MtxHeight];
	for (int i = 0; i < MtxHeight; i++)
	{
		NewMtxBuf[i] = new unsigned char[BmpWidth];
		for (int j = 0; j < MtxWidth; j++)
		{
			NewMtxBuf[i][j] = 0xff;
		}
	}

	//calculate
	for (int i = 0; i < MtxHeight; i++)
	{
		for (int j = 0; j < MtxWidth; j++)
		{
			/*if (i == 0 || i == MtxHeight - 1)  NewMtxBuf[i][j] = 150;
			if (j == 0 || j == BmpWidth - 1)   NewMtxBuf[i][j] = 150; */
			if ((i == 0) || (j == 0) || (i == MtxHeight - 1) || (j == MtxWidth - 1)) continue;
			if ((MtxBuf[i - 1][j] != MtxBuf[i + 1][j]) || (MtxBuf[i][j + 1] != MtxBuf[i][j - 1]))
			{
				NewMtxBuf[i][j] = 0;
			}
			else NewMtxBuf[i][j] = 0xff;
		}
	}

	//write file
	FILE *file2 = fopen(DestBmpName, "wb");
	fwrite(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file2);
	fwrite(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file2);
	fwrite(ClrTab, 1024, 1, file2);
	for (int i = 0; i < MtxHeight; i++)
	{
		fwrite(NewMtxBuf[i], 1, BmpWidth, file2);
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

	for (int i = 0; i < MtxHeight; i++)
	{
		delete[] NewMtxBuf[i];
		NewMtxBuf[i] = NULL;
	}
	delete[] NewMtxBuf;
	NewMtxBuf = NULL;

	return true;
}

bool GenerateBuffer(const char * SrcBmpName, float radius, const char* DestBmpName)
{
	//open source file
	FILE *file1 = fopen(SrcBmpName, "rb");

	//load bmp file
	BITMAPFILEHEADER BmpFileHeader;
	BITMAPINFOHEADER BmpInfoHeader;
	fread(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file1);
	fread(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file1);
	unsigned MtxWidth, MtxHeight, BmpWidth;
	MtxWidth = BmpInfoHeader.biWidth;
	MtxHeight = BmpInfoHeader.biHeight;
	BmpWidth = (MtxWidth + 3) / 4 * 4;
	//read matrix
	float * LineBuf = new float[MtxWidth];//read data line buffer
	float ** MtxBuf = new float *[MtxHeight];
	for (int i = 0; i < MtxHeight; i++)
	{
		MtxBuf[i] = new float[MtxWidth];
		fread(LineBuf, sizeof(float), MtxWidth, file1);
		for (int j = 0; j < MtxWidth; j++)
		{
			MtxBuf[i][j] = LineBuf[j];
		}
	}

	//initialize
	unsigned char ** NewMtxBuf = new unsigned char *[MtxHeight];
	for (int i = 0; i < MtxHeight; i++)
	{
		NewMtxBuf[i] = new unsigned char[BmpWidth];
		for (int j = 0; j < MtxWidth; j++)
		{
			NewMtxBuf[i][j] = 0xff;
		}
	}

	//calculate
	for (int i = 0; i < MtxHeight; i++)
	{
		for (int j = 0; j < MtxWidth; j++)
		{
			if (MtxBuf[i][j] < radius) NewMtxBuf[i][j] = 0;
			else NewMtxBuf[i][j] = 0xff;
		}
	}

	//write file
	FILE *file2 = fopen("distribution.bmp", "rb");
	BITMAPFILEHEADER IdxFileHeader;
	BITMAPINFOHEADER IdxInfoHeader;
	unsigned char ClrTab[256 * 4];// color table : r + b + g + alpha
	fread(&IdxFileHeader, sizeof(BITMAPFILEHEADER), 1, file2);
	fread(&IdxInfoHeader, sizeof(BITMAPINFOHEADER), 1, file2);
	fread(ClrTab, 1024, 1, file2);

	FILE *file3 = fopen(DestBmpName, "wb");
	fwrite(&IdxFileHeader, sizeof(BITMAPFILEHEADER), 1, file3);
	fwrite(&IdxInfoHeader, sizeof(BITMAPINFOHEADER), 1, file3);
	fwrite(ClrTab, 1024, 1, file3);
	for (int i = 0; i < MtxHeight; i++)
	{
		fwrite(NewMtxBuf[i], 1, BmpWidth, file3);
	}
	fclose(file1);
	fclose(file2);
	fclose(file3);
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

	for (int i = 0; i < MtxHeight; i++)
	{
		delete[] NewMtxBuf[i];
		NewMtxBuf[i] = NULL;
	}
	delete[] NewMtxBuf;
	NewMtxBuf = NULL;

	return true;
}

bool AdhesionTransform(const char * SrcBmpName, float outRadius, float inRadius)
{
	//粘连变换：先生成外距填补凹陷，再生成内距，保凸减凹

	//外距
	CManHattanTmp *manHattanTmp = new CManHattanTmp;
	DisTransform(SrcBmpName, manHattanTmp, "adDistribution1.bmp", "adDis1.bmp", 0xFF);
	GenerateBuffer("adDis1.bmp", outRadius, "adBuffered1.bmp");

	//内距
	DisTransform("adBuffered1.bmp", manHattanTmp, "adDistribution2.bmp", "adDis2.bmp", 0);
	GenerateBuffer("adDis2.bmp", inRadius, "adhesionTransformed.bmp");
	delete manHattanTmp;
	return true;
}

bool AxisTransform(const char * SrcBmpName, const char * DestBmpName)
{
	CManHattanTmp *manHattanTmp = new CManHattanTmp;
	DisTransform(SrcBmpName, manHattanTmp, "axisDistribution1.bmp", "adDis1.bmp", 0);
	GenerateBoundary("axisDistribution1.bmp", DestBmpName);
	delete manHattanTmp;
	return true;
}

bool DelauneyTransform(const char * SrcBmpName, const char * DestBmpName)
{
	COctagonTmp *tmp = new COctagonTmp;
	GetPtCoors(SrcBmpName, "pts.txt");
	DisTransform("delanueyTest.bmp", tmp, "delauneyDistribution1.bmp", "delauneyDistance1.bmp", 0xff);
	GeneratePointPairs("delauneyDistribution1.bmp", "pairs.txt", "delauneyBoundray1.bmp");
	LinkPts("delanueyTest.bmp", "pts.txt", "pairs.txt", DestBmpName);
	return true;
}

bool GetPtCoors(const char * SrcBmpName, const char * outPtTxt)
{
	//open source file
	FILE *file1 = fopen(SrcBmpName, "rb");

	//load bmp file
	BITMAPFILEHEADER BmpFileHeader;
	BITMAPINFOHEADER BmpInfoHeader;
	unsigned char ClrTab[256 * 4];// color table : r + b + g + alpha
	fread(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file1);
	fread(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file1);
	fread(ClrTab, 1024, 1, file1);
	unsigned MtxWidth, MtxHeight, BmpWidth;
	MtxWidth = BmpInfoHeader.biWidth;
	MtxHeight = BmpInfoHeader.biHeight;
	BmpWidth = (MtxWidth + 3) / 4 * 4;

	//read matrix
	unsigned char * LineBuf = new unsigned char[BmpWidth];//read data line buffer
	FILE *fp = fopen(outPtTxt, "w");
	for (int i = 0; i < MtxHeight; i++)
	{
		int ptX = -1, ptY = -1;
		int color = -1;
		fread(LineBuf, sizeof(unsigned char), BmpWidth, file1);

		for (int j = 0; j < MtxWidth; j++)
		{
			if (LineBuf[j] == 0xFF) continue;
			ptX = i;
			ptY = j;
			color = LineBuf[j];
			fprintf(fp, "%d\t%d\t%d\n", color, ptX, ptY);
		}

	}
	fclose(fp);
	fclose(file1);

	//free storage
	delete[] LineBuf;
	LineBuf = NULL;

	return true;
}

bool GeneratePointPairs(const char * SrcBmpName, const char * outPairsTxt, const char* DestBoundaryBmp)
{
	//open source file
	FILE *file1 = fopen(SrcBmpName, "rb");

	//load bmp file
	BITMAPFILEHEADER BmpFileHeader;
	BITMAPINFOHEADER BmpInfoHeader;
	unsigned char ClrTab[256 * 4];// color table : r + b + g + alpha
	fread(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file1);
	fread(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file1);
	fread(ClrTab, 1024, 1, file1);
	unsigned MtxWidth, MtxHeight, BmpWidth;
	MtxWidth = BmpInfoHeader.biWidth;
	BmpWidth = (MtxWidth + 3) / 4 * 4;
	MtxHeight = BmpInfoHeader.biHeight;

	//read matrix
	unsigned char * LineBuf = new unsigned char[BmpWidth];//read data line buffer
	unsigned char ** MtxBuf = new unsigned char *[MtxHeight];
	for (int i = 0; i < MtxHeight; i++)
	{
		MtxBuf[i] = new unsigned char[MtxWidth];
		fread(LineBuf, sizeof(unsigned char), BmpWidth, file1);
		for (int j = 0; j < MtxWidth; j++)
		{
			MtxBuf[i][j] = LineBuf[j];
		}
	}

	//initialize
	unsigned char ** NewMtxBuf = new unsigned char *[MtxHeight];
	for (int i = 0; i < MtxHeight; i++)
	{
		NewMtxBuf[i] = new unsigned char[BmpWidth];
		for (int j = 0; j < MtxWidth; j++)
		{
			NewMtxBuf[i][j] = 0xff;
		}
	}

	//calculate
	std::vector<PtPair> *pairs = new std::vector<PtPair>;
	int id1=-1, id2=-1;
	for (int i = 0; i < MtxHeight; i++)
	{
		for (int j = 0; j < MtxWidth; j++)
		{
			if ((i == 0) || (j == 0) || (i == MtxHeight - 1) || (j == MtxWidth - 1)) continue;
			if ((MtxBuf[i - 1][j] != MtxBuf[i + 1][j]) || (MtxBuf[i][j + 1] != MtxBuf[i][j - 1]))
			{
				NewMtxBuf[i][j] = 0;
				if (MtxBuf[i - 1][j] > MtxBuf[i + 1][j]) 
				{
					id1 = MtxBuf[i - 1][j]; id2 = MtxBuf[i + 1][j];
					addPair(pairs,id1,id2);
				} 
				if (MtxBuf[i + 1][j] > MtxBuf[i - 1][j])
				{
					id1 = MtxBuf[i + 1][j]; id2 = MtxBuf[i - 1][j];
					addPair(pairs, id1, id2);
				}
				if (MtxBuf[i][j + 1] > MtxBuf[i][j - 1])
				{
					id1 = MtxBuf[i][j + 1]; id2 = MtxBuf[i][j - 1];
					addPair(pairs, id1, id2);
				}
				if (MtxBuf[i][j - 1] > MtxBuf[i][j + 1])
				{
					id1 = MtxBuf[i][j - 1]; id2 = MtxBuf[i][j + 1];
					addPair(pairs, id1, id2);
				}
			}
			else NewMtxBuf[i][j] = 0xff;
		}
	}

	//write file
	FILE *fp = fopen(outPairsTxt, "w");
	for (int i = 0; i < pairs->size(); i++)
	{
		fprintf(fp,"%d\t%d\n",pairs->at(i).sId, pairs->at(i).eId);
	}
	fclose(fp);

	FILE *file2 = fopen(DestBoundaryBmp, "wb");
	fwrite(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file2);
	fwrite(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file2);
	fwrite(ClrTab, 1024, 1, file2);
	for (int i = 0; i < MtxHeight; i++)
	{
		fwrite(NewMtxBuf[i], 1, BmpWidth, file2);
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

	for (int i = 0; i < MtxHeight; i++)
	{
		delete[] NewMtxBuf[i];
		NewMtxBuf[i] = NULL;
	}
	delete[] NewMtxBuf;
	NewMtxBuf = NULL;

	pairs->clear();
	pairs->shrink_to_fit();
	delete pairs;

	return true;
}

bool addPair(std::vector<PtPair>* pairs, int s, int e)
{
	if (s == 255 || s == 0 || e == 255 || e == 0) return false;
	for (int i = 0; i < pairs->size(); i++)
	{
		if (pairs->at(i).sId == s)
		{
			if (pairs->at(i).eId == e) return true;
			else continue;
		}
	}
	PtPair tmp(s, e);
	pairs->push_back(tmp);
	return true;
}

bool LinkPts(const char * SrcBmpName, const char * outPtTxt, const char * outPairsTxt, const char* DestBmp)
{
	//open source file
	FILE *file1 = fopen(SrcBmpName, "rb");

	//load bmp file
	BITMAPFILEHEADER BmpFileHeader;
	BITMAPINFOHEADER BmpInfoHeader;
	unsigned char ClrTab[256 * 4];// color table : r + b + g + alpha
	fread(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file1);
	fread(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file1);
	fread(ClrTab, 1024, 1, file1);
	unsigned BmpWidth, MtxHeight, MtxWidth;
	MtxWidth = BmpInfoHeader.biWidth;
	BmpWidth = (MtxWidth + 3) / 4 * 4;
	MtxHeight = BmpInfoHeader.biHeight;

	//read matrix
	unsigned char * LineBuf = new unsigned char[BmpWidth];//read data line buffer
	unsigned char ** MtxBuf = new unsigned char *[MtxHeight];
	for (int i = 0; i < MtxHeight; i++)
	{
		MtxBuf[i] = new unsigned char[MtxWidth];
		fread(LineBuf, sizeof(unsigned char), BmpWidth, file1);
		for (int j = 0; j < MtxWidth; j++)
		{
			MtxBuf[i][j] = LineBuf[j];
		}
	}

	//read txt
	std::vector<PtPair> *pairs = new std::vector<PtPair>;
	std::vector<Pt> *pts = new std::vector<Pt>;

	FILE *ptFp = fopen(outPtTxt, "rb");
	int color=-1, x=-1, y=-1;
	while (!feof(ptFp))
	{
		fscanf(ptFp, "%d %d %d", &color, &x, &y);
		Pt tmp(color, x, y);
		pts->push_back(tmp);
	}
	fclose(ptFp);

	FILE *pairFp = fopen(outPairsTxt, "rb");
	int s=-1,e=-1;
	while (!feof(pairFp))
	{
		fscanf(pairFp, "%d %d", &s, &e);
		PtPair tmp(s,e);
		pairs->push_back(tmp);
	}
	fclose(pairFp);

	//wirte matrix
	unsigned char ** NewMtxBuf = new unsigned char *[MtxHeight];
	for (int i = 0; i < MtxHeight; i++)
	{
		NewMtxBuf[i] = new unsigned char[BmpWidth];
		for (int j = 0; j < MtxWidth; j++)
		{
			NewMtxBuf[i][j] = 0xff;
		}
	}

	for (int i = 0; i < pairs->size(); i++) 
	{
		ddaLine(pts, NewMtxBuf, pairs->at(i).sId, pairs->at(i).eId);
	}

	//write file
	FILE *file2 = fopen(DestBmp, "wb");
	fwrite(&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file2);
	fwrite(&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file2);
	fwrite(ClrTab, 1024, 1, file2);
	for (int i = 0; i < MtxHeight; i++)
	{
		fwrite(NewMtxBuf[i], 1, BmpWidth, file2);
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

	for (int i = 0; i < MtxHeight; i++)
	{
		delete[] NewMtxBuf[i];
		NewMtxBuf[i] = NULL;
	}
	delete[] NewMtxBuf;
	NewMtxBuf = NULL;

	pairs->clear();
	pairs->shrink_to_fit();
	delete pairs;

	pts->clear();
	pts->shrink_to_fit();
	delete pts;

	return true;
}

bool ddaLine(std::vector<Pt> *pts, unsigned char ** NewMtxBuf, int sId, int eId)
{
	int xa=-1, ya=-1, xb=-1, yb=-1;
	for (int i = 0; i < pts->size(); i++)
	{
		if (pts->at(i).color == sId)
		{
			xa = pts->at(i).x;
			ya = pts->at(i).y;
		}
		if (pts->at(i).color == eId)
		{
			xb = pts->at(i).x;
			yb = pts->at(i).y;
		}
	}
	int dx = xb - xa;                            
	int dy = yb - ya;                             
	int n;
	float xinc, yinc, x, y;
	if (abs(dx) > abs(dy))                         
	{
		n = abs(dx);
	}
	else
	{
		n = abs(dy);
	}
	xinc = (float)dx/n;         
	yinc = (float)dy/n;         
	x = (float) xa; y = (float) ya;
	NewMtxBuf[xa][ya] = 0;
	for(int k = 1; k <= n; k++)
	{
		x += xinc;                          
		y += yinc;  
		NewMtxBuf[(int)x][(int)y] = 0;
	}
	NewMtxBuf[xb][yb] = 0;
	return true;
}
