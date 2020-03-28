#include "QtProject.h"

QtProject::QtProject(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	test_DlgMatchShape();

	//ui.label->setStyleSheet("background-color:green");
	return;

	qRegisterMetaType<MyStruct>("MyStruct");
	m_info.age = 10;
	m_info.name = "hhkk";
	m_ThreadWork = new ThreadWork();
	
	bool r=connect(ui.Btn_SendMessage, SIGNAL(clicked(bool)), this, SLOT(sendMsg()));
	r=connect(this, SIGNAL(signalMystruct(QString)), m_ThreadWork, SLOT(receive_msg(QString)),Qt::DirectConnection);
	m_ThreadWork->start();
}
void QtProject::sendMsg()
{
	m_info.age = m_info.age + 1;
	//emit signalMystruct(m_info);
}
void QtProject::test_DlgMatchShape()
{
	m_DlgMatchShape = new DlgMatchShape();
	m_DlgMatchShape->show();
}


bool QtProject::eventFilter(QObject *watched, QEvent *event)
{
	//if (watched == ui.label && event->type() == QEvent::Paint)
		drawRedCircle();
	//return QWidget::eventFilter(watched, event);
		return true;
}

void QtProject::drawRedCircle()
{
	 
	QPainter painter(ui.label);
	painter.setPen(Qt::NoPen);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setBrush(QColor(241, 50, 50));
	painter.drawEllipse(27, 12, 140, 140);
}