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
	// �����ļ�·���������ж���ļ������Ӧ��λ��
	QString sourefilename;
	QString outfilename; // ����ļ�·��
private:
	Ui::MAClass ui;
	int flag; // ���÷���flag
	// �����ź����
	void createActions();
private slots:
	bool readBmp();
	// ��ɫ����
	void bmpInverse();
	// 256 to 32
	void bmp256To32();
	// ��ֵ�˲�
	void averageFilter();
	// ��ֵ�˲�
	void midFilter();
	// ����任
	void distanceTransform();
	// ����任32bit
	void distanceTransform32b();
	// �ϰ��任
	void barrierTransform();
	// Vͼ��ȡ
	void getVoronoi();
	// ��������ȡ
	void getBuffer();
	// �ӿǱ任
	void shellAddTransform();
	// ��Ƥ�任
	void shellMinusTransform();
	// ճ���任
	void adhesionTransform();
	// ʴ�ϱ任
	void corrosionTransform();
	// ��ȡdelauney������
	void getDelauney();
};