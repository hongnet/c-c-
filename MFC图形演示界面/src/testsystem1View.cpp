
// testsystem1View.cpp : Ctestsystem1View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "testsystem1.h"
#endif

#include "testsystem1Doc.h"
#include "testsystem1View.h"
#include"LEFTTOP.h"
#include"RIGHTTOPVIEW.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ctestsystem1View

IMPLEMENT_DYNCREATE(Ctestsystem1View, CView)

BEGIN_MESSAGE_MAP(Ctestsystem1View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// Ctestsystem1View 构造/析构

Ctestsystem1View::Ctestsystem1View()
{
	// TODO:  在此处添加构造代码

}

Ctestsystem1View::~Ctestsystem1View()
{
}

BOOL Ctestsystem1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Ctestsystem1View 绘制

void Ctestsystem1View::OnDraw(CDC* /*pDC*/)
{
	Ctestsystem1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  在此处为本机数据添加绘制代码
}


// Ctestsystem1View 打印

BOOL Ctestsystem1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Ctestsystem1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加额外的打印前进行的初始化过程
}

void Ctestsystem1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加打印后进行的清理过程
}


// Ctestsystem1View 诊断

#ifdef _DEBUG
void Ctestsystem1View::AssertValid() const
{
	CView::AssertValid();
}

void Ctestsystem1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Ctestsystem1Doc* Ctestsystem1View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ctestsystem1Doc)));
	return (Ctestsystem1Doc*)m_pDocument;
}
#endif //_DEBUG


// Ctestsystem1View 消息处理程序



/*int Ctestsystem1View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	CRect rect;
	GetClientRect(&rect); // 获得窗口的创建信息指针
	CCreateContext *pContext = (CCreateContext *)lpCreateStruct->lpCreateParams;
	m_wndSplitter2.CreateStatic(this, 1, 2); //产生第二次的静态分割
	//为第一个窗格产生视图
	m_wndSplitter2.CreateView(0, 0,//窗口的行列序数
		RUNTIME_CLASS(CLEFTTOP),//视图类
		CSize(rect.Width() / 4, rect.Height()),//
		pContext);
	//为第二个窗格产生视图
	m_wndSplitter2.CreateView(0, 1, RUNTIME_CLASS(CRIGHTTOPVIEW), CSize(1, 1), pContext);
	return 0;
}
void Ctestsystem1View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);
	int x = rect.Width();
	int y = rect.Height();
	m_wndSplitter2.MoveWindow(-2, -2, x, y + 3);
	m_wndSplitter2.SetColumnInfo(0, x / 4, 0); //左边窗格位置
	m_wndSplitter2.SetColumnInfo(1, x - x / 4, 0); //右边窗格位置
	m_wndSplitter2.RecalcLayout();

}*/
