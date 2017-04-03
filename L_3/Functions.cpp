#include "Functions.h"
#include <math.h>

CRectD::CRectD(double l, double t, double r, double b)
{
	left = l;
	right = r;
	bottom = b;
	top = t;
}
void CRectD::setRectD(double l, double t, double r, double b)
{
	left = l;
	right = r;
	bottom = b;
	top = t;
}
CSizeD CRectD::SizeD()
{
	CSizeD cz;
	cz.cx = fabs(right - left);
	cz.cy = fabs(top - bottom);

	return cz;
}

CMatrix SpaceToWindow(CRectD& rs, CRect& rw)
{
	CMatrix M(3, 3);
	CSize sz = rw.Size();
	int dwx = sz.cx;
	int dwy = sz.cy;
	CSizeD szd = rs.SizeD();

	double dsx = szd.cx;
	double dsy = szd.cy;
	double kx = (double)dwx / dsx;
	double ky = (double)dwy / dsy;

	M(0, 0) = kx;
	M(0, 1) = 0;
	M(0, 2) = (double)rw.left - kx*rs.left;
	M(1, 0) = 0;
	M(1, 1) = -ky;
	M(1, 2) = (double)rw.bottom + ky*rs.bottom;
	M(2, 0) = 0;
	M(2, 1) = 0;
	M(2, 2) = 1;

	return M;
}

void SetMyMode(CDC& dc, CRectD& RS, CRect& RW)
{
	int dsx = RS.right - RS.left;
	int dsy = RS.top - RS.bottom;
	int xsL = RS.left;
	int ysL = RS.bottom;

	int dwx = RW.right - RW.left;
	int dwy = RW.bottom-RW.top;
	int xwL = RW.left;
	int ywH = RW.bottom;

	dc.SetMapMode(MM_ANISOTROPIC);
	dc.SetWindowExt(dsx, dsy);
	dc.SetViewportExt(dwx, -dwy);
	dc.SetWindowOrg(xsL, ysL);
	dc.SetViewportOrg(xwL, ywH);
}

void CPlot2D::SetParams(CMatrix& XX, CMatrix& YY, CRect& RWX)
{
	int nRowsX = XX.rows();
	int nRowsY = YY.rows();
	if (nRowsX != nRowsY)
	{
		TCHAR* error = _T("SetParams: неправильная размерность матриц");
		MessageBox(NULL, error, _T("error"),NULL);
		exit(1);
	}
	X.RedimMatrix(nRowsX);
	Y.RedimMatrix(nRowsY);
	X = XX;
	Y = YY;
	double x_max = X.MaxElement();
	double x_min = X.MinElement();
	double y_max = Y.MaxElement();
	double y_min = Y.MinElement();
	RS.setRectD(x_min, y_max, x_max, y_min);
	RW.SetRect(RWX.left, RWX.top, RWX.right, RWX.bottom);
	K = SpaceToWindow(RS, RW);
}

void CPlot2D::SetWindowRect(CRect & RWX)
{
	RW.SetRect(RWX.left, RWX.top, RWX.right, RWX.bottom);
	K = SpaceToWindow(RS, RW);
}

void CPlot2D::SetPenLine(CMyPen& PLine)
{
	PenLine.PenStyle = PLine.PenStyle;
	PenLine.PenWidth = PLine.PenWidth;
	PenLine.PenColor = PLine.PenColor;
}

void CPlot2D::SetPenAxis(CMyPen& PLine)
{
	PenAxis.PenStyle = PLine.PenStyle;
	PenAxis.PenWidth = PLine.PenWidth;
	PenAxis.PenColor = PLine.PenColor;
}

void CPlot2D::GetRS(CRectD& RS)
{
	RS.left = (this->RS).left;
	RS.top = (this->RS).top;
	RS.right = (this->RS).right;
	RS.bottom = (this->RS).bottom;
}

void CPlot2D::GetWindowCoords(double xs, double ys, int &xw, int &yw)
{
	CMatrix V(3), W(3);
	V(0) = xs;
	V(1) = ys;
	V(2) = 1;
	W = K*V;
	xw = (int)W(0);
	yw = (int)W(1);
}

void CPlot2D::Draw(CDC& dc, int Ind1, int Int2)
{
	double xs, ys;
	int xw, yw;
	if (Ind1 == 1) dc.Rectangle(RW);
	if (Int2 == 1)
	{
		CPen MyPen(PenAxis.PenStyle, PenAxis.PenWidth, PenAxis.PenColor);
		CPen* pOldPen = dc.SelectObject(&MyPen);
		if (RS.left*RS.right <= 0)
		{
			xs = 0; 
			ys = RS.top;
			GetWindowCoords(xs, ys, xw, yw);
			dc.MoveTo(xw, yw);
			xs = 0;
			ys = RS.bottom;
			GetWindowCoords(xs, ys, xw, yw);
			dc.LineTo(xw, yw);
		}
		if (RS.top*RS.bottom <= 0)
		{
			xs = RS.left; 
			ys = 0;
			GetWindowCoords(xs, ys, xw, yw);
			dc.MoveTo(xw, yw);
			xs = RS.right;
			ys = 0;
			GetWindowCoords(xs, ys, xw, yw);
			dc.LineTo(xw, yw);
		}
		dc.SelectObject(pOldPen);
		xs = X(0);
		ys = Y(0);
		GetWindowCoords(xs, ys, xw, yw);
		dc.MoveTo(xw, yw);
		CPen MyPen1(PenLine.PenStyle, PenLine.PenWidth, PenLine.PenColor);
		CPen* pOldPen1 = dc.SelectObject(&MyPen1);
		for (int i = 1; i < X.rows(); i++)
		{
			xs = X(i);
			ys = Y(i);
			GetWindowCoords(xs, ys, xw, yw);
			dc.LineTo(xw, yw);
		}
		dc.SelectObject(pOldPen1);
	}
}

void CPlot2D::Draw1(CDC& dc, int Ind1, int Int2)
{
	if (Ind1 == 1) dc.Rectangle(RW);
	SetMyMode(dc, RS, RW);
	if (Int2 == 1)
	{
		CPen MyPen(PenAxis.PenStyle, PenAxis.PenWidth, PenAxis.PenColor);
		CPen* pOldPen = dc.SelectObject(&MyPen);
		if (RS.left*RS.right <= 0)
		{
			dc.MoveTo(0, RS.top);
			dc.LineTo(0, RS.bottom);
		}
		if (RS.top*RS.bottom <= 0)
		{
			dc.MoveTo(RS.left, 0);
			dc.LineTo(RS.right, 0);
		}
		dc.SelectObject(pOldPen);
		CPen MyPen1(PenLine.PenStyle, PenLine.PenWidth, PenLine.PenColor);
		CPen* pOldPen1 = dc.SelectObject(&MyPen1);
		dc.MoveTo((int)X(0), (int)Y(0));
		for (int i = 1; i < X.rows(); i++)
		{
			dc.LineTo((int)X(i), (int)Y(i));
		}
		dc.SelectObject(pOldPen1);
	}
}