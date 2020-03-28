#pragma once

#include <QWidget>
#include "ui_DlgMatchShape.h"
#include <qpainter.h>
#include <qevent.h>

class DlgMatchShape : public QWidget
{
	Q_OBJECT

public:
	DlgMatchShape(QWidget *parent = Q_NULLPTR);
	~DlgMatchShape();
	bool mousePressed;
	bool drawStarted;
	int selectedTool;

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);

	private slots:
	void on_btnLine_clicked();
	void on_btnRect_clicked();
private:
	//we declare a linewidget pointer object using namespace identifier;
	 
	QPainter painter;
	QPixmap mPix;
	QLine mLine;
	QRect mRect;

private:
	Ui::DlgMatchShape ui;
};
