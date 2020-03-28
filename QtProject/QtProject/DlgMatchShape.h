#pragma once

#include <QWidget>
#include "ui_DlgMatchShape.h"
#include <qpainter.h>
#include <qevent.h>
#include <qfiledialog.h>
#include <qdebug.h>

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
	//void paintEvent(QPaintEvent *event);
	void draw();
protected:
	bool eventFilter(QObject *watched, QEvent *event);
	void resizeEvent(QResizeEvent *event);

	private slots:
	void on_btnLine_clicked();
	void on_btnRect_clicked();
	void on_btnClearWindow();
	void on_btnOpenImage();
private:
	//we declare a linewidget pointer object using namespace identifier;
	 
	QPainter painter;
	QPixmap mPix;
	QLine mLine;
	QRect mRect;
	bool _openflag = false;
	bool _drag = false;
	QPoint _begin;

private:
	Ui::DlgMatchShape ui;
};
