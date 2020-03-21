#pragma once

#include <QThread>
#include <qdebug.h>
#include "Cmessagebox.h"

class QThreadWork : public QThread
{
	Q_OBJECT

public:
	QThreadWork(QObject *parent,QString msg);
	~QThreadWork();
	void run();
private:
	QString m_msg;
};
