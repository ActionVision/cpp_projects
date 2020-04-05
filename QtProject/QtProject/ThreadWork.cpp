
#include "ThreadWork.h"
ThreadWork::ThreadWork(int index)
{
	m_index = index;
}

ThreadWork::~ThreadWork()
{

}
void ThreadWork::run()
{
	while (true)
	{
		//qDebug() << "RUNNNING";
		sleep(1);
	}
}

void ThreadWork::receive_msg(MyStruct str)
{
	qDebug() << QString::number(m_index)<< "ThreadWork receive:"<< QThread::currentThreadId <<str.name<<QString::number(str.age);
	MyStruct temp;
	temp.name = str.name;
	temp.age = str.age + 1;
 
}
