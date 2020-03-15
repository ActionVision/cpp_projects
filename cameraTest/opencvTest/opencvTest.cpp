// opencvTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>

using namespace cv;
using namespace cv::dnn;
using namespace std;



void test_faceDetect()
{
	const size_t inWidth = 300;
	const size_t inHeight = 300;
	const double inScaleFactor = 0.007843;
	const Scalar meanVal(104.0, 117, 123.0);
	const float confidence = 0.5;

	string model = "yufacedetectnet-open-v2.caffemodel";
	string config = "yufacedetectnet-open-v2.prototxt";
	// read network
	Net net = readNetFromCaffe(config, model);
	VideoCapture cap(0);
	Mat frame;
	while (true) {
		bool ret = cap.read(frame);
		if (!ret) {
			break;

		}
		int64 start = getTickCount();
		Mat input_data = blobFromImage(frame, inScaleFactor, Size(320, 240), meanVal, false, false);
		net.setInput(input_data);

		// 人脸检测
		Mat detection = net.forward();
		Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

		// 推断时间
		vector<double> layersTimings;
		double freq = getTickFrequency() / 1000;
		double time = net.getPerfProfile(layersTimings) / freq;

		ostringstream ss;
		for (int i = 0; i < detectionMat.rows; i++)
		{
			// 置信度 0～1之间
			float score = detectionMat.at<float>(i, 2);
			if (score > confidence)
			{
				int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
				int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
				int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
				int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);

				Rect object((int)xLeftBottom, (int)yLeftBottom,
					(int)(xRightTop - xLeftBottom),
					(int)(yRightTop - yLeftBottom));

				rectangle(frame, object, Scalar(0, 255, 0));

				ss << score;
				String conf(ss.str());
				String label = "Face: " + conf;
				int baseLine = 0;
				Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
				rectangle(frame, Rect(Point(xLeftBottom, yLeftBottom - labelSize.height),
					Size(labelSize.width, labelSize.height + baseLine)),
					Scalar(255, 255, 255), FILLED);
				putText(frame, label, Point(xLeftBottom, yLeftBottom),
					FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
			}
		}
		float fps = getTickFrequency() / (getTickCount() - start);
		ss.str("");
		ss << "FPS: " << fps << " ; inference time: " << time << " ms";
		putText(frame, ss.str(), Point(20, 20), 0, 0.75, Scalar(0, 0, 255), 2, 8);
		imshow("dnn_face_detection", frame);
		if (waitKey(1) >= 0) break;

	}
	waitKey(0);
}

void test_blob()
{
	Mat src = imread("morph.png",0);
	
	Mat binaryMat,dst;
	threshold(src, binaryMat, 200, 255, THRESH_BINARY);
	Mat se = getStructuringElement(MORPH_RECT, Size(11, 11), Point(-1, -1));
	morphologyEx(binaryMat, dst, MORPH_OPEN, se);
	imshow("src", src);
	
	vector<vector<Point>> contours;
	vector<Vec4i> hiearchy;
	findContours(binaryMat, contours, hiearchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	
	for (int i = 0; i < hiearchy.size(); i++)
	{
		//Scalar color = Scalar(rand() % 255, rand() % 255, rand() % 255);
		drawContours(src, contours, i, Scalar(0,0,255), 0, 8, hiearchy);
	}
	 
	
	imshow("binary", binaryMat);
	imshow("dst", dst);
	imshow("result", src);
	waitKey(0);
}

//针对颜色提取的方法，以及轮廓的圆拟合
void test_color_detect()
{
	Mat image = imread("药丸.jpg");
	imshow("input", image);
	// 色彩空间转换
    Mat hsv, mask;
	cvtColor(image, hsv, COLOR_BGR2HSV);
	inRange(hsv, Scalar(40, 43, 46), Scalar(50, 255, 255), mask);
	imshow("mask", mask);
    // 形态学开操作
	Mat se = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));
	morphologyEx(mask, mask, MORPH_OPEN, se);
	imshow("binary", mask);
	 // 轮廓发现
    vector<vector<Point>> contours;
	vector<Vec4i> hiearchy;
	RotatedRect resultRect;
	findContours(mask, contours, hiearchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++) {
		//最小外接矩形
		resultRect = minAreaRect(contours[i]);//获取轮廓的最小外接矩形		
		Point2f pt[4];
		resultRect.points(pt);//获取最小外接矩形的四个顶点坐标
							  //绘制最小外接矩形
		line(image, pt[0], pt[1], Scalar(255, 0, 0), 2, 8);
		line(image, pt[1], pt[2], Scalar(255, 0, 0), 2, 8);
		line(image, pt[2], pt[3], Scalar(255, 0, 0), 2, 8);
		line(image, pt[3], pt[0], Scalar(255, 0, 0), 2, 8);
	
		
		// 圆拟合
	    RotatedRect rrt = fitEllipse(contours[i]);
	   Point ct = rrt.center;
	   int h = rrt.size.height;
	   int w = rrt.size.width;
	   printf("height : %d, width : %d \n", h, w);
	   circle(image, ct, 2, Scalar(0, 0, 255), 2, 8);
	   circle(image, ct, (h + w) / 4, Scalar(255, 0, 0), 2, 8, 0);
	}
	imshow("result", image);
    waitKey(0);
}

void test_blur()
{
	Mat src;
	src = imread("药丸.jpg", 0);
	Mat dst;
	int dis = 30;
	dst = src(Rect(dis, dis, src.cols-dis, src.rows-dis));//通过构造函数的方法设定ROI
	blur(dst, dst, Size(3, 3), Point(0, 0), 4);
	namedWindow("src", WINDOW_GUI_NORMAL);
	imshow("src", src);
	namedWindow("dst", WINDOW_GUI_NORMAL);
	imshow("dst", dst);
	waitKey(0);
}

int test_coutour_area()
{
	Mat srcImage = imread("modules_08.png");
	imshow("【原图】", srcImage);

	//首先对图像进行空间的转换 ?
	Mat grayImage;
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	//对灰度图进行滤波 ?
	GaussianBlur(grayImage, grayImage, Size(3, 3), 0, 0);
	imshow("【滤波后的图像】", grayImage);

	//为了得到二值图像，对灰度图进行边缘检测 ?
	Mat cannyImage;
	Canny(grayImage, cannyImage, 128, 255, 3);
	//在得到的二值图像中寻找轮廓 ?
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(cannyImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));


	//绘制轮廓 ?
	for (int i = 0; i < (int)contours.size(); i++)
	{
		drawContours(cannyImage, contours, i, Scalar(255), 1, 8);
	}
	imshow("【处理后的图像】", cannyImage);


	//计算轮廓的面积 ?
	cout << "【筛选前总共轮廓个数为】：" << (int)contours.size() << endl;
	for (int i = 0; i < (int)contours.size(); i++)
	{
		double g_dConArea = contourArea(contours[i], true);
		cout << "【用轮廓面积计算函数计算出来的第" << i << "个轮廓的面积为：】" << g_dConArea << endl;
	}

	//筛选剔除掉面积小于100的轮廓
	vector <vector<Point>>::iterator iter = contours.begin();
	for (; iter != contours.end();)
	{
		double g_dConArea = contourArea(*iter);
		if (g_dConArea < 100)
		{
			iter = contours.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	cout << "【筛选后总共轮廓个数为：" << (int)contours.size() << endl;
	for (int i = 0; i < (int)contours.size(); i++)
	{
		double g_dConArea = contourArea(contours[i], true);
		cout << "【用轮廓面积计算函数计算出来的第" << i << "个轮廓的面积为：】" << g_dConArea << endl;
	}
	Mat result(srcImage.size(), CV_8U, Scalar(0));
	drawContours(result, contours, -1, Scalar(255), 1);  
	namedWindow("result");
	imshow("result", result);
	waitKey(0);
	return 0;

}

void test_filter()
{
	Mat src, blurMat;
	src = imread("morph.png", 1);
	cvtColor(src, src, COLOR_BGR2GRAY);
	namedWindow("src", WINDOW_NORMAL);
	imshow("src", src);

	blur(src, blurMat, Size(5, 5), Point(-1, -1), 4);
	imshow("blurMat", blurMat);

	Mat blurXMat;
	blur(src, blurXMat, Size(30, 1), Point(-1, -1), 4);//X方向模糊，
	imshow("blurXMat", blurXMat);

	Mat blurYMat;
	blur(src, blurYMat, Size(1, 30), Point(-1, -1), 4);//X方向模糊，
	imshow("blurYMat", blurYMat);

	Mat gaussMat;
	GaussianBlur(src, gaussMat, Size(5, 5), 0, 0, 4);
	imshow("gaussMat", gaussMat);
	
	Mat medianMat;
	medianBlur(src, medianMat, 5);
	imshow("medianMat", medianMat);
	
	Mat	  bilaterlMat;
	bilateralFilter(src, bilaterlMat, 15, 120, 10, 4);
	imshow("bilaterlMat", bilaterlMat);


	int ksize = 15;
	Mat kernel = Mat::ones(ksize, ksize, CV_32F) / (float)(ksize*ksize);
	Mat filterMat;
	filter2D(src, filterMat, -1, kernel, Point(-1, -1), 0.0, 4);
	imshow("filterMat", filterMat);

	//图像的边缘提取
	Mat filterMatContours;
	Mat kernel1 = (Mat_<char>(3, 3) << -1, -1, -1, -1, 8, -1, -1, -1, -1);
	filter2D(src, filterMatContours, -1, kernel1, Point(-1, -1), 0.0, 4);
	imshow("filterMatContours", filterMatContours);

	//图像的锐化
	Mat filterMat1;
	Mat kernel2 = (Mat_<char>(3, 3) << -1, -1, -1, -1, 9, -1, -1, -1, -1);
	filter2D(src, filterMat1, -1, kernel2, Point(-1, -1), 0.0, 4);
	imshow("filterMat1", filterMat1);

	//robot算子
	Mat filterMat2;
	Mat kernel3 = (Mat_<char>(2, 2) << -1, 0, 0, 1);
	filter2D(src, filterMat2, -1, kernel3, Point(-1, -1), 0.0, 4);
	imshow("filterMat2", filterMat2);

	Mat filterMat4;
	Mat kernel4 = (Mat_<char>(3, 3) << 1, 2, 1, 0, 0, 0, -1, -2, -1);
	filter2D(src, filterMat4, -1, kernel4, Point(-1, -1), 0.0, 4);
	imshow("filterMat4", filterMat4);
	waitKey(0);
}


int main()
{
	//test_faceDetect();
	//test_blob();
	//test_color_detect();
	//test_blur();
	//test_coutour_area();
	test_filter();
	return 0;
}

