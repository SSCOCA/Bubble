/*******************************************
* Name:      WndBase.h
* Describe:  窗口基类
* Author:    SSCOCA
* Version:   1.00
* Date:      2018年8月24日
* Other:
*********************************************/
#ifndef _C_WND_BASE_
#define _C_WND_BASE_
#include <windows.h>
#include "public.h"
#include "CDuiApp.h"

class CBubbleManger;
class CWndBase
{
public:
	CWndBase(HINSTANCE hInstance=CDuiApp::hInstance());
	virtual ~CWndBase();

	void show(bool isModal = false, CWndBase* parentWnd = nullptr);
	inline HWND hWnd()const{ return m_hWnd; }
	
	virtual LRESULT OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnDestory(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnTimer(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual int preDispatchMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int dispatchMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	void init();
	bool create();
	bool registerWindow();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	HWND m_hWnd;			//窗口句柄
	HINSTANCE m_hInstance;	//程序实例
	CBubbleManger* m_pBubbleManger;
};

#endif