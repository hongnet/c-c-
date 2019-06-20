#pragma once


// testdlg 对话框

class testdlg : public CDialogEx
{
	DECLARE_DYNAMIC(testdlg)

public:
	testdlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~testdlg();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
