#include "MA.h"
#include <QtWidgets/QApplication>
#include <qfile.h>
#include <qtextstream.h>
#include "MAUtil.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	// qss�ļ�
	QFile f(":qdarkstyle/style.qss");
	if (!f.exists())
	{
		printf("Unable to set stylesheet, file not found\n");
	}
	else
	{
		f.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&f);
		// ������ʽ��
		qApp->setStyleSheet(ts.readAll());
	}
	// ����icon
	qApp->setWindowIcon(QIcon(":image/address.svg"));
	MA w;
	w.show();
	CDistanceTmp* tmp = new OctagonTmp();
	//MAUtil::distanceTransformIn("F:\\cworkspace\\MA\\256.bmp","F:\\cworkspace\\MA\\dis.bmp", "F:\\cworkspace\\MA\\MA\\dis.bmp", tmp);
	//BmpInverse("stickTF.bmp", "test.bmp");
	return a.exec();
}
