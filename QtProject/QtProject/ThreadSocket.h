#pragma once
#include <qthread.h>
 
#include <qdebug.h>
#include <qmetatype.h>
#include "ThreadWork.h"

class ThreadSocket :public QThread
{
	Q_OBJECT
public :
	ThreadSocket(int index);
	~ThreadSocket();
 protected:
	void run();
signals:
	void send(MyStruct str);
private :
	int m_index;
};
 