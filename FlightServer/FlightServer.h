
// FlightServer.h : FlightServer Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CFlightServerApp:
// �йش����ʵ�֣������ FlightServer.cpp
//

class CFlightServerApp : public CWinApp
{
public:
	CFlightServerApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFlightServerApp theApp;
