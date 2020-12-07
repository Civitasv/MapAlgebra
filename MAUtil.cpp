#include "MAUtil.h"

unsigned char** MAUtil::readBmp(const char* sourceFileName)
{
	// 循环变量
	unsigned int i, j;
	// 文件路径不能为空
	assert(sourceFileName != NULL);
	// 打开源文件
	FILE* sourceFileStream;
	fopen_s(&sourceFileStream, sourceFileName, "rb");
	// 文件不存在，则结束
	if (!sourceFileStream)
		return 0;
	// 头文件
	BITMAPFILEHEADER sourceFileHead;
	// 信息文件
	BITMAPINFOHEADER sourceFileInfo;
	// 颜色表
	unsigned char colorIdx[1024];
	// 读取数据
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
	if (sourceFileInfo.biBitCount != 8) { // 如果不是256色图
		fclose(sourceFileStream);
		return 0;
	}
	int fileWidth = (sourceFileInfo.biWidth + 3) / 4 * 4;
	// 源文件数据缓冲
	unsigned char** pSrcCellBuf = new unsigned char* [sourceFileInfo.biHeight];
	int size = 3;
	AverageFilter average(size);// 3*3均值滤波器
	for (unsigned i = 0; i < sourceFileInfo.biHeight; i++)// 从上向下
	{
		pSrcCellBuf[i] = new unsigned char[fileWidth];
		fread(pSrcCellBuf[i], 1, fileWidth, sourceFileStream);
	}

	fclose(sourceFileStream);

	return pSrcCellBuf;
}

bool MAUtil::inverseBmp(const char* sourceFileName, const char* destFileName) {
	// 循环变量
	unsigned int i, j;
	// 文件路径不能为空
	assert(sourceFileName != NULL);
	// 打开源文件
	FILE* sourceFileStream;
	fopen_s(&sourceFileStream, sourceFileName, "rb");
	// 文件不存在，则结束
	if (!sourceFileStream)
		return false;
	// 头文件
	BITMAPFILEHEADER sourceFileHead;
	// 信息文件
	BITMAPINFOHEADER sourceFileInfo;
	// 颜色表
	unsigned char colorIdx[1024];
	// 读取数据
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
	if (sourceFileInfo.biBitCount != 8) { // 如果不是256色图
		fclose(sourceFileStream);
		return false;
	}
	int lineBytes = (sourceFileInfo.biWidth + 3) / 4 * 4;
	// 源文件行缓冲
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
	/*--------------------输出文件----------------------------*/
	FILE* DestFile = fopen(destFileName, "wb");
	if (DestFile == NULL) return false; // 实体索引矩阵文件无法创建
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

	return true; // 返回成功
}

bool MAUtil::overlayAnalysis(const char* sourceFileName1, const char* sourceFileName2, const char* destFileName)
{
	// 循环变量
	unsigned int i, j;
	// 文件路径不能为空
	assert(sourceFileName1 != NULL);
	assert(sourceFileName2 != NULL);
	// 打开源文件
	FILE* sourceFileStream1;
	FILE* sourceFileStream2;
	fopen_s(&sourceFileStream1, sourceFileName1, "rb");
	fopen_s(&sourceFileStream2, sourceFileName2, "rb");
	// 文件不存在，则结束
	if (!sourceFileStream1)
		return false;
	if (!sourceFileStream2)
		return false;
	// 头文件
	BITMAPFILEHEADER sourceFileHead1;
	// 信息文件
	BITMAPINFOHEADER sourceFileInfo1;
	// 头文件
	BITMAPFILEHEADER sourceFileHead2;
	// 信息文件
	BITMAPINFOHEADER sourceFileInfo2;
	// 颜色表
	unsigned char colorIdx[1024];
	unsigned char colorIdx2[1024];
	// 读取数据
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
	if (sourceFileInfo1.biBitCount != 8) { // 如果不是256色图
		fclose(sourceFileStream1);
		return false;
	}
	int fileWidth1 = (sourceFileInfo1.biWidth + 3) / 4 * 4;
	// 源文件数据缓冲
	unsigned char** pSrcCellBuf1 = new unsigned char* [sourceFileInfo1.biHeight];

	// 读取数据
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
	if (sourceFileInfo2.biBitCount != 8) { // 如果不是256色图
		fclose(sourceFileStream2);
		return false;
	}
	int fileWidth2 = (sourceFileInfo2.biWidth + 3) / 4 * 4;
	// 源文件数据缓冲
	unsigned char** pSrcCellBuf2 = new unsigned char* [sourceFileInfo2.biHeight];
	// 如果两张图片大小不一样，fail
	if (sourceFileInfo2.biWidth != sourceFileInfo1.biWidth || sourceFileInfo2.biHeight != sourceFileInfo1.biHeight) {
		return false;
	}

	unsigned char** pDestCellBuf = new unsigned char* [sourceFileInfo1.biHeight];
	for (unsigned i = 0; i < sourceFileInfo1.biHeight; i++)// 从上向下
	{
		pSrcCellBuf1[i] = new unsigned char[fileWidth1];
		pSrcCellBuf2[i] = new unsigned char[fileWidth2];
		fread(pSrcCellBuf1[i], 1, fileWidth1, sourceFileStream1);
		fread(pSrcCellBuf2[i], 1, fileWidth2, sourceFileStream2);
		pDestCellBuf[i] = new unsigned char[fileWidth1];
	}
	for (unsigned i = 0; i < sourceFileInfo1.biHeight; i++)// 从上向下
	{	// 均值滤波开始
		for (unsigned j = 0; j < fileWidth1; j++) {//从左到右
			int sum = 0;
			int flag = 1;
			pDestCellBuf[i][j] = pSrcCellBuf1[i][j] * 0.5 + pSrcCellBuf2[i][j] * 0.5;
		}
	}


	/*--------------------输出文件----------------------------*/
	FILE* DestFile = fopen(destFileName, "wb");
	if (DestFile == NULL) return false; // 实体索引矩阵文件无法创建
	fwrite(&sourceFileHead1, sizeof(BITMAPFILEHEADER), 1, DestFile);
	fwrite(&sourceFileInfo1, sizeof(BITMAPINFOHEADER), 1, DestFile);
	fwrite(colorIdx, 1024, 1, DestFile);
	// 写入 and 回收
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
	// 循环变量
	unsigned i, j;
	// 文件路径不能为空
	assert(sourceFileName != NULL);
	// 打开源文件
	FILE* SrcFileStream = fopen(sourceFileName, "rb");
	if (SrcFileStream == NULL) return false; // 文件不存在

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

	if (SrcFileInfo.biBitCount != 8) // 256色图处理
	{
		fclose(SrcFileStream);
		return false; // 文件格式不合要求
	}

	// 编制实体索引
	BITMAPFILEHEADER IdxFileHead;
	BITMAPINFOHEADER IdxFileInfo;
	int LineBytes = (SrcFileInfo.biWidth + 3) / 4 * 4;
	int LineBytes2 = SrcFileInfo.biWidth;

	IdxFileHead.bfType = SrcFileHead.bfType;
	// 54为文件头+信息头
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


	// 构造原始矩阵
	unsigned char* pSrcCellLineBuf = new unsigned char[LineBytes];   // 源文件行缓冲
	unsigned** pIdxCellMatixBuf = new unsigned* [IdxFileInfo.biHeight]; //索引矩阵
	for (i = 0; i < IdxFileInfo.biHeight; i++)
	{
		fseek(SrcFileStream, SrcFileHead.bfOffBits + i * sizeof(char) * LineBytes, 0);
		fread(pSrcCellLineBuf, sizeof(char), LineBytes, SrcFileStream);
		pIdxCellMatixBuf[i] = new unsigned[LineBytes2];
		for (j = 0; j < LineBytes2; j++)
			// 如果是白色
			if (pSrcCellLineBuf[j] == 0xFF) pIdxCellMatixBuf[i][j] = 0xFFFFFFFF;
			else
			{
				int k = pSrcCellLineBuf[j];
				unsigned uintTmp = k;
				/*  取对应颜色*/
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
	if (ObjIdxFile == NULL) return false; // 实体索引矩阵文件无法创建
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
	// 循环变量
	unsigned int i, j;
	// 文件路径不能为空
	assert(sourceFileName != NULL);
	// 打开源文件
	FILE* sourceFileStream;
	fopen_s(&sourceFileStream, sourceFileName, "rb");
	// 文件不存在，则结束
	if (!sourceFileStream)
		return false;
	// 头文件
	BITMAPFILEHEADER sourceFileHead;
	// 信息文件
	BITMAPINFOHEADER sourceFileInfo;
	// 颜色表
	unsigned char colorIdx[1024];
	// 读取数据
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
	if (sourceFileInfo.biBitCount != 8) { // 如果不是256色图
		fclose(sourceFileStream);
		return false;
	}
	int fileWidth = (sourceFileInfo.biWidth + 3) / 4 * 4;
	// 源文件数据缓冲
	unsigned char** pSrcCellBuf = new unsigned char* [sourceFileInfo.biHeight];
	unsigned char** pDestCellBuf = new unsigned char* [sourceFileInfo.biHeight];
	int size = 3;
	AverageFilter average(size);// 3*3均值滤波器
	for (unsigned i = 0; i < sourceFileInfo.biHeight; i++)// 从上向下
	{
		pSrcCellBuf[i] = new unsigned char[fileWidth];
		fread(pSrcCellBuf[i], 1, fileWidth, sourceFileStream);
		pDestCellBuf[i] = new unsigned char[fileWidth];
	}
	for (unsigned i = 0; i < sourceFileInfo.biHeight; i++)// 从上向下
	{	// 均值滤波开始
		for (unsigned j = 0; j < fileWidth; j++) {//从左到右
			int sum = 0;
			int flag = 1;
			for (int k = 0; k < average.Size(); k++) {
				char offX = average.getOffX(k);
				char offY = average.getOffY(k);
				int CurrX = j + offX;              // 计算模板元素对应的栅格位置
				int CurrY = i + offY;
				// 判断该位置是否越界
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


	/*--------------------输出文件----------------------------*/
	FILE* DestFile = fopen(destFileName, "wb");
	if (DestFile == NULL) return false; // 实体索引矩阵文件无法创建
	fwrite(&sourceFileHead, sizeof(BITMAPFILEHEADER), 1, DestFile);
	fwrite(&sourceFileInfo, sizeof(BITMAPINFOHEADER), 1, DestFile);
	fwrite(colorIdx, 1024, 1, DestFile);
	// 写入 and 回收
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

	return true; // 返回成功
}

bool MAUtil::midFilter(const char* sourceFileName, const char* destFileName) {
	// 循环变量
	unsigned int i, j;
	// 文件路径不能为空
	assert(sourceFileName != NULL);
	// 打开源文件
	FILE* sourceFileStream;
	fopen_s(&sourceFileStream, sourceFileName, "rb");
	// 文件不存在，则结束
	if (!sourceFileStream)
		return false;
	// 头文件
	BITMAPFILEHEADER sourceFileHead;
	// 信息文件
	BITMAPINFOHEADER sourceFileInfo;
	// 颜色表
	unsigned char colorIdx[1024];
	// 读取数据
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
	if (sourceFileInfo.biBitCount != 8) { // 如果不是256色图
		fclose(sourceFileStream);
		return false;
	}
	int fileWidth = (sourceFileInfo.biWidth + 3) / 4 * 4;
	// 源文件数据缓冲
	unsigned char** pSrcCellBuf = new unsigned char* [sourceFileInfo.biHeight];
	unsigned char** pDestCellBuf = new unsigned char* [sourceFileInfo.biHeight];
	int size = 3;
	for (unsigned i = 0; i < sourceFileInfo.biHeight; i++)// 从上向下
	{
		pSrcCellBuf[i] = new unsigned char[fileWidth];
		fread(pSrcCellBuf[i], 1, fileWidth, sourceFileStream);
		pDestCellBuf[i] = new unsigned char[fileWidth];
	}
	for (unsigned i = 0; i < sourceFileInfo.biHeight; i++)// 从上向下
	{	// 3*3中值滤波开始
		for (unsigned j = 0; j < fileWidth; j++) {//从左到右
			vector<int> val;
			int flag = 1;
			for (int k = 0; k < size * size; k++) {
				char offX = k % size - size / 2;
				char offY = k / size - size / 2;
				int CurrX = j + offX;              // 计算模板元素对应的栅格位置
				int CurrY = i + offY;
				if ((CurrX < 0) || (CurrX > sourceFileInfo.biWidth - 1)) {
					flag = -1;
					break;  // 判断该位置是否越界
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

	/*--------------------输出文件----------------------------*/
	FILE* DestFile = fopen(destFileName, "wb");
	if (DestFile == NULL) return false; // 实体索引矩阵文件无法创建
	fwrite(&sourceFileHead, sizeof(BITMAPFILEHEADER), 1, DestFile);
	fwrite(&sourceFileInfo, sizeof(BITMAPINFOHEADER), 1, DestFile);
	fwrite(colorIdx, 1024, 1, DestFile);
	// 写入 and 回收
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

	return true; // 返回成功
}

bool MAUtil::distanceTransform(const char* sourceFileName, const char* destFileName1, const char* destFileName2, CDistanceTmp* templet)
{
	// 1. 定义距离模板

	// 2. 读取原始位图，确定场的大小规格
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	char ColorTAB[1024];

	FILE* bmpFile = NULL;
	bmpFile = fopen(sourceFileName, "rb");
	// 文件不存在，则结束
	if (!bmpFile)
		return false;
	fseek(bmpFile, 0, SEEK_SET);

	// 读取位图文件头
	int kk = fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, bmpFile);
	// 读取位图文件信息头
	kk = fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, bmpFile);
	// 256色读取调色板
	kk = fread(ColorTAB, 1024, 1, bmpFile);

	unsigned Height, Width;
	// BMP图像的高度，单位像素
	Height = infoHeader.biHeight;
	// BMP图像的宽度，单位像素
	Width = infoHeader.biWidth;
	// 
	unsigned widthFile = (Width + 3) / 4 * 4;

	// 3. 建立初始 距离场、 分配场
	unsigned char** DisMatix = NULL;
	// 分配场定义
	unsigned char** LocMatix = NULL;

	LocMatix = new unsigned char* [Height];
	DisMatix = new unsigned char* [Height];
	for (unsigned i = 0; i < Height; i++)
	{
		LocMatix[i] = new unsigned char[widthFile];
		// 3.1 分配场数据直接从位图读取 0/255即背景值/前景值
		fread(LocMatix[i], 1, widthFile, bmpFile);
		DisMatix[i] = new unsigned char[widthFile];
		// 3.2 根据分配场的实体像素分布初始化距离场
		for (unsigned j = 0; j < widthFile; j++)
			if (LocMatix[i][j] != 255) DisMatix[i][j] = 0;
			else DisMatix[i][j] = 255;// 白色为待传播区域
	}
	fclose(bmpFile);

	// 4. 模板距离变换
	// 4.1 从左下到右上方向
	for (int y = 0; y < Height; y++) // 2重循环，对每一个栅格
	{
		for (int x = 0; x < widthFile; x++)
		{
			unsigned int tmpMin = DisMatix[y][x] + 0; // 搜索模板最小值初始化
			if (tmpMin == 0) continue;             // 为0，表示黑点无需计算
			for (unsigned k = 0; k < templet->Size() / 2; k++) // 对上半模板逐个元素循环
			{
				int offx = templet->getOffX(k); // k=0,offx=-6
				int offy = templet->getOffY(k);
				float tmpDis = templet->getTmpDis(k);     // 去除模板要素，x,y偏移及对应模板相对距离

				int CurrX = x + offx;              // 计算模板元素对应的栅格位置
				int CurrY = y + offy;
				if ((CurrX < 0) || (CurrX > Width - 1)) continue;  // 判断该位置是否越界
				if ((CurrY < 0) || (CurrY > Height - 1)) continue;

				if (DisMatix[CurrY][CurrX] == 255) continue; // 该位置如果是无穷大不可计算，只计算背景

				float dis1 = DisMatix[CurrY][CurrX] + tmpDis; // 当前栅格距离+模板相对距离
				if (dis1 < tmpMin)  // 求最小值
				{
					tmpMin = dis1;
					DisMatix[y][x] = dis1;                   // 最小距离填入距离矩阵
					LocMatix[y][x] = LocMatix[CurrY][CurrX]; // 最小距离来源填入分配矩阵 黑边+白色
				}
			}
		}
	}
	// 4.2 从右上到左下方向
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


	// 5. 写入结果文件
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



	// 6. 清理，释放内存
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
	// 1. 定义距离模板

	// 2. 读取原始位图，确定场的大小规格
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	char ColorTAB[1024];

	FILE* bmpFile = NULL;
	bmpFile = fopen(sourceFileName, "rb");
	// 文件不存在，则结束
	if (!bmpFile)
		return false;
	fseek(bmpFile, 0, SEEK_SET);

	// 读取位图文件头
	int kk = fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, bmpFile);
	// 读取位图文件信息头
	kk = fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, bmpFile);
	// 256色读取调色板
	kk = fread(ColorTAB, 1024, 1, bmpFile);

	unsigned Height, Width;
	// BMP图像的高度，单位像素
	Height = infoHeader.biHeight;
	// BMP图像的宽度，单位像素
	Width = infoHeader.biWidth;
	// 
	unsigned widthFile = (Width + 3) / 4 * 4;

	// 3. 建立初始 距离场、 分配场
	unsigned char** DisMatix = NULL;
	// 分配场定义
	unsigned char** LocMatix = NULL;

	LocMatix = new unsigned char* [Height];
	DisMatix = new unsigned char* [Height];
	for (unsigned i = 0; i < Height; i++)
	{
		LocMatix[i] = new unsigned char[widthFile];
		// 3.1 分配场数据直接从位图读取 0/255即背景值/前景值
		fread(LocMatix[i], 1, widthFile, bmpFile);
		DisMatix[i] = new unsigned char[widthFile];
		// 3.2 根据分配场的实体像素分布初始化距离场
		for (unsigned j = 0; j < widthFile; j++)
			if (LocMatix[i][j] == 255) DisMatix[i][j] = 0;
			else DisMatix[i][j] = 255;// 黑色为待传播区域
	}
	fclose(bmpFile);

	// 4. 模板距离变换
	// 4.1 从左下到右上方向
	for (int y = 0; y < Height; y++) // 2重循环，对每一个栅格
	{
		for (int x = 0; x < widthFile; x++)
		{
			unsigned int tmpMin = DisMatix[y][x] + 0; // 搜索模板最小值初始化
			if (tmpMin == 0) continue;             // 为0，表示黑点无需计算
			for (unsigned k = 0; k < templet->Size() / 2; k++) // 对上半模板逐个元素循环
			{
				int offx = templet->getOffX(k); // k=0,offx=-6
				int offy = templet->getOffY(k);
				float tmpDis = templet->getTmpDis(k);     // 去除模板要素，x,y偏移及对应模板相对距离

				int CurrX = x + offx;              // 计算模板元素对应的栅格位置
				int CurrY = y + offy;
				if ((CurrX < 0) || (CurrX > Width - 1)) continue;  // 判断该位置是否越界
				if ((CurrY < 0) || (CurrY > Height - 1)) continue;

				if (DisMatix[CurrY][CurrX] == 255) continue; // 该位置如果是无穷大不可计算，只计算背景

				float dis1 = DisMatix[CurrY][CurrX] + tmpDis; // 当前栅格距离+模板相对距离
				if (dis1 < tmpMin)  // 求最小值
				{
					tmpMin = dis1;
					DisMatix[y][x] = dis1;                   // 最小距离填入距离矩阵
					LocMatix[y][x] = LocMatix[CurrY][CurrX]; // 最小距离来源填入分配矩阵 黑边+白色
				}
			}
		}
	}
	// 4.2 从右上到左下方向
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


	// 5. 写入结果文件
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



	// 6. 清理，释放内存
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

	// 3. 建立初始 距离场、 分配场
	unsigned char** LocMatix = NULL;
	LocMatix = new unsigned char* [Height];

	float** DisMatix = NULL;
	DisMatix = new float* [Height];
	for (unsigned i = 0; i < Height; i++)
	{
		LocMatix[i] = new unsigned char[widthFile];
		// 3.1 分配场数据直接从位图读取
		fread(LocMatix[i], 1, widthFile, bmpFile);
		DisMatix[i] = new float[Width];
		// 3.2 根据分配场的实体像素分布初始化距离场
		for (unsigned j = 0; j < Width; j++)
		{
			if (LocMatix[i][j] == 255) DisMatix[i][j] = MaxFloat;// 无穷大，待传播
			else DisMatix[i][j] = 0;
		}
	}
	fclose(bmpFile);

	// 4. 模板距离变换
	// 4.1 从左下到右上方向
	for (int y = 0; y < Height; y++) // 2重循环，对每一个栅格
	{
		for (int x = 0; x < Width; x++)
		{
			float tmpMin = DisMatix[y][x] + 0; // 搜索模板最小值初始化
			if (fabs(tmpMin) < MinFloat) continue;             // 为0，表示黑点无需计算
			for (unsigned k = 0; k < templet->Size() / 2; k++) // 对上半模板逐个元素循环
			{
				char offx = templet->getOffX(k);
				char offy = templet->getOffY(k);
				float tmpDis = templet->getTmpDis(k);     // 去除模板要素，x,y偏移及对应模板相对距离

				int CurrX = x + offx;              // 计算模板元素对应的栅格位置
				int CurrY = y + offy;
				if ((CurrX < 0) || (CurrX > Width - 1)) continue;  // 判断该位置是否越界
				if ((CurrY < 0) || (CurrY > Height - 1)) continue;

				if (fabs(DisMatix[CurrY][CurrX] - MaxFloat) < MinFloat) continue; // 该位置如果是无穷大不可计算

				float dis1 = DisMatix[CurrY][CurrX] + tmpDis; // 当前栅格距离+模板相对距离
				if (dis1 < tmpMin)  // 求最小值
				{
					tmpMin = dis1;
					DisMatix[y][x] = dis1;                   // 最小距离填入距离矩阵
					LocMatix[y][x] = LocMatix[CurrY][CurrX]; // 最小距离来源填入分配矩阵
				}
			}
		}
	}
	// 4.2 从右上到左下方向
	for (int y = Height - 1; y > -1; y--)
	{
		for (int x = Width - 1; x > -1; x--)
		{
			float tmpMin = DisMatix[y][x] + 0; // 搜索模板最小值初始化
			if (fabs(tmpMin) < MinFloat) continue;             // 为0，表示黑点无需计算
			for (unsigned k = templet->Size() / 2 + 1; k < templet->Size(); k++)
			{
				char offx = templet->getOffX(k);
				char offy = templet->getOffY(k);
				float tmpDis = templet->getTmpDis(k);     // 去除模板要素，x,y偏移及对应模板相对距离

				int CurrX = x + offx;              // 计算模板元素对应的栅格位置
				int CurrY = y + offy;
				if ((CurrX < 0) || (CurrX > Width - 1)) continue;  // 判断该位置是否越界
				if ((CurrY < 0) || (CurrY > Height - 1)) continue;

				if (fabs(DisMatix[CurrY][CurrX] - MaxFloat) < MinFloat) continue; // 该位置如果是无穷大不可计算

				float dis1 = DisMatix[CurrY][CurrX] + tmpDis; // 当前栅格距离+模板相对距离
				if (dis1 < tmpMin)  // 求最小值
				{
					tmpMin = dis1;
					DisMatix[y][x] = dis1;                   // 最小距离填入距离矩阵
					LocMatix[y][x] = LocMatix[CurrY][CurrX]; // 最小距离来源填入分配矩阵
				}
			}
		}
	}


	// 5. 写入结果文件
	// 5.1 写入距离矩阵32位float型
	BITMAPFILEHEADER IdxFileHead; // 32位位图头结构
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

	// 5.2 写入分配场矩阵8位unsigned char型
	FILE* bmpWrite1 = fopen(destFileName2, "wb");
	fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, bmpWrite1);
	fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, bmpWrite1);
	fwrite(ColorTAB, 1024, 1, bmpWrite1);
	for (int y = 0; y < Height; y++)
	{
		fwrite(LocMatix[y], 1, widthFile, bmpWrite1);
	}
	fclose(bmpWrite1);



	// 6. 清理，释放内存
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
	// 1. 定义距离模板

	// 2. 读取原始位图，确定场的大小规格
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	char ColorTAB[1024];

	FILE* bmpFile = NULL;
	bmpFile = fopen(sourceFileName, "rb");
	// 文件不存在，则结束
	if (!bmpFile)
		return false;
	fseek(bmpFile, 0, SEEK_SET);

	// 读取位图文件头
	int kk = fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, bmpFile);
	// 读取位图文件信息头
	kk = fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, bmpFile);
	// 256色读取调色板
	kk = fread(ColorTAB, 1024, 1, bmpFile);

	unsigned Height, Width;
	// BMP图像的高度，单位像素
	Height = infoHeader.biHeight;
	// BMP图像的宽度，单位像素
	Width = infoHeader.biWidth;
	// 
	unsigned widthFile = (Width + 3) / 4 * 4;

	// 3. 建立初始 距离场、 分配场
	unsigned char** DisMatix = NULL;
	// 分配场定义
	unsigned char** LocMatix = NULL;

	LocMatix = new unsigned char* [Height];
	DisMatix = new unsigned char* [Height];
	int barrierX = Height / 4;
	int barrierY = widthFile / 4;
	for (unsigned i = 0; i < Height; i++)
	{
		LocMatix[i] = new unsigned char[widthFile];
		// 3.1 分配场数据直接从位图读取 0/255即背景值/前景值
		fread(LocMatix[i], 1, widthFile, bmpFile);
		DisMatix[i] = new unsigned char[widthFile];
		// 3.2 根据分配场的实体像素分布初始化距离场
		for (unsigned j = 0; j < widthFile; j++) {
			if (LocMatix[i][j] == 0)
				DisMatix[i][j] = 10000;
			else if (LocMatix[i][j] == 255)
				DisMatix[i][j] = 255;// 白色为待传播
			else
				DisMatix[i][j] = 0;
		}
	}
	fclose(bmpFile);

	// 4. 模板距离变换
	// 4.1 从左下到右上方向
	for (int y = 0; y < Height; y++) // 2重循环，对每一个栅格
	{
		for (int x = 0; x < widthFile; x++)
		{
			unsigned int tmpMin = DisMatix[y][x] + 0; // 搜索模板最小值初始化
			if (tmpMin == 0) continue;             // 为0，表示外距变换，内部点无需计算
			if (tmpMin == 10000) continue; // 障碍点无需计算
			for (unsigned k = 0; k < templet->Size() / 2; k++) // 对上半模板逐个元素循环
			{
				int offx = templet->getOffX(k); // k=0,offx=-6
				int offy = templet->getOffY(k);
				float tmpDis = templet->getTmpDis(k);     // 去除模板要素，x,y偏移及对应模板相对距离

				int CurrX = x + offx;              // 计算模板元素对应的栅格位置
				int CurrY = y + offy;
				if ((CurrX < 0) || (CurrX > Width - 1)) continue;  // 判断该位置是否越界
				if ((CurrY < 0) || (CurrY > Height - 1)) continue;

				if (DisMatix[CurrY][CurrX] == 255) continue; // 该位置如果是无穷大不可计算，只计算背景
				if (DisMatix[CurrY][CurrX] == 10000) continue; // 障碍点不参与比较

				float dis1 = DisMatix[CurrY][CurrX] + tmpDis; // 当前栅格距离+模板相对距离
				if (dis1 < tmpMin)  // 求最小值
				{
					tmpMin = dis1;
					DisMatix[y][x] = dis1;                   // 最小距离填入距离矩阵
					LocMatix[y][x] = LocMatix[CurrY][CurrX]; // 最小距离来源填入分配矩阵 黑边+白色
				}
			}
		}
	}
	// 4.2 从右上到左下方向
	for (int y = Height - 1; y > -1; y--)
	{
		for (int x = Width - 1; x > -1; x--)
		{
			unsigned int tmpMin = DisMatix[y][x];
			if (tmpMin == 0) continue;
			if (tmpMin == 10000) continue; // 障碍点无需计算
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
				if (DisMatix[CurrY][CurrX] == 10000) continue; // 障碍点不参与比较
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

	// 5. 写入结果文件
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



	// 6. 清理，释放内存
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
	unsigned i, j; // 循环变量

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

	if (SrcFileInfo.biBitCount != 8) // 256色图处理
	{
		fclose(SrcFileStream);
		return false; // 文件格式不合要求
	}


	//  编制实体索引
	int LineBytes = (SrcFileInfo.biWidth + 3) / 4 * 4;


	//  读取分配场矩阵
	unsigned char** pSrcCellLineBuf = new unsigned char* [SrcFileInfo.biHeight];   // 源文件行缓冲
	for (i = 0; i < SrcFileInfo.biHeight; i++)
	{
		pSrcCellLineBuf[i] = new unsigned char[LineBytes];
		fread(pSrcCellLineBuf[i], sizeof(char), LineBytes, SrcFileStream);
	}

	// 提取边界
	unsigned char** BdyMtx = new unsigned char* [SrcFileInfo.biHeight];
	for (int y = 0; y < SrcFileInfo.biHeight; y++) // 2重循环，对每一个栅格
	{
		BdyMtx[y] = new unsigned char[LineBytes];
		if ((y == 0) || (y == SrcFileInfo.biHeight - 1)) continue;

		for (int x = 1; x < SrcFileInfo.biWidth - 1; x++)
		{
			unsigned char currClr = pSrcCellLineBuf[y][x];
			// 四邻域判断
			unsigned char upClr = pSrcCellLineBuf[y + 1][x];
			unsigned char boClr = pSrcCellLineBuf[y - 1][x];
			unsigned char lfClr = pSrcCellLineBuf[y][x - 1];
			unsigned char rtClr = pSrcCellLineBuf[y][x + 1];

			// 与周围不一样
			if ((currClr != upClr) || (currClr != boClr) || (currClr != lfClr) || (currClr != rtClr))
				BdyMtx[y][x] = pSrcCellLineBuf[y][x];
			else BdyMtx[y][x] = 255;
		}
	}


	FILE* DestFile = fopen(destFileName, "wb");
	if (DestFile == NULL) return false; // 实体索引矩阵文件无法创建
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

	unsigned i, j; // 循环变量

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

	if (SrcFileInfo.biBitCount != 8) // 256色图处理
	{
		fclose(SrcFileStream);
		return false; // 文件格式不合要求
	}


	//  编制实体索引
	int LineBytes = (SrcFileInfo.biWidth + 3) / 4 * 4;


	//  读取距离场矩阵
	unsigned char** pSrcCellLineBuf = new unsigned char* [SrcFileInfo.biHeight];   // 源文件行缓冲
	for (i = 0; i < SrcFileInfo.biHeight; i++)
	{
		pSrcCellLineBuf[i] = new unsigned char[LineBytes];
		fread(pSrcCellLineBuf[i], sizeof(char), LineBytes, SrcFileStream);
	}
	// 分离缓冲区区域
	for (i = 0; i < SrcFileInfo.biHeight; i++)
		for (j = 0; j < SrcFileInfo.biWidth; j++)
		{
			if (pSrcCellLineBuf[i][j] > bufWidth) pSrcCellLineBuf[i][j] = 249; // 标记大于缓冲区半径颜色
			else pSrcCellLineBuf[i][j] = 252;
		}

	// 提取边界
	unsigned char** BdyMtx = new unsigned char* [SrcFileInfo.biHeight];
	for (int y = 0; y < SrcFileInfo.biHeight; y++) // 2重循环，对每一个栅格
	{
		BdyMtx[y] = new unsigned char[LineBytes];
		if ((y == 0) || (y == SrcFileInfo.biHeight - 1)) continue;

		for (int x = 1; x < SrcFileInfo.biWidth - 1; x++)
		{
			unsigned char currClr = pSrcCellLineBuf[y][x];
			// 四邻域判断
			unsigned char upClr = pSrcCellLineBuf[y + 1][x];
			unsigned char boClr = pSrcCellLineBuf[y - 1][x];
			unsigned char lfClr = pSrcCellLineBuf[y][x - 1];
			unsigned char rtClr = pSrcCellLineBuf[y][x + 1];

			// 与周围不一样
			if ((currClr != upClr) || (currClr != boClr) || (currClr != lfClr) || (currClr != rtClr))
				BdyMtx[y][x] = pSrcCellLineBuf[y][x];
			else BdyMtx[y][x] = 255;
		}
	}


	FILE* DestFile = fopen(destFileName, "wb");
	if (DestFile == NULL) return false; // 实体索引矩阵文件无法创建
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
	unsigned i, j; // 循环变量

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

	if (SrcFileInfo.biBitCount != 8) // 256色图处理
	{
		fclose(SrcFileStream);
		return false; // 文件格式不合要求
	}


	//  编制实体索引
	int LineBytes = (SrcFileInfo.biWidth + 3) / 4 * 4;


	//  读取距离场矩阵
	unsigned char** pSrcCellLineBuf = new unsigned char* [SrcFileInfo.biHeight];   // 源文件行缓冲
	for (i = 0; i < SrcFileInfo.biHeight; i++)
	{
		pSrcCellLineBuf[i] = new unsigned char[LineBytes];
		fread(pSrcCellLineBuf[i], sizeof(char), LineBytes, SrcFileStream);
	}
	// 分离缓冲区区域
	for (i = 0; i < SrcFileInfo.biHeight; i++)
		for (j = 0; j < SrcFileInfo.biWidth; j++)
		{
			if (pSrcCellLineBuf[i][j] > bufWidth) pSrcCellLineBuf[i][j] = 255; // 标记大于缓冲区半径颜色
			else pSrcCellLineBuf[i][j] = 0;
		}


	FILE* DestFile = fopen(destFileName, "wb");
	if (DestFile == NULL) return false; // 实体索引矩阵文件无法创建
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
	unsigned i, j; // 循环变量

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

	if (SrcFileInfo.biBitCount != 8) // 256色图处理
	{
		fclose(SrcFileStream);
		return false; // 文件格式不合要求
	}


	//  编制实体索引
	int LineBytes = (SrcFileInfo.biWidth + 3) / 4 * 4;


	//  读取距离场矩阵
	unsigned char** pSrcCellLineBuf = new unsigned char* [SrcFileInfo.biHeight];   // 源文件行缓冲
	for (i = 0; i < SrcFileInfo.biHeight; i++)
	{
		pSrcCellLineBuf[i] = new unsigned char[LineBytes];
		fread(pSrcCellLineBuf[i], sizeof(char), LineBytes, SrcFileStream);
	}
	// 分离缓冲区区域
	for (i = 0; i < SrcFileInfo.biHeight; i++)
		for (j = 0; j < SrcFileInfo.biWidth; j++)
		{
			if (pSrcCellLineBuf[i][j] > bufWidth) pSrcCellLineBuf[i][j] = 0; // 标记大于缓冲区半径颜色
			else pSrcCellLineBuf[i][j] = 255;
		}


	FILE* DestFile = fopen(destFileName, "wb");
	if (DestFile == NULL) return false; // 实体索引矩阵文件无法创建
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
	// 外距变换
	distanceTransform(sourcefilename, disFile, locFile, templet);
	// 加壳变换
	shellAddTransform(disFile, bufWidth, outFileName);
	// 内距变换
	distanceTransformIn(outFileName, disFile, locFile, templet);
	// 蜕皮变换
	shellMinusTransform(disFile, bufWidth, outFileName);
	return true;
}

bool MAUtil::corrosionTransform(const char* sourcefilename, int bufWidth, const char* disFile, const char* locFile, CDistanceTmp* templet, const char* outFileName)
{
	// 内距变换
	distanceTransformIn(sourcefilename,disFile, locFile, templet);
	// 蜕皮变换
	shellMinusTransform(disFile, bufWidth, outFileName);
	// 外距变换
	distanceTransform(outFileName, disFile, locFile, templet);
	// 加壳变换
	shellAddTransform(disFile, bufWidth, outFileName);
	return true;
}

bool MAUtil::scanSrcPtCoors(const char* sourceFileName, const char* colorsTableFile)
{
	unsigned i, j; // 循环变量

	FILE* SrcFileStream = fopen(sourceFileName, "rb");

	BITMAPFILEHEADER SrcFileHead;
	BITMAPINFOHEADER SrcFileInfo;
	unsigned char ColorIdx[1024];

	fread(&SrcFileHead, sizeof(BITMAPFILEHEADER), 1, SrcFileStream);
	fread(&SrcFileInfo, sizeof(BITMAPINFOHEADER), 1, SrcFileStream);
	fread(ColorIdx, 1024, 1, SrcFileStream) != 1;


	// *2 编制实体索引
	int LineBytes = (SrcFileInfo.biWidth + 3) / 4 * 4;


	// *2.1 构造原始矩阵
	FILE* DestFile = fopen(colorsTableFile, "wt");

	unsigned char* pSrcCellLineBuf = new unsigned char[LineBytes];   // 源文件行缓冲
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
	// 1. 读取分配场数据存入矩阵
	unsigned i, j;

	FILE* SrcFileStream = fopen(locFileName, "rb");

	BITMAPFILEHEADER SrcFileHead;
	BITMAPINFOHEADER SrcFileInfo;
	unsigned char ColorIdx[1024];

	fread(&SrcFileHead, sizeof(BITMAPFILEHEADER), 1, SrcFileStream);
	fread(&SrcFileInfo, sizeof(BITMAPINFOHEADER), 1, SrcFileStream);
	fread(ColorIdx, 1024, 1, SrcFileStream);

	// 行大小补齐4的倍数字节
	int LineBytes = (SrcFileInfo.biWidth + 3) / 4 * 4;

	//  读取分配场矩阵
	unsigned char** pSrcCellLineBuf = new unsigned char* [SrcFileInfo.biHeight];   // 源文件行缓冲
	for (i = 0; i < SrcFileInfo.biHeight; i++)
	{
		pSrcCellLineBuf[i] = new unsigned char[LineBytes];
		fread(pSrcCellLineBuf[i], sizeof(char), LineBytes, SrcFileStream);
	}
	fclose(SrcFileStream);

	// 2. 扫描分配场，获取点对信息存入点对数组
	// 点对数组
	unsigned char Clr1[500];
	unsigned char Clr2[500];
	int PtsPairNum = 0;

	// 扫描
	for (int y = 0; y < SrcFileInfo.biHeight; y++) // 2重循环，对每一个栅格
	{
		if ((y == 0) || (y == SrcFileInfo.biHeight - 1)) continue;
		for (int x = 1; x < SrcFileInfo.biWidth - 1; x++)
		{
			unsigned char currClr = pSrcCellLineBuf[y][x];

			unsigned char upClr = pSrcCellLineBuf[y + 1][x];
			unsigned char boClr = pSrcCellLineBuf[y - 1][x];
			unsigned char lfClr = pSrcCellLineBuf[y][x - 1];
			unsigned char rtClr = pSrcCellLineBuf[y][x + 1];

			// 找出4邻域中的异色
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

			// 查找点对数组中是否有重复项(注：这里用了尾部插入和顺序查找，应该用插入排序加二分查找效率更高）
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


	//  3. 点对数组写入文件
	FILE* DestFile = fopen(ptPairsFile, "wt");
	for (i = 0; i < PtsPairNum; i++)
		fprintf(DestFile, "%d\t%d\n", Clr1[i], Clr2[i]);
	fclose(DestFile);

	// 4. 扫尾
	for (i = 0; i < SrcFileInfo.biHeight; i++)
	{
		delete[] pSrcCellLineBuf[i];
	}
	delete[] pSrcCellLineBuf;

	return true; // 返回成功代码1
}

bool MAUtil::linkPts(const char* sourceFileName, const char* ptPairsFile, const char* colorsTableFile, const char* destFileName)
{

	// 1. 读取原图数据矩阵
	unsigned i, j;

	FILE* SrcFileStream = fopen(sourceFileName, "rb");

	BITMAPFILEHEADER SrcFileHead;
	BITMAPINFOHEADER SrcFileInfo;
	unsigned char ColorIdx[1024];

	fread(&SrcFileHead, sizeof(BITMAPFILEHEADER), 1, SrcFileStream);
	fread(&SrcFileInfo, sizeof(BITMAPINFOHEADER), 1, SrcFileStream);
	fread(ColorIdx, 1024, 1, SrcFileStream);

	// 行大小补齐4的倍数字节
	int LineBytes = (SrcFileInfo.biWidth + 3) / 4 * 4;

	//  读取源点场矩阵
	unsigned char** SrcMtx = new unsigned char* [SrcFileInfo.biHeight];   // 源文件行缓冲
	for (i = 0; i < SrcFileInfo.biHeight; i++)
	{
		SrcMtx[i] = new unsigned char[LineBytes];
		fread(SrcMtx[i], sizeof(char), LineBytes, SrcFileStream);
	}
	fclose(SrcFileStream);

	// 2. 读取初始点的坐标信息
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

	// 3. 读取点对信息
	FILE* PPairFile = fopen(ptPairsFile, "rt");
	while (!feof(PPairFile))
	{
		unsigned Clr1;
		unsigned Clr2;
		fscanf(PPairFile, "%d\t%d\n", &Clr1, &Clr2);

		// 4. 在数据矩阵中连接点对之间的直线
		int xa, ya, xb, yb;
		xa = TabX[Clr1];
		ya = TabY[Clr1];
		xb = TabX[Clr2];
		yb = TabY[Clr2];

		// DDA 连直线段
		int dx, dy, n, k; float xinc, yinc, x, y;
		dx = xb - xa; dy = yb - ya;
		if (abs(dx) > abs(dy))  n = abs(dx);
		else  n = abs(dy);
		xinc = (float)dx / n;   yinc = (float)dy / n;
		x = (float)xa; y = (float)ya;
		for (k = 1; k <= n; k++)
		{
			SrcMtx[int(y + 0.5)][int(x + 0.5)] = 0; //黑色;  
			x += xinc;    y += yinc;
		}
	}
	fclose(PPairFile);

	// 5. 存入Delauney三角网位图数据
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

	// 扫尾
	return true;
}

