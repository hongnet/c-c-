
// testsystem1View.h : Ctestsystem1View ��Ľӿ�
//

#pragma once

#include"testsystem1Doc.h"
class Ctestsystem1View : public CView
{
protected: // �������л�����
	Ctestsystem1View();
	DECLARE_DYNCREATE(Ctestsystem1View)

// ����
public:
	Ctestsystem1Doc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//virtual  int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//virtual void OnSize(UINT nType, int cx, int cy);

// ʵ��
public:
	virtual ~Ctestsystem1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
	//CSplitterWnd  m_wndSplitter2;
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
//	virtual BOOL OnCreateAggregates();
};

#ifndef _DEBUG  // testsystem1View.cpp �еĵ��԰汾
inline Ctestsystem1Doc* Ctestsystem1View::GetDocument() const
   { return reinterpret_cast<Ctestsystem1Doc*>(m_pDocument); }
#endif

