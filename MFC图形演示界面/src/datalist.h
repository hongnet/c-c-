#pragma once
#include "afxcmn.h"


// datalist �Ի���

class datalist : public CDialogEx
{
	DECLARE_DYNAMIC(datalist)

public:
	datalist(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~datalist();

// �Ի�������
	enum { IDD = IDD_DIALOG_compairison };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
