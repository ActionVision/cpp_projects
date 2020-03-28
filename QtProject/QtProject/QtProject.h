#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtProject.h"
#include <qstring.h>
#include "ThreadWork.h"
#include "DlgMatchShape.h"

class QtProject : public QMainWindow
{
	Q_OBJECT

public:
	QtProject(QWidget *parent = Q_NULLPTR);
	ThreadWork *m_ThreadWork;
	void drawRedCircle();

protected:
	bool eventFilter(QObject *watched, QEvent *event);


private:
	Ui::QtProjectClass ui;
	MyStruct m_info;
	void test_DlgMatchShape();
	DlgMatchShape* m_DlgMatchShape;

signals:
	void signalMystruct(QString str);
private slots:
void   sendMsg();
};
