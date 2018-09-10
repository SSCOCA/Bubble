#ifndef _CBUBBLE_H_
#define _CBUBBLE_H_
#include "public.h"
#define pow2(x) ((x)*(x))
class CBubble
{
public:
	CBubble(const Point&pt,  Color& color, int radius=80);
	~CBubble();
	void paint(HDC hdc);
	void hit(CBubble* bubble);
	void move();
	int getRadius(){ return m_radius; }
	Point getCenter(){ return Point(m_x + m_radius, m_y + m_radius); }
	bool operator< (const CBubble& b2)const
	{
		return (pow2(m_x + m_radius) + pow2(m_y + m_radius)) < (pow2(b2.m_x + m_radius) + pow2(b2.m_y + m_radius));
	}
private:
	int m_radius;//°ë¾¶
	Color m_color;//ÑÕÉ«
	double m_x;
	double m_y;
	double m_vx;
	double m_vy;
	long m_count;
	CBubble *m_lastHit;
};

#endif
