
#include "ThreadWork.h"
ThreadWork::ThreadWork()
{

}

ThreadWork::~ThreadWork()
{

}
void ThreadWork::run()
{
	while (true)
	{
		qDebug() << "RUNNNING";
		sleep(1);
	}
}

void ThreadWork::receive_msg(MyStruct str)
{
	qDebug() << "ThreadWork receive:"<<str.name<<QString::number(str.age);
	MyStruct temp;
	temp.name = str.name;
	temp.age = str.age + 1;
	emit send_msg(temp);
}
