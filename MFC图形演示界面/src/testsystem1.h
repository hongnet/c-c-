
// testsystem1.h : testsystem1 Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// Ctestsystem1App:
// �йش����ʵ�֣������ testsystem1.cpp
//

class Ctestsystem1App : public CWinApp
{
public:
	Ctestsystem1App();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	MYSQL m_mysql;
	
};

extern Ctestsystem1App theApp;
