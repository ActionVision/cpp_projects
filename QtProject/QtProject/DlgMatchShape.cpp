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
	bool ret = mPix.load("D:\\modules_08.png");
	mPix_origin = mPix;

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
	
	painter.drawPixmap(0, 0, mPix);
	if (selectedTool == 1)
	{
		painter.drawRect(mRect);
	}
	if (selectedTool == 2)
	{
		painter.drawLine(mLine);
	}
}

void DlgMatchShape::on_btnLine_clicked()
{
	selectedTool = 2;
	update();
}

void DlgMatchShape::on_btnRect_clicked()
{
	selectedTool = 1;
	update();
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
	 
	QString picPath = QFileDialog::getOpenFileName(this, tr("��"), "", "Image Files(*.jpg *.png *.bmp)");
	if (!picPath.isEmpty())//�û�ѡ�����ļ�
	{
		bool ret = mPix_origin.load(picPath);
		if (!ret)
		{
			qDebug() << "load image fail!!!!";
			return;
		}
		//mPix = mPix.scaled(ui.label_Image->size());//�մ��µ�ͼƬ�ʹ�����Ҫ��Ӧ
		mPix = mPix_origin;
		resizeEvent(NULL);
		qDebug() << "load image success";
		_openflag = true;//�����ļ��򿪱�־
		update();//���������ػ棬��ͼƬ��������
		
		
	}
}

void DlgMatchShape::resizeEvent(QResizeEvent *event)
{
	double originW = mPix.size().width();
	double originH = mPix.size().height();
	double originLeftX = mRect.topLeft().x();
	double originLeftY = mRect.topLeft().y();
	double originRightX = mRect.bottomRight().x();
	double originRightY = mRect.bottomRight().y();

	 mPix= mPix_origin.scaled(ui.label_Image->size());//һ��Ҫ���أ���Ȼ�޷��ı��С��
	  qDebug() << "ui.label_Image:" << ui.label_Image->size().width() << ";" << ui.label_Image->size().height();
	  qDebug() << "mPix:" << mPix.size().width() << ";"<<mPix.size().height();
	  //��������ʱ������ҲҪ���ǻ�ͼ���ţ�
	  
	  double newW = mPix.size().width();
	  double newH = mPix.size().height();

	  double rationX = newW/ originW;
	  double rationY = newH / originH;

	  int newPosLeftX= originLeftX* rationX;
	  int newPosLeftY = originLeftY* rationY;
	  mRect.setTopLeft(QPoint(newPosLeftX, newPosLeftY));
		
	  int newPosRightX = originRightX* rationX;
	  int newPosRightY = originRightY* rationY;
	  mRect.setBottomRight(QPoint(newPosRightX, newPosRightY));
	  
	  int dx1 = newPosLeftX - originLeftX;
	  int dy1 = newPosLeftY - originLeftY;
	  int dx2 = newPosRightX - originRightX;
	  int dy2 = newPosRightY - originRightY;
	  mRect.adjusted(dx1, dy1, dx2, dy2);
	 
	  update();
}