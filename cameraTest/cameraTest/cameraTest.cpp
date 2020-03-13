// cameraTest.cpp : �������̨Ӧ�ó������ڵ㡣
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
	//��ʼ������
	PylonInitialize();
	try
	{
		//����һ�����ʵ��
		CInstantCamera camera(CTlFactory::GetInstance().CreateFirstDevice());
		cout << "using device " << camera.GetDeviceInfo().GetModelName() << endl;
		camera.MaxNumBuffer = 5;
		//��ʼ������ͼ���ɼ�100��
		camera.StartGrabbing(c_countOfImageToGrab);
		CGrabResultPtr ptrGrabResult;
		while (camera.IsGrabbing())
		{
			camera.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);
			if (ptrGrabResult->GrabSucceeded())
			{
				//��ȡͼ��Ĵ�С
				cout << "sizeX:" << ptrGrabResult->GetWidth() << endl;
				cout << "sizeY:" << ptrGrabResult->GetHeight() << endl;
				//��ȡͼ����ڴ�
				const uint8_t *pImageBuffer = (uint8_t*)ptrGrabResult->GetBuffer();
				//��һ�����ص�����ֵ
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
	catch (const GenericException &e)//�쳣����
	{
		cout << "\nerror happened:" << e.GetDescription() << endl;
		exitCode = 1;
	}
	//�ͷ���Դ
	PylonTerminate();

}


int main()
{
	test_Grab();

    return 0;
}

