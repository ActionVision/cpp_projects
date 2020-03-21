#include "Cmessagebox.h"
#include "qtimer.h"
#include "QtTest.h"

MessageBoxMT::MessageBoxMT(const QString &title, const QString &message) :
	m_title(title),
	m_message(message)
{ }

void MessageBoxMT::show(const QString &title, const QString &message)
{
	QEventLoop eventLoop;
	auto messageBox = new MessageBoxMT(title, message);
	connect(messageBox, SIGNAL(destroyed()), &eventLoop, SLOT(quit()));
	messageBox->readyShow();
	eventLoop.exec();
}

void MessageBoxMT::readyShow(void)
{
	this->moveToThread(qApp->thread());
	QTimer::singleShot(0, this, SLOT(onShow()));
}

void MessageBoxMT::onShow(void)
{
	QMessageBox::information(NULL, m_title, m_message);
	this->deleteLater();
}