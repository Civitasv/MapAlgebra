#include "MAUtil.h"

unsigned char** MAUtil::readBmp(const char* sourceFileName)
{
	// ѭ������
	unsigned int i, j;
	// �ļ�·������Ϊ��
	assert(sourceFileName != NULL);
	// ��Դ�ļ�
	FILE* sourceFileStream;
	fopen_s(&sourceFileStream, sourceFileName, "rb");
	// �ļ������ڣ������
	if (!sourceFileStream)
		return 0;
	// ͷ�ļ�
	BITMAPFILEHEADER sourceFileHead;
	// ��Ϣ�ļ�
	BITMAPINFOHEADER sourceFileInfo;
	// ��ɫ��
	unsigned char colorIdx[1024];
	// ��ȡ����
	if (fread(&sourceFileHead, sizeof(BITMAPFILEHEADER), 1, sourceFileStream) != 1)
	{
		fclose(sourceFileStream);
		return 0;
	}
	if (fread(&sourceFileInfo, sizeof(BITMAPINFOHEADER), 1, sourceFileStream) != 1)
	{
		fclose(sourceFileStream);
		return 0;
	}
	if (fread(colorIdx, 1024, 1, sourceFileStream) != 1)
	{
		fclose(sourceFileStream);
		return 0;
	}
	if (sourceFileInfo.biBitCount != 8) { // �������256ɫͼ
		fclose(sourceFileStream);
		return 0;
	}
	int fileWidth = (sourceFileInfo.biWidth + 3) / 4 * 4;
	// Դ�ļ����ݻ���
	unsigned char** pSrcCellBuf = new unsigned char* [sourceFileInfo.biHeight];
	int size = 3;
	AverageFilter average(size);// 3*3��ֵ�˲���
	for (unsigned i = 0; i < sourceFileInfo.biHeight; i++)// ��������
	{
		pSrcCellBuf[i] = new unsigned char[fileWidth];
		fread(pSrcCellBuf[i], 1, fileWidth, sourceFileStream);
	}

	fclose(sourceFileStream);

	return pSrcCellBuf;
}

bool MAUtil::inverseBmp(const char* sourceFileName, const char* destFileName) {
	// ѭ������
	unsigned int i, j;
	// �ļ�·������Ϊ��
	assert(sourceFileName != NULL);
	// ��Դ�ļ�
	FILE* sourceFileStream;
	fopen_s(&sourceFileStream, sourceFileName, "rb");
	// �ļ������ڣ������
	if (!sourceFileStream)
		return false;
	// ͷ�ļ�
	BITMAPFILEHEADER sourceFileHead;
	// ��Ϣ�ļ�
	BITMAPINFOHEADER sourceFileInfo;
	// ��ɫ��
	unsigned char colorIdx[1024];
	// ��ȡ����
	if (fread(&sourceFileHead, sizeof(BITMAPFILEHEADER), 1, sourceFileStream) != 1)
	{
		fclose(sourceFileStream);
		return false;
	}
	if (fread(&sourceFileInfo, sizeof(BITMAPINFOHEADER), 1, sourceFileStream) != 1)
	{
		fclose(sourceFileStream);
		return false;
	}
	if (fread(colorIdx, 1024, 1, sourceFileStream) != 1)
	{
		fclose(sourceFileStream);
		return false;
	}
	if (sourceFileInfo.biBitCount != 8) { // �������256ɫͼ
		fclose(sourceFileStream);
		return false;
	}
	int lineBytes = (sourceFileInfo.biWidth + 3) / 4 * 4;
	// Դ�ļ��л���
	unsigned char* pSrcCellLineBuf = new unsigned char[lineBytes];
	unsigned char** pDestCellBuf = new unsigned char* [sourceFileInfo.biHeight];
	for (i = 0; i < sourceFileInfo.biHeight; i++)
	{
		fseek(sourceFileStream, sourceFileHead.bfOffBits + i * sizeof(char) * lineBytes, 0);
		fread(pSrcCellLineBuf, sizeof(char), lineBytes, sourceFileStream);
		pDestCellBuf[i] = new unsigned char[lineBytes];
		for (j = 0; j < lineBytes; j++) {
			pDestCellBuf[i][j] = 255 - pSrcCellLineBuf[j];
		}
	}
	/*--------------------����ļ�----------------------------*/
	FILE* DestFile = fopen(destFileName, "wb");
	if (DestFile == NULL) return false; // ʵ�����������ļ��޷�����
	fwrite(&sourceFileHead, sizeof(BITMAPFILEHEADER), 1, DestFile);
	fwrite(&sourceFileInfo, sizeof(BITMAPINFOHEADER), 1, DestFile);
	fwrite(colorIdx, 1024, 1, DestFile);

	for (i = 0; i < sourceFileInfo.biHeight; i++)
	{
		fwrite(pDestCellBuf[i], sizeof(unsigned char), lineBytes, DestFile);
		delete[] pDestCellBuf[i];
		pDestCellBuf[i] = NULL;
	}

	delete[] pDestCellBuf;
	delete[] pSrcCellLineBuf;

	fclose(DestFile);
	fclose(sourceFileStream);

	return true; // ���سɹ�
}

bool MAUtil::overlayAnalysis(const char* sourceFileName1, const char* sourceFileName2, const char* destFileName)
{
	// ѭ������
	unsigned int i, j;
	// �ļ�·������Ϊ��
	assert(sourceFileName1 != NULL);
	assert(sourceFileName2 != NULL);
	// ��Դ�ļ�
	FILE* sourceFileStream1;
	FILE* sourceFileStream2;
	fopen_s(&sourceFileStream1, sourceFileName1, "rb");
	fopen_s(&sourceFileStream2, sourceFileName2, "rb");
	// �ļ������ڣ������
	if (!sourceFileStream1)
		return false;
	if (!sourceFileStream2)
		return false;
	// ͷ�ļ�
	BITMAPFILEHEADER sourceFileHead1;
	// ��Ϣ�ļ�
	BITMAPINFOHEADER sourceFileInfo1;
	// ͷ�ļ�
	BITMAPFILEHEADER sourceFileHead2;
	// ��Ϣ�ļ�
	BITMAPINFOHEADER sourceFileInfo2;
	// ��ɫ��
	unsigned char colorIdx[1024];
	unsigned char colorIdx2[1024];
	// ��ȡ����
	if (fread(&sourceFileHead1, sizeof(BITMAPFILEHEADER), 1, sourceFileStream1) != 1)
	{
		fclose(sourceFileStream1);
		return false;
	}
	if (fread(&sourceFileInfo1, sizeof(BITMAPINFOHEADER), 1, sourceFileStream1) != 1)
	{
		fclose(sourceFileStream1);
		return false;
	}
	if (fread(colorIdx, 1024, 1, sourceFileStream1) != 1)
	{
		fclose(sourceFileStream1);
		return false;
	}
	if (sourceFileInfo1.biBitCount != 8) { // �������256ɫͼ
		fclose(sourceFileStream1);
		return false;
	}
	int fileWidth1 = (sourceFileInfo1.biWidth + 3) / 4 * 4;
	// Դ�ļ����ݻ���
	unsigned char** pSrcCellBuf1 = new unsigned char* [sourceFileInfo1.biHeight];

	// ��ȡ����
	if (fread(&sourceFileHead2, sizeof(BITMAPFILEHEADER), 1, sourceFileStream2) != 1)
	{
		fclose(sourceFileStream2);
		return false;
	}
	if (fread(&sourceFileInfo2, sizeof(BITMAPINFOHEADER), 1, sourceFileStream2) != 1)
	{
		fclose(sourceFileStream2);
		return false;
	}
	if (fread(colorIdx2, 1024, 1, sourceFileStream2) != 1)
	{
		fclose(sourceFileStream2);
		return false;
	}
	if (sourceFileInfo2.biBitCount != 8) { // �������256ɫͼ
		fclose(sourceFileStream2);
		return false;
	}
	int fileWidth2 = (sourceFileInfo2.biWidth + 3) / 4 * 4;
	// Դ�ļ����ݻ���
	unsigned char** pSrcCellBuf2 = new unsigned char* [sourceFileInfo2.biHeight];
	// �������ͼƬ��С��һ����fail
	if (sourceFileInfo2.biWidth != sourceFileInfo1.biWidth || sourceFileInfo2.biHeight != sourceFileInfo1.biHeight) {
		return false;
	}

	unsigned char** pDestCellBuf = new unsigned char* [sourceFileInfo1.biHeight];
	for (unsigned i = 0; i < sourceFileInfo1.biHeight; i++)// ��������
	{
		pSrcCellBuf1[i] = new unsigned char[fileWidth1];
		pSrcCellBuf2[i] = new unsigned char[fileWidth2];
		fread(pSrcCellBuf1[i], 1, fileWidth1, sourceFileStream1);
		fread(pSrcCellBuf2[i], 1, fileWidth2, sourceFileStream2);
		pDestCellBuf[i] = new unsigned char[fileWidth1];
	}
	for (unsigned i = 0; i < sourceFileInfo1.biHeight; i++)// ��������
	{	// ��ֵ�˲���ʼ
		for (unsigned j = 0; j < fileWidth1; j++) {//������
			int sum = 0;
			int flag = 1;
			pDestCellBuf[i][j] = pSrcCellBuf1[i][j] * 0.5 + pSrcCellBuf2[i][j] * 0.5;
		}
	}


	/*--------------------����ļ�----------------------------*/
	FILE* DestFile = fopen(destFileName, "wb");
	if (DestFile == NULL) return false; // ʵ�����������ļ��޷�����
	fwrite(&sourceFileHead1, sizeof(BITMAPFILEHEADER), 1, DestFile);
	fwrite(&sourceFileInfo1, sizeof(BITMAPINFOHEADER), 1, DestFile);
	fwrite(colorIdx, 1024, 1, DestFile);
	// д�� and ����
	for (i = 0; i < sourceFileInfo1.biHeight; i++)
	{
		fwrite(pDestCellBuf[i], sizeof(unsigned char), fileWidth1, DestFile);
		delete[] pDestCellBuf[i];
		pDestCellBuf[i] = NULL;
	}
	for (i = 0; i < sourceFileInfo1.biHeight; i++)
	{
		delete[] pSrcCellBuf1[i];
		delete[] pSrcCellBuf2[i];
		pSrcCellBuf1[i] = NULL;
		pSrcCellBuf2[i] = NULL;
	}
	delete[] pDestCellBuf;
	delete[] pSrcCellBuf1;
	delete[] pSrcCellBuf2;

	fclose(DestFile);
	fclose(sourceFileStream1);
	fclose(sourceFileStream2);

	return true;
}

bool MAUtil::bmp256to32b(const char* sourceFileName, const char* idxFileName) {
	// ѭ������
	unsigned i, j;
	// �ļ�·������Ϊ��
	assert(sourceFileName != NULL);
	// ��Դ�ļ�
	FILE* SrcFileStream = fopen(sourceFileName, "rb");
	if (SrcFileStream == NULL) return false; // �ļ�������

	BITMAPFILEHEADER SrcFileHead;
	BITMAPINFOHEADER SrcFileInfo;
	unsigned char ColorIdx[1024];

	if (fread(&SrcFileHead, sizeof(BITMAPFILEHEADER), 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return false;
	}
	if (fread(&SrcFileInfo, sizeof(BITMAPINFOHEADER), 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return false;
	}
	if (fread(ColorIdx, 1024, 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return false;
	}

	if (SrcFileInfo.biBitCount != 8) // 256ɫͼ����
	{
		fclose(SrcFileStream);
		return false; // �ļ���ʽ����Ҫ��
	}

	// ����ʵ������
	BITMAPFILEHEADER IdxFileHead;
	BITMAPINFOHEADER IdxFileInfo;
	int LineBytes = (SrcFileInfo.biWidth + 3) / 4 * 4;
	int LineBytes2 = SrcFileInfo.biWidth;

	IdxFileHead.bfType = SrcFileHead.bfType;
	// 54Ϊ�ļ�ͷ+��Ϣͷ
	IdxFileHead.bfSize = 54 + LineBytes2 * 4 * SrcFileInfo.biHeight;
	IdxFileHead.bfReserved1 = 0;
	IdxFileHead.bfReserved2 = 0;
	IdxFileHead.bfOffBits = 54;

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


	// ����ԭʼ����
	unsigned char* pSrcCellLineBuf = new unsigned char[LineBytes];   // Դ�ļ��л���
	unsigned** pIdxCellMatixBuf = new unsigned* [IdxFileInfo.biHeight]; //��������
	for (i = 0; i < IdxFileInfo.biHeight; i++)
	{
		fseek(SrcFileStream, SrcFileHead.bfOffBits + i * sizeof(char) * LineBytes, 0);
		fread(pSrcCellLineBuf, sizeof(char), LineBytes, SrcFileStream);
		pIdxCellMatixBuf[i] = new unsigned[LineBytes2];
		for (j = 0; j < LineBytes2; j++)
			// ����ǰ�ɫ
			if (pSrcCellLineBuf[j] == 0xFF) pIdxCellMatixBuf[i][j] = 0xFFFFFFFF;
			else
			{
				int k = pSrcCellLineBuf[j];
				unsigned uintTmp = k;
				/*  ȡ��Ӧ��ɫ*/
				uintTmp = ColorIdx[k * 4 + 3];
				uintTmp <<= 8;
				uintTmp += ColorIdx[k * 4 + 2];
				uintTmp <<= 8;
				uintTmp += ColorIdx[k * 4 + 1];
				uintTmp <<= 8;
				uintTmp += ColorIdx[k * 4];

				pIdxCellMatixBuf[i][j] = uintTmp;
			}
	}

	FILE* ObjIdxFile = fopen(idxFileName, "wb");
	if (ObjIdxFile == NULL) return false; // ʵ�����������ļ��޷�����
	fseek(ObjIdxFile, 0, 0);
	fwrite(&IdxFileHead, sizeof(BITMAPFILEHEADER), 1, ObjIdxFile);
	fwrite(&IdxFileInfo, sizeof(BITMAPINFOHEADER), 1, ObjIdxFile);
	for (i = 0; i < IdxFileInfo.biHeight; i++)
	{
		fseek(ObjIdxFile, 54 + i * sizeof(unsigned) * LineBytes2, 0);
		fwrite(pIdxCellMatixBuf[i], sizeof(unsigned), LineBytes2, ObjIdxFile);
		delete[] pIdxCellMatixBuf[i];
		pIdxCellMatixBuf[i] = NULL;
	}

	delete[] pIdxCellMatixBuf;
	delete[] pSrcCellLineBuf;
	fclose(ObjIdxFile);
	fclose(SrcFileStream);
	return true;
}

bool MAUtil::averageFilter(const char* sourceFileName, const char* destFileName) {
	// ѭ������
	unsigned int i, j;
	// �ļ�·������Ϊ��
	assert(sourceFileName != NULL);
	// ��Դ�ļ�
	FILE* sourceFileStream;
	fopen_s(&sourceFileStream, sourceFileName, "rb");
	// �ļ������ڣ������
	if (!sourceFileStream)
		return false;
	// ͷ�ļ�
	BITMAPFILEHEADER sourceFileHead;
	// ��Ϣ�ļ�
	BITMAPINFOHEADER sourceFileInfo;
	// ��ɫ��
	unsigned char colorIdx[1024];
	// ��ȡ����
	if (fread(&sourceFileHead, sizeof(BITMAPFILEHEADER), 1, sourceFileStream) != 1)
	{
		fclose(sourceFileStream);
		return false;
	}
	if (fread(&sourceFileInfo, sizeof(BITMAPINFOHEADER), 1, sourceFileStream) != 1)
	{
		fclose(sourceFileStream);
		return false;
	}
	if (fread(colorIdx, 1024, 1, sourceFileStream) != 1)
	{
		fclose(sourceFileStream);
		return false;
	}
	if (sourceFileInfo.biBitCount != 8) { // �������256ɫͼ
		fclose(sourceFileStream);
		return false;
	}
	int fileWidth = (sourceFileInfo.biWidth + 3) / 4 * 4;
	// Դ�ļ����ݻ���
	unsigned char** pSrcCellBuf = new unsigned char* [sourceFileInfo.biHeight];
	unsigned char** pDestCellBuf = new unsigned char* [sourceFileInfo.biHeight];
	int size = 3;
	AverageFilter average(size);// 3*3��ֵ�˲���
	for (unsigned i = 0; i < sourceFileInfo.biHeight; i++)// ��������
	{
		pSrcCellBuf[i] = new unsigned char[fileWidth];
		fread(pSrcCellBuf[i], 1, fileWidth, sourceFileStream);
		pDestCellBuf[i] = new unsigned char[fileWidth];
	}
	for (unsigned i = 0; i < sourceFileInfo.biHeight; i++)// ��������
	{	// ��ֵ�˲���ʼ
		for (unsigned j = 0; j < fileWidth; j++) {//������
			int sum = 0;
			int flag = 1;
			for (int k = 0; k < average.Size(); k++) {
				char offX = average.getOffX(k);
				char offY = average.getOffY(k);
				int CurrX = j + offX;              // ����ģ��Ԫ�ض�Ӧ��դ��λ��
				int CurrY = i + offY;
				// �жϸ�λ���Ƿ�Խ��
				if ((CurrX < 0) || (CurrX > sourceFileInfo.biWidth - 1)) {
					flag = -1;
					break;
				}
				if ((CurrY < 0) || (CurrY > sourceFileInfo.biHeight - 1)) {
					flag = -1;
					break;
				}
				sum += pSrcCellBuf[CurrY][CurrX];
			}
			if (flag == 1) {
				pDestCellBuf[i][j] = sum / average.Size();
			}
			else {
				pDestCellBuf[i][j] = pSrcCellBuf[i][j];
			}
		}
	}


	/*--------------------����ļ�----------------------------*/
	FILE* DestFile = fopen(destFileName, "wb");
	if (DestFile == NULL) return false; // ʵ�����������ļ��޷�����
	fwrite(&sourceFileHead, sizeof(BITMAPFILEHEADER), 1, DestFile);
	fwrite(&sourceFileInfo, sizeof(BITMAPINFOHEADER), 1, DestFile);
	fwrite(colorIdx, 1024, 1, DestFile);
	// д�� and ����
	for (i = 0; i < sourceFileInfo.biHeight; i++)
	{
		fwrite(pDestCellBuf[i], sizeof(unsigned char), fileWidth, DestFile);
		delete[] pDestCellBuf[i];
		pDestCellBuf[i] = NULL;
	}
	for (i = 0; i < sourceFileInfo.biHeight; i++)
	{
		delete[] pSrcCellBuf[i];
		pSrcCellBuf[i] = NULL;
	}
	delete[] pDestCellBuf;
	delete[] pSrcCellBuf;

	fclose(DestFile);
	fclose(sourceFileStream);

	return true; // ���سɹ�
}

bool MAUtil::midFilter(const char* sourceFileName, const char* destFileName) {
	// ѭ������
	unsigned int i, j;
	// �ļ�·������Ϊ��
	assert(sourceFileName != NULL);
	// ��Դ�ļ�
	FILE* sourceFileStream;
	fopen_s(&sourceFileStream, sourceFileName, "rb");
	// �ļ������ڣ������
	if (!sourceFileStream)
		return false;
	// ͷ�ļ�
	BITMAPFILEHEADER sourceFileHead;
	// ��Ϣ�ļ�
	BITMAPINFOHEADER sourceFileInfo;
	// ��ɫ��
	unsigned char colorIdx[1024];
	// ��ȡ����
	if (fread(&sourceFileHead, sizeof(BITMAPFILEHEADER), 1, sourceFileStream) != 1)
	{
		fclose(sourceFileStream);
		return false;
	}
	if (fread(&sourceFileInfo, sizeof(BITMAPINFOHEADER), 1, sourceFileStream) != 1)
	{
		fclose(sourceFileStream);
		return false;
	}
	if (fread(colorIdx, 1024, 1, sourceFileStream) != 1)
	{
		fclose(sourceFileStream);
		return false;
	}
	if (sourceFileInfo.biBitCount != 8) { // �������256ɫͼ
		fclose(sourceFileStream);
		return false;
	}
	int fileWidth = (sourceFileInfo.biWidth + 3) / 4 * 4;
	// Դ�ļ����ݻ���
	unsigned char** pSrcCellBuf = new unsigned char* [sourceFileInfo.biHeight];
	unsigned char** pDestCellBuf = new unsigned char* [sourceFileInfo.biHeight];
	int size = 3;
	for (unsigned i = 0; i < sourceFileInfo.biHeight; i++)// ��������
	{
		pSrcCellBuf[i] = new unsigned char[fileWidth];
		fread(pSrcCellBuf[i], 1, fileWidth, sourceFileStream);
		pDestCellBuf[i] = new unsigned char[fileWidth];
	}
	for (unsigned i = 0; i < sourceFileInfo.biHeight; i++)// ��������
	{	// 3*3��ֵ�˲���ʼ
		for (unsigned j = 0; j < fileWidth; j++) {//������
			vector<int> val;
			int flag = 1;
			for (int k = 0; k < size * size; k++) {
				char offX = k % size - size / 2;
				char offY = k / size - size / 2;
				int CurrX = j + offX;              // ����ģ��Ԫ�ض�Ӧ��դ��λ��
				int CurrY = i + offY;
				if ((CurrX < 0) || (CurrX > sourceFileInfo.biWidth - 1)) {
					flag = -1;
					break;  // �жϸ�λ���Ƿ�Խ��
				}
				if ((CurrY < 0) || (CurrY > sourceFileInfo.biHeight - 1)) {
					flag = -1;
					break;
				}
				val.push_back(pSrcCellBuf[CurrY][CurrX]);
			}
			if (flag == 1) {
				sort(val.begin(), val.end());
				int Dm = val.at(val.size() / 2);
				pDestCellBuf[i][j] = Dm;
			}
			else {
				pDestCellBuf[i][j] = pSrcCellBuf[i][j];
			}
		}
	}

	/*--------------------����ļ�----------------------------*/
	FILE* DestFile = fopen(destFileName, "wb");
	if (DestFile == NULL) return false; // ʵ�����������ļ��޷�����
	fwrite(&sourceFileHead, sizeof(BITMAPFILEHEADER), 1, DestFile);
	fwrite(&sourceFileInfo, sizeof(BITMAPINFOHEADER), 1, DestFile);
	fwrite(colorIdx, 1024, 1, DestFile);
	// д�� and ����
	for (i = 0; i < sourceFileInfo.biHeight; i++)
	{
		fwrite(pDestCellBuf[i], sizeof(unsigned char), fileWidth, DestFile);
		delete[] pDestCellBuf[i];
		pDestCellBuf[i] = NULL;
	}
	for (i = 0; i < sourceFileInfo.biHeight; i++)
	{
		delete[] pSrcCellBuf[i];
		pSrcCellBuf[i] = NULL;
	}
	delete[] pDestCellBuf;
	delete[] pSrcCellBuf;

	fclose(DestFile);
	fclose(sourceFileStream);

	return true; // ���سɹ�
}

bool MAUtil::distanceTransform(const char* sourceFileName, const char* destFileName1, const char* destFileName2, CDistanceTmp* templet)
{
	// 1. �������ģ��

	// 2. ��ȡԭʼλͼ��ȷ�����Ĵ�С���
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	char ColorTAB[1024];

	FILE* bmpFile = NULL;
	bmpFile = fopen(sourceFileName, "rb");
	// �ļ������ڣ������
	if (!bmpFile)
		return false;
	fseek(bmpFile, 0, SEEK_SET);

	// ��ȡλͼ�ļ�ͷ
	int kk = fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, bmpFile);
	// ��ȡλͼ�ļ���Ϣͷ
	kk = fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, bmpFile);
	// 256ɫ��ȡ��ɫ��
	kk = fread(ColorTAB, 1024, 1, bmpFile);

	unsigned Height, Width;
	// BMPͼ��ĸ߶ȣ���λ����
	Height = infoHeader.biHeight;
	// BMPͼ��Ŀ�ȣ���λ����
	Width = infoHeader.biWidth;
	// 
	unsigned widthFile = (Width + 3) / 4 * 4;

	// 3. ������ʼ ���볡�� ���䳡
	unsigned char** DisMatix = NULL;
	// ���䳡����
	unsigned char** LocMatix = NULL;

	LocMatix = new unsigned char* [Height];
	DisMatix = new unsigned char* [Height];
	for (unsigned i = 0; i < Height; i++)
	{
		LocMatix[i] = new unsigned char[widthFile];
		// 3.1 ���䳡����ֱ�Ӵ�λͼ��ȡ 0/255������ֵ/ǰ��ֵ
		fread(LocMatix[i], 1, widthFile, bmpFile);
		DisMatix[i] = new unsigned char[widthFile];
		// 3.2 ���ݷ��䳡��ʵ�����طֲ���ʼ�����볡
		for (unsigned j = 0; j < widthFile; j++)
			if (LocMatix[i][j] != 255) DisMatix[i][j] = 0;
			else DisMatix[i][j] = 255;// ��ɫΪ����������
	}
	fclose(bmpFile);

	// 4. ģ�����任
	// 4.1 �����µ����Ϸ���
	for (int y = 0; y < Height; y++) // 2��ѭ������ÿһ��դ��
	{
		for (int x = 0; x < widthFile; x++)
		{
			unsigned int tmpMin = DisMatix[y][x] + 0; // ����ģ����Сֵ��ʼ��
			if (tmpMin == 0) continue;             // Ϊ0����ʾ�ڵ��������
			for (unsigned k = 0; k < templet->Size() / 2; k++) // ���ϰ�ģ�����Ԫ��ѭ��
			{
				int offx = templet->getOffX(k); // k=0,offx=-6
				int offy = templet->getOffY(k);
				float tmpDis = templet->getTmpDis(k);     // ȥ��ģ��Ҫ�أ�x,yƫ�Ƽ���Ӧģ����Ծ���

				int CurrX = x + offx;              // ����ģ��Ԫ�ض�Ӧ��դ��λ��
				int CurrY = y + offy;
				if ((CurrX < 0) || (CurrX > Width - 1)) continue;  // �жϸ�λ���Ƿ�Խ��
				if ((CurrY < 0) || (CurrY > Height - 1)) continue;

				if (DisMatix[CurrY][CurrX] == 255) continue; // ��λ�����������󲻿ɼ��㣬ֻ���㱳��

				float dis1 = DisMatix[CurrY][CurrX] + tmpDis; // ��ǰդ�����+ģ����Ծ���
				if (dis1 < tmpMin)  // ����Сֵ
				{
					tmpMin = dis1;
					DisMatix[y][x] = dis1;                   // ��С��������������
					LocMatix[y][x] = LocMatix[CurrY][CurrX]; // ��С������Դ���������� �ڱ�+��ɫ
				}
			}
		}
	}
	// 4.2 �����ϵ����·���
	for (int y = Height - 1; y > -1; y--)
	{
		for (int x = Width - 1; x > -1; x--)
		{
			unsigned int tmpMin = DisMatix[y][x];
			if (tmpMin == 0) continue;
			for (unsigned k = templet->Size() / 2 + 1; k < templet->Size(); k++)
			{
				int offx = templet->getOffX(k);
				int offy = templet->getOffY(k);
				float tmpDis = templet->getTmpDis(k);

				int CurrX = x + offx;
				int CurrY = y + offy;
				if ((CurrX < 0) || (CurrX > Width - 1)) continue;
				if ((CurrY < 0) || (CurrY > Height - 1)) continue;

				if (DisMatix[CurrY][CurrX] == 255) continue;

				float dis1 = DisMatix[CurrY][CurrX] + tmpDis;
				if (dis1 < tmpMin)
				{
					tmpMin = dis1;
					DisMatix[y][x] = dis1;
					LocMatix[y][x] = LocMatix[CurrY][CurrX];
				}
			}
		}
	}


	// 5. д�����ļ�
	FILE* bmpWrite = fopen(destFileName1, "wb");
	fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, bmpWrite);
	fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, bmpWrite);
	fwrite(ColorTAB, 1024, 1, bmpWrite);
	for (int y = 0; y < Height; y++)
	{
		fwrite(DisMatix[y], 1, widthFile, bmpWrite);
	}
	fclose(bmpWrite);

	FILE* bmpWrite1 = fopen(destFileName2, "wb");
	fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, bmpWrite1);
	fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, bmpWrite1);
	fwrite(ColorTAB, 1024, 1, bmpWrite1);
	for (int y = 0; y < Height; y++)
	{
		fwrite(LocMatix[y], 1, widthFile, bmpWrite1);
	}
	fclose(bmpWrite1);



	// 6. �����ͷ��ڴ�
	for (unsigned j = 0; j < Height; j++)
	{
		delete[]LocMatix[j];
		delete[]DisMatix[j];
	}
	delete[]DisMatix;
	delete[]LocMatix;


	return true;
}

bool MAUtil::distanceTransformIn(const char* sourceFileName, const char* destFileName1, const char* destFileName2, CDistanceTmp* templet)
{
	// 1. �������ģ��

	// 2. ��ȡԭʼλͼ��ȷ�����Ĵ�С���
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	char ColorTAB[1024];

	FILE* bmpFile = NULL;
	bmpFile = fopen(sourceFileName, "rb");
	// �ļ������ڣ������
	if (!bmpFile)
		return false;
	fseek(bmpFile, 0, SEEK_SET);

	// ��ȡλͼ�ļ�ͷ
	int kk = fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, bmpFile);
	// ��ȡλͼ�ļ���Ϣͷ
	kk = fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, bmpFile);
	// 256ɫ��ȡ��ɫ��
	kk = fread(ColorTAB, 1024, 1, bmpFile);

	unsigned Height, Width;
	// BMPͼ��ĸ߶ȣ���λ����
	Height = infoHeader.biHeight;
	// BMPͼ��Ŀ�ȣ���λ����
	Width = infoHeader.biWidth;
	// 
	unsigned widthFile = (Width + 3) / 4 * 4;

	// 3. ������ʼ ���볡�� ���䳡
	unsigned char** DisMatix = NULL;
	// ���䳡����
	unsigned char** LocMatix = NULL;

	LocMatix = new unsigned char* [Height];
	DisMatix = new unsigned char* [Height];
	for (unsigned i = 0; i < Height; i++)
	{
		LocMatix[i] = new unsigned char[widthFile];
		// 3.1 ���䳡����ֱ�Ӵ�λͼ��ȡ 0/255������ֵ/ǰ��ֵ
		fread(LocMatix[i], 1, widthFile, bmpFile);
		DisMatix[i] = new unsigned char[widthFile];
		// 3.2 ���ݷ��䳡��ʵ�����طֲ���ʼ�����볡
		for (unsigned j = 0; j < widthFile; j++)
			if (LocMatix[i][j] == 255) DisMatix[i][j] = 0;
			else DisMatix[i][j] = 255;// ��ɫΪ����������
	}
	fclose(bmpFile);

	// 4. ģ�����任
	// 4.1 �����µ����Ϸ���
	for (int y = 0; y < Height; y++) // 2��ѭ������ÿһ��դ��
	{
		for (int x = 0; x < widthFile; x++)
		{
			unsigned int tmpMin = DisMatix[y][x] + 0; // ����ģ����Сֵ��ʼ��
			if (tmpMin == 0) continue;             // Ϊ0����ʾ�ڵ��������
			for (unsigned k = 0; k < templet->Size() / 2; k++) // ���ϰ�ģ�����Ԫ��ѭ��
			{
				int offx = templet->getOffX(k); // k=0,offx=-6
				int offy = templet->getOffY(k);
				float tmpDis = templet->getTmpDis(k);     // ȥ��ģ��Ҫ�أ�x,yƫ�Ƽ���Ӧģ����Ծ���

				int CurrX = x + offx;              // ����ģ��Ԫ�ض�Ӧ��դ��λ��
				int CurrY = y + offy;
				if ((CurrX < 0) || (CurrX > Width - 1)) continue;  // �жϸ�λ���Ƿ�Խ��
				if ((CurrY < 0) || (CurrY > Height - 1)) continue;

				if (DisMatix[CurrY][CurrX] == 255) continue; // ��λ�����������󲻿ɼ��㣬ֻ���㱳��

				float dis1 = DisMatix[CurrY][CurrX] + tmpDis; // ��ǰդ�����+ģ����Ծ���
				if (dis1 < tmpMin)  // ����Сֵ
				{
					tmpMin = dis1;
					DisMatix[y][x] = dis1;                   // ��С��������������
					LocMatix[y][x] = LocMatix[CurrY][CurrX]; // ��С������Դ���������� �ڱ�+��ɫ
				}
			}
		}
	}
	// 4.2 �����ϵ����·���
	for (int y = Height - 1; y > -1; y--)
	{
		for (int x = Width - 1; x > -1; x--)
		{
			unsigned int tmpMin = DisMatix[y][x];
			if (tmpMin == 0) continue;
			for (unsigned k = templet->Size() / 2 + 1; k < templet->Size(); k++)
			{
				int offx = templet->getOffX(k);
				int offy = templet->getOffY(k);
				float tmpDis = templet->getTmpDis(k);

				int CurrX = x + offx;
				int CurrY = y + offy;
				if ((CurrX < 0) || (CurrX > Width - 1)) continue;
				if ((CurrY < 0) || (CurrY > Height - 1)) continue;

				if (DisMatix[CurrY][CurrX] == 255) continue;

				float dis1 = DisMatix[CurrY][CurrX] + tmpDis;
				if (dis1 < tmpMin)
				{
					tmpMin = dis1;
					DisMatix[y][x] = dis1;
					LocMatix[y][x] = LocMatix[CurrY][CurrX];
				}
			}
		}
	}


	// 5. д�����ļ�
	FILE* bmpWrite = fopen(destFileName1, "wb");
	fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, bmpWrite);
	fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, bmpWrite);
	fwrite(ColorTAB, 1024, 1, bmpWrite);
	for (int y = 0; y < Height; y++)
	{
		fwrite(DisMatix[y], 1, widthFile, bmpWrite);
	}
	fclose(bmpWrite);

	FILE* bmpWrite1 = fopen(destFileName2, "wb");
	fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, bmpWrite1);
	fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, bmpWrite1);
	fwrite(ColorTAB, 1024, 1, bmpWrite1);
	for (int y = 0; y < Height; y++)
	{
		fwrite(LocMatix[y], 1, widthFile, bmpWrite1);
	}
	fclose(bmpWrite1);



	// 6. �����ͷ��ڴ�
	for (unsigned j = 0; j < Height; j++)
	{
		delete[]LocMatix[j];
		delete[]DisMatix[j];
	}
	delete[]DisMatix;
	delete[]LocMatix;


	return true;
}

bool MAUtil::distanceTransform32b(const char* sourceFileName, const char* destFileName1, const char* destFileName2, CDistanceTmp* templet)
{
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	char ColorTAB[1024];

	FILE* bmpFile = NULL;
	bmpFile = fopen(sourceFileName, "rb");
	fseek(bmpFile, 0, SEEK_SET);

	fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, bmpFile);
	fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, bmpFile);
	fread(ColorTAB, 1024, 1, bmpFile);

	unsigned Height, Width;
	Height = infoHeader.biHeight;
	Width = infoHeader.biWidth;
	unsigned widthFile = (Width + 3) / 4 * 4;

	// 3. ������ʼ ���볡�� ���䳡
	unsigned char** LocMatix = NULL;
	LocMatix = new unsigned char* [Height];

	float** DisMatix = NULL;
	DisMatix = new float* [Height];
	for (unsigned i = 0; i < Height; i++)
	{
		LocMatix[i] = new unsigned char[widthFile];
		// 3.1 ���䳡����ֱ�Ӵ�λͼ��ȡ
		fread(LocMatix[i], 1, widthFile, bmpFile);
		DisMatix[i] = new float[Width];
		// 3.2 ���ݷ��䳡��ʵ�����طֲ���ʼ�����볡
		for (unsigned j = 0; j < Width; j++)
		{
			if (LocMatix[i][j] == 255) DisMatix[i][j] = MaxFloat;// ����󣬴�����
			else DisMatix[i][j] = 0;
		}
	}
	fclose(bmpFile);

	// 4. ģ�����任
	// 4.1 �����µ����Ϸ���
	for (int y = 0; y < Height; y++) // 2��ѭ������ÿһ��դ��
	{
		for (int x = 0; x < Width; x++)
		{
			float tmpMin = DisMatix[y][x] + 0; // ����ģ����Сֵ��ʼ��
			if (fabs(tmpMin) < MinFloat) continue;             // Ϊ0����ʾ�ڵ��������
			for (unsigned k = 0; k < templet->Size() / 2; k++) // ���ϰ�ģ�����Ԫ��ѭ��
			{
				char offx = templet->getOffX(k);
				char offy = templet->getOffY(k);
				float tmpDis = templet->getTmpDis(k);     // ȥ��ģ��Ҫ�أ�x,yƫ�Ƽ���Ӧģ����Ծ���

				int CurrX = x + offx;              // ����ģ��Ԫ�ض�Ӧ��դ��λ��
				int CurrY = y + offy;
				if ((CurrX < 0) || (CurrX > Width - 1)) continue;  // �жϸ�λ���Ƿ�Խ��
				if ((CurrY < 0) || (CurrY > Height - 1)) continue;

				if (fabs(DisMatix[CurrY][CurrX] - MaxFloat) < MinFloat) continue; // ��λ�����������󲻿ɼ���

				float dis1 = DisMatix[CurrY][CurrX] + tmpDis; // ��ǰդ�����+ģ����Ծ���
				if (dis1 < tmpMin)  // ����Сֵ
				{
					tmpMin = dis1;
					DisMatix[y][x] = dis1;                   // ��С��������������
					LocMatix[y][x] = LocMatix[CurrY][CurrX]; // ��С������Դ����������
				}
			}
		}
	}
	// 4.2 �����ϵ����·���
	for (int y = Height - 1; y > -1; y--)
	{
		for (int x = Width - 1; x > -1; x--)
		{
			float tmpMin = DisMatix[y][x] + 0; // ����ģ����Сֵ��ʼ��
			if (fabs(tmpMin) < MinFloat) continue;             // Ϊ0����ʾ�ڵ��������
			for (unsigned k = templet->Size() / 2 + 1; k < templet->Size(); k++)
			{
				char offx = templet->getOffX(k);
				char offy = templet->getOffY(k);
				float tmpDis = templet->getTmpDis(k);     // ȥ��ģ��Ҫ�أ�x,yƫ�Ƽ���Ӧģ����Ծ���

				int CurrX = x + offx;              // ����ģ��Ԫ�ض�Ӧ��դ��λ��
				int CurrY = y + offy;
				if ((CurrX < 0) || (CurrX > Width - 1)) continue;  // �жϸ�λ���Ƿ�Խ��
				if ((CurrY < 0) || (CurrY > Height - 1)) continue;

				if (fabs(DisMatix[CurrY][CurrX] - MaxFloat) < MinFloat) continue; // ��λ�����������󲻿ɼ���

				float dis1 = DisMatix[CurrY][CurrX] + tmpDis; // ��ǰդ�����+ģ����Ծ���
				if (dis1 < tmpMin)  // ����Сֵ
				{
					tmpMin = dis1;
					DisMatix[y][x] = dis1;                   // ��С��������������
					LocMatix[y][x] = LocMatix[CurrY][CurrX]; // ��С������Դ����������
				}
			}
		}
	}


	// 5. д�����ļ�
	// 5.1 д��������32λfloat��
	BITMAPFILEHEADER IdxFileHead; // 32λλͼͷ�ṹ
	BITMAPINFOHEADER IdxFileInfo;

	IdxFileHead.bfType = fileHeader.bfType;
	IdxFileHead.bfSize = 54 + infoHeader.biWidth * 4 * infoHeader.biHeight;
	IdxFileHead.bfReserved1 = 0;
	IdxFileHead.bfReserved2 = 0;
	IdxFileHead.bfOffBits = 54;

	IdxFileInfo.biSize = 40;
	IdxFileInfo.biWidth = infoHeader.biWidth;
	IdxFileInfo.biHeight = infoHeader.biHeight;
	IdxFileInfo.biPlanes = 1;
	IdxFileInfo.biBitCount = 32;
	IdxFileInfo.biCompression = 0;
	IdxFileInfo.biSizeImage = 0;
	IdxFileInfo.biXPelsPerMeter = 0;
	IdxFileInfo.biYPelsPerMeter = 0;
	IdxFileInfo.biClrUsed = 0;
	IdxFileInfo.biClrImportant = 0;

	FILE* bmpWrite = fopen(destFileName1, "wb");
	fwrite(&IdxFileHead, sizeof(BITMAPFILEHEADER), 1, bmpWrite);
	fwrite(&IdxFileInfo, sizeof(BITMAPINFOHEADER), 1, bmpWrite);
	for (int y = 0; y < Height; y++)
	{
		fwrite(DisMatix[y], sizeof(float), Width, bmpWrite);
	}
	fclose(bmpWrite);

	// 5.2 д����䳡����8λunsigned char��
	FILE* bmpWrite1 = fopen(destFileName2, "wb");
	fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, bmpWrite1);
	fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, bmpWrite1);
	fwrite(ColorTAB, 1024, 1, bmpWrite1);
	for (int y = 0; y < Height; y++)
	{
		fwrite(LocMatix[y], 1, widthFile, bmpWrite1);
	}
	fclose(bmpWrite1);



	// 6. �����ͷ��ڴ�
	for (unsigned j = 0; j < Height; j++)
	{
		delete[]LocMatix[j];
		delete[]DisMatix[j];
	}
	delete[]DisMatix;
	delete[]LocMatix;


	return true;
}

bool MAUtil::barrierDistanceTransform(const char* sourceFileName, const char* destFileName1, const char* destFileName2, CDistanceTmp* templet)
{
	// 1. �������ģ��

	// 2. ��ȡԭʼλͼ��ȷ�����Ĵ�С���
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	char ColorTAB[1024];

	FILE* bmpFile = NULL;
	bmpFile = fopen(sourceFileName, "rb");
	// �ļ������ڣ������
	if (!bmpFile)
		return false;
	fseek(bmpFile, 0, SEEK_SET);

	// ��ȡλͼ�ļ�ͷ
	int kk = fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, bmpFile);
	// ��ȡλͼ�ļ���Ϣͷ
	kk = fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, bmpFile);
	// 256ɫ��ȡ��ɫ��
	kk = fread(ColorTAB, 1024, 1, bmpFile);

	unsigned Height, Width;
	// BMPͼ��ĸ߶ȣ���λ����
	Height = infoHeader.biHeight;
	// BMPͼ��Ŀ�ȣ���λ����
	Width = infoHeader.biWidth;
	// 
	unsigned widthFile = (Width + 3) / 4 * 4;

	// 3. ������ʼ ���볡�� ���䳡
	unsigned char** DisMatix = NULL;
	// ���䳡����
	unsigned char** LocMatix = NULL;

	LocMatix = new unsigned char* [Height];
	DisMatix = new unsigned char* [Height];
	int barrierX = Height / 4;
	int barrierY = widthFile / 4;
	for (unsigned i = 0; i < Height; i++)
	{
		LocMatix[i] = new unsigned char[widthFile];
		// 3.1 ���䳡����ֱ�Ӵ�λͼ��ȡ 0/255������ֵ/ǰ��ֵ
		fread(LocMatix[i], 1, widthFile, bmpFile);
		DisMatix[i] = new unsigned char[widthFile];
		// 3.2 ���ݷ��䳡��ʵ�����طֲ���ʼ�����볡
		for (unsigned j = 0; j < widthFile; j++) {
			if (LocMatix[i][j] == 0)
				DisMatix[i][j] = 10000;
			else if (LocMatix[i][j] == 255)
				DisMatix[i][j] = 255;// ��ɫΪ������
			else
				DisMatix[i][j] = 0;
		}
	}
	fclose(bmpFile);

	// 4. ģ�����任
	// 4.1 �����µ����Ϸ���
	for (int y = 0; y < Height; y++) // 2��ѭ������ÿһ��դ��
	{
		for (int x = 0; x < widthFile; x++)
		{
			unsigned int tmpMin = DisMatix[y][x] + 0; // ����ģ����Сֵ��ʼ��
			if (tmpMin == 0) continue;             // Ϊ0����ʾ���任���ڲ����������
			if (tmpMin == 10000) continue; // �ϰ����������
			for (unsigned k = 0; k < templet->Size() / 2; k++) // ���ϰ�ģ�����Ԫ��ѭ��
			{
				int offx = templet->getOffX(k); // k=0,offx=-6
				int offy = templet->getOffY(k);
				float tmpDis = templet->getTmpDis(k);     // ȥ��ģ��Ҫ�أ�x,yƫ�Ƽ���Ӧģ����Ծ���

				int CurrX = x + offx;              // ����ģ��Ԫ�ض�Ӧ��դ��λ��
				int CurrY = y + offy;
				if ((CurrX < 0) || (CurrX > Width - 1)) continue;  // �жϸ�λ���Ƿ�Խ��
				if ((CurrY < 0) || (CurrY > Height - 1)) continue;

				if (DisMatix[CurrY][CurrX] == 255) continue; // ��λ�����������󲻿ɼ��㣬ֻ���㱳��
				if (DisMatix[CurrY][CurrX] == 10000) continue; // �ϰ��㲻����Ƚ�

				float dis1 = DisMatix[CurrY][CurrX] + tmpDis; // ��ǰդ�����+ģ����Ծ���
				if (dis1 < tmpMin)  // ����Сֵ
				{
					tmpMin = dis1;
					DisMatix[y][x] = dis1;                   // ��С��������������
					LocMatix[y][x] = LocMatix[CurrY][CurrX]; // ��С������Դ���������� �ڱ�+��ɫ
				}
			}
		}
	}
	// 4.2 �����ϵ����·���
	for (int y = Height - 1; y > -1; y--)
	{
		for (int x = Width - 1; x > -1; x--)
		{
			unsigned int tmpMin = DisMatix[y][x];
			if (tmpMin == 0) continue;
			if (tmpMin == 10000) continue; // �ϰ����������
			for (unsigned k = templet->Size() / 2 + 1; k < templet->Size(); k++)
			{
				int offx = templet->getOffX(k);
				int offy = templet->getOffY(k);
				float tmpDis = templet->getTmpDis(k);

				int CurrX = x + offx;
				int CurrY = y + offy;
				if ((CurrX < 0) || (CurrX > Width - 1)) continue;
				if ((CurrY < 0) || (CurrY > Height - 1)) continue;

				if (DisMatix[CurrY][CurrX] == 255) continue;
				if (DisMatix[CurrY][CurrX] == 10000) continue; // �ϰ��㲻����Ƚ�
				float dis1 = DisMatix[CurrY][CurrX] + tmpDis;
				if (dis1 < tmpMin)
				{
					tmpMin = dis1;
					DisMatix[y][x] = dis1;
					LocMatix[y][x] = LocMatix[CurrY][CurrX];
				}
			}
		}
	}

	// 5. д�����ļ�
	FILE* bmpWrite = fopen(destFileName1, "wb");
	fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, bmpWrite);
	fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, bmpWrite);
	fwrite(ColorTAB, 1024, 1, bmpWrite);
	for (int y = 0; y < Height; y++)
	{
		fwrite(DisMatix[y], 1, widthFile, bmpWrite);
	}
	fclose(bmpWrite);

	FILE* bmpWrite1 = fopen(destFileName2, "wb");
	fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, bmpWrite1);
	fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, bmpWrite1);
	fwrite(ColorTAB, 1024, 1, bmpWrite1);
	for (int y = 0; y < Height; y++)
	{
		fwrite(LocMatix[y], 1, widthFile, bmpWrite1);
	}
	fclose(bmpWrite1);



	// 6. �����ͷ��ڴ�
	for (unsigned j = 0; j < Height; j++)
	{
		delete[]LocMatix[j];
		delete[]DisMatix[j];
	}
	delete[]DisMatix;
	delete[]LocMatix;


	return true;
}

bool MAUtil::getBdyBmp(const char* LocFileName, const char* destFileName)
{
	unsigned i, j; // ѭ������

	FILE* SrcFileStream = fopen(LocFileName, "rb");
	if (SrcFileStream == NULL) return false;

	BITMAPFILEHEADER SrcFileHead;
	BITMAPINFOHEADER SrcFileInfo;
	unsigned char ColorIdx[1024];

	if (fread(&SrcFileHead, sizeof(BITMAPFILEHEADER), 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return false;
	}
	if (fread(&SrcFileInfo, sizeof(BITMAPINFOHEADER), 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return false;
	}
	if (fread(ColorIdx, 1024, 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return false;
	}

	if (SrcFileInfo.biBitCount != 8) // 256ɫͼ����
	{
		fclose(SrcFileStream);
		return false; // �ļ���ʽ����Ҫ��
	}


	//  ����ʵ������
	int LineBytes = (SrcFileInfo.biWidth + 3) / 4 * 4;


	//  ��ȡ���䳡����
	unsigned char** pSrcCellLineBuf = new unsigned char* [SrcFileInfo.biHeight];   // Դ�ļ��л���
	for (i = 0; i < SrcFileInfo.biHeight; i++)
	{
		pSrcCellLineBuf[i] = new unsigned char[LineBytes];
		fread(pSrcCellLineBuf[i], sizeof(char), LineBytes, SrcFileStream);
	}

	// ��ȡ�߽�
	unsigned char** BdyMtx = new unsigned char* [SrcFileInfo.biHeight];
	for (int y = 0; y < SrcFileInfo.biHeight; y++) // 2��ѭ������ÿһ��դ��
	{
		BdyMtx[y] = new unsigned char[LineBytes];
		if ((y == 0) || (y == SrcFileInfo.biHeight - 1)) continue;

		for (int x = 1; x < SrcFileInfo.biWidth - 1; x++)
		{
			unsigned char currClr = pSrcCellLineBuf[y][x];
			// �������ж�
			unsigned char upClr = pSrcCellLineBuf[y + 1][x];
			unsigned char boClr = pSrcCellLineBuf[y - 1][x];
			unsigned char lfClr = pSrcCellLineBuf[y][x - 1];
			unsigned char rtClr = pSrcCellLineBuf[y][x + 1];

			// ����Χ��һ��
			if ((currClr != upClr) || (currClr != boClr) || (currClr != lfClr) || (currClr != rtClr))
				BdyMtx[y][x] = pSrcCellLineBuf[y][x];
			else BdyMtx[y][x] = 255;
		}
	}


	FILE* DestFile = fopen(destFileName, "wb");
	if (DestFile == NULL) return false; // ʵ�����������ļ��޷�����
	fwrite(&SrcFileHead, sizeof(BITMAPFILEHEADER), 1, DestFile);
	fwrite(&SrcFileInfo, sizeof(BITMAPINFOHEADER), 1, DestFile);
	fwrite(ColorIdx, 1024, 1, DestFile);

	for (i = 0; i < SrcFileInfo.biHeight; i++)
	{
		fwrite(BdyMtx[i], sizeof(unsigned char), LineBytes, DestFile);
		delete[] BdyMtx[i];
		BdyMtx[i] = NULL;
		delete[] pSrcCellLineBuf[i];
	}

	delete[] BdyMtx;
	delete[] pSrcCellLineBuf;

	fclose(DestFile);
	fclose(SrcFileStream);

	return true;
}

bool MAUtil::getBufferBmp(const char* disFileName, int bufWidth, const char* destFileName)
{

	unsigned i, j; // ѭ������

	FILE* SrcFileStream = fopen(disFileName, "rb");
	if (SrcFileStream == NULL) return false;

	BITMAPFILEHEADER SrcFileHead;
	BITMAPINFOHEADER SrcFileInfo;
	unsigned char ColorIdx[1024];

	if (fread(&SrcFileHead, sizeof(BITMAPFILEHEADER), 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return false;
	}
	if (fread(&SrcFileInfo, sizeof(BITMAPINFOHEADER), 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return false;
	}
	if (fread(ColorIdx, 1024, 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return false;
	}

	if (SrcFileInfo.biBitCount != 8) // 256ɫͼ����
	{
		fclose(SrcFileStream);
		return false; // �ļ���ʽ����Ҫ��
	}


	//  ����ʵ������
	int LineBytes = (SrcFileInfo.biWidth + 3) / 4 * 4;


	//  ��ȡ���볡����
	unsigned char** pSrcCellLineBuf = new unsigned char* [SrcFileInfo.biHeight];   // Դ�ļ��л���
	for (i = 0; i < SrcFileInfo.biHeight; i++)
	{
		pSrcCellLineBuf[i] = new unsigned char[LineBytes];
		fread(pSrcCellLineBuf[i], sizeof(char), LineBytes, SrcFileStream);
	}
	// ���뻺��������
	for (i = 0; i < SrcFileInfo.biHeight; i++)
		for (j = 0; j < SrcFileInfo.biWidth; j++)
		{
			if (pSrcCellLineBuf[i][j] > bufWidth) pSrcCellLineBuf[i][j] = 249; // ��Ǵ��ڻ������뾶��ɫ
			else pSrcCellLineBuf[i][j] = 252;
		}

	// ��ȡ�߽�
	unsigned char** BdyMtx = new unsigned char* [SrcFileInfo.biHeight];
	for (int y = 0; y < SrcFileInfo.biHeight; y++) // 2��ѭ������ÿһ��դ��
	{
		BdyMtx[y] = new unsigned char[LineBytes];
		if ((y == 0) || (y == SrcFileInfo.biHeight - 1)) continue;

		for (int x = 1; x < SrcFileInfo.biWidth - 1; x++)
		{
			unsigned char currClr = pSrcCellLineBuf[y][x];
			// �������ж�
			unsigned char upClr = pSrcCellLineBuf[y + 1][x];
			unsigned char boClr = pSrcCellLineBuf[y - 1][x];
			unsigned char lfClr = pSrcCellLineBuf[y][x - 1];
			unsigned char rtClr = pSrcCellLineBuf[y][x + 1];

			// ����Χ��һ��
			if ((currClr != upClr) || (currClr != boClr) || (currClr != lfClr) || (currClr != rtClr))
				BdyMtx[y][x] = pSrcCellLineBuf[y][x];
			else BdyMtx[y][x] = 255;
		}
	}


	FILE* DestFile = fopen(destFileName, "wb");
	if (DestFile == NULL) return false; // ʵ�����������ļ��޷�����
	fwrite(&SrcFileHead, sizeof(BITMAPFILEHEADER), 1, DestFile);
	fwrite(&SrcFileInfo, sizeof(BITMAPINFOHEADER), 1, DestFile);
	fwrite(ColorIdx, 1024, 1, DestFile);

	for (i = 0; i < SrcFileInfo.biHeight; i++)
	{
		fwrite(BdyMtx[i], sizeof(unsigned char), LineBytes, DestFile);
		delete[] BdyMtx[i];
		BdyMtx[i] = NULL;
		delete[] pSrcCellLineBuf[i];
	}

	delete[] BdyMtx;
	delete[] pSrcCellLineBuf;

	fclose(DestFile);
	fclose(SrcFileStream);

	return true;
}

bool MAUtil::shellAddTransform(const char* disFileName, int bufWidth, const char* destFileName)
{
	unsigned i, j; // ѭ������

	FILE* SrcFileStream = fopen(disFileName, "rb");
	if (SrcFileStream == NULL) return false;

	BITMAPFILEHEADER SrcFileHead;
	BITMAPINFOHEADER SrcFileInfo;
	unsigned char ColorIdx[1024];

	if (fread(&SrcFileHead, sizeof(BITMAPFILEHEADER), 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return false;
	}
	if (fread(&SrcFileInfo, sizeof(BITMAPINFOHEADER), 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return false;
	}
	if (fread(ColorIdx, 1024, 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return false;
	}

	if (SrcFileInfo.biBitCount != 8) // 256ɫͼ����
	{
		fclose(SrcFileStream);
		return false; // �ļ���ʽ����Ҫ��
	}


	//  ����ʵ������
	int LineBytes = (SrcFileInfo.biWidth + 3) / 4 * 4;


	//  ��ȡ���볡����
	unsigned char** pSrcCellLineBuf = new unsigned char* [SrcFileInfo.biHeight];   // Դ�ļ��л���
	for (i = 0; i < SrcFileInfo.biHeight; i++)
	{
		pSrcCellLineBuf[i] = new unsigned char[LineBytes];
		fread(pSrcCellLineBuf[i], sizeof(char), LineBytes, SrcFileStream);
	}
	// ���뻺��������
	for (i = 0; i < SrcFileInfo.biHeight; i++)
		for (j = 0; j < SrcFileInfo.biWidth; j++)
		{
			if (pSrcCellLineBuf[i][j] > bufWidth) pSrcCellLineBuf[i][j] = 255; // ��Ǵ��ڻ������뾶��ɫ
			else pSrcCellLineBuf[i][j] = 0;
		}


	FILE* DestFile = fopen(destFileName, "wb");
	if (DestFile == NULL) return false; // ʵ�����������ļ��޷�����
	fwrite(&SrcFileHead, sizeof(BITMAPFILEHEADER), 1, DestFile);
	fwrite(&SrcFileInfo, sizeof(BITMAPINFOHEADER), 1, DestFile);
	fwrite(ColorIdx, 1024, 1, DestFile);

	for (i = 0; i < SrcFileInfo.biHeight; i++)
	{
		fwrite(pSrcCellLineBuf[i], sizeof(unsigned char), LineBytes, DestFile);
		delete[] pSrcCellLineBuf[i];
		pSrcCellLineBuf[i] = NULL;
	}

	delete[] pSrcCellLineBuf;

	fclose(DestFile);
	fclose(SrcFileStream);

	return true;
}

bool MAUtil::shellMinusTransform(const char* disFileName, int bufWidth, const char* destFileName)
{
	unsigned i, j; // ѭ������

	FILE* SrcFileStream = fopen(disFileName, "rb");
	if (SrcFileStream == NULL) return false;

	BITMAPFILEHEADER SrcFileHead;
	BITMAPINFOHEADER SrcFileInfo;
	unsigned char ColorIdx[1024];

	if (fread(&SrcFileHead, sizeof(BITMAPFILEHEADER), 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return false;
	}
	if (fread(&SrcFileInfo, sizeof(BITMAPINFOHEADER), 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return false;
	}
	if (fread(ColorIdx, 1024, 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return false;
	}

	if (SrcFileInfo.biBitCount != 8) // 256ɫͼ����
	{
		fclose(SrcFileStream);
		return false; // �ļ���ʽ����Ҫ��
	}


	//  ����ʵ������
	int LineBytes = (SrcFileInfo.biWidth + 3) / 4 * 4;


	//  ��ȡ���볡����
	unsigned char** pSrcCellLineBuf = new unsigned char* [SrcFileInfo.biHeight];   // Դ�ļ��л���
	for (i = 0; i < SrcFileInfo.biHeight; i++)
	{
		pSrcCellLineBuf[i] = new unsigned char[LineBytes];
		fread(pSrcCellLineBuf[i], sizeof(char), LineBytes, SrcFileStream);
	}
	// ���뻺��������
	for (i = 0; i < SrcFileInfo.biHeight; i++)
		for (j = 0; j < SrcFileInfo.biWidth; j++)
		{
			if (pSrcCellLineBuf[i][j] > bufWidth) pSrcCellLineBuf[i][j] = 0; // ��Ǵ��ڻ������뾶��ɫ
			else pSrcCellLineBuf[i][j] = 255;
		}


	FILE* DestFile = fopen(destFileName, "wb");
	if (DestFile == NULL) return false; // ʵ�����������ļ��޷�����
	fwrite(&SrcFileHead, sizeof(BITMAPFILEHEADER), 1, DestFile);
	fwrite(&SrcFileInfo, sizeof(BITMAPINFOHEADER), 1, DestFile);
	fwrite(ColorIdx, 1024, 1, DestFile);

	for (i = 0; i < SrcFileInfo.biHeight; i++)
	{
		fwrite(pSrcCellLineBuf[i], sizeof(unsigned char), LineBytes, DestFile);
		delete[] pSrcCellLineBuf[i];
		pSrcCellLineBuf[i] = NULL;
	}

	delete[] pSrcCellLineBuf;

	fclose(DestFile);
	fclose(SrcFileStream);
	return true;
}

bool MAUtil::adhesionTransform(const char* sourcefilename, int bufWidth, const char* disFile, const char* locFile, CDistanceTmp* templet, const char* outFileName)
{
	// ���任
	distanceTransform(sourcefilename, disFile, locFile, templet);
	// �ӿǱ任
	shellAddTransform(disFile, bufWidth, outFileName);
	// �ھ�任
	distanceTransformIn(outFileName, disFile, locFile, templet);
	// ��Ƥ�任
	shellMinusTransform(disFile, bufWidth, outFileName);
	return true;
}

bool MAUtil::corrosionTransform(const char* sourcefilename, int bufWidth, const char* disFile, const char* locFile, CDistanceTmp* templet, const char* outFileName)
{
	// �ھ�任
	distanceTransformIn(sourcefilename,disFile, locFile, templet);
	// ��Ƥ�任
	shellMinusTransform(disFile, bufWidth, outFileName);
	// ���任
	distanceTransform(outFileName, disFile, locFile, templet);
	// �ӿǱ任
	shellAddTransform(disFile, bufWidth, outFileName);
	return true;
}

bool MAUtil::scanSrcPtCoors(const char* sourceFileName, const char* colorsTableFile)
{
	unsigned i, j; // ѭ������

	FILE* SrcFileStream = fopen(sourceFileName, "rb");

	BITMAPFILEHEADER SrcFileHead;
	BITMAPINFOHEADER SrcFileInfo;
	unsigned char ColorIdx[1024];

	fread(&SrcFileHead, sizeof(BITMAPFILEHEADER), 1, SrcFileStream);
	fread(&SrcFileInfo, sizeof(BITMAPINFOHEADER), 1, SrcFileStream);
	fread(ColorIdx, 1024, 1, SrcFileStream) != 1;


	// *2 ����ʵ������
	int LineBytes = (SrcFileInfo.biWidth + 3) / 4 * 4;


	// *2.1 ����ԭʼ����
	FILE* DestFile = fopen(colorsTableFile, "wt");

	unsigned char* pSrcCellLineBuf = new unsigned char[LineBytes];   // Դ�ļ��л���
	for (i = 0; i < SrcFileInfo.biHeight; i++)
	{
		fread(pSrcCellLineBuf, sizeof(char), LineBytes, SrcFileStream);
		for (j = 0; j < SrcFileInfo.biWidth; j++)
			if (pSrcCellLineBuf[j] != 0xFF)
			{
				fprintf(DestFile, "%d\t%d\t%d\n", pSrcCellLineBuf[j], j, i);
				//*******************************
			}
	}
	delete[] pSrcCellLineBuf;

	fclose(DestFile);
	fclose(SrcFileStream);
	return true;
	return false;
}

bool MAUtil::getTinPtPairs(const char* locFileName, const char* ptPairsFile)
{
	// 1. ��ȡ���䳡���ݴ������
	unsigned i, j;

	FILE* SrcFileStream = fopen(locFileName, "rb");

	BITMAPFILEHEADER SrcFileHead;
	BITMAPINFOHEADER SrcFileInfo;
	unsigned char ColorIdx[1024];

	fread(&SrcFileHead, sizeof(BITMAPFILEHEADER), 1, SrcFileStream);
	fread(&SrcFileInfo, sizeof(BITMAPINFOHEADER), 1, SrcFileStream);
	fread(ColorIdx, 1024, 1, SrcFileStream);

	// �д�С����4�ı����ֽ�
	int LineBytes = (SrcFileInfo.biWidth + 3) / 4 * 4;

	//  ��ȡ���䳡����
	unsigned char** pSrcCellLineBuf = new unsigned char* [SrcFileInfo.biHeight];   // Դ�ļ��л���
	for (i = 0; i < SrcFileInfo.biHeight; i++)
	{
		pSrcCellLineBuf[i] = new unsigned char[LineBytes];
		fread(pSrcCellLineBuf[i], sizeof(char), LineBytes, SrcFileStream);
	}
	fclose(SrcFileStream);

	// 2. ɨ����䳡����ȡ�����Ϣ����������
	// �������
	unsigned char Clr1[500];
	unsigned char Clr2[500];
	int PtsPairNum = 0;

	// ɨ��
	for (int y = 0; y < SrcFileInfo.biHeight; y++) // 2��ѭ������ÿһ��դ��
	{
		if ((y == 0) || (y == SrcFileInfo.biHeight - 1)) continue;
		for (int x = 1; x < SrcFileInfo.biWidth - 1; x++)
		{
			unsigned char currClr = pSrcCellLineBuf[y][x];

			unsigned char upClr = pSrcCellLineBuf[y + 1][x];
			unsigned char boClr = pSrcCellLineBuf[y - 1][x];
			unsigned char lfClr = pSrcCellLineBuf[y][x - 1];
			unsigned char rtClr = pSrcCellLineBuf[y][x + 1];

			// �ҳ�4�����е���ɫ
			unsigned char tmpClr;
			if (currClr != upClr) tmpClr = upClr;
			else if (currClr != boClr) tmpClr = boClr;
			else if (currClr != lfClr) tmpClr = lfClr;
			else if (currClr != rtClr) tmpClr = rtClr;
			else continue;
			unsigned char minClr = currClr;
			unsigned char maxClr = tmpClr;
			if (tmpClr < currClr)
			{
				maxClr = currClr;
				minClr = tmpClr;
			}

			// ���ҵ���������Ƿ����ظ���(ע����������β�������˳����ң�Ӧ���ò�������Ӷ��ֲ���Ч�ʸ��ߣ�
			bool isPtsPairExist = false;
			for (int k = 0; k < PtsPairNum; k++)
				if ((minClr == Clr1[k]) && (maxClr == Clr2[k]))
				{
					isPtsPairExist = true;
					break;
				}

			if (!isPtsPairExist)
			{
				Clr1[PtsPairNum] = minClr;
				Clr2[PtsPairNum] = maxClr;
				PtsPairNum++;
			}
		}
	}


	//  3. �������д���ļ�
	FILE* DestFile = fopen(ptPairsFile, "wt");
	for (i = 0; i < PtsPairNum; i++)
		fprintf(DestFile, "%d\t%d\n", Clr1[i], Clr2[i]);
	fclose(DestFile);

	// 4. ɨβ
	for (i = 0; i < SrcFileInfo.biHeight; i++)
	{
		delete[] pSrcCellLineBuf[i];
	}
	delete[] pSrcCellLineBuf;

	return true; // ���سɹ�����1
}

bool MAUtil::linkPts(const char* sourceFileName, const char* ptPairsFile, const char* colorsTableFile, const char* destFileName)
{

	// 1. ��ȡԭͼ���ݾ���
	unsigned i, j;

	FILE* SrcFileStream = fopen(sourceFileName, "rb");

	BITMAPFILEHEADER SrcFileHead;
	BITMAPINFOHEADER SrcFileInfo;
	unsigned char ColorIdx[1024];

	fread(&SrcFileHead, sizeof(BITMAPFILEHEADER), 1, SrcFileStream);
	fread(&SrcFileInfo, sizeof(BITMAPINFOHEADER), 1, SrcFileStream);
	fread(ColorIdx, 1024, 1, SrcFileStream);

	// �д�С����4�ı����ֽ�
	int LineBytes = (SrcFileInfo.biWidth + 3) / 4 * 4;

	//  ��ȡԴ�㳡����
	unsigned char** SrcMtx = new unsigned char* [SrcFileInfo.biHeight];   // Դ�ļ��л���
	for (i = 0; i < SrcFileInfo.biHeight; i++)
	{
		SrcMtx[i] = new unsigned char[LineBytes];
		fread(SrcMtx[i], sizeof(char), LineBytes, SrcFileStream);
	}
	fclose(SrcFileStream);

	// 2. ��ȡ��ʼ���������Ϣ
	int TabX[256];
	int TabY[256];
	FILE* CoorsFile = fopen(colorsTableFile, "rt");

	while (!feof(CoorsFile))
	{
		unsigned Clr;
		unsigned x, y;
		fscanf(CoorsFile, "%d\t%d\t%d\n", &Clr, &x, &y);
		TabX[Clr] = x;
		TabY[Clr] = y;
	}
	fclose(CoorsFile);

	// 3. ��ȡ�����Ϣ
	FILE* PPairFile = fopen(ptPairsFile, "rt");
	while (!feof(PPairFile))
	{
		unsigned Clr1;
		unsigned Clr2;
		fscanf(PPairFile, "%d\t%d\n", &Clr1, &Clr2);

		// 4. �����ݾ��������ӵ��֮���ֱ��
		int xa, ya, xb, yb;
		xa = TabX[Clr1];
		ya = TabY[Clr1];
		xb = TabX[Clr2];
		yb = TabY[Clr2];

		// DDA ��ֱ�߶�
		int dx, dy, n, k; float xinc, yinc, x, y;
		dx = xb - xa; dy = yb - ya;
		if (abs(dx) > abs(dy))  n = abs(dx);
		else  n = abs(dy);
		xinc = (float)dx / n;   yinc = (float)dy / n;
		x = (float)xa; y = (float)ya;
		for (k = 1; k <= n; k++)
		{
			SrcMtx[int(y + 0.5)][int(x + 0.5)] = 0; //��ɫ;  
			x += xinc;    y += yinc;
		}
	}
	fclose(PPairFile);

	// 5. ����Delauney������λͼ����
	FILE* TINFileStream = fopen(destFileName, "wb");

	fwrite(&SrcFileHead, sizeof(BITMAPFILEHEADER), 1, TINFileStream);
	fwrite(&SrcFileInfo, sizeof(BITMAPINFOHEADER), 1, TINFileStream);
	fwrite(ColorIdx, 1024, 1, TINFileStream);
	for (i = 0; i < SrcFileInfo.biHeight; i++)
	{
		fwrite(SrcMtx[i], sizeof(char), LineBytes, TINFileStream);
		delete[]SrcMtx[i];
	}
	delete[]SrcMtx;
	fclose(TINFileStream);

	// ɨβ
	return true;
}

