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
	ThreadWork(int index);
	~ThreadWork();
 
protected:
	void run();
private slots:
	void receive_msg(MyStruct str);
private:
	int m_index;
};
 
