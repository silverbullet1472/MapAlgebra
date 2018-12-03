#include "MapAlgebraFunc.h"
#include <stdio.h>
#include <windows.h>
#include <assert.h>
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
	unsigned MtxWidth, MtxHeight, BmpWidth;
	BmpWidth = BmpInfoHeader.biWidth;
	MtxWidth = (BmpWidth + 3) / 4 * 4;
	// Windows规定一个扫描行所占的字节数必须是4 的倍数 ( 即以 long 为单位 ), 不足的以 0 填充，具体数据举例：
	// 也就是说，写入图片一行的像素信息的时候，每一行的字节数都必须是4的倍数，不足的在后面补0，然后才又开始写入下一行的像素信息
	// 在本函数中，每个像素用 1 byte = 8 bit来表示颜色信息
	MtxHeight = BmpInfoHeader.biHeight;

	//read matrix
	unsigned char * LineBuf = new unsigned char[MtxWidth];//read data line buffer
	unsigned char ** MtxBuf = new unsigned char *[MtxHeight];
	for (int i = 0; i < MtxHeight; i++)
	{
		MtxBuf[i] = new unsigned char[MtxWidth];
		for (int j = 0; j < MtxWidth; j++)
		{
			MtxBuf[i][j] = 0xFF; // 0xFF = 1111 1111 = 255
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

	unsigned MtxWidth, MtxHeight, BmpWidth;
	BmpWidth = BmpInfoHeader.biWidth;
	MtxWidth = (BmpWidth + 3) / 4 * 4;
	// Windows规定一个扫描行所占的字节数必须是4 的倍数 ( 即以 long 为单位 ), 不足的以 0 填充，具体数据举例：
	// 也就是说，写入图片一行的像素信息的时候，每一行的字节数都必须是4的倍数，不足的在后面补0，然后才又开始写入下一行的像素信息
	// 在本函数中，每个像素用 1 byte = 8 bit来表示颜色信息
	MtxHeight = BmpInfoHeader.biHeight;

	//read matrix
	unsigned char * LineBuf1 = new unsigned char[MtxWidth];//read data line buffer
	unsigned char * LineBuf2 = new unsigned char[MtxWidth];//read data line buffer
	unsigned char ** MtxBuf = new unsigned char *[MtxHeight];
	for (int i = 0; i < MtxHeight; i++)
	{
		MtxBuf[i] = new unsigned char[MtxWidth];
		for (int j = 0; j < MtxWidth; j++)
		{
			MtxBuf[i][j] = 0xFF; // 0xFF = 1111 1111 = 255
		}
	}

	//reverse color
	for (int i = 0; i < MtxHeight; i++)
	{
		fread(LineBuf1, sizeof(unsigned char), MtxWidth, file1);
		fread(LineBuf2, sizeof(unsigned char), MtxWidth, file2);
		for (int j = 0; j < MtxWidth; j++)
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
		fwrite(MtxBuf[i], 1, MtxWidth, file3);
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

bool BmpFocal(const char * SrcBmpName, const char * DestBmpName)
{
	return false;
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


bool DisTransform(const char* SrcBmpName, CDisTmp *pDisTmp) {

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
			if (LocMtx[i][j] == 0xFF)
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
			float tmpMin = DisMtx[y][x] + 0;
			if (fabs(tmpMin) < MinFloat) continue;
			for (int k = 0; k < pDisTmp->GetSize() / 2; k++)
			{
				char OffX = pDisTmp->GetOffX(k);
				char OffY = pDisTmp->GetOffY(k);
				float tempDis = pDisTmp->GetDis(k);
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
				float tempDis = pDisTmp->GetDis(k);
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
	FILE * bmpWrite1 = fopen("Distance.bmp", "wb");
	fwrite(&IdxFileHead, sizeof(BITMAPFILEHEADER), 1, bmpWrite1);
	fwrite(&IdxFileInfo, sizeof(BITMAPINFOHEADER), 1, bmpWrite1);
	for (int y = 0; y < MtxHeight; y++)
	{
		fwrite(DisMtx[y], sizeof(float), MtxWidth, bmpWrite1);
	}
	fclose(bmpWrite1);

	// 5.2 write distribution file 8位unsigned char型
	FILE * bmpWrite2 = fopen("Distribution.bmp", "wb");
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