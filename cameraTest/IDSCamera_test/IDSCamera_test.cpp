// IDSCamera_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ids_cam.h"
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
using namespace cv;


void test_open_camera()
{
	IDS_Cam Camera;
	Camera.OpenCamera();
	Mat src;
	int i = 0;
	namedWindow("src", WINDOW_GUI_NORMAL);
	while (true)
	{
		src = Camera.Read();
		if (src.rows != 0)
		{
			imshow("src", src);
			waitKey(5);
			i++;
			std::cout << i<<" :get frame" << std::endl;
		}
	}
}

int main()
{
	test_open_camera();
	return 0;
}

