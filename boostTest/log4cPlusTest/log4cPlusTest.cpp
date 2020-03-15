// log4cPlusTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <log4cplus/logger.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/initializer.h>
#include <conio.h>
#include <configurator.h>
using namespace std;

using namespace log4cplus;
using namespace log4cplus::helpers;

void test_log4cplus()
{
	/* step 1: Instantiate an appender object */
	SharedAppenderPtr _append(new ConsoleAppender());
	_append->setName("append test");
	/* step 4: Instantiate a logger object */
	Logger _logger = Logger::getInstance("test");
	/* step 5: Attach the appender object to the logger  */
	_logger.addAppender(_append);
	/* log activity */
	LOG4CPLUS_DEBUG(_logger, "This is the FIRST log message...");
	LOG4CPLUS_WARN(_logger, "This is the SECOND log message...");

	/*PropertyConfigurator::doConfigure("d:\\log4cplus.cfg");
	Logger logger = Logger::getRoot();
	LOG4CPLUS_DEBUG(logger, "This is a DEBUG message");
	LOG4CPLUS_INFO(logger, "This is a INFO message");
	LOG4CPLUS_WARN(logger, "This is a WARN message");
	LOG4CPLUS_ERROR(logger, "This is a ERROR message");
	LOG4CPLUS_FATAL(logger, "This is a FATAL message");*/
}

int main()
{
    return 0;
}

