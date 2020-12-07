#include "MA.h"

MA::MA(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	flag = 0;
	createActions();
}

MA::~MA() {

}
void MA::createActions() {
	connect(ui.actionopen, &QAction::triggered, this, &MA::readBmp);

}

bool MA::readBmp() {
	bool isok = false;
	QString message = tr("Bmp reading...");
	statusBar()->showMessage(message);
	QString sourefileName = QFileDialog::getOpenFileName(this, tr("file"), "", tr("*.bmp"));
	if (!sourefileName.isNull()) {
		QImage image;
		if (image.load(sourefileName)) {
			QGraphicsScene* scene = new QGraphicsScene;

			QPixmap pixmap = QPixmap::fromImage(image);
			if (ui.graphicsView_2->width() > ui.graphicsView_2->height()) {
				scene->addPixmap(pixmap.scaledToHeight(ui.graphicsView_2->height()));
			}
			else {
				scene->addPixmap(pixmap.scaledToWidth(ui.graphicsView_2->width()));
			}

			ui.graphicsView_2->setScene(scene);
			isok = true;
			sourefilename = sourefileName;
		}
	}
	return isok;
}

void MA::bmpInverse() {
	if (sourefilename.isEmpty())
	{
		QMessageBox::critical(NULL, QString::fromLocal8Bit("��ɫ"), QString::fromLocal8Bit("δ����ԭʼͼ��!"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	// ѡ������ļ���
	QString outdirectory = QFileDialog::getExistingDirectory(this, "select a directory...", "./");

	if (outdirectory.isEmpty())
	{
		return;
	}
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyyMMddhhmmss");
	outfilename = outdirectory + "/" + current_date + ".bmp";
	QByteArray ba = sourefilename.toLocal8Bit();
	const char* sourefileName = ba.data();
	QByteArray ba2 = outfilename.toLocal8Bit();
	const char* outfileName = ba2.data();
	if (MAUtil::inverseBmp(sourefileName, outfileName)) {
		QImage image;
		if (image.load(outfilename)) {
			QGraphicsScene* scene = new QGraphicsScene;

			QPixmap pixmap = QPixmap::fromImage(image);
			if (ui.graphicsView->width() > ui.graphicsView->height()) {
				scene->addPixmap(pixmap.scaledToHeight(ui.graphicsView->height()));
			}
			else {
				scene->addPixmap(pixmap.scaledToWidth(ui.graphicsView->width()));
			}
			ui.graphicsView->setScene(scene);
		}
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ɫ"), QString::fromLocal8Bit("��ɫ�ɹ���"), QMessageBox::Yes, QMessageBox::Yes);
	}
	else {
		QMessageBox::critical(NULL, QString::fromLocal8Bit("��ɫ"), QString::fromLocal8Bit("��ɫת��ʧ�ܣ�����!"), QMessageBox::Yes, QMessageBox::Yes);
	}
}

void MA::bmp256To32() {
	if (sourefilename.isEmpty())
	{
		QMessageBox::critical(NULL, QString::fromLocal8Bit("256 to 32"), QString::fromLocal8Bit("δ����ԭʼͼ��!"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	// ѡ������ļ���
	QString outdirectory = QFileDialog::getExistingDirectory(this, "select a directory...", "./");
	if (outdirectory.isEmpty())
	{
		return;
	}
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyyMMddhhmmss");
	outfilename = outdirectory + "/" + current_date + ".bmp";
	QByteArray ba = sourefilename.toLocal8Bit();
	const char* sourefileName = ba.data();
	QByteArray ba2 = outfilename.toLocal8Bit();
	const char* outfileName = ba2.data();
	if (MAUtil::bmp256to32b(sourefileName, outfileName)) {
		QImage image;
		if (image.load(outfilename)) {
			QGraphicsScene* scene = new QGraphicsScene;
			QPixmap pixmap = QPixmap::fromImage(image);
			if (ui.graphicsView->width() > ui.graphicsView->height()) {
				scene->addPixmap(pixmap.scaledToHeight(ui.graphicsView->height()));
			}
			else {
				scene->addPixmap(pixmap.scaledToWidth(ui.graphicsView->width()));
			}
			ui.graphicsView->setScene(scene);
		}
		QMessageBox::information(NULL, QString::fromLocal8Bit("256 to 32"), QString::fromLocal8Bit("ת���ɹ���"), QMessageBox::Yes, QMessageBox::Yes);
	}
	else {
		QMessageBox::critical(NULL, QString::fromLocal8Bit("256 to 32"), QString::fromLocal8Bit("ת��ʧ�ܣ�����!"), QMessageBox::Yes, QMessageBox::Yes);
	}

}

void MA::averageFilter()
{
	if (sourefilename.isEmpty())
	{
		QMessageBox::critical(NULL, QString::fromLocal8Bit("��ֵ�˲�"), QString::fromLocal8Bit("δ����ԭʼͼ��!"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	// ѡ������ļ���
	QString outdirectory = QFileDialog::getExistingDirectory(this, "select a directory...", "./");
	if (outdirectory.isEmpty())
	{
		return;
	}
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyyMMddhhmmss");
	outfilename = outdirectory + "/" + current_date + ".bmp";
	QByteArray ba = sourefilename.toLocal8Bit();
	const char* sourefileName = ba.data();
	QByteArray ba2 = outfilename.toLocal8Bit();
	const char* outfileName = ba2.data();
	if (MAUtil::averageFilter(sourefileName, outfileName)) {
		QImage image;
		if (image.load(outfilename)) {
			QGraphicsScene* scene = new QGraphicsScene;
			QPixmap pixmap = QPixmap::fromImage(image);
			if (ui.graphicsView->width() > ui.graphicsView->height()) {
				scene->addPixmap(pixmap.scaledToHeight(ui.graphicsView->height()));
			}
			else {
				scene->addPixmap(pixmap.scaledToWidth(ui.graphicsView->width()));
			}
			ui.graphicsView->setScene(scene);
		}
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ֵ�˲�"), QString::fromLocal8Bit("��ֵ�˲��ɹ���"), QMessageBox::Yes, QMessageBox::Yes);

	}
	else {
		QMessageBox::critical(NULL, QString::fromLocal8Bit("��ֵ�˲�"), QString::fromLocal8Bit("ƽ��ʧ�ܣ�����!"), QMessageBox::Yes, QMessageBox::Yes);
	}
}

void MA::midFilter()
{
	if (sourefilename.isEmpty())
	{
		QMessageBox::critical(NULL, QString::fromLocal8Bit("��ֵ�˲�"), QString::fromLocal8Bit("δ����ԭʼͼ��!"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	// ѡ������ļ���
	QString outdirectory = QFileDialog::getExistingDirectory(this, "select a directory...", "./");
	if (outdirectory.isEmpty())
	{
		return;
	}
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyyMMddhhmmss");
	outfilename = outdirectory + "/" + current_date + ".bmp";
	QByteArray ba = sourefilename.toLocal8Bit();
	const char* sourefileName = ba.data();
	QByteArray ba2 = outfilename.toLocal8Bit();
	const char* outfileName = ba2.data();
	if (MAUtil::midFilter(sourefileName, outfileName)) {
		QImage image;
		if (image.load(outfilename)) {
			QGraphicsScene* scene = new QGraphicsScene;
			QPixmap pixmap = QPixmap::fromImage(image);
			if (ui.graphicsView->width() > ui.graphicsView->height()) {
				scene->addPixmap(pixmap.scaledToHeight(ui.graphicsView->height()));
			}
			else {
				scene->addPixmap(pixmap.scaledToWidth(ui.graphicsView->width()));
			}
			ui.graphicsView->setScene(scene);
		}
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ֵ�˲�"), QString::fromLocal8Bit("��ֵ�˲��ɹ���"), QMessageBox::Yes, QMessageBox::Yes);

	}
	else {
		QMessageBox::critical(NULL, QString::fromLocal8Bit("��ֵ�˲�"), QString::fromLocal8Bit("ƽ��ʧ�ܣ�����!"), QMessageBox::Yes, QMessageBox::Yes);
	}
}

void MA::distanceTransform()
{
	if (sourefilename.isEmpty())
	{
		QMessageBox::critical(NULL, QString::fromLocal8Bit("�˱��ξ���任"), QString::fromLocal8Bit("δ����ԭʼͼ��!"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	// ѡ������ļ���
	QString outdirectory = QFileDialog::getExistingDirectory(this, "select a directory...", "./");
	if (outdirectory.isEmpty())
	{
		return;
	}
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyyMMddhhmmss");
	outfilename = outdirectory + "/" + current_date + "dis.bmp";
	QString outfilename2 = outdirectory + "/" + current_date + "loc.bmp";
	QByteArray ba = sourefilename.toLocal8Bit();
	const char* sourefileName = ba.data();
	QByteArray ba2 = outfilename.toLocal8Bit();
	const char* outfileName = ba2.data();
	QByteArray ba3 = outfilename2.toLocal8Bit();
	const char* outfileName2 = ba3.data();
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	CDistanceTmp* temp;
	qDebug() << btn->text();
	QString btnText = btn->text();
	if (btnText.compare(QString::fromLocal8Bit("�˱��ξ���任")) == 0) {
		temp = new OctagonTmp();
	}
	else if (btnText.compare(QString::fromLocal8Bit("�����پ���任")) == 0) {
		temp = new CManhattanTmp();
	}
	else if (btnText.compare(QString::fromLocal8Bit("ŷʽ����任(13*13)")) == 0) {
		temp = new CEuTmp(13);
	}
	else if (btnText.compare(QString::fromLocal8Bit("ŷʽ����任(5*5)")) == 0) {
		temp = new CEuTmp(5);
	}
	else if (btnText.compare(QString::fromLocal8Bit("���̾���任")) == 0) {
		temp = new CChessTmp(5);
	}

	if (MAUtil::distanceTransform(sourefileName, outfileName, outfileName2, temp)) {
		QImage image;
		if (image.load(outfilename)) {
			QGraphicsScene* scene = new QGraphicsScene;
			QPixmap pixmap = QPixmap::fromImage(image);
			if (ui.graphicsView->width() > ui.graphicsView->height()) {
				scene->addPixmap(pixmap.scaledToHeight(ui.graphicsView->height()));
			}
			else {
				scene->addPixmap(pixmap.scaledToWidth(ui.graphicsView->width()));
			}
			ui.graphicsView->setScene(scene);
		}
		QMessageBox::information(NULL, btnText, QString::fromLocal8Bit("����任�ɹ���"), QMessageBox::Yes, QMessageBox::Yes);

	}
	else {
		QMessageBox::critical(NULL, btnText, QString::fromLocal8Bit("����任ʧ�ܣ�����!"), QMessageBox::Yes, QMessageBox::Yes);
	}
}

void MA::distanceTransform32b()
{
	if (sourefilename.isEmpty())
	{
		QMessageBox::critical(NULL, QString::fromLocal8Bit("�˱��ξ���任32bit"), QString::fromLocal8Bit("δ����ԭʼͼ��!"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	// ѡ������ļ���
	QString outdirectory = QFileDialog::getExistingDirectory(this, "select a directory...", "./");
	if (outdirectory.isEmpty())
	{
		return;
	}
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyyMMddhhmmss");
	outfilename = outdirectory + "/" + current_date + "dis32b.bmp";
	QString outfilename2 = outdirectory + "/" + current_date + "loc32b.bmp";
	QByteArray ba = sourefilename.toLocal8Bit();
	const char* sourefileName = ba.data();
	QByteArray ba2 = outfilename.toLocal8Bit();
	const char* outfileName = ba2.data();
	QByteArray ba3 = outfilename2.toLocal8Bit();
	const char* outfileName2 = ba3.data();
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	CDistanceTmp* temp;
	qDebug() << btn->text();
	QString btnText = btn->text();
	if (btnText.compare(QString::fromLocal8Bit("�˱��ξ���任32bit")) == 0) {
		temp = new OctagonTmp();
	}

	if (MAUtil::distanceTransform32b(sourefileName, outfileName, outfileName2, temp)) {
		QImage image;
		if (image.load(outfilename)) {
			QGraphicsScene* scene = new QGraphicsScene;
			QPixmap pixmap = QPixmap::fromImage(image);
			if (ui.graphicsView->width() > ui.graphicsView->height()) {
				scene->addPixmap(pixmap.scaledToHeight(ui.graphicsView->height()));
			}
			else {
				scene->addPixmap(pixmap.scaledToWidth(ui.graphicsView->width()));
			}
			ui.graphicsView->setScene(scene);
		}
		QMessageBox::information(NULL, btnText, QString::fromLocal8Bit("�˱��ξ���任�ɹ���"), QMessageBox::Yes, QMessageBox::Yes);

	}
	else {
		QMessageBox::critical(NULL, btnText, QString::fromLocal8Bit("�˱��ξ���任ʧ�ܣ�����!"), QMessageBox::Yes, QMessageBox::Yes);
	}
}

void MA::barrierTransform()
{
	if (sourefilename.isEmpty())
	{
		QMessageBox::critical(NULL, QString::fromLocal8Bit("�ϰ�ŷʽ����任"), QString::fromLocal8Bit("δ����ԭʼͼ��!"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	// ѡ������ļ���
	QString outdirectory = QFileDialog::getExistingDirectory(this, "select a directory...", "./");
	if (outdirectory.isEmpty())
	{
		return;
	}
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyyMMddhhmmss");
	outfilename = outdirectory + "/" + current_date + "dis.bmp";
	QString outfilename2 = outdirectory + "/" + current_date + "loc.bmp";
	QByteArray ba = sourefilename.toLocal8Bit();
	const char* sourefileName = ba.data();
	QByteArray ba2 = outfilename.toLocal8Bit();
	const char* outfileName = ba2.data();
	QByteArray ba3 = outfilename2.toLocal8Bit();
	const char* outfileName2 = ba3.data();
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	CDistanceTmp* temp;
	qDebug() << btn->text();
	QString btnText = btn->text();
	if (btnText.compare(QString::fromLocal8Bit("�ϰ�ŷʽ����任")) == 0) {
		temp = new CEuTmp(5);
	}
	/*else if (btnText.compare(QString::fromLocal8Bit("�����پ���任")) == 0) {
		temp = new CManhattanTmp();
	}
	else if (btnText.compare(QString::fromLocal8Bit("ŷʽ����任(13*13)")) == 0) {
		temp = new CEuTmp(13);
	}
	else if (btnText.compare(QString::fromLocal8Bit("ŷʽ����任(5*5)")) == 0) {
		temp = new CEuTmp(5);
	}
	else if (btnText.compare(QString::fromLocal8Bit("���̾���任")) == 0) {
		temp = new CChessTmp(5);
	}*/

	if (MAUtil::barrierDistanceTransform(sourefileName, outfileName, outfileName2, temp)) {
		QImage image;
		if (image.load(outfilename)) {
			QGraphicsScene* scene = new QGraphicsScene;
			QPixmap pixmap = QPixmap::fromImage(image);
			if (ui.graphicsView->width() > ui.graphicsView->height()) {
				scene->addPixmap(pixmap.scaledToHeight(ui.graphicsView->height()));
			}
			else {
				scene->addPixmap(pixmap.scaledToWidth(ui.graphicsView->width()));
			}
			ui.graphicsView->setScene(scene);
		}
		QMessageBox::information(NULL, btnText, QString::fromLocal8Bit("�ϰ�����任�ɹ���"), QMessageBox::Yes, QMessageBox::Yes);

	}
	else {
		QMessageBox::critical(NULL, btnText, QString::fromLocal8Bit("����任ʧ�ܣ�����!"), QMessageBox::Yes, QMessageBox::Yes);
	}
}

void MA::getVoronoi()
{
	if (sourefilename.isEmpty())
	{
		QMessageBox::critical(NULL, QString::fromLocal8Bit("Vͼ����"), QString::fromLocal8Bit("δ����ԭʼͼ��!"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	// ѡ������ļ���
	QString outdirectory = QFileDialog::getExistingDirectory(this, "select a directory...", "./");
	if (outdirectory.isEmpty())
	{
		return;
	}
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyyMMddhhmmss");
	outfilename = outdirectory + "/" + current_date + ".bmp";
	QByteArray ba = sourefilename.toLocal8Bit();
	const char* sourefileName = ba.data();
	QByteArray ba2 = outfilename.toLocal8Bit();
	const char* outfileName = ba2.data();
	if (MAUtil::getBdyBmp(sourefileName, outfileName)) {
		QImage image;
		if (image.load(outfilename)) {
			QGraphicsScene* scene = new QGraphicsScene;
			QPixmap pixmap = QPixmap::fromImage(image);
			if (ui.graphicsView->width() > ui.graphicsView->height()) {
				scene->addPixmap(pixmap.scaledToHeight(ui.graphicsView->height()));
			}
			else {
				scene->addPixmap(pixmap.scaledToWidth(ui.graphicsView->width()));
			}
			ui.graphicsView->setScene(scene);
		}
		QMessageBox::information(NULL, QString::fromLocal8Bit("Vͼ����"), QString::fromLocal8Bit("Vͼ���ɳɹ���"), QMessageBox::Yes, QMessageBox::Yes);

	}
	else {
		QMessageBox::critical(NULL, QString::fromLocal8Bit("Vͼ����"), QString::fromLocal8Bit("����ʧ�ܣ�����!"), QMessageBox::Yes, QMessageBox::Yes);
	}
}

void MA::getBuffer()
{
	if (sourefilename.isEmpty())
	{
		QMessageBox::critical(NULL, QString::fromLocal8Bit("��ȡ������"), QString::fromLocal8Bit("δ����ԭʼͼ��!"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	// ѡ������ļ���
	QString outdirectory = QFileDialog::getExistingDirectory(this, "select a directory...", "./");
	if (outdirectory.isEmpty())
	{
		return;
	}
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyyMMddhhmmss");
	outfilename = outdirectory + "/" + current_date + ".bmp";
	QByteArray ba = sourefilename.toLocal8Bit();
	const char* sourefileName = ba.data();
	QByteArray ba2 = outfilename.toLocal8Bit();
	const char* outfileName = ba2.data();
	float bufferWidth = 20;
	if (MAUtil::getBufferBmp(sourefileName, bufferWidth, outfileName)) {
		QImage image;
		if (image.load(outfilename)) {
			QGraphicsScene* scene = new QGraphicsScene;
			QPixmap pixmap = QPixmap::fromImage(image);
			if (ui.graphicsView->width() > ui.graphicsView->height()) {
				scene->addPixmap(pixmap.scaledToHeight(ui.graphicsView->height()));
			}
			else {
				scene->addPixmap(pixmap.scaledToWidth(ui.graphicsView->width()));
			}
			ui.graphicsView->setScene(scene);
		}
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ȡ������"), QString::fromLocal8Bit("��ȡ�������ɹ���"), QMessageBox::Yes, QMessageBox::Yes);

	}
	else {
		QMessageBox::critical(NULL, QString::fromLocal8Bit("��ȡ������"), QString::fromLocal8Bit("��ȡ������ʧ�ܣ�����!"), QMessageBox::Yes, QMessageBox::Yes);
	}
}

void MA::shellAddTransform()
{
	if (sourefilename.isEmpty())
	{
		QMessageBox::critical(NULL, QString::fromLocal8Bit("�ӿǱ任"), QString::fromLocal8Bit("δ����ԭʼͼ��!"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	// ѡ������ļ���
	QString outdirectory = QFileDialog::getExistingDirectory(this, "select a directory...", "./");
	if (outdirectory.isEmpty())
	{
		return;
	}
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyyMMddhhmmss");
	outfilename = outdirectory + "/" + current_date + ".bmp";
	QByteArray ba = sourefilename.toLocal8Bit();
	const char* sourefileName = ba.data();
	QByteArray ba2 = outfilename.toLocal8Bit();
	const char* outfileName = ba2.data();
	float bufferWidth = 10;
	if (MAUtil::shellAddTransform(sourefileName, bufferWidth, outfileName)) {
		QImage image;
		if (image.load(outfilename)) {
			QGraphicsScene* scene = new QGraphicsScene;
			QPixmap pixmap = QPixmap::fromImage(image);
			if (ui.graphicsView->width() > ui.graphicsView->height()) {
				scene->addPixmap(pixmap.scaledToHeight(ui.graphicsView->height()));
			}
			else {
				scene->addPixmap(pixmap.scaledToWidth(ui.graphicsView->width()));
			}
			ui.graphicsView->setScene(scene);
		}
		QMessageBox::information(NULL, QString::fromLocal8Bit("�ӿǱ任"), QString::fromLocal8Bit("�ӿǱ任�ɹ���"), QMessageBox::Yes, QMessageBox::Yes);

	}
	else {
		QMessageBox::critical(NULL, QString::fromLocal8Bit("�ӿǱ任"), QString::fromLocal8Bit("�ӿǱ任ʧ�ܣ�����!"), QMessageBox::Yes, QMessageBox::Yes);
	}
}

void MA::shellMinusTransform()
{
	if (sourefilename.isEmpty())
	{
		QMessageBox::critical(NULL, QString::fromLocal8Bit("��Ƥ�任"), QString::fromLocal8Bit("δ����ԭʼͼ��!"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	// ѡ������ļ���
	QString outdirectory = QFileDialog::getExistingDirectory(this, "select a directory...", "./");
	if (outdirectory.isEmpty())
	{
		return;
	}
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyyMMddhhmmss");
	outfilename = outdirectory + "/" + current_date + ".bmp";
	QByteArray ba = sourefilename.toLocal8Bit();
	const char* sourefileName = ba.data();
	QByteArray ba2 = outfilename.toLocal8Bit();
	const char* outfileName = ba2.data();
	float bufferWidth = 10;
	if (MAUtil::shellMinusTransform(sourefileName, bufferWidth, outfileName)) {
		QImage image;
		if (image.load(outfilename)) {
			QGraphicsScene* scene = new QGraphicsScene;
			QPixmap pixmap = QPixmap::fromImage(image);
			if (ui.graphicsView->width() > ui.graphicsView->height()) {
				scene->addPixmap(pixmap.scaledToHeight(ui.graphicsView->height()));
			}
			else {
				scene->addPixmap(pixmap.scaledToWidth(ui.graphicsView->width()));
			}
			ui.graphicsView->setScene(scene);
		}
		QMessageBox::information(NULL, QString::fromLocal8Bit("��Ƥ�任"), QString::fromLocal8Bit("��Ƥ�任�ɹ���"), QMessageBox::Yes, QMessageBox::Yes);

	}
	else {
		QMessageBox::critical(NULL, QString::fromLocal8Bit("��Ƥ�任"), QString::fromLocal8Bit("��Ƥ�任ʧ�ܣ�����!"), QMessageBox::Yes, QMessageBox::Yes);
	}
}

void MA::adhesionTransform()
{
	if (sourefilename.isEmpty())
	{
		QMessageBox::critical(NULL, QString::fromLocal8Bit("ճ���任"), QString::fromLocal8Bit("δ����ԭʼͼ��!"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	// ѡ������ļ���
	QString outdirectory = QFileDialog::getExistingDirectory(this, "select a directory...", "./");
	if (outdirectory.isEmpty())
	{
		return;
	}
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyyMMddhhmmss");
	outfilename = outdirectory + "/" + current_date + "Adhesion.bmp";
	QString disFile = outdirectory + "/" + current_date + "dis.bmp";
	QString locFile = outdirectory + "/" + current_date + "loc.bmp";
	QByteArray ba = sourefilename.toLocal8Bit();
	const char* sourefileName = ba.data();

	QByteArray ba2 = outfilename.toLocal8Bit();
	const char* outfileName = ba2.data();

	QByteArray ba3 = disFile.toLocal8Bit();
	const char* DisFile = ba3.data();

	QByteArray ba4 = locFile.toLocal8Bit();
	const char* LocFile = ba4.data();

	CDistanceTmp* temp = new CEuTmp(5);
	int bufferWidth = 5;
	if (MAUtil::adhesionTransform(sourefileName, bufferWidth,DisFile,LocFile,temp,outfileName)) {
		QImage image;
		if (image.load(outfilename)) {
			QGraphicsScene* scene = new QGraphicsScene;
			QPixmap pixmap = QPixmap::fromImage(image);
			if (ui.graphicsView->width() > ui.graphicsView->height()) {
				scene->addPixmap(pixmap.scaledToHeight(ui.graphicsView->height()));
			}
			else {
				scene->addPixmap(pixmap.scaledToWidth(ui.graphicsView->width()));
			}
			ui.graphicsView->setScene(scene);
		}
		QMessageBox::information(NULL, QString::fromLocal8Bit("ճ���任"), QString::fromLocal8Bit("ճ���任�ɹ���"), QMessageBox::Yes, QMessageBox::Yes);

	}
	else {
		QMessageBox::critical(NULL, QString::fromLocal8Bit("ճ���任"), QString::fromLocal8Bit("ճ���任ʧ�ܣ�����!"), QMessageBox::Yes, QMessageBox::Yes);
	}
}

void MA::corrosionTransform()
{
	if (sourefilename.isEmpty())
	{
		QMessageBox::critical(NULL, QString::fromLocal8Bit("ʴ�ϱ任"), QString::fromLocal8Bit("δ����ԭʼͼ��!"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	// ѡ������ļ���
	QString outdirectory = QFileDialog::getExistingDirectory(this, "select a directory...", "./");
	if (outdirectory.isEmpty())
	{
		return;
	}
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyyMMddhhmmss");
	outfilename = outdirectory + "/" + current_date + "Adhesion.bmp";
	QString disFile = outdirectory + "/" + current_date + "dis.bmp";
	QString locFile = outdirectory + "/" + current_date + "loc.bmp";
	QByteArray ba = sourefilename.toLocal8Bit();
	const char* sourefileName = ba.data();

	QByteArray ba2 = outfilename.toLocal8Bit();
	const char* outfileName = ba2.data();

	QByteArray ba3 = disFile.toLocal8Bit();
	const char* DisFile = ba3.data();

	QByteArray ba4 = locFile.toLocal8Bit();
	const char* LocFile = ba4.data();

	CDistanceTmp* temp = new CEuTmp(5);
	int bufferWidth = 5;
	if (MAUtil::corrosionTransform(sourefileName, bufferWidth, DisFile, LocFile, temp, outfileName)) {
		QImage image;
		if (image.load(outfilename)) {
			QGraphicsScene* scene = new QGraphicsScene;
			QPixmap pixmap = QPixmap::fromImage(image);
			if (ui.graphicsView->width() > ui.graphicsView->height()) {
				scene->addPixmap(pixmap.scaledToHeight(ui.graphicsView->height()));
			}
			else {
				scene->addPixmap(pixmap.scaledToWidth(ui.graphicsView->width()));
			}
			ui.graphicsView->setScene(scene);
		}
		QMessageBox::information(NULL, QString::fromLocal8Bit("ʴ�ϱ任"), QString::fromLocal8Bit("ʴ�ϱ任�ɹ���"), QMessageBox::Yes, QMessageBox::Yes);

	}
	else {
		QMessageBox::critical(NULL, QString::fromLocal8Bit("ʴ�ϱ任"), QString::fromLocal8Bit("ʴ�ϱ任ʧ�ܣ�����!"), QMessageBox::Yes, QMessageBox::Yes);
	}
}

void MA::getDelauney()
{
	if (sourefilename.isEmpty())
	{
		QMessageBox::critical(NULL, QString::fromLocal8Bit("��ȡ������"), QString::fromLocal8Bit("δ����ԭʼͼ��!"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	// ѡ������ļ���
	QString outdirectory = QFileDialog::getExistingDirectory(this, "select a directory...", "./");
	if (outdirectory.isEmpty())
	{
		return;
	}
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyyMMddhhmmss");
	QString colortableFile = outdirectory + "/" + current_date + "ColorsTab.txt";
	QString disFile = outdirectory + "/" + current_date + "dis.bmp";
	QString locFile = outdirectory + "/" + current_date + "loc.bmp";
	QString tinPtPairs = outdirectory + "/" + current_date + "TINPtPairs.txt";

	outfilename = outdirectory + "/" + current_date + ".bmp";
	QByteArray ba = sourefilename.toLocal8Bit();
	const char* sourefileName = ba.data();
	QByteArray ba2 = colortableFile.toLocal8Bit();
	const char* ColortableFile = ba2.data();
	QByteArray ba3 = disFile.toLocal8Bit();
	const char* DisFile = ba3.data();
	QByteArray ba4 = locFile.toLocal8Bit();
	const char* LocFile = ba4.data();
	QByteArray ba5 = tinPtPairs.toLocal8Bit();
	const char* TinPtPairs = ba5.data();
	QByteArray ba6 = outfilename.toLocal8Bit();
	const char* outfileName = ba6.data();
	CDistanceTmp* temp = new OctagonTmp();
	if (MAUtil::scanSrcPtCoors(sourefileName, ColortableFile))
		if (MAUtil::distanceTransform32b(sourefileName, DisFile, LocFile, temp))
			if (MAUtil::getTinPtPairs(LocFile, TinPtPairs))
				if (MAUtil::linkPts(sourefileName, TinPtPairs, ColortableFile, outfileName))
				{
					QImage image;
					if (image.load(outfilename)) {
						QGraphicsScene* scene = new QGraphicsScene;
						QPixmap pixmap = QPixmap::fromImage(image);
						if (ui.graphicsView->width() > ui.graphicsView->height()) {
							scene->addPixmap(pixmap.scaledToHeight(ui.graphicsView->height()));
						}
						else {
							scene->addPixmap(pixmap.scaledToWidth(ui.graphicsView->width()));
						}
						ui.graphicsView->setScene(scene);
					}
					QMessageBox::information(NULL, QString::fromLocal8Bit("��ȡ������"), QString::fromLocal8Bit("��ȡ�������ɹ���"), QMessageBox::Yes, QMessageBox::Yes);

				}
				else {
					QMessageBox::critical(NULL, QString::fromLocal8Bit("��ȡ������"), QString::fromLocal8Bit("��ȡ������ʧ�ܣ�����!"), QMessageBox::Yes, QMessageBox::Yes);
				}
			else {
				QMessageBox::critical(NULL, QString::fromLocal8Bit("��ȡ������"), QString::fromLocal8Bit("��ȡ������ʧ�ܣ�����!"), QMessageBox::Yes, QMessageBox::Yes);

			}
		else {
			QMessageBox::critical(NULL, QString::fromLocal8Bit("��ȡ������"), QString::fromLocal8Bit("��ȡ������ʧ�ܣ�����!"), QMessageBox::Yes, QMessageBox::Yes);

		}
	else {
		QMessageBox::critical(NULL, QString::fromLocal8Bit("��ȡ������"), QString::fromLocal8Bit("��ȡ������ʧ�ܣ�����!"), QMessageBox::Yes, QMessageBox::Yes);

	}
}

