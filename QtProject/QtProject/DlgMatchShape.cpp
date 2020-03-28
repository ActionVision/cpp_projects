#include "DlgMatchShape.h"

DlgMatchShape::DlgMatchShape(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//mPix = QPixmap(400, 400);
	//mPix.fill(Qt::white);
	mousePressed = false;
	drawStarted = false;
	selectedTool = 2;
	
	int w = ui.label_Image->width();
	int h = ui.label_Image->height();
	
	
	mPix = QPixmap(w, h);
	//mPix.fill(Qt::white);
	bool ret = mPix.load("D:\\modules_08.png");

	//ui.label_Image->setStyleSheet("background-color:green");
	ui.label_Image->installEventFilter(this);
	//mPix.scaledToHeight(h);
	//mPix.scaledToWidth(w);
	//mPix.fromImage(QImage);
	//ui.label_Image->setPixmap(mPix);
	
	ui.label_Image->setScaledContents(true);
	
}

DlgMatchShape::~DlgMatchShape()
{
}
void DlgMatchShape::mousePressEvent(QMouseEvent* event) {
	 
	mousePressed = true;
	int a = event->pos().x() -ui.label_Image->geometry().x();
	int b = event->pos().y() -ui.label_Image->geometry().y();
	QPoint temp(a, b);

	if (selectedTool == 1) 
	{
		mRect.setTopLeft(temp);
		mRect.setBottomRight(temp);
	}
	else if (selectedTool == 2) 
	{
		mLine.setP1(temp);
		mLine.setP2(temp);
	}

}

void DlgMatchShape::mouseMoveEvent(QMouseEvent* event)
{

	

	if ( event->type() == QEvent::MouseMove) 
	{
		int a = event->pos().x()- ui.label_Image->geometry().x();
		int b = event->pos().y() - ui.label_Image->geometry().y();
		QPoint temp1(a, b);

		if (selectedTool == 1)
		{
			mRect.setBottomRight(temp1);
		}
		else if (selectedTool == 2)
		{
			mLine.setP2(temp1);
		}
	}
	
	update();

	
}

void DlgMatchShape::mouseReleaseEvent(QMouseEvent *event) {

	//When mouse is released update for the one last time
	mousePressed = false;

	
	update();
	
}

//void DlgMatchShape::paintEvent(QPaintEvent *event) 
//{
//	painter.begin(this);
//	painter.drawPixmap(0, 0, mPix);
//	if (mousePressed) 
//	{
//		painter.drawPixmap(0, 0, mPix);
//		if (selectedTool == 1)
//			painter.drawRect(mRect);
//		else if (selectedTool == 2)
//			painter.drawLine(mLine);
//
//		drawStarted = true;
//	}
//	else if (drawStarted)
//	{
//		QPainter tempPainter(&mPix);
//		if (selectedTool == 1)
//			tempPainter.drawRect(mRect);
//		else if (selectedTool == 2)
//			tempPainter.drawLine(mLine);
//
//		painter.drawPixmap(0, 0, mPix);
//
//		
//	}
//	
//	painter.end();
//}
bool DlgMatchShape::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == ui.label_Image && event->type() == QEvent::Paint)
		draw();
	return QWidget::eventFilter(watched, event);
}
void DlgMatchShape::draw()
{
	
	QPainter painter(ui.label_Image);
	QPen pen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);
	//QPixmap pix = mPix;
	
	painter.drawPixmap(0, 0, mPix);
	painter.drawRect(mRect);
	painter.drawLine(mLine);
}

void DlgMatchShape::on_btnLine_clicked()
{
	selectedTool = 2;
}

void DlgMatchShape::on_btnRect_clicked()
{
	selectedTool = 1;
}

void DlgMatchShape::on_btnClearWindow()
{
	drawStarted = false;
	 
	mRect.setBottomLeft(QPoint(0, 0));
	mRect.setBottomRight(QPoint(0, 0));
	mLine.setP1(QPoint(0, 0));
	mLine.setP2(QPoint(0, 0));
	update();

}
void DlgMatchShape::on_btnOpenImage()
{
	 
	QString picPath = QFileDialog::getOpenFileName(this, tr("打开"), "", "Image Files(*.jpg *.png *.bmp)");
	if (!picPath.isEmpty())//用户选择了文件
	{
		bool ret = mPix.load(picPath);
		if (!ret)
		{
			qDebug() << "load image fail!!!!";
			return;
		}
		qDebug() << "load image success";
		_openflag = true;//设置文件打开标志
		update();//触发窗体重绘，将图片画到窗体
		
		
	}
}

void DlgMatchShape::resizeEvent(QResizeEvent *event)
{
	  mPix.scaled(ui.label_Image->size());
}