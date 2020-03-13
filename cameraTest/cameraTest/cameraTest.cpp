// cameraTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <pylon/PylonIncludes.h>
#ifdef PYLON_WIN_BUILD
# include <pylon/PylonGUI.h>
#endif
using namespace Pylon;
using namespace std;

static const uint32_t c_countOfImageToGrab = 100;

void test_Grab()
{
	int exitCode = 0;
	//初始化操作
	PylonInitialize();
	try
	{
		//创建一个相机实例
		CInstantCamera camera(CTlFactory::GetInstance().CreateFirstDevice());
		cout << "using device " << camera.GetDeviceInfo().GetModelName() << endl;
		camera.MaxNumBuffer = 5;
		//开始持续采图，采集100张
		camera.StartGrabbing(c_countOfImageToGrab);
		CGrabResultPtr ptrGrabResult;
		while (camera.IsGrabbing())
		{
			camera.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);
			if (ptrGrabResult->GrabSucceeded())
			{
				//获取图像的大小
				cout << "sizeX:" << ptrGrabResult->GetWidth() << endl;
				cout << "sizeY:" << ptrGrabResult->GetHeight() << endl;
				//获取图像的内存
				const uint8_t *pImageBuffer = (uint8_t*)ptrGrabResult->GetBuffer();
				//第一个像素的像素值
				cout << "\ngrab value of first pixel:" << (uint8_t)pImageBuffer[0] << endl;
#ifdef PYLON_WIN_BUILD
				Pylon::DisplayImage(1, ptrGrabResult);
#endif
			}
			else
			{
				cout << "\nerror :" << ptrGrabResult->GetErrorCode() << endl;
			}
		}

	}
	catch (const GenericException &e)//异常处理
	{
		cout << "\nerror happened:" << e.GetDescription() << endl;
		exitCode = 1;
	}
	//释放资源
	PylonTerminate();

}


int main()
{
	test_Grab();

    return 0;
}

