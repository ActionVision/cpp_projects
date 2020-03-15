#pragma once
#include <vector>
#include "uEye.h"
#include "opencv2/opencv.hpp"
#include "uEye_tools.h"
#include "string.h"

#define MAXCAMERA 4 
#define MAX_EV 11
class IDS_Cam
{
public:
    IDS_Cam();
    IDS_Cam(int width, int height);
    ~IDS_Cam();

    bool OpenCamera();
	bool OpenCamera(int camID);

    void CloseCamera();

    void release(){ CloseCamera(); }

    void GetMaxImageSize(INT *pnSizeX, INT *pnSizeY);

    bool GetExposure(std::vector<double>& exp_max_min_inc_cur);

    int GetGain();

    void SetAutoParam();

    void SetImageSize(int w, int h);

    bool SetExposure(double exposure_time);

    bool SetExposure(uchar expected_brightness, cv::Rect roi);

    bool SetGain(double gain);

    cv::Mat Read();
 
	cv::Mat Read1();

    void operator>>(cv::Mat& im);

    bool isOpened();

    void PrintSensorInfo();

    void PrintAutoInfo(void);
    void DisableAutoParam(void);

	void SetExternalTrigger(void);
	void SetGPIO(int num,bool isOpen);
	void GetFlash();
	void SetLedRed();
	void SetGamma(int gamma);
	double GetGamma();
	//
	bool OpenCameraforLoadPara();
	bool LoadParameter(std::string path);
	
	bool isGetCameraList();
	bool saveParametertoCamera();
	bool LoadParameterFromCamera();
	bool stopSnap();
	bool getCurrentFPS(double &NEWFPS);
	bool GetCurrentExposure(double &currentExposure);

	void EnableAllEvent();
	void DisableAllEvent();
	void StartLive();
	void EvEnumerate();

	int numSelect;
	HIDS    m_hCam;
	
	HWND	m_hWndDisplay ;	// handle to diplay window
	INT		m_lMemoryId ;	// grabber memory - buffer ID
	char*	m_ImageMemory ;  // grabber memory - pointer to buffer
	int m_aviId;
	HANDLE m_hEvent[MAX_EV];
	UEYE_CAMERA_LIST  *pucl;
	INT m_nEvUI[MAX_EV];

private:
	int InitCamera(HIDS *hCam, HWND hWnd);
	
private:
   
    INT		m_Ret;			// return value of uEye SDK functions
    INT		m_nColorMode;	// Y8/RGB16/RGB24/REG32
    INT		m_nBitsPerPixel;// number of bits needed store one pixel
	int		img_width ;		// width of video 
	int		img_height ;		// height of video
   
    INT     m_nRenderMode;  // render  mode
    SENSORINFO m_sInfo;	    // sensor information struct

    double m_MinExp, m_MaxExp, m_IncExp, m_CurExp;  // min, max, increment of exposure time in ms
    double m_CurGain;
};

