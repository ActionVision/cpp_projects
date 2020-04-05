#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtProject.h"
#include <qstring.h>
#include "ThreadWork.h"
#include "DlgMatchShape.h"
#include "mypaint.h"

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
	MyPaint*         m_myPaint;

signals:
	void signalMystruct(QString str);
private slots:
void   sendMsg();
void  dockA();
void  dockB();

public:
void removeAllDock();
void showDock(const QList<int>& index = QList<int>());
 
private :
	QList<QDockWidget*> m_docks;

};
