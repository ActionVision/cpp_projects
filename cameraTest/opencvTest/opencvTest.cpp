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
	findContours(mask, contours, hiearchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++) {
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
int main()
{
	//test_faceDetect();
	test_blob();
	//test_color_detect();
	return 0;
}

