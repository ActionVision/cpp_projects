#pragma once
#include <qthread.h>
 
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
	Q_OBJECT
public :
	ThreadWork();
	~ThreadWork();
protected:
	void run();
signals:
	void send_msg(MyStruct str);
	private slots:
	void receive_msg(MyStruct str);
};
