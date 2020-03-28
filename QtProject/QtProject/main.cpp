#include "QtProject.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtProject w;
	w.show();
	return a.exec();
}
