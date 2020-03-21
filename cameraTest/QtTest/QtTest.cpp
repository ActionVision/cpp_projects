#include "QtTest.h"
#include "QThreadWork.h"

QtTest::QtTest(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	QThreadWork *a = new QThreadWork(this,"aaaaa");
	a->start();

	/*QThreadWork *b = new QThreadWork(this, "bbbbb");
	b->start();*/
}
