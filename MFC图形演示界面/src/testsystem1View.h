
// testsystem1View.h : Ctestsystem1View 类的接口
//

#pragma once

#include"testsystem1Doc.h"
class Ctestsystem1View : public CView
{
protected: // 仅从序列化创建
	Ctestsystem1View();
	DECLARE_DYNCREATE(Ctestsystem1View)

// 特性
public:
	Ctestsystem1Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//virtual  int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//virtual void OnSize(UINT nType, int cx, int cy);

// 实现
public:
	virtual ~Ctestsystem1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
	//CSplitterWnd  m_wndSplitter2;
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
//	virtual BOOL OnCreateAggregates();
};

#ifndef _DEBUG  // testsystem1View.cpp 中的调试版本
inline Ctestsystem1Doc* Ctestsystem1View::GetDocument() const
   { return reinterpret_cast<Ctestsystem1Doc*>(m_pDocument); }
#endif

