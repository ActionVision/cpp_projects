// IDSCamera_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ids_cam.h"
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
using namespace cv;
#include <iostream>
using namespace std;


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

void test_event()
{
	IDS_Cam Camera;
	Camera.OpenCamera();
	//Camera.LoadParameter("test.ini");
	Camera.EnableAllEvent();
	Camera.StartLive();
	namedWindow("src", WINDOW_GUI_NORMAL);
	int i = 0;
	while (true)
	{
		//DWORD dwRet = WaitForSingleObject(Camera.m_hEvent, INFINITE);
		DWORD dwRet = WaitForMultipleObjects(MAX_EV, Camera.m_hEvent, FALSE, INFINITE);
		if (Camera.isOpened())
		{
			cout << "camera is opened" << endl;
		}
		if (dwRet == WAIT_TIMEOUT)
		{
			cout << "time out" << endl;
		}
		else if (dwRet == WAIT_OBJECT_0)
		{
			i++;
			cout <<i<< "IS_SET_EVENT_FRAME" << endl;
			
			void *pMemVoid; //pointer to where the image is stored
			is_GetImageMem(Camera.m_hCam, &pMemVoid);
			int w, h;
			Camera.GetMaxImageSize(&w, &h);
			Mat im = cv::Mat(cv::Size(w, h), CV_8UC3, pMemVoid).clone();
			imshow("src", im);
			waitKey(5);

		}
		else if (dwRet == WAIT_OBJECT_0+5)
		{
			cout << i << "IS_SET_EVENT_DEVICE_RECONNECTED" << endl;
		}
		else if (dwRet == WAIT_OBJECT_0 + 7)
		{
			cout << "IS_SET_EVENT_REMOVE" << endl;
		}
		
	}

	
}



int main()
{
	//test_open_camera();
	test_event();
	return 0;
}

