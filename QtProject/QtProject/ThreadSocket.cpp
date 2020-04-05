
#include "ThreadSocket.h"
ThreadSocket::ThreadSocket(int index)
{
	m_index = index;
}

ThreadSocket::~ThreadSocket()
{

}
void ThreadSocket::run()
{
	MyStruct str;
	str.age = 10;
	str.name = "sss";
	while (true)
	{
		qDebug() << QString::number(m_index) << QThread::currentThreadId;
		emit send(str);
		sleep(1);
		 
	}
}

 