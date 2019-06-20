
// MainFrm.cpp : CMainFrame ���ʵ��
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
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO:  �ڴ���ӳ�Ա��ʼ������
	
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
			TRACE0("δ�ܴ���������\n");
			return -1;      // δ�ܴ���
		}

		if (!m_wndStatusBar.Create(this))
		{
			TRACE0("δ�ܴ���״̬��\n");
			return -1;      // δ�ܴ���
		}
		m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

		// TODO:  �������Ҫ��ͣ������������ɾ��������
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
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	cs.cx = 1500;
	cs.cy = 1300;
	cs.style &= ~FWS_ADDTOTITLE;
	//return CFrameWnd::PreCreateWindow(cs);
	return true;
}

// CMainFrame ���

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


// CMainFrame ��Ϣ�������



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO:  �ڴ����ר�ô����/����û���
	CRect rect;
	GetClientRect(&rect); //������һ�ξ�̬�ָ�
	m_wndSplitter1.CreateStatic(this, //������ָ��
		2, 2); //����������
	/*m_wndSplitter1.CreateView(0, 0, //�������������
		RUNTIME_CLASS(CRIGHTTOPVIEW),//��ͼ��

		CSize(rect.Width(), rect.Height() - rect.Height() / 5), pContext);//�����ڴ�������
	m_wndSplitter1.CreateView(1, 0, RUNTIME_CLASS(CEditView),
		CSize(rect.Width(), rect.Height() / 5), pContext);*/
	//���ڵ��û���� OncreateClient ����
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
		if (!m_wndSplitter1.CreateView(0, 0, RUNTIME_CLASS(CLEFTTOP), CSize(rect.Width()/2,rect.Height()*2/4), pContext))//1��1�У�
			return false;
		if (!m_wndSplitter1.CreateView(0, 1, RUNTIME_CLASS(CRIGHTTOPVIEW), CSize(rect.Width()/2, rect.Height()*2/4), pContext))//1��2��
			return false;
		if (!m_wndSplitter1.CreateView(1, 0, RUNTIME_CLASS(leftdownv), CSize(rect.Width()/2, rect.Height()/2), pContext))//2��1��
			return false;
		if (!m_wndSplitter1.CreateView(1, 1, RUNTIME_CLASS(rightdownv), CSize(rect.Width()/2, rect.Height()/4), pContext))//2row2column
			return false;
	RecalcLayout();
	CRIGHTTOPVIEW *pView = (CRIGHTTOPVIEW*)m_wndSplitter1.GetPane(0, 1);//CXXXView* pV = (CXXXView*)m_wndSplitter.GetPane(i,j);get view ָ��
		pView->OnInitialUpdate();
	return true;
}
