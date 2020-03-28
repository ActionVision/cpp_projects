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
		sleep(100);
	}
}
void ThreadWork::receive_msg(QString str)
{
	qDebug() << "str.age";
}