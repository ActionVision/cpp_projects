#include "DlgMatchShape.h"

DlgMatchShape::DlgMatchShape(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	mPix = QPixmap(400, 400);
	mPix.fill(Qt::white);

	//set everything to false as nothing has started yet
	mousePressed = false;
	drawStarted = false;

	//default is line
	selectedTool = 2;
	
	int w = ui.label_Image->width();
	int h = ui.label_Image->height();
	
	
	mPix = QPixmap(w, h);
	//mPix.fill(Qt::white);
	bool ret = mPix.load("D:\\modules_08.png");
	//mPix.scaledToHeight(h);
	//mPix.scaledToWidth(w);
	//mPix.fromImage(QImage);
	//ui.label_Image->setPixmap(mPix);
	//ui.label_Image->setScaledContents(true);
	
}

DlgMatchShape::~DlgMatchShape()
{
}
void DlgMatchShape::mousePressEvent(QMouseEvent* event) {
	 
	mousePressed = true;

	if (selectedTool == 1) {
		mRect.setTopLeft(event->pos());
		mRect.setBottomRight(event->pos());
	}
	else if (selectedTool == 2) {
		mLine.setP1(event->pos());
		mLine.setP2(event->pos());
	}
}

void DlgMatchShape::mouseMoveEvent(QMouseEvent* event) {

	if (event->type() == QEvent::MouseMove) {
		if (selectedTool == 1) {
			mRect.setBottomRight(event->pos());
		}
		else if (selectedTool == 2) {
			mLine.setP2(event->pos());
		}
	}
	//it calls the paintEven() function continuously
	update();
}

void DlgMatchShape::mouseReleaseEvent(QMouseEvent *event) {

	//When mouse is released update for the one last time
	mousePressed = false;
	update();
}

void DlgMatchShape::paintEvent(QPaintEvent *event) {

	painter.begin(this);
	//When the mouse is pressed
	if (mousePressed) {
		// we are taking QPixmap reference again and again
		//on mouse move and drawing a line again and again
		//hence the painter view has a feeling of dynamic drawing
		//ui.label_Image->setPixmap(mPix);
		painter.drawPixmap(0, 0, mPix);
		if (selectedTool == 1)
			painter.drawRect(mRect);
		else if (selectedTool == 2)
			painter.drawLine(mLine);

		drawStarted = true;
	}
	else if (drawStarted) {
		// It created a QPainter object by taking  a reference
		// to the QPixmap object created earlier, then draws a line
		// using that object, then sets the earlier painter object
		// with the newly modified QPixmap object
		QPainter tempPainter(&mPix);
		if (selectedTool == 1)
			tempPainter.drawRect(mRect);
		else if (selectedTool == 2)
			tempPainter.drawLine(mLine);

		painter.drawPixmap(0, 0, mPix);
		
	}

	painter.end();
}

 
//only two button is on the ui btnLine and btnRect
void DlgMatchShape::on_btnLine_clicked()
{
	selectedTool = 2;
}

void DlgMatchShape::on_btnRect_clicked()
{
	selectedTool = 1;
}