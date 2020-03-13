// cameraEvent.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<pylon/PylonIncludes.h>
#include "CameraEventPrinter.h"
#include "ConfigurationEventPrinter.h"

using namespace Pylon;
#include <pylon/BaslerUniversalInstantCamera.h>

using namespace Basler_UniversalCameraParams;
using namespace std;

//ö�ٲ�ͬ���¼�����
enum MyEvents
{
	eMyExposureEndEvent = 100,
	eMyEventOverrunEvent = 200
};
//��ͼ��������
static const uint32_t c_countOfImagesToGrab = 5;

//��д����¼�����,�̳���ͨ������¼�
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

//��д����¼�
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
	//��ʼ������
	PylonInitialize();
	CSampleCameraEventHandler* pHandler1 = new CSampleCameraEventHandler;
	CCameraEventPrinter*  pHandler2 = new CCameraEventPrinter;
	try
	{
		//����һ�����ʵ��
		CBaslerUniversalInstantCamera camera(CTlFactory::GetInstance().CreateFirstDevice());

		//�������������Ĭ�ϵ�����
		camera.RegisterConfiguration(new CSoftwareTriggerConfiguration, RegistrationMode_ReplaceAll, Cleanup_Delete);
		//���ô�ӡ�¼�
		camera.RegisterConfiguration(new CConfigurationEventPrinter, RegistrationMode_Append, Cleanup_Delete); // Camera use.
		//ע��ͼƬ�¼�																									   // For demonstration purposes only, register another image event handler.
		camera.RegisterImageEventHandler(new CSampleImageEventHandler, RegistrationMode_Append, Cleanup_Delete);
		//��������¼�Ϊtrue
		camera.GrabCameraEvents = true;

		//����汾���ж�������
		if (camera.GetSfncVersion() >= Sfnc_2_0_0)
		{
			//ע������ع��¼�
			camera.RegisterCameraEventHandler(pHandler1, "EventExposureEndData", eMyExposureEndEvent, RegistrationMode_ReplaceAll, Cleanup_None);
			//ע������ع�֡ID�¼�
			camera.RegisterCameraEventHandler(pHandler2, "EventExposureEndFrameID", eMyExposureEndEvent, RegistrationMode_Append, Cleanup_None);
			//ע������ع�ʱ����¼�
			camera.RegisterCameraEventHandler(pHandler2, "EventExposureEndTimestamp", eMyExposureEndEvent, RegistrationMode_Append, Cleanup_None);
		}
		else
		{
			//ע������ع��¼�
			camera.RegisterCameraEventHandler(pHandler1, "ExposureEndEventData", eMyExposureEndEvent, RegistrationMode_ReplaceAll, Cleanup_None);
			//ע�����overrun�¼�
			camera.RegisterCameraEventHandler(pHandler1, "EventOverrunEventData", eMyEventOverrunEvent, RegistrationMode_Append, Cleanup_None);

			//ע�����frameid��ʱ����¼�
			camera.RegisterCameraEventHandler(pHandler2, "ExposureEndEventFrameID", eMyExposureEndEvent, RegistrationMode_Append, Cleanup_None);
			camera.RegisterCameraEventHandler(pHandler2, "ExposureEndEventTimestamp", eMyExposureEndEvent, RegistrationMode_Append, Cleanup_None);
		}
		//�����
		camera.Open();
		// �ж�����Ƿ�֧���¼�
		if (!camera.EventSelector.IsWritable())
		{
			throw RUNTIME_EXCEPTION("The device doesn't support events.");
		}
		//ѡ������Ľ����¼�
		camera.EventSelector.SetValue(EventSelector_ExposureEnd);
		//ʹ��������ع��¼�
		if (!camera.EventNotification.TrySetValue(EventNotification_On))
		{
			// GIGE�������
			camera.EventNotification.SetValue(EventNotification_GenICamEvent);
		}
		// ʹ������¼�
		if (camera.EventSelector.TrySetValue(EventSelector_EventOverrun))
		{
			if (!camera.EventNotification.TrySetValue(EventNotification_On))
			{
				// GIGE�������
				camera.EventNotification.SetValue(EventNotification_GenICamEvent);
			}
		}
		//��ʼ������ͼ
		camera.StartGrabbing(c_countOfImagesToGrab);

		//����ͼƬ��ָ��
		CGrabResultPtr ptrGrabResult;
		//������ͼ
		while (camera.IsGrabbing())
		{
			// �ȴ�1s�ӣ��ȴ�ͼƬ����
			if (camera.WaitForFrameTriggerReady(1000, TimeoutHandling_ThrowException))
			{
				camera.ExecuteSoftwareTrigger();
			}
			// Retrieve grab results and notify the camera event and image event handlers.
			camera.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);
			// Nothing to do here with the grab result, the grab results are handled by the registered event handler.
		}
		// �ر��ع��¼�
		camera.EventSelector.SetValue(EventSelector_ExposureEnd);
		camera.EventNotification.SetValue(EventNotification_Off);
		// �ر�overrun�¼�
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

