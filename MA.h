#pragma once

#include <QtWidgets/QMainWindow>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qdatetime.h>
#include <qgraphicsview.h>
#include <qdebug.h>
#include "ui_MA.h"
#include "MAUtil.h"
#include "OctagonTmp.h"
#include "EuTmp.h"
#include "ManhattanTmp.h"
#include "ChessTmp.h"

class MA : public QMainWindow
{
	Q_OBJECT

public:
	MA(QWidget *parent = Q_NULLPTR);
	~MA();
private:
	// 输入文件路径（可能有多个文件）与对应的位数
	QString sourefilename;
	QString outfilename; // 输出文件路径
private:
	Ui::MAClass ui;
	int flag; // 叠置分析flag
	// 连接信号与槽
	void createActions();
private slots:
	bool readBmp();
	// 反色处理
	void bmpInverse();
	// 256 to 32
	void bmp256To32();
	// 均值滤波
	void averageFilter();
	// 中值滤波
	void midFilter();
	// 距离变换
	void distanceTransform();
	// 距离变换32bit
	void distanceTransform32b();
	// 障碍变换
	void barrierTransform();
	// V图提取
	void getVoronoi();
	// 缓冲区提取
	void getBuffer();
	// 加壳变换
	void shellAddTransform();
	// 蜕皮变换
	void shellMinusTransform();
	// 粘连变换
	void adhesionTransform();
	// 蚀断变换
	void corrosionTransform();
	// 提取delauney三角网
	void getDelauney();
};