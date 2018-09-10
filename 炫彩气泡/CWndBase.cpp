#include <tchar.h>
#include "CWndBase.h"
#include "resource.h"
#include "CBubble.h"
#include "CBubbleManger.h"
#include <gdiplusbrush.h>
#define MSG_UNDONE 0
#define MSG_DONE 1
#define className "WindowBase"

CWndBase::CWndBase(HINSTANCE hInstance) :m_hInstance(hInstance), m_hWnd(nullptr)
{
	registerWindow();
	create();
	m_pBubbleManger = new CBubbleManger(10);
}

CWndBase::~CWndBase()
{

}

bool CWndBase::registerWindow(){
	WNDCLASSEX wcex;
	if (GetClassInfoEx(m_hInstance, _T(className), &wcex))
	{
		return true;
	}

	memset(&wcex, 0, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_ICON2));
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));//(HBRUSH)(COLOR_WINDOW + 2);
	
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _T(className);
	wcex.hIconSm = NULL;

	if (0 == ::RegisterClassEx(&wcex))
	{
		return false;
	}else{
		return true;
	}
}

bool CWndBase::create()
{
	if (this->m_hWnd == nullptr)
	{
		DWORD dwStyle = WS_POPUP|WS_OVERLAPPED;
		HWND hwnd = CreateWindow(_T(className),
								_T(className),
								dwStyle,
								0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
								NULL,
								NULL,
								m_hInstance,
								(LPVOID)this);

		if (hwnd == NULL)
		{
			this->m_hWnd = NULL;
			wchar_t msg[100];
			::wsprintf(msg, _T("CreateWindow()失败:%ld"), ::GetLastError());
			::MessageBox(NULL, msg, _T("错误"), MB_OK);
			return false;
		}
		m_hWnd = hwnd;
		LONG nRet = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
		nRet |= WS_EX_LAYERED;
		SetWindowLong(m_hWnd, GWL_EXSTYLE, nRet);
		//SetLayeredWindowAttributes(m_hWnd, RGB(0, 0, 0), 255, LWA_COLORKEY|LWA_ALPHA);
		return true;
	}
	return true;
}

void CWndBase::show(bool isModal, CWndBase* parentWnd){
	if (isModal && parentWnd)//如果是模态窗口则禁用父窗口
	{
		EnableWindow(parentWnd->hWnd(), false);
	}
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
}


LRESULT CWndBase::OnDestory(HWND hWnd, WPARAM wParam, LPARAM lParam){
	// 此时，窗口对象已经销毁，通过设置hWnd=NULL，来通知C++对象
	PostQuitMessage(0);
	m_hWnd = NULL;
	return MSG_DONE;
}


LRESULT CWndBase::OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	PostMessageW(hWnd, WM_PAINT, NULL, NULL);
	m_pBubbleManger->move();
	return MSG_DONE;
}

LRESULT CWndBase::OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam){
	RECT rect;
	GetClientRect(hWnd, &rect);

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);	
	HDC m_hdcMem = CreateCompatibleDC(hdc);
	HBITMAP m_hBitmap = CreateCompatibleBitmap(hdc, rect.right-rect.left, rect.bottom-rect.top); //建立和屏幕兼容的bitmap    
	HBITMAP m_hOldBitmap = (HBITMAP)SelectObject(m_hdcMem, m_hBitmap);//将memBitmap选入内存DC   
	m_pBubbleManger->show(m_hdcMem);

	BLENDFUNCTION _Blend;
	_Blend.BlendOp = 0;
	_Blend.BlendFlags = 0;
	_Blend.AlphaFormat = 1;
	_Blend.SourceConstantAlpha = 255;
	POINT pt = { 0, 0};
	SIZE sz = { rect.right - rect.left, rect.bottom - rect.top };
	UpdateLayeredWindow(m_hWnd, hdc, &pt, &sz, m_hdcMem, &pt, 0, &_Blend, ULW_ALPHA);

	//BitBlt(hdc, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, m_hdcMem, rect.left, rect.top, SRCCOPY);
	EndPaint(hWnd, &ps);
	SelectObject(m_hdcMem, m_hOldBitmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hdcMem);

	return 0;
}

int CWndBase::preDispatchMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			SendMessage(m_hWnd, WM_CLOSE, NULL, NULL);
			return MSG_DONE;
		}
		return MSG_UNDONE;
		
	default:
		return MSG_UNDONE;
	}
}


int CWndBase::dispatchMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch (message)
	{
	case WM_DESTROY:
		return OnDestory(hWnd, wParam, lParam);

	//case WM_MOVE:
	//	return OnMove(hWnd, wParam, lParam);

	case WM_PAINT:
		return OnPaint(hWnd, wParam, lParam);

	case WM_TIMER:
		return OnTimer(hWnd, wParam, lParam);

	default:
		return MSG_UNDONE;//消息未处理，让默认函数去处理
	}		
}


LRESULT CALLBACK CWndBase::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	// 在此消息收到时，把窗口对象句柄赋给C++对象成员
	// 同时把C++对象地址赋给窗口对象成员
	CWndBase* pWnd = NULL;
	if (message == WM_CREATE){
		pWnd = (CWndBase*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
		pWnd->m_hWnd = hWnd;	                                         // 在此处获取HWND，此时CreateWindow()尚未返回。
		::SetWindowLong(hWnd, GWL_USERDATA, (LONG)pWnd);             // 通过USERDATA把HWND和C++对象关联起来
		PostMessageW(hWnd, WM_PAINT, NULL, NULL);
	}

	pWnd = (CWndBase*)::GetWindowLong(hWnd, GWL_USERDATA);
	if (pWnd!=nullptr)
	{
		//消息提前拦截，如果被处理，则不再响应
		if (MSG_DONE == pWnd->preDispatchMessage(hWnd, message, wParam, lParam))
		{
			return MSG_DONE;
		}

		if (MSG_DONE == pWnd->dispatchMessage(hWnd, message, wParam, lParam))
		{
			return MSG_DONE;	
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}else{//注意以下逻辑比较关键，否则无法创建窗口
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
