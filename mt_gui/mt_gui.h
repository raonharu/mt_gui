
// mt_gui.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// Cmt_guiApp:
// �� Ŭ������ ������ ���ؼ��� mt_gui.cpp�� �����Ͻʽÿ�.
//

class Cmt_guiApp : public CWinApp
{
public:
	Cmt_guiApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern Cmt_guiApp theApp;