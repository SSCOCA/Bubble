#include "CDuiApp.h"
#include "CWndBase.h"

int CALLBACK WinMain(
	_In_  HINSTANCE hInstance,
	_In_  HINSTANCE hPrevInstance,
	_In_  LPSTR lpCmdLine,
	_In_  int nCmdShow)
{
	CDuiApp app(hInstance);
	CWndBase wnd1;
	wnd1.show();
	SetTimer(wnd1.hWnd(), 0, 5, NULL);
	app.run();
	return 0;
}