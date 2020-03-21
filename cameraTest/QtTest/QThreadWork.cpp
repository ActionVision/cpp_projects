#include "QThreadWork.h"
#include <qthread>
#include <qmessagebox.h>

QThreadWork::QThreadWork(QObject *parent,QString msg)
	: QThread(parent)
{
	m_msg = msg;
}

QThreadWork::~QThreadWork()
{

}
void QThreadWork::run()
{
	while (true)
	{
		qDebug()<<m_msg;
		MessageBoxMT::show("waring", m_msg);// ����OK
		//QMessageBox::information(NULL, "WARNING", "AAAA"); //���������
		sleep(0.1);
		qDebug() << "       \n";
	}
}
