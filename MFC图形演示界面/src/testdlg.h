#pragma once


// testdlg �Ի���

class testdlg : public CDialogEx
{
	DECLARE_DYNAMIC(testdlg)

public:
	testdlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~testdlg();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
