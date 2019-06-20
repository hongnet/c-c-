#pragma once
#include "afxwin.h"
#include"rightdownv.h"
#include"MainFrm.h"
//#include"testsystem1Doc.h"
//extern BOOL isOnBnClickedButalogrun;
//extern int butalog, systemnumber, generationnumber, popunum;

// CLEFTTOP 窗体视图

class CLEFTTOP : public CFormView
{
	DECLARE_DYNCREATE(CLEFTTOP)

protected:
	CLEFTTOP();           // 动态创建所使用的受保护的构造函数
	virtual ~CLEFTTOP();

public:
	enum { IDD = IDD_DIALOG_SETVAR };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	int sysnum;
	CEdit m_edit1;
	CEdit m_edit2;
	CEdit m_edit3;
	CEdit m_edit4;
	CEdit m_edit5;
	CEdit m_edit6;
	CEdit m_edit7;
	CEdit timelimit;
	CEdit time0;
	CComboBox m_alogcombox;
//	int pop;
//	int gen;
	afx_msg void OnBnClickedButalog();
//	afx_msg void OnCbnSelchangeCombo2();
//	afx_msg void OnCbnSelchangeCombo1();
//	int pop;
//	int gen;
//	int sysnum;
	int sysnum;
	int pop;
	int gen1;
	CComboBox *pCoboBox;

		
		virtual void OnInitialUpdate();
//		afx_msg void OnCbnSelchangeCombo1();
		afx_msg void OnCbnSelchangeCombo1();
		CButton m_duibi;
		afx_msg void OnBnClickedButton3();
//		afx_msg void OnBnDoubleclickedButton3();
		afx_msg void OnCbnSelchangeCombo2();
		CBitmapButton m_sysmodel;
		afx_msg void OnBnClickedButton1();
		afx_msg void OnBnClickedButton4();
		void  zq100sc0();
		void zq250sc0();
		void zq50sc1();
		void zq100sc1();
		void zq250sc1();
		void zq50sc2();
		void zq100sc2();
		void zq250sc2();
		void gn50sc0();
		void gn100sc0();
		void gn250sc0();
		void gn50sc1();
		void gn100sc1();
		void gn250sc1();
		void gn50sc2();
		void gn100sc2();
		void gn250sc2();
		void datarun();
};


