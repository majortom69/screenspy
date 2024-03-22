#ifndef GETDATA_CLASS_H
#define GETDATA_CLASS_H

#include <Windows.h> 
#include <fstream>
#include <wincodec.h> 
#include <string>
#include <gdiplus.h>
#include <time.h>
#include <sstream>
#include <winternl.h>
#include <Shlwapi.h> // Include for PathCombine function
#include <tchar.h> 

#include <iostream>



class getData {
public:
	getData();
	~getData();

	
	void takescreenshot(int screenWidth, int screenHeight);
	wchar_t getTempPath();
	
private:
	char* getTgPath();
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
};

#endif