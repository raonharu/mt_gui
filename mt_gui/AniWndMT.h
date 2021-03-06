@@ -0,0 +1,29 @@
#pragma once


// CAniWndMT

class CAniWndMT : public CWnd
{
	DECLARE_DYNAMIC(CAniWndMT)

public:
	CAniWndMT();
	virtual ~CAniWndMT();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);

public:
	CEvent m_eventExit;
	CEvent m_eventDraw;
	HANDLE m_hDrawThread;
};

