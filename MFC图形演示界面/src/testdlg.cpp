// testdlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testsystem1.h"
#include "testdlg.h"
#include "afxdialogex.h"


// testdlg �Ի���

IMPLEMENT_DYNAMIC(testdlg, CDialogEx)

testdlg::testdlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(testdlg::IDD, pParent)
{

}

testdlg::~testdlg()
{
}

void testdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(testdlg, CDialogEx)
END_MESSAGE_MAP()


// testdlg ��Ϣ�������
