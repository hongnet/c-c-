
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "testsystem1.h"

#include "MainFrm.h"
#include"LOGINDLG.h"
#include "testsystem1View.h"
#include "leftdownv.h" 
#include"LEFTTOP.h"
#include"RIGHTTOPVIEW.h"
#include"rightdownv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO:  在此添加成员初始化代码
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CLOGINDLG LoginDlg;
	if (IDOK == LoginDlg.DoModal()){
		if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
			return -1;

		if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
			!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
		{
			TRACE0("未能创建工具栏\n");
			return -1;      // 未能创建
		}

		if (!m_wndStatusBar.Create(this))
		{
			TRACE0("未能创建状态栏\n");
			return -1;      // 未能创建
		}
		m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

		// TODO:  如果不需要可停靠工具栏，则删除这三行
		m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
		EnableDocking(CBRS_ALIGN_ANY);
		DockControlBar(&m_wndToolBar);
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.cx = 1500;
	cs.cy = 1300;
	cs.style &= ~FWS_ADDTOTITLE;
	//return CFrameWnd::PreCreateWindow(cs);
	return true;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO:  在此添加专用代码和/或调用基类
	CRect rect;
	GetClientRect(&rect); //产生第一次静态分割
	m_wndSplitter1.CreateStatic(this, //父窗口指针
		2, 2); //行数与列数
	/*m_wndSplitter1.CreateView(0, 0, //窗格的行列序数
		RUNTIME_CLASS(CRIGHTTOPVIEW),//视图类

		CSize(rect.Width(), rect.Height() - rect.Height() / 5), pContext);//父窗口创建参数
	m_wndSplitter1.CreateView(1, 0, RUNTIME_CLASS(CEditView),
		CSize(rect.Width(), rect.Height() / 5), pContext);*/
	//不在调用基类的 OncreateClient 函数
	int height = ::GetSystemMetrics(SM_CXSCREEN);
	int width = ::GetSystemMetrics(SM_CYSCREEN);
	/*if (!m_wndSplitter2.CreateStatic(&m_wndSplitter1, 1, 2, WS_CHILD | WS_VISIBLE, m_wndSplitter1.IdFromRowCol(0, 0)))
	{
		return false;
	}
	if (!m_wndSplitter3.CreateStatic(&m_wndSplitter1,1,2,WS_CHILD|WS_VISIBLE,m_wndSplitter1.IdFromRowCol(1,0)))
	{
		return false;
	}*/
	//m_wndSplitter1.SetRowInfo(0, height / 2, 1);
	//m_wndSplitter1.SetColumnInfo(0, width / 4, 1);
	//m_wndSplitter2.SetColumnInfo(0, width / 4, 1);
	//m_wndSplitter3.SetColumnInfo(0, width / 4, 1);
		if (!m_wndSplitter1.CreateView(0, 0, RUNTIME_CLASS(CLEFTTOP), CSize(rect.Width()/2,rect.Height()*2/4), pContext))//1行1列，
			return false;
		if (!m_wndSplitter1.CreateView(0, 1, RUNTIME_CLASS(CRIGHTTOPVIEW), CSize(rect.Width()/2, rect.Height()*2/4), pContext))//1行2列
			return false;
		if (!m_wndSplitter1.CreateView(1, 0, RUNTIME_CLASS(leftdownv), CSize(rect.Width()/2, rect.Height()/2), pContext))//2行1列
			return false;
		if (!m_wndSplitter1.CreateView(1, 1, RUNTIME_CLASS(rightdownv), CSize(rect.Width()/2, rect.Height()/4), pContext))//2row2column
			return false;
	RecalcLayout();
	CRIGHTTOPVIEW *pView = (CRIGHTTOPVIEW*)m_wndSplitter1.GetPane(0, 1);//CXXXView* pV = (CXXXView*)m_wndSplitter.GetPane(i,j);get view 指针
		pView->OnInitialUpdate();
	return true;
}
