#include "CBubbleManger.h"
#include "CBubble.h"
#include <windows.h>
#include <algorithm>

bool comp(const CBubble *b1, const CBubble *b2)
{
	return (*b1) < (*b2);
}

CBubbleManger::CBubbleManger(int num)
{
	init(num);
}


CBubbleManger::~CBubbleManger()
{
	for (int i = 0; i < m_vBubble.size(); i++)
	{
		delete m_vBubble[i];
		m_vBubble[i] = nullptr;
	}

	m_vBubble.clear();
}

void CBubbleManger::init(int num)
{

	int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	int gapX = ScreenWidth / num;
	int gapY = ScreenHeight / num;

	for (int i = 0; i < num; i++)
	{
		m_vBubble.push_back(new CBubble(Point(gapX * i + rand() %50,  rand() % ScreenHeight), Color(60, rand() % 255, rand() % 255, rand() % 255)));
	}
}

void CBubbleManger::move()
{
	collisDetect();
	for each (CBubble* pBubble in m_vBubble)
	{
		pBubble->move();
	}
}

void CBubbleManger::collisDetect()
{
	typedef vector<CBubble*>::iterator Iter;
	sort(m_vBubble.begin(), m_vBubble.end(), comp);
	for (Iter iter = m_vBubble.begin(); iter != m_vBubble.end(); iter++)
	{
		for (Iter iter2 = iter + 1; iter2 != m_vBubble.end(); iter2++)
		{
			Point pt1 = (*iter)->getCenter();
			Point pt2 = (*iter2)->getCenter();
			int radius1 = (*iter)->getRadius();
			int radius2 = (*iter2)->getRadius();

			if ((pow2(pt1.X-pt2.X)+pow2(pt1.Y-pt2.Y))<pow2(radius1+radius2))
			{
				(*iter)->hit(*iter2);
			}
			else
			{
				//break;
			}
		}
	}
}

void CBubbleManger::show(HDC hdc)
{
	for each (CBubble* pBubble in m_vBubble)
	{
		pBubble->paint(hdc);
	}
}