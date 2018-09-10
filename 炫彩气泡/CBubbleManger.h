#pragma once
#include <vector>
#include <map>
#include <windows.h>
using namespace std;
class CBubble;
class CBubbleManger
{
public:
	CBubbleManger(int num);
	~CBubbleManger();
	void show(HDC hdc);
	void move();
private:
	void init(int num);
	void collisDetect();

private:
	vector<CBubble*> m_vBubble;
};

