/*******************************************
* Name:      WndApp.h
* Describe:  CWndApp类-应用程序的主类
* Author:    SSCOCA
* Version:   1.00
* Date:      2018年8月24日
* Other:
*********************************************/
#ifndef _C_WND_APP_
#define _C_WND_APP_
#include <windows.h>

class CWndBase;
class CDuiApp
{
public:
	CDuiApp(HINSTANCE hInstance);
	~CDuiApp();
	
	void run();
	static HINSTANCE hInstance();
private:
	void init();
	void release();

private:
	static HINSTANCE m_hInstance;
	ULONG_PTR m_gdiplusToken;
};

#endif