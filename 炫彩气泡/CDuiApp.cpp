
#include "CDuiApp.h"
#include "CWndBase.h"
#include "public.h"

HINSTANCE CDuiApp::m_hInstance = 0;

CDuiApp::CDuiApp(HINSTANCE hInstance) 
{
	m_hInstance = hInstance;

	//1.初始化
	init();
}


CDuiApp::~CDuiApp()
{
}

void CDuiApp::init(){
	//初始化gdi+资源
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

void CDuiApp::release(){
	//释放gdi+资源
	GdiplusShutdown(m_gdiplusToken);
}

void CDuiApp::run()
{
	//2.开启消息循环
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//3.释放资源
	release();
	return;
}

HINSTANCE CDuiApp::hInstance(){
	return m_hInstance;
}
