#include "ids_cam.h"
#include <ueye_deprecated.h>
#include <string.h>
#include <string>
using namespace std;

IDS_Cam::IDS_Cam( )
{
     m_hCam = 0;
	 
    SetImageSize(752, 480);

    m_nRenderMode = IS_RENDER_FIT_TO_WINDOW;
}

IDS_Cam::IDS_Cam(int width, int height)
{
    //m_hCam = 0;

    SetImageSize(width, height);

    m_nRenderMode = IS_RENDER_FIT_TO_WINDOW;

    OpenCamera();		// open a camera handle

    //SetAutoParam();
}

IDS_Cam::~IDS_Cam()
{
    CloseCamera();
    m_hCam = 0;
	 
}

bool IDS_Cam::OpenCamera()
{
	if (m_hCam != 0)
    {
        //free old image mem.
		is_FreeImageMem(m_hCam, m_ImageMemory, m_lMemoryId);
		is_ExitCamera(m_hCam);
    }

    // init camera
	m_hCam = (HIDS)0;
    //m_hCam = (HIDS)20;						// open next camera
	m_Ret = InitCamera(&m_hCam, m_hWndDisplay);		// init camera - no window handle required
    if (m_Ret != IS_SUCCESS)
    {
        printf("InitCamera failed! Return value: %d\n", m_Ret);
        return false;
    }
    printf("m_hCam: %d\n", m_hCam);

    // Get sensor info
	is_GetSensorInfo(m_hCam, &m_sInfo);

    int maxSizeX, maxSizeY;
    GetMaxImageSize(&maxSizeX, &maxSizeY);

    // setup the color depth to the current windows setting
	is_SetColorMode(m_hCam, /*m_nColorMode*/IS_CM_BGR8_PACKED);
	is_GetColorDepth(m_hCam, &m_nBitsPerPixel, &m_nColorMode);
    printf("m_hCam, m_nBitsPerPixel, m_nColorMode: %d %d %d\n",m_hCam, m_nBitsPerPixel, m_nColorMode);

    int img_bpp = 24;
    // memory initialization
    //is_AllocImageMem(m_hCam, img_width, img_height, img_bpp, &m_ImageMemory, &m_lMemoryId);
	is_AllocImageMem(m_hCam, maxSizeX, maxSizeY, img_bpp, &m_ImageMemory, &m_lMemoryId);

	is_SetImageMem(m_hCam, m_ImageMemory, m_lMemoryId);	// set memory active

   // is_SetImageSize(m_hCam, img_width, img_height);
	is_SetImageSize(m_hCam, maxSizeX, maxSizeY);

    // display initialization
    IS_SIZE_2D imageSize;
   // imageSize.s32Width = img_width;
   // imageSize.s32Height = img_height;
	imageSize.s32Width = maxSizeX;
	imageSize.s32Height = maxSizeY;
    //is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));

	is_SetDisplayMode(m_hCam, IS_SET_DM_DIB);

	DisableAutoParam();

    return true;
}

bool IDS_Cam::OpenCamera(int camID)
{
	if (m_hCam != 0)
	{
		//free old image mem.
		is_FreeImageMem(m_hCam, m_ImageMemory, m_lMemoryId);
		is_ExitCamera(m_hCam);
	}

	// init camera
	m_hCam = (HIDS)(camID);
	m_Ret = InitCamera(&m_hCam, m_hWndDisplay);		// init camera - no window handle required
	if (m_Ret != IS_SUCCESS)
	{
		printf("InitCamera failed! Return value: %d\n", m_Ret);
		return false;
	}
	printf("m_hCam: %d\n", m_hCam);

	// Get sensor info
	is_GetSensorInfo(m_hCam, &m_sInfo);

	int maxSizeX, maxSizeY;
	GetMaxImageSize(&maxSizeX, &maxSizeY);

	// setup the color depth to the current windows setting
	is_SetColorMode(m_hCam, /*m_nColorMode*/IS_CM_BGR8_PACKED);
	is_GetColorDepth(m_hCam, &m_nBitsPerPixel, &m_nColorMode);
	printf("m_hCam, m_nBitsPerPixel, m_nColorMode: %d %d %d\n", m_hCam, m_nBitsPerPixel, m_nColorMode);

	int img_bpp = 24;
	// memory initialization
	//is_AllocImageMem(m_hCam, img_width, img_height, img_bpp, &m_ImageMemory, &m_lMemoryId);
	is_AllocImageMem(m_hCam, maxSizeX, maxSizeY, img_bpp, &m_ImageMemory, &m_lMemoryId);

	is_SetImageMem(m_hCam, m_ImageMemory, m_lMemoryId);	// set memory active

	// is_SetImageSize(m_hCam, img_width, img_height);
	is_SetImageSize(m_hCam, maxSizeX, maxSizeY);

	// display initialization
	IS_SIZE_2D imageSize;
	// imageSize.s32Width = img_width;
	// imageSize.s32Height = img_height;
	imageSize.s32Width = maxSizeX;
	imageSize.s32Height = maxSizeY;
	//is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));

	is_SetDisplayMode(m_hCam, IS_SET_DM_DIB);

	DisableAutoParam();

	return true;
}

void IDS_Cam::CloseCamera()
{
    if (m_hCam != 0)
    {
        //free old image mem.
		is_FreeImageMem(m_hCam, m_ImageMemory, m_lMemoryId);
		is_ExitCamera(m_hCam);
		m_hCam = 0;
    }
}

int IDS_Cam::InitCamera(HIDS *hCam, HWND hWnd)
{
    int nRet = is_InitCamera(hCam, hWnd);
    /************************************************************************************************/
    /*  If the camera returns with "IS_STARTER_FW_UPLOAD_NEEDED", an upload of a new firmware       */
    /*  is necessary. This upload can take several seconds. We recommend to check the required      */
    /*  time with the function is_GetDuration().                                                    */
    /*  In this case, the camera can only be opened if the flag "IS_ALLOW_STARTER_FW_UPLOAD"        */
    /*  is "OR"-ed to m_hCam. This flag allows an automatic upload of the firmware.                 */
    /************************************************************************************************/
    if (nRet == IS_STARTER_FW_UPLOAD_NEEDED)
    {
        // Time for the firmware upload = 25 seconds by default
        int nUploadTime = 25000;
        is_GetDuration(*hCam, IS_STARTER_FW_UPLOAD, &nUploadTime);

        std::string Str1, Str2;
        Str1 = "This camera requires a new firmware. The upload will take about";
        Str2 = "seconds. Please wait ...";
        printf("%s %d %s", Str1.c_str(), nUploadTime / 1000, Str2.c_str());


        // Try again to open the camera. This time we allow the automatic upload of the firmware by
        // specifying "IS_ALLOW_STARTER_FIRMWARE_UPLOAD"
        *hCam = (HIDS)(((int)*hCam) | IS_ALLOW_STARTER_FW_UPLOAD);
        nRet = is_InitCamera(hCam, hWnd);
    }

    return nRet;
}

void IDS_Cam::GetMaxImageSize(INT *pnSizeX, INT *pnSizeY)
{
    // Check if the camera supports an arbitrary AOI
    // Only the ueye xs does not support an arbitrary AOI
    INT nAOISupported = 0;
    BOOL bAOISupported = TRUE;
	if (is_ImageFormat(m_hCam,
        IMGFRMT_CMD_GET_ARBITRARY_AOI_SUPPORTED,
        (void*)&nAOISupported,
        sizeof(nAOISupported)) == IS_SUCCESS)
    {
        bAOISupported = (nAOISupported != 0);
    }

    if (bAOISupported)
    {
        // All other sensors
        // Get maximum image size
        SENSORINFO sInfo;
		is_GetSensorInfo(m_hCam, &sInfo);
        *pnSizeX = sInfo.nMaxWidth;
        *pnSizeY = sInfo.nMaxHeight;
    }
    else
    {
        // Only ueye xs
        // Get image size of the current format
        IS_SIZE_2D imageSize;
		is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

        *pnSizeX = imageSize.s32Width;
        *pnSizeY = imageSize.s32Height;
    }
}

bool IDS_Cam::GetExposure(std::vector<double>& exp_max_min_inc_cur)
{
    double dTemp[3];

    // get exposure range
	is_Exposure(m_hCam, IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE, (void*)&dTemp, sizeof(dTemp));

    m_MinExp = dTemp[0];
    m_MaxExp = dTemp[1];
    m_IncExp = dTemp[2];
    printf("min,max,inc exp: %f %f %f\n", m_MinExp, m_MaxExp, m_IncExp);

	int error = is_Exposure(m_hCam, IS_EXPOSURE_CMD_GET_EXPOSURE, (void*)&m_CurExp, sizeof(m_CurExp));

    exp_max_min_inc_cur.clear();
    exp_max_min_inc_cur.push_back(m_MaxExp);
    exp_max_min_inc_cur.push_back(m_MinExp);
    exp_max_min_inc_cur.push_back(m_IncExp);
    exp_max_min_inc_cur.push_back(m_CurExp);

    return error == IS_SUCCESS;
}

int IDS_Cam::GetGain()
{
	m_CurGain = is_SetHardwareGain(m_hCam, IS_GET_MASTER_GAIN, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
    return m_CurGain;
}

void IDS_Cam::SetAutoParam()
{
    double enable = 1;
    double disable = 0;
	is_SetAutoParameter(m_hCam, IS_SET_ENABLE_AUTO_GAIN, &enable, 0);
	is_SetAutoParameter(m_hCam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &enable, 0);
	is_SetAutoParameter(m_hCam, IS_SET_ENABLE_AUTO_FRAMERATE, &disable, 0);
	is_SetAutoParameter(m_hCam, IS_SET_ENABLE_AUTO_SHUTTER, &enable, 0);
	is_SetAutoParameter(m_hCam, IS_SET_ENABLE_AUTO_SENSOR_GAIN, &enable, 0);
	is_SetAutoParameter(m_hCam, IS_SET_ENABLE_AUTO_SENSOR_WHITEBALANCE, &enable, 0);
	is_SetAutoParameter(m_hCam, IS_SET_ENABLE_AUTO_SENSOR_SHUTTER, &disable, 0);
    //is_SetAutoParameter(m_hCam, IS_SET_ENABLE_AUTO_SENSOR_SHUTTER, &enable, 0);
    
    double FPS, NEWFPS;
    FPS = 15;
	is_SetFrameRate(m_hCam, FPS, &NEWFPS);

    UINT uiCaps = 0;
	INT nRet = is_Focus(m_hCam, FDT_CMD_GET_CAPABILITIES, &uiCaps, sizeof (uiCaps));
    if (nRet == IS_SUCCESS && (uiCaps & FOC_CAP_AUTOFOCUS_SUPPORTED))
    {
        printf("If supported, enable auto focus\n");
		nRet = is_Focus(m_hCam, FOC_CMD_SET_DISABLE_AUTOFOCUS, NULL, 0);
    }
}

void IDS_Cam::DisableAutoParam(void)
{
	//disable auto gain and auto exposure
	double disable = 0.0;
	is_AutoParameter(m_hCam, IS_AES_CMD_SET_ENABLE, &disable, 0);
	is_SetAutoParameter(m_hCam, IS_SET_ENABLE_AUTO_GAIN, &disable, 0);

    UINT disableAuto = IS_AUTOPARAMETER_DISABLE;
	is_AutoParameter(m_hCam, IS_AWB_CMD_SET_ENABLE, (void *)&disableAuto, sizeof(disableAuto));
}


void IDS_Cam::SetImageSize(int w, int h)
{
	img_width = w;
	img_height = h;
}

cv::Mat IDS_Cam::Read()
{
    cv::Mat im;
    if (m_hCam){
		if (is_FreezeVideo(m_hCam, IS_WAIT) == IS_SUCCESS){
            void *pMemVoid; //pointer to where the image is stored
			is_GetImageMem(m_hCam, &pMemVoid);
            //im = cv::Mat(cv::Size(img_width, img_height), CV_8UC3, pMemVoid).clone();
			int w, h;
			GetMaxImageSize(&w, &h);
			im = cv::Mat(cv::Size(w, h), CV_8UC3, pMemVoid).clone();
		}
    }
    return im;
}

 
cv::Mat IDS_Cam::Read1()
{
	cv::Mat im;
	//if (m_hCam){
	//	if (is_FreezeVideo(m_hCam, IS_DONT_WAIT) == IS_SUCCESS){
	//		void *pMemVoid; //pointer to where the image is stored
	//		is_GetImageMem(m_hCam, &pMemVoid);
	//		//im = cv::Mat(cv::Size(img_width, img_height), CV_8UC3, pMemVoid).clone();
	//		int w, h;
	//		GetMaxImageSize(&w, &h);
	//		im = cv::Mat(cv::Size(w, h), CV_8UC3, pMemVoid).clone();
	//	}
	//}
	return im;
}

bool IDS_Cam::SetExposure(double exposure_time)
{
	int error = is_Exposure(m_hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*)&exposure_time, sizeof(exposure_time));
    if (error != IS_SUCCESS) {
        printf("set exposure failed: %f\n", exposure_time);
        return false;
    }
    return true;
}

bool IDS_Cam::SetExposure(uchar expected_brightness, cv::Rect roi)
{
    cv::Scalar val(0);

    std::vector<double> exp_max_min_inc_cur;
    GetExposure(exp_max_min_inc_cur);
    double exp_cur = exp_max_min_inc_cur[3];
    double step = (exp_max_min_inc_cur[0] - exp_cur) / 5;
    int i = 0;
    while (abs(val[0] - expected_brightness) > step && i++<5){
        cv::Mat im = Read();
        cv::Mat img = im;
        if (roi.width > 0 && roi.width<im.cols && roi.height > 0 && roi.height<im.rows){
            img = im(roi);
        }
        val = mean(im);
        if (im.channels() == 3){
            val[0] = (val[0] + val[1] + val[2]) / 3;
        }

        exp_cur = val[0] > expected_brightness ? exp_cur - step : exp_cur + step;
        SetExposure(exp_cur);
    }

    return true;
}

bool IDS_Cam::SetGain(double gain)
{
	int err = is_SetHardwareGain(m_hCam, gain, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);

    return err == IS_SUCCESS;
}

void IDS_Cam::operator>>(cv::Mat& im)
{
    im = Read();
}

bool IDS_Cam::isOpened()
{
    return m_hCam != 0;
}

void IDS_Cam::PrintSensorInfo()
{
    printf("Sensor Info: \n"
        "\t SensorID: 0x%x\n"
        "\t strSensorName: %s\n"
        "\t nColorMode: %d\n"
        "\t nMaxWidth: %d\n"
        "\t nMaxHeight: %d\n"
        "\t wPixelSize: %d\n"
        , m_sInfo.SensorID
        , m_sInfo.strSensorName
        , m_sInfo.nColorMode
        , m_sInfo.nMaxWidth
        , m_sInfo.nMaxHeight
        , m_sInfo.wPixelSize
        );
}

void IDS_Cam::PrintAutoInfo(void)
{
	UEYE_AUTO_INFO auto_info;
	is_GetAutoInfo(m_hCam, &auto_info);
    UINT enableAWB = 0;
	is_AutoParameter(m_hCam, IS_AWB_CMD_GET_ENABLE, (void *)&enableAWB, sizeof(enableAWB));
    std::cout << "AAntiFlickerCaps: " << auto_info.AAntiFlickerCaps
        << "\nAGainPhotomCaps: " << auto_info.AGainPhotomCaps
        << "\nAShutterPhotomCaps: " << auto_info.AShutterPhotomCaps
        << "\nAutoAbility: " << auto_info.AutoAbility
        << "\nsBrightCtrlStatus curCtrlStatus: " << auto_info.sBrightCtrlStatus.curCtrlStatus
        << "\nSensorWBModeCaps: " << auto_info.SensorWBModeCaps
        << "\nsWBCtrlStatus curController: " << auto_info.sWBCtrlStatus.curController
        << "\nAWBEnabled: " << enableAWB
		<< std::endl;

	double enable = 0.0;
	is_AutoParameter(m_hCam, IS_AES_CMD_GET_ENABLE, &enable, 0);
	std::cout << "auto aes enabled: " << enable << std::endl;

	is_SetAutoParameter(m_hCam, IS_GET_ENABLE_AUTO_GAIN, &enable, 0);
	std::cout << "auto gain enabled: " << enable << std::endl;
}

void IDS_Cam::SetExternalTrigger()
{
	if (is_CaptureVideo(m_hCam, IS_WAIT) == IS_SUCCESS)
	{
		is_StopLiveVideo(m_hCam, IS_WAIT);
	}
	is_SetExternalTrigger(m_hCam	, IS_SET_TRIGGER_HI_LO);
	is_CaptureVideo(m_hCam, IS_DONT_WAIT);
	
	//设置相机的触发延时 
	is_SetTriggerDelay(m_hCam, 1000/*1020990*/);
	is_SetTimeout(m_hCam, IS_TRIGGER_TIMEOUT, 9999999999999);
}


void IDS_Cam::SetGPIO(int num,bool isOpen)
{
	INT nRet = IS_SUCCESS;
		IO_GPIO_CONFIGURATION gpioConfiguration;
	if (num == 1)
	{
		// Set configuration of GPIO1 (OUTPUT LOW)
		gpioConfiguration.u32Gpio = IO_GPIO_1;
		gpioConfiguration.u32Configuration = IS_GPIO_OUTPUT;
	}
	if (num == 2)
	{
		// Set configuration of GPIO2 (OUTPUT LOW)
		gpioConfiguration.u32Gpio = IO_GPIO_2;
		gpioConfiguration.u32Configuration = IS_GPIO_OUTPUT;
	}
	
	if (isOpen == true)
	{
		gpioConfiguration.u32State = 1;
	}
	if (isOpen==false)
	{
		gpioConfiguration.u32State = 0;
	}
	
	nRet = is_IO(m_hCam, IS_IO_CMD_GPIOS_SET_CONFIGURATION, (void*)&gpioConfiguration,

		sizeof(gpioConfiguration));
}

void IDS_Cam::GetFlash()
{
	INT nRet1 = IS_SUCCESS;
	// Disable flash
	UINT nMode = IO_FLASH_MODE_OFF;
	nRet1 = is_IO(m_hCam, IS_IO_CMD_FLASH_SET_MODE, (void*)&nMode, sizeof(nMode));
	nMode = IO_FLASH_MODE_CONSTANT_LOW;
	nRet1 = is_IO(m_hCam, IS_IO_CMD_FLASH_SET_MODE, (void*)&nMode, sizeof(nMode));
	
	 nMode = IO_FLASH_MODE_OFF;
	 nRet1 = is_IO(m_hCam, IS_IO_CMD_FLASH_SET_MODE, (void*)&nMode, sizeof(nMode));
	nMode = IO_FLASH_MODE_CONSTANT_HIGH;
	nRet1 = is_IO(m_hCam, IS_IO_CMD_FLASH_SET_MODE, (void*)&nMode, sizeof(nMode));
	
	return; 

	INT nRet = IS_SUCCESS;
	// Read the global flash params
	IO_FLASH_PARAMS flashParams;
	nRet = is_IO(m_hCam, IS_IO_CMD_FLASH_GET_GLOBAL_PARAMS,
		(void*)&flashParams, sizeof(flashParams));
	if (nRet == IS_SUCCESS)
	{
		INT nDelay = flashParams.s32Delay;
		UINT nDuration = flashParams.u32Duration;
	}
	nRet = is_IO(m_hCam, IS_IO_CMD_FLASH_APPLY_GLOBAL_PARAMS, NULL, 0);
}

void IDS_Cam::SetLedRed()
{
	INT nRet = IS_SUCCESS;
	UINT nCurrentState = 0;

	nRet = is_IO(m_hCam, IS_IO_CMD_LED_GET_STATE, (void*)&nCurrentState, sizeof(nCurrentState));

	nCurrentState = IO_LED_STATE_1;
	nRet = is_IO(m_hCam, IS_IO_CMD_LED_SET_STATE, (void*)&nCurrentState, sizeof(nCurrentState));
}

void IDS_Cam::SetGamma(int gamma)
{
	INT nRet = is_Gamma(m_hCam, IS_GAMMA_CMD_SET, (void*)&gamma, sizeof(gamma));
}

double IDS_Cam::GetGamma()
{
	INT nGamma;
	INT nRet = is_Gamma(m_hCam, IS_GAMMA_CMD_GET, (void*)&nGamma, sizeof(nGamma));
	return nGamma;
}

bool IDS_Cam::OpenCameraforLoadPara()
{
	if (m_hCam != 0)
	{
		//free old image mem.
		is_FreeImageMem(m_hCam, m_ImageMemory, m_lMemoryId);
		is_ExitCamera(m_hCam);
	}

	// init camera
	m_hCam = (HIDS)10;						// open next camera
	m_Ret = InitCamera(&m_hCam, NULL);		// init camera - no window handle required
	//////////////////////////
	
	/////////////////////////
	if (m_Ret == IS_SUCCESS)
	{
		// Get sensor info
		is_GetSensorInfo(m_hCam, &m_sInfo);

		GetMaxImageSize(&img_width, &img_height);

		// setup the color depth to the current windows setting
		is_GetColorDepth(m_hCam, &m_nBitsPerPixel, &m_nColorMode);
		is_SetColorMode(m_hCam, m_nColorMode);

		// memory initialization
		is_AllocImageMem(m_hCam,
			img_width,
			img_height,
			m_nBitsPerPixel,
			&m_ImageMemory,
			&m_lMemoryId);
		is_SetImageMem(m_hCam, m_ImageMemory, m_lMemoryId);	// set memory active

		// display initialization
		IS_SIZE_2D imageSize;
		imageSize.s32Width = img_width;
		imageSize.s32Height = img_height;

		is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));

		is_SetDisplayMode(m_hCam, IS_SET_DM_DIB);
		return true;
	}
	return false;
}

bool IDS_Cam::LoadParameter()
{
	INT nRet = is_ParameterSet(m_hCam, IS_PARAMETERSET_CMD_LOAD_FILE, L"CameraConfig_shanggai.ini", NULL);
	//INT nRet = is_ParameterSet(m_hCam, IS_PARAMETERSET_CMD_LOAD_FILE, L"D:\\CameraConfig\\1.ini", NULL);

	if (nRet != IS_SUCCESS)
	{
		return false;
	}
	is_FreeImageMem(m_hCam, m_ImageMemory, m_lMemoryId);

	IS_SIZE_2D imageSize;
	is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

	INT nAllocSizeX = 0;
	INT nAllocSizeY = 0;

	img_width = nAllocSizeX = imageSize.s32Width;
	img_height = nAllocSizeY = imageSize.s32Height;

	UINT nAbsPosX = 0;
	UINT nAbsPosY = 0;

	// absolute pos?
	is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_X_ABS, (void*)&nAbsPosX, sizeof(nAbsPosX));
	is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_Y_ABS, (void*)&nAbsPosY, sizeof(nAbsPosY));

	if (nAbsPosX)
	{
		nAllocSizeX = m_sInfo.nMaxWidth;
	}
	if (nAbsPosY)
	{
		nAllocSizeY = m_sInfo.nMaxHeight;
	}

	switch (is_SetColorMode(m_hCam, IS_GET_COLOR_MODE))
	{
	case IS_CM_RGBA12_UNPACKED:
	case IS_CM_BGRA12_UNPACKED:
		m_nBitsPerPixel = 64;
		break;

	case IS_CM_RGB12_UNPACKED:
	case IS_CM_BGR12_UNPACKED:
	case IS_CM_RGB10_UNPACKED:
	case IS_CM_BGR10_UNPACKED:
		m_nBitsPerPixel = 48;
		break;

	case IS_CM_RGBA8_PACKED:
	case IS_CM_BGRA8_PACKED:
	case IS_CM_RGB10_PACKED:
	case IS_CM_BGR10_PACKED:
	case IS_CM_RGBY8_PACKED:
	case IS_CM_BGRY8_PACKED:
		m_nBitsPerPixel = 32;
		break;

	case IS_CM_RGB8_PACKED:
	case IS_CM_BGR8_PACKED:
		m_nBitsPerPixel = 24;
		break;

	case IS_CM_BGR565_PACKED:
	case IS_CM_UYVY_PACKED:
	case IS_CM_CBYCRY_PACKED:
		m_nBitsPerPixel = 16;
		break;

	case IS_CM_BGR5_PACKED:
		m_nBitsPerPixel = 15;
		break;

	case IS_CM_MONO16:
	case IS_CM_SENSOR_RAW16:
	case IS_CM_MONO12:
	case IS_CM_SENSOR_RAW12:
	case IS_CM_MONO10:
	case IS_CM_SENSOR_RAW10:
		m_nBitsPerPixel = 16;
		break;

	case IS_CM_RGB8_PLANAR:
		m_nBitsPerPixel = 24;
		break;

	case IS_CM_MONO8:
	case IS_CM_SENSOR_RAW8:
	default:
		m_nBitsPerPixel = 8;
		break;
	}

	// memory initialization
	is_AllocImageMem(m_hCam, nAllocSizeX, nAllocSizeY, m_nBitsPerPixel, &m_ImageMemory, &m_lMemoryId);

	is_SetImageMem(m_hCam, m_ImageMemory, m_lMemoryId);	// set memory active

	// display initialization
	imageSize.s32Width = img_width;
	imageSize.s32Height = img_height;

	is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));
	return true;
}

void IDS_Cam::RecordVideo()
{
	int ret = isavi_InitAVI(&m_aviId, m_hCam);

	int nWidth, nHeight, nBits, nPitch;
	ret = is_InquireImageMem(m_hCam, m_ImageMemory, m_lMemoryId, &nWidth, &nHeight, &nBits, &nPitch);
	IS_POINT_2D nOffset;
	ret = is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS, (void*)&nOffset, sizeof(nOffset));

	INT nPitchPx = 0;
	nPitchPx = (nPitch * 8) / nBits;

	INT nAviWidth = nWidth / 8 * 8; // Width must be multiple of 8
	INT LineOffsetPx = nPitchPx - nAviWidth;
	ret = isavi_SetImageSize(m_aviId, m_nColorMode, nAviWidth, nHeight, nOffset.s32X, nOffset.s32Y, LineOffsetPx);

	ret = isavi_SetImageQuality(m_aviId, 100);

	ret = isavi_OpenAVI(m_aviId, "E:\\1.avi");

	ret = isavi_SetFrameRate(m_aviId, 15.0);

	ret = isavi_StartAVI(m_aviId);
	int i = 0;
	is_CaptureVideo(m_hCam, IS_WAIT);
	if (m_ImageMemory != NULL)
	{
		/*while (i<100)
		{
			i++;
			ret = isavi_AddFrame(m_aviId, m_ImageMemory);
		}*/
	}
	/*
	 ret = isavi_CloseAVI(m_aviId);
	ret = isavi_ExitAVI(m_aviId);*/

	//m_bStartRecord = true;
	//m_nCount = 100;
}

void IDS_Cam::stopVideo()
{
	
	/*INT ret = isavi_CloseAVI(m_aviId);
	ret = isavi_ExitAVI(m_aviId);*/
}

void IDS_Cam::Record()
{
	CAMINFO* pInfo = 0;
	// needed memory
	char* m_pcImgMem;
	int m_nImgId;
	m_aviId = 0;
	
	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (m_hEvent == NULL)
	{
		return  ;
	}
	is_InitEvent(m_hCam, m_hEvent, IS_SET_EVENT_FRAME);

	is_EnableEvent(m_hCam, IS_SET_EVENT_FRAME);


	
	// Init Camera
	int RetVal;
	RetVal = is_SetDisplayMode(m_hCam, IS_SET_DM_DIB);
		RetVal = is_GetCameraInfo(m_hCam, pInfo);
	SENSORINFO MySensor;
	RetVal = is_GetSensorInfo(m_hCam, &MySensor);
	if (RetVal != IS_SUCCESS)
	{
		cout << "set sensor mode faild" << endl;
	}

	// setup color format
	int nbpp;
	//if (MySensor.nColorMode == IS_COLORMODE_BAYER)
	//{
	//    RetVal = is_SetColorMode(m_hCam, IS_CM_RGB8_PACKED);
	//    if(RetVal != IS_SUCCESS)
	//    {
	//        cout << "set color mode RGB8 faild" << endl;
	//    }
	//    nbpp = 24;
	//}
	//else
	//{
	RetVal = is_SetColorMode(m_hCam, IS_CM_MONO8);
	if (RetVal != IS_SUCCESS)
	{
		cout << "set color mode MONO8 faild" << endl;
	}
	nbpp = 8;
	//}

	// allocate image buffer
	RetVal = is_AllocImageMem(m_hCam, MySensor.nMaxWidth, MySensor.nMaxHeight, nbpp, &m_pcImgMem, &m_nImgId);
	if (RetVal != IS_SUCCESS)
	{
		cout << "alloca memory faild" << endl;
	}
	// set the image buffer for grabbing
	RetVal = is_SetImageMem(m_hCam, m_pcImgMem, m_nImgId);
	if (RetVal != IS_SUCCESS)
	{
		cout << "set memory faild" << endl;
	}

	int ret = 0;
		ret = isavi_InitAVI(&m_aviId, m_hCam);
 
	int nWidth, nHeight, nBits, nPitch;
	ret = is_InquireImageMem(m_hCam, m_pcImgMem, m_nImgId, &nWidth, &nHeight, &nBits, &nPitch);
 
	IS_POINT_2D nOffset;
	ret = is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS, (void*)&nOffset, sizeof(nOffset));
 
	INT nPitchPx = 0;
	nPitchPx = (nPitch * 8) / nBits;

	INT nAviWidth = nWidth / 8 * 8; // Width must be multiple of 8
	INT LineOffsetPx = nPitchPx - nAviWidth;
	ret = isavi_SetImageSize(m_aviId, IS_CM_MONO8, nAviWidth, nHeight, nOffset.s32X, nOffset.s32Y, LineOffsetPx);
 
	ret = isavi_SetImageQuality(m_aviId, 100);
 
	ret = isavi_OpenAVI(m_aviId, "E:\\1.avi");
 
	ret = isavi_SetFrameRate(m_aviId, 10.0);
 
	ret = isavi_StartAVI(m_aviId);
 
	RetVal = is_CaptureVideo(m_hCam, IS_DONT_WAIT);
 
	int nRet = -1;
	int nFrameCount = 50;
	while (nFrameCount--)
	{
		//nRet = is_WaitEvent(m_hCam, IS_SET_EVENT_FRAME, 1000);
		if (WaitForSingleObject(m_hEvent, 10000) != WAIT_OBJECT_0)
		{
			/* wait timed out */
			cout << "capture time-out!" << endl;
		}
		else
		{
			//Sleep(200);
			int ret = isavi_AddFrame(m_aviId, m_pcImgMem);
			if (ret != IS_SUCCESS)
			{
				cout << "set avi add frame faild" << endl;
			}
			//cout<<++nFrameCount<<endl;
		}
	}

	 ret = isavi_StopAVI(m_aviId);
 //
	 ret = isavi_CloseAVI(m_aviId);
 

	is_DisableEvent(m_hCam, IS_SET_EVENT_FRAME);
	is_ExitEvent(m_hCam, IS_SET_EVENT_FRAME);
	CloseHandle(m_hEvent);

	 
}

bool IDS_Cam::isGetCameraList()
{
	INT nNumCam;
	if (is_GetNumberOfCameras(&nNumCam) == IS_SUCCESS)
	{
		if (nNumCam >= 1) {
			// Create new list with suitable size
			pucl = (UEYE_CAMERA_LIST*) new BYTE[sizeof (DWORD)+nNumCam * sizeof (UEYE_CAMERA_INFO)];
			pucl->dwCount = nNumCam;
			if (is_GetCameraList(pucl) == IS_SUCCESS)
			{
				return true;
				/*int iCamera;
				for (iCamera = 0; iCamera < (int)pucl->dwCount; iCamera++)
				{
					printf("Camera %i CameraId: %d DeviceID :%d SensorModel:%s  SerialNO %s\n", iCamera, pucl->uci[iCamera].dwCameraID, pucl->uci[iCamera].dwDeviceID,pucl->uci[iCamera].Model, pucl->uci[iCamera].SerNo);
				}*/
			}
			//delete[] pucl;
		}
		return false;
	}
}

bool IDS_Cam::saveParametertoCamera()
{
	INT nRet = is_ParameterSet(m_hCam, IS_PARAMETERSET_CMD_SAVE_EEPROM, NULL, NULL);
	if (nRet != IS_SUCCESS)
	{
		return false;
	}
	return true;
}
bool IDS_Cam::LoadParameterFromCamera()
{
	INT nRet = is_ParameterSet(m_hCam, IS_PARAMETERSET_CMD_LOAD_EEPROM, NULL, NULL);
	if (nRet != IS_SUCCESS)
	{
		return false;
	}
	return true;
}
bool IDS_Cam::stopSnap()
{
	if (m_hCam != 0)
	{
		is_CaptureVideo(m_hCam, IS_DONT_WAIT);
	}
	INT nRet=is_StopLiveVideo(m_hCam, IS_FORCE_VIDEO_STOP);
	if (nRet != IS_SUCCESS)
	{
		return false;
	}
	return true;
}

bool IDS_Cam::getCurrentFPS(double &NEWFPS)
{
	double FPS;
	FPS = 15;
	int ret=is_SetFrameRate(m_hCam, FPS, &NEWFPS);
	if (ret == IS_SUCCESS)
	{
		return true;
	}
}

bool IDS_Cam::GetCurrentExposure(double &currentExposure)
{
	int error = is_Exposure(m_hCam, IS_EXPOSURE_CMD_GET_EXPOSURE, (void*)&m_CurExp, sizeof(m_CurExp));
	currentExposure = m_CurExp;
	return error == IS_SUCCESS;
}