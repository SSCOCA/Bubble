#include "CBubble.h"
#include <math.h>
#include <time.h>

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

CBubble::CBubble(const Point&pt, Color& color, int radius) :m_x(pt.X), m_y(pt.Y),
															m_vx((rand() % 10 - 5)/2),
															m_vy((rand() % 10 - 5)/2),
															m_radius(radius), m_color(color)
{
}


CBubble::~CBubble()
{
}

void CBubble::paint(HDC hdc)
{
	Bitmap bmp(2*m_radius, 2*m_radius, PixelFormat32bppARGB);
	Graphics g(&bmp);
	g.Clear(Color(0, 0, 0, 0));
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	GraphicsPath path;
	path.AddEllipse(0, 0, 2*m_radius, 2*m_radius);
	PathGradientBrush PGBrush(&path);
	PGBrush.SetCenterColor(Color(200, 255, 255, 255));
	Color colors[] = {m_color};
	int count = 1;
	PGBrush.SetSurroundColors(colors, &count);
	PGBrush.SetCenterPoint(Point(m_radius, m_radius));
	g.FillEllipse(&PGBrush, 0, 0, 2*m_radius, 2*m_radius);
	g.DrawEllipse(&Pen(m_color), 0, 0, 2*m_radius, 2*m_radius);
	//g.FillRectangle(&PGBrush, 0, 0, 200, 200);

	Graphics graph(hdc);
	graph.TranslateTransform(m_x, m_y);
	graph.SetSmoothingMode(SmoothingModeAntiAlias);
	graph.DrawImage(&bmp, 0, 0);
	
	//CLSID pngClsid;
	//GetEncoderClsid(L"image/png", &pngClsid);
	//bmp.Save(L"Mosaic2.png", &pngClsid, NULL);
	
	
	//Graphics graph(hdc);
	//graph.SetSmoothingMode(SmoothingModeAntiAlias);
	//for (int i = 0; i < 200; i++)
	//{
	//	float x = 100;
	//	float y = 100;
	//	float R = i;
	//	RectF rect(x-R, y-R, 2*R, 2*R);
	//	graph.DrawEllipse(&Pen(Color(255, 200, 200, 100)), rect);
	//}
	//graph.DrawImage(&bmp, 0, 0);

	//Graphics graph(hdc);
	//graph.SetSmoothingMode(SmoothingModeHighQuality);
	//Bitmap bmp(L"Mosaic2.png");
	//graph.DrawImage(&bmp, 0, 0);


	//Graphics graph(hdc);
	//graph.Clear(Color(0, 0, 0, 0));
	//graph.SetSmoothingMode(SmoothingModeAntiAlias);
	//LinearGradientBrush lBrush(Point(100, 100), Point(200, 200), Color(200, 200, 100,100), Color(0, 200, 100,100));
	////graph.FillEllipse(&lBrush, Rect(100, 100, 200, 200));

	//Graphics graphics(hdc);
	//graphics.Clear(Color(0, 255, 255, 255));
	//GraphicsPath path;
	//path.AddEllipse(0, 0, 200, 200);
	//PathGradientBrush PGBrush(&path);
	//PGBrush.SetCenterColor(Color(255, 255, 2, 255));
	//Color colors[] = { Color(255, 255, 2, 255) };
	//int count = 1;
	//PGBrush.SetSurroundColors(colors, &count);
	//PGBrush.SetCenterPoint(Point(100, 100));
	//graphics.FillRectangle(&PGBrush, 0, 0, 200, 200);

	//SolidBrush brush(Color(10, 200, 200, 250));
	//graphics.TranslateTransform(100, 100);
	//graphics.FillRectangle(&brush, 10, 200, 200, 200);
	
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes
	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure
	GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}
	free(pImageCodecInfo);
	return -1;  // Failure
}

void CBubble::move()
{
	static int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	static int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	m_x += m_vx;
	m_y += m_vy;

	if (m_x > (ScreenWidth - 2 * m_radius) && m_vx>0)
	{
		m_vx *= -1;
	} 
	if( m_x<0 && m_vx<0)
	{
		m_vx *= -1;
	}
	
	if (m_y>(ScreenHeight-2*m_radius)&& m_vy>0)
	{
		m_vy *= -1;
	}
	if (m_y<0&&m_vy<0)
	{
		m_vy *= -1;
	}
}

void CBubble::hit(CBubble* pBubble)
{
	Color tmpColor = m_color;
	int tmpVx = m_vx;
	int tmpVy = m_vy;

	//m_color = pBubble->m_color;
	m_vx = pBubble->m_vx;
	m_vy = pBubble->m_vy;
	
	pBubble->m_vx = tmpVx;
	pBubble->m_vy = tmpVy;
	//pBubble->m_color = tmpColor;

	m_lastHit = pBubble;
}