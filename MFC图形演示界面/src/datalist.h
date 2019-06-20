#pragma once
#include "afxcmn.h"


// datalist 对话框

class datalist : public CDialogEx
{
	DECLARE_DYNAMIC(datalist)

public:
	datalist(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~datalist();

// 对话框数据
	enum { IDD = IDD_DIALOG_compairison };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio11();
	afx_msg void OnBnClickedRadio12();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	void showdata();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedButton1();
};
