#pragma once
#include "afxwin.h"


// CLOGINDLG �Ի���

class CLOGINDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CLOGINDLG)

public:
	CLOGINDLG(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLOGINDLG();

// �Ի�������
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edituser;
	CEdit m_editpassword;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonRegister();
};
