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
	// Windows�涨һ��ɨ������ռ���ֽ���������4 �ı��� ( ���� long Ϊ��λ ), ������� 0 ��䣬�������ݾ�����
	// Ҳ����˵��д��ͼƬһ�е�������Ϣ��ʱ��ÿһ�е��ֽ�����������4�ı�����������ں��油0��Ȼ����ֿ�ʼд����һ�е�������Ϣ
	// �ڱ������У�ÿ�������� 1 byte = 8 bit����ʾ��ɫ��Ϣ
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
	// Windows�涨һ��ɨ������ռ���ֽ���������4 �ı��� ( ���� long Ϊ��λ ), ������� 0 ��䣬�������ݾ�����
	// Ҳ����˵��д��ͼƬһ�е�������Ϣ��ʱ��ÿһ�е��ֽ�����������4�ı�����������ں��油0��Ȼ����ֿ�ʼд����һ�е�������Ϣ
	// �ڱ������У�ÿ�������� 1 byte = 8 bit����ʾ��ɫ��Ϣ
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
	unsigned i, j; // ѭ������
	// 1 ����Դ�ļ�
	assert(SourceFileName != NULL);

	FILE* SrcFileStream = fopen(SourceFileName, "rb");
	if (SrcFileStream == NULL) return -1; // �������-1���ļ�������

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

	if (SrcFileInfo.biBitCount != 8) // 256ɫͼ����
	{
		fclose(SrcFileStream);
		return -2; // �������2���ļ���ʽ����Ҫ��
	}


	// 2 ����ʵ������
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
	int bfType; // λͼ�ļ������ͣ�����Ϊ ' B '' M '������ĸ (0-1�ֽ� ) ��
	int bfSize; // λͼ�ļ��Ĵ�С�����ֽ�Ϊ��λ (2-5 �ֽ� ) ��
	int bfReserved1; // λͼ�ļ������֣�����Ϊ 0(6-7 �ֽ� ) ����
	int bfReserved2; // λͼ�ļ������֣�����Ϊ 0(8-9 �ֽ� ) ��
	int bfOffBits; // λͼ���ݵ���ʼλ�ã��������λͼ (10-13 �ֽ� ) ��
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
	int Size; // ���ṹ��ռ���ֽ��� (14-17 �ֽ� ) ��
	int image_width; // λͼ�Ŀ�ȣ�������Ϊ��λ (18-21 �ֽ� ) ����
	int image_heigh; // λͼ�ĸ߶ȣ�������Ϊ��λ (22-25 �ֽ� ) ��
	int Planes; // Ŀ���豸�ļ��𣬱���Ϊ 1(26-27 �ֽ� ) ����
	int biBitCount;// ÿ�����������λ���������ǻ�1,4,8 24(// ���ɫ ) ֮һ����(28-29 �ֽ�)
	int biCompression; // λͼѹ�����ͣ������� 0( ��ѹ�� ), 1(BI_RLE8 ѹ������ ) ��// 2(BI_RLE4 ѹ������ ) ֮һ����(30-33 �ֽ� )
	int SizeImage; // λͼ���ݵĴ�С�����ֽ�Ϊ��λ (34-37 �ֽ� ) ����
	int biXPelsPerMeter; // λͼˮƽ�ֱ��ʣ�ÿ�������� (38-41 �ֽ� ) ����
	int biYPelsPerMeter; // λͼ��ֱ�ֱ��ʣ�ÿ�������� (42-45 �ֽ� ) ����
	int biClrUsed;// λͼʵ��ʹ�õ���ɫ���е���ɫ�� (46-49 �ֽ� ) ����
	int biClrImportant;// λͼ��ʾ��������Ҫ����ɫ�� (50-53 �ֽ� )
	*/


	// 2.1 ����ԭʼ����
	unsigned char * pSrcCellLineBuf = new unsigned char[LineBytes];   // Դ�ļ��л���
	unsigned **pIdxCellMatixBuf = new unsigned*[IdxFileInfo.biHeight]; //��������
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
				//  ȡ��Ӧ��ɫ

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
	if (ObjIdxFile == NULL) return -3; // �������3��ʵ�����������ļ��޷�����
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
	return 1; // ���سɹ�����1
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

	// 5.2 write distribution file 8λunsigned char��
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