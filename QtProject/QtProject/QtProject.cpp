#include "QtProject.h"

QtProject::QtProject(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	//test_DlgMatchShape();
	//m_myPaint = new MyPaint();
	//m_myPaint->show();
	//return;
	//ui.label->setStyleSheet("background-color:green");
	QWidget* p = takeCentralWidget();
	if (p)
		delete p;
	//允许嵌套dock
	setDockNestingEnabled(true);
	//记录所有的dock指针
	//m_docks.append(ui.dockWidget_A);
	//m_docks.append(ui.dockWidget_B);

	//showDock(m_docks);
	splitDockWidget(ui.dockWidget_D, ui.dockWidget_E, Qt::Horizontal);
	splitDockWidget(ui.dockWidget_E, ui.dockWidget_A, Qt::Horizontal);
	splitDockWidget(ui.dockWidget_A, ui.dockWidget_C, Qt::Horizontal);
	splitDockWidget(ui.dockWidget_D, ui.dockWidget_B, Qt::Vertical);
	
	
	
	
	qRegisterMetaType<MyStruct>("MyStruct");
	m_info.age = 10;
	m_info.name = "hhkk";
	m_ThreadWork = new ThreadWork();
	
	bool r=connect(ui.Btn_SendMessage, SIGNAL(clicked(bool)), this, SLOT(sendMsg()));
	bool ret = connect(this, SIGNAL(signalMystruct(MyStruct)), m_ThreadWork, SLOT(receive_msg(MyStruct)));//,Qt::DirectConnection;
	ret = connect(m_ThreadWork, SIGNAL(send_msg(MyStruct)), this, SLOT(receive_msg1(MyStruct)));
 
	m_ThreadWork->start();
	return;
}
void  QtProject::dockA()
{
	qDebug() << "A MOVE";
	//addDockWidget(Qt::LeftDockWidgetArea, ui.dockWidget_A);
	//addDockWidget(Qt::RightDockWidgetArea, ui.dockWidget_B);
	//splitDockWidget(ui.dockWidget_A, ui.dockWidget_B, Qt::Horizontal);

}
void  QtProject::dockB()
{
	qDebug() << "B MOVE";
	//addDockWidget(Qt::TopDockWidgetArea, ui.dockWidget_A);
	//addDockWidget(Qt::BottomDockWidgetArea, ui.dockWidget_B);
}
void QtProject::removeAllDock()
{
	for (int i = 0;i<2;++i)
	{
		removeDockWidget(m_docks[i]);
	}
}

void QtProject::showDock(const QList<int> &index)
{
	if (index.isEmpty())
	{
		for (int i = 0;i<2;++i)
		{
			m_docks[i]->show();
		}
	}
	else
	{
		foreach(int i, index) 
		{
			m_docks[i]->show();
		}
	}
}

void QtProject::mousePressEvent(QMouseEvent *event)
{
	
}

void QtProject::mouseMoveEvent(QMouseEvent *event)
{

}

void QtProject::dragEnterEvent(QDragEnterEvent *event)
{
	
}

void QtProject::dragMoveEvent(QDragMoveEvent *event)
{
	
}

void QtProject::dropEvent(QDropEvent *event)
{

}



void QtProject::sendMsg()
{
	m_info.age = m_info.age + 1;
	emit signalMystruct(m_info);
}
void QtProject::receive_msg1(MyStruct STR)
{
	qDebug() << "main ui get:"<<STR.age<<QString::number(STR.age);
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