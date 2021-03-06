#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtProject.h"
#include <qstring.h>
#include "ThreadWork.h"
#include "DlgMatchShape.h"
#include "mypaint.h"
#include <qevent.h>
#include <QDrag>
#include <QMimeData>
#include "ThreadSocket.h"

class QtProject : public QMainWindow
{
	Q_OBJECT

public:
	QtProject(QWidget *parent = Q_NULLPTR);
	ThreadWork *m_ThreadWork[3];
	ThreadSocket *m_ThreadSocket[3];
	void drawRedCircle();

protected:
	bool eventFilter(QObject *watched, QEvent *event);

protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent(QDropEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	Ui::QtProjectClass ui;
	MyStruct m_info;
	void test_DlgMatchShape();
	DlgMatchShape* m_DlgMatchShape;
	MyPaint*         m_myPaint;
	QPoint mStartPoint;
	QHBoxLayout  *m_HBox;

signals:
	void signalMystruct(MyStruct str);
private slots:
void   sendMsg();

void  dockA();
void  dockB();

public:
void removeAllDock();
void showDock(const QList<int>& index = QList<int>());
 
private slots:

void receive_msg1(MyStruct STR);

private :
	QList<QDockWidget*> m_docks;

};
