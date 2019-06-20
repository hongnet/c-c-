#pragma once
#include "afxwin.h"


// CLOGINDLG 对话框

class CLOGINDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CLOGINDLG)

public:
	CLOGINDLG(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLOGINDLG();

// 对话框数据
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edituser;
	CEdit m_editpassword;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonRegister();
};
