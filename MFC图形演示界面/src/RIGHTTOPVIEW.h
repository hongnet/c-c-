#pragma once

//#include"tchart.h"
#include"..\TeeFiles\tchart.h"

// CRIGHTTOPVIEW ������ͼ

class CRIGHTTOPVIEW : public CFormView
{   
	DECLARE_DYNCREATE(CRIGHTTOPVIEW)

protected:
	CRIGHTTOPVIEW();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CRIGHTTOPVIEW();

public:
	enum { IDD = IDD_DIALOG2 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int sanwei;
	int erwei;
	virtual void OnInitialUpdate();
//	CTchart1 m_teechart;
	afx_msg void OnBnClickedRadio1();
//	CTchart1 m_teechart;
//	CTChart m_teechart;
//	CTChart m_teechart;
//	CTChart m_teechart;
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CTChart m_teechart;
	afx_msg void OnBnClickedRadio2();
};


