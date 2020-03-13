// cameraEvent.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<pylon/PylonIncludes.h>
#include "CameraEventPrinter.h"
#include "ConfigurationEventPrinter.h"

using namespace Pylon;
#include <pylon/BaslerUniversalInstantCamera.h>

using namespace Basler_UniversalCameraParams;
using namespace std;

//枚举不同的事件类型
enum MyEvents
{
	eMyExposureEndEvent = 100,
	eMyEventOverrunEvent = 200
};
//采图数量设置
static const uint32_t c_countOfImagesToGrab = 5;

//重写相机事件函数,继承于通用相机事件
class CSampleCameraEventHandler : public CBaslerUniversalCameraEventHandler
{
public:
	virtual void OnCameraEvent(CBaslerUniversalInstantCamera& camera, intptr_t userProvidedId, GenApi::INode* /* pNode */)
	{
		std::cout << std::endl;
		switch (userProvidedId)
		{
		case eMyExposureEndEvent: // Exposure End event
			if (camera.EventExposureEndFrameID.IsReadable())
			{
				cout << "Exposure End event. FrameID: " << camera.EventExposureEndFrameID.GetValue() << " Timestamp: " << camera.EventExposureEndTimestamp.GetValue() << std::endl << std::endl;
			}
			else
			{
				cout << "Exposure End event. FrameID: " << camera.ExposureEndEventFrameID.GetValue() << " Timestamp: " << camera.ExposureEndEventTimestamp.GetValue() << std::endl << std::endl;
			}
			break;
		case eMyEventOverrunEvent:  // Event Overrun event
			cout << "Event Overrun event. FrameID: " << camera.EventOverrunEventFrameID.GetValue() << " Timestamp: " << camera.EventOverrunEventTimestamp.GetValue() << std::endl << std::endl;
			break;
		}
	}
};

//重写相机事件
class CSampleImageEventHandler : public CImageEventHandler
{
public:
	virtual void OnImageGrabbed(CInstantCamera& camera, const CGrabResultPtr& ptrGrabResult)
	{
		cout << "CSampleImageEventHandler::OnImageGrabbed called." << std::endl;
		cout << std::endl;
		cout << std::endl;
	}
};


int main()
{
	int exitCode = 0;
	//初始化操作
	PylonInitialize();
	CSampleCameraEventHandler* pHandler1 = new CSampleCameraEventHandler;
	CCameraEventPrinter*  pHandler2 = new CCameraEventPrinter;
	try
	{
		//创建一个相机实例
		CBaslerUniversalInstantCamera camera(CTlFactory::GetInstance().CreateFirstDevice());

		//用软触发配置替代默认的配置
		camera.RegisterConfiguration(new CSoftwareTriggerConfiguration, RegistrationMode_ReplaceAll, Cleanup_Delete);
		//配置打印事件
		camera.RegisterConfiguration(new CConfigurationEventPrinter, RegistrationMode_Append, Cleanup_Delete); // Camera use.
		//注册图片事件																									   // For demonstration purposes only, register another image event handler.
		camera.RegisterImageEventHandler(new CSampleImageEventHandler, RegistrationMode_Append, Cleanup_Delete);
		//设置相机事件为true
		camera.GrabCameraEvents = true;

		//相机版本的判断与设置
		if (camera.GetSfncVersion() >= Sfnc_2_0_0)
		{
			//注册相机曝光事件
			camera.RegisterCameraEventHandler(pHandler1, "EventExposureEndData", eMyExposureEndEvent, RegistrationMode_ReplaceAll, Cleanup_None);
			//注册相机曝光帧ID事件
			camera.RegisterCameraEventHandler(pHandler2, "EventExposureEndFrameID", eMyExposureEndEvent, RegistrationMode_Append, Cleanup_None);
			//注册相机曝光时间戳事件
			camera.RegisterCameraEventHandler(pHandler2, "EventExposureEndTimestamp", eMyExposureEndEvent, RegistrationMode_Append, Cleanup_None);
		}
		else
		{
			//注册相机曝光事件
			camera.RegisterCameraEventHandler(pHandler1, "ExposureEndEventData", eMyExposureEndEvent, RegistrationMode_ReplaceAll, Cleanup_None);
			//注册相机overrun事件
			camera.RegisterCameraEventHandler(pHandler1, "EventOverrunEventData", eMyEventOverrunEvent, RegistrationMode_Append, Cleanup_None);

			//注册相机frameid和时间戳事件
			camera.RegisterCameraEventHandler(pHandler2, "ExposureEndEventFrameID", eMyExposureEndEvent, RegistrationMode_Append, Cleanup_None);
			camera.RegisterCameraEventHandler(pHandler2, "ExposureEndEventTimestamp", eMyExposureEndEvent, RegistrationMode_Append, Cleanup_None);
		}
		//打开相机
		camera.Open();
		// 判断相机是否支持事件
		if (!camera.EventSelector.IsWritable())
		{
			throw RUNTIME_EXCEPTION("The device doesn't support events.");
		}
		//选择相机的接收事件
		camera.EventSelector.SetValue(EventSelector_ExposureEnd);
		//使能相机的曝光事件
		if (!camera.EventNotification.TrySetValue(EventNotification_On))
		{
			// GIGE相机设置
			camera.EventNotification.SetValue(EventNotification_GenICamEvent);
		}
		// 使能相机事件
		if (camera.EventSelector.TrySetValue(EventSelector_EventOverrun))
		{
			if (!camera.EventNotification.TrySetValue(EventNotification_On))
			{
				// GIGE相机设置
				camera.EventNotification.SetValue(EventNotification_GenICamEvent);
			}
		}
		//开始连续采图
		camera.StartGrabbing(c_countOfImagesToGrab);

		//接收图片的指针
		CGrabResultPtr ptrGrabResult;
		//持续采图
		while (camera.IsGrabbing())
		{
			// 等待1s钟，等待图片到来
			if (camera.WaitForFrameTriggerReady(1000, TimeoutHandling_ThrowException))
			{
				camera.ExecuteSoftwareTrigger();
			}
			// Retrieve grab results and notify the camera event and image event handlers.
			camera.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);
			// Nothing to do here with the grab result, the grab results are handled by the registered event handler.
		}
		// 关闭曝光事件
		camera.EventSelector.SetValue(EventSelector_ExposureEnd);
		camera.EventNotification.SetValue(EventNotification_Off);
		// 关闭overrun事件
		if (camera.EventSelector.TrySetValue(EventSelector_EventOverrun))
		{
			camera.EventNotification.SetValue(EventNotification_Off);
		}
	}
	catch (const GenericException &e)
	{
		cerr << "An exception occurred." << endl
			<< e.GetDescription() << endl;
		exitCode = 1;
	}
	delete pHandler1;
	delete pHandler2;

	cerr << endl << "Press Enter to exit." << endl;
	while (cin.get() != '\n');

	PylonTerminate();
	return 0;
}

