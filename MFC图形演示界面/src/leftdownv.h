#pragma once


// leftdownv 视图

class leftdownv : public CEditView
{
	DECLARE_DYNCREATE(leftdownv)

protected:
	leftdownv();           // 动态创建所使用的受保护的构造函数
	virtual ~leftdownv();
protected:
	//DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};


