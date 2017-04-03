#include <afxwin.h>
#include <windows.h>
#include "CMatrix.h"
#include "Functions.h"
#define ID_F1 2002
#define ID_F2 2003
#define ID_F3 2004
#define ID_F4 2005
#define ID_F1234 2006
#define pi 3.14159
int f = 0;
class CMyMainWnd : public CFrameWnd
{
public:
	CMyMainWnd()
	{
		Create(NULL, L"CG&G Laba 3 CPlot2D");
	}
	CPlot2D p1, p2, p3, p4;
	CChildView funct;
	CMenu *menu;
	afx_msg int OnCreate(LPCREATESTRUCT);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT flags, CPoint Loc);
	afx_msg void OnLButtonUp(UINT flags, CPoint Loc);
	void F1();
	void F2();
	void F3();
	void F4();
	void F1234();
	DECLARE_MESSAGE_MAP();
};
class CMyApp : public CWinApp
{
public:
	CMyApp(){};
	virtual BOOL InitInstance()
	{
		m_pMainWnd = new CMyMainWnd();
		m_pMainWnd->ShowWindow(SW_SHOW);
		return TRUE;
	}
};
BEGIN_MESSAGE_MAP(CMyMainWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_COMMAND(ID_F1, F1)
	ON_COMMAND(ID_F2, F2)
	ON_COMMAND(ID_F3, F3)
	ON_COMMAND(ID_F4, F4)
	ON_COMMAND(ID_F1234, F1234)
END_MESSAGE_MAP()

afx_msg void CMyMainWnd::OnPaint()
{
	CPaintDC dc(this);
	if (f == 1)
	{
		dc.TextOut(400, 20, "y=sin(x) / x,    [-3pi;3pi]  dx=pi/36");
		p1.Draw(dc,1,1);
	}
	if (f == 2)
	{
		dc.TextOut(400, 20, "y=pow(x,3),    [-5;5]  dx=0.25");
		p2.Draw1(dc,1, 1);
	}
	if (f == 3)
	{
		dc.TextOut(400, 20, "y=sqrt(x)*sin(x),    [0;6pi]  dx=pi/36");
		p3.Draw(dc, 1, 1);
	}
	if (f == 4)
	{
		dc.TextOut(400, 20, "y=pow(x,2),    [-10;10]  dx=0.25");
		p4.Draw1(dc, 1, 1);
	}
	if (f == 5)
	{
		dc.TextOut(30, 20, "y=sin(x) / x,    [-3pi;3pi]  dx=pi/36");
		p1.Draw(dc, 1, 1);
		dc.TextOut(700, 20, "y=pow(x,3),    [-5;5]  dx=0.25");
		p2.Draw(dc, 1, 1);
		dc.TextOut(30, 340, "y=sqrt(x)*sin(x),    [0;6pi]  dx=pi/36");
		p3.Draw(dc, 1, 1);
		dc.TextOut(700, 340, "y=pow(x,2),    [-10;10]  dx=0.25");
		p4.Draw(dc, 1, 1);
	}
	dc.SetMapMode(MM_TEXT);
}

afx_msg int CMyMainWnd::OnCreate(LPCREATESTRUCT)
{
	menu = new CMenu();
	menu->CreateMenu();
	CMenu *subMenu = new CMenu();
	subMenu->CreatePopupMenu();
	subMenu->AppendMenu(MF_STRING, ID_F1, _T("F1"));
	subMenu->AppendMenu(MF_STRING, ID_F2, _T("F2"));
	subMenu->AppendMenu(MF_STRING, ID_F3, _T("F3"));
	subMenu->AppendMenu(MF_STRING, ID_F4, _T("F4"));
	subMenu->AppendMenu(MF_STRING, ID_F1234, _T("F1234"));
	this->menu->AppendMenu(MF_POPUP | MF_STRING, (UINT)subMenu->m_hMenu, _T("Tests_F"));
	SetMenu(menu);
	return 0;
}

void CMyMainWnd::F1()
{
	int i = 0;
	double  dx = pi / 36;
	int n = 36 * 6 + 1;
	CMatrix XX(n), YY(n);
	XX(0) = -3 * pi;
	for (; XX(i)+dx <= 3 * pi;i++)
	{
		YY(i) = funct.MyF1(XX(i));
		XX(i + 1) = XX(i) + dx;
	}
	YY(i) = funct.MyF1(XX(i));
	CRect r(400, 100, 1000, 600);
	p1.SetParams(XX, YY, r);

	CMyPen p;p.Set(PS_SOLID, 1, RGB(255, 0, 0));
	p1.SetPenLine(p);
	p.Set(PS_SOLID, 2, RGB(0, 0, 255));
	p1.SetPenAxis(p);
	f = 1;
	InvalidateRect(0);
}

void CMyMainWnd::F2()
{
	int i = 0;
	double  dx = 25;
	int n = 10/0.25 + 1;
	CMatrix XX(n), YY(n);
	XX(0) = -500;
	for (; XX(i)+dx <= 500; i++)
	{
		YY(i) = funct.MyF2(XX(i));
		XX(i + 1) = XX(i) + dx;
	}
	YY(i) = funct.MyF2(XX(i));
	CRect r(400, 100, 1000, 600);
	p2.SetParams(XX, YY, r);
	CMyPen p; p.Set(PS_SOLID, 1, RGB(0, 255, 0));
	p2.SetPenLine(p);
	p.Set(PS_SOLID, 2, RGB(0, 0, 255));
	p2.SetPenAxis(p);
	f = 2;
	InvalidateRect(0);
}

void CMyMainWnd::F3()
{
	int i = 0;
	double  dx = pi / 36;
	int n = 36 * 6 + 1;
	CMatrix XX(n), YY(n);
	XX(0) = 0;
	for (; XX(i) + dx <= 6 * pi; i++)
	{
		YY(i) = funct.MyF3(XX(i));
		XX(i + 1) = XX(i) + dx;
	}
	YY(i) = funct.MyF1(XX(i));
	CRect r(400, 100, 1000, 600);
	p3.SetParams(XX, YY, r);

	CMyPen p; p.Set(PS_DASHDOTDOT, 3, RGB(255, 0, 0));
	p3.SetPenLine(p);
	p.Set(PS_SOLID, 2, RGB(0, 0, 0));
	p3.SetPenAxis(p);
	f = 3;
	InvalidateRect(0);
}

void CMyMainWnd::F4()
{
	int i = 0;
	double  dx = 25;
	int n = 20 / 0.25 + 1;
	CMatrix XX(n), YY(n);
	XX(0) = -1000;
	for (; XX(i) + dx <= 1000; i++)
	{
		YY(i) = funct.MyF4(XX(i));
		XX(i + 1) = XX(i) + dx;
	}
	YY(i) = funct.MyF4(XX(i));
	CRect r(400, 100, 1000, 600);
	p4.SetParams(XX, YY, r);
	CMyPen p; p.Set(PS_SOLID, 2, RGB(255, 0, 0));
	p4.SetPenLine(p);
	p.Set(PS_SOLID, 2, RGB(0, 0, 255));
	p4.SetPenAxis(p);
	f = 4;
	InvalidateRect(0);
}

void CMyMainWnd::F1234()
{
	
	p1.SetWindowRect(CRect(30, 40, 500, 320));
	p2.SetWindowRect(CRect(700, 40, 1200, 320));
	p3.SetWindowRect(CRect(30, 360, 500, 660));
	p4.SetWindowRect(CRect(700, 360, 1200, 660));
	f = 5;
	InvalidateRect(0);
}
CMyApp theApp;