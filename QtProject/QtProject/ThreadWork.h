#pragma once
#include <qthread.h>
//#include "QtProject.h"
#include <qdebug.h>
#include <qmetatype.h>
struct MyStruct
{
	QString name;
	int age;
};
Q_DECLARE_METATYPE(MyStruct);

class ThreadWork :public QThread
{
public :
	ThreadWork();
	~ThreadWork();
	void run();
public slots:
	void receive_msg(QString str);
};
