@@ -0,0 +1,243 @@
// AniWndMT.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "gui_mt.h"
#include "AniWndMT.h"

#include <Gdiplus.h>
// CAniWndMT

IMPLEMENT_DYNAMIC(CAniWndMT, CWnd)

CAniWndMT::CAniWndMT()
{
	m_hDrawThread = NULL;
}

CAniWndMT::~CAniWndMT()
{
}


BEGIN_MESSAGE_MAP(CAniWndMT, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()

	ON_WM_PAINT()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()



// CAniWndMT �޽��� ó�����Դϴ�.
class CDrawMT
{
public:
	CDrawMT()
	{
		srand(GetTickCount());
		m_pWnd = NULL;

		dwCountFrame = 0;
		dwCount1 = 0;
		dwCount2 = 0;
		dwTime1 = 1;
		dwTime2 = 2;

		m_pbm = new Gdiplus::Bitmap(1,1);
		m_gBuffer = new Gdiplus::Graphics(m_pbm);
	}
	~CDrawMT()
	{
		if(m_pbm != NULL)
			delete m_pbm;

	}

public:
	void SetWindow(CWnd* pWnd)
	{
		m_pWnd = pWnd;
	}

	void Draw()
	{
		ASSERT(m_pWnd != NULL);
		ASSERT(m_pbm != NULL);
		ASSERT(m_gBuffer != NULL);

		IncreseFrameCount();
		CDC* pDC = m_pWnd->GetDC();
		Gdiplus::Graphics g(pDC->GetSafeHdc());
		CRect rcWnd;

		m_pWnd->GetClientRect(rcWnd);

		if(rcWnd.Width() != m_pbm->GetWidth() || rcWnd.Height() != m_pbm->GetWidth())
		{
			delete m_gBuffer;
			delete m_pbm;

			m_pbm = new Gdiplus::Bitmap(rcWnd.Width(),rcWnd.Height());
			m_gBuffer = new Gdiplus::Graphics(m_pbm);
		}

		OnDraw(*m_gBuffer,rcWnd);
		//////////////////////////////////////////////////////////////////////////
		//

		Gdiplus::CachedBitmap cbm(m_pbm,&g);
		g.DrawCachedBitmap(&cbm,0,0);
		m_pWnd->ReleaseDC(pDC);
		m_pWnd->ValidateRect(rcWnd);
	}


protected:

	virtual void OnDraw(Gdiplus::Graphics &g, CRect rcWnd)
	{
		g.Clear(Gdiplus::Color(rand()%256,rand()%256,rand()%256));

		Gdiplus::Font fnt(_T("Arial"),15.0f,Gdiplus::FontStyleRegular, Gdiplus::UnitPoint);
		Gdiplus::StringFormat sf();
		CString str;
		str.Format(_T("%d, %.1f"),dwCountFrame, GetFPS());
		g.DrawString(str,str.GetLength(),&fnt, Gdiplus::PointF(0.f,20.f),&Gdiplus::SolidBrush(Gdiplus::Color::White));

	}

	void IncreseFrameCount()
	{
		dwCountFrame++;

		fFPS = (dwCount2-dwCount1) * 1000.f / (dwTime2-dwTime1) ;

		if(GetTickCount() - dwTime2 > 1000)
		{
			dwTime1 = dwTime2;
			dwTime2 = GetTickCount();
			dwCount1 = dwCount2;
			dwCount2 = dwCountFrame;
		}
	}

	float GetFPS() {return fFPS;}


private:
	CWnd* m_pWnd;

	float fFPS;
	DWORD dwCountFrame ;
	DWORD dwCount1 ;
	DWORD dwCount2 ;
	DWORD dwTime1 ;
	DWORD dwTime2 ;

	Gdiplus::Bitmap *m_pbm;
	Gdiplus::Graphics *m_gBuffer;
};

DWORD __stdcall DrawThread(void* pParam)
{
	CAniWndMT* pWnd = (CAniWndMT*)pParam;

	CDrawMT drawer;

	drawer.SetWindow(pWnd);

	while(WaitForSingleObject(pWnd->m_eventExit,10) != WAIT_OBJECT_0)
	{
		//pWnd->GetUpdateRgn()
		drawer.Draw();
	}


	return 0;
	
}

int CAniWndMT::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwThreadID = NULL;
	m_hDrawThread = CreateThread(NULL,NULL,DrawThread,this,NULL,&dwThreadID);


	return 0;
}


void CAniWndMT::OnDestroy()
{
	CWnd::OnDestroy();


	
	m_eventExit.SetEvent();
	WaitForSingleObject(m_hDrawThread,INFINITE);

}




void CAniWndMT::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	//Gdiplus::Graphics g(dc.GetSafeHdc());

	//g.Clear(Gdiplus::Color(rand()%256,rand()%256,rand()%256));
	
}


LRESULT CAniWndMT::OnNcHitTest(CPoint point)
{
	CRect rc;
	GetWindowRect(rc);

	const int nRange = 5;

	CPoint pointClient = point;
	pointClient.x -= rc.left;
	pointClient.y -= rc.top;

	if(pointClient.x < nRange )
	{
		if( pointClient.y < nRange )
			return HTTOPLEFT;
		else if ( pointClient.y > rc.Height() - nRange)
			return HTBOTTOMLEFT;
		else
			return HTLEFT;
	}
	else if (pointClient.x > rc.Width() - nRange)
	{
		if( pointClient.y < nRange )
			return HTTOPRIGHT;
		else if ( pointClient.y > rc.Height() - nRange)
			return HTBOTTOMRIGHT;
		else
			return HTRIGHT;
	}
	else
	{
		if( pointClient.y < nRange )
			return HTTOP;
		else if ( pointClient.y > rc.Height() - nRange)
			return HTBOTTOM;
		else
			return HTCAPTION;
	}
	
	return __super::OnNcHitTest(pointClient);
	TRACE("%d,%d\n",pointClient.x,pointClient.y);

	return HTCAPTION;
}