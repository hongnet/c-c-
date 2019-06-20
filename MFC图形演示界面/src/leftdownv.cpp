// leftdownv.cpp : 实现文件
//

#include "stdafx.h"
#include "testsystem1.h"
#include "leftdownv.h"
#include"Resource.h"
#include"testsystem1Doc.h"
#include"LEFTTOP.h"
//extern CBitmapButton m_sysmodel;
// leftdownv
BEGIN_MESSAGE_MAP(leftdownv, CEditView)
	ON_WM_PAINT()
END_MESSAGE_MAP()
IMPLEMENT_DYNCREATE(leftdownv, CEditView)

leftdownv::leftdownv()
{

}

leftdownv::~leftdownv()
{
}

//BEGIN_MESSAGE_MAP(leftdownv, CEditView)
//END_MESSAGE_MAP()


// leftdownv 诊断

#ifdef _DEBUG
void leftdownv::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void leftdownv::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// leftdownv 消息处理程序


void leftdownv::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	// TODO:  在此添加专用代码和/或调用基类
	//m_buttonstart.LoadBitmaps(idb_sysmodel, IDB_StartMovie2);
	//m_buttonfix.LoadBitmaps(IDB_StartMovie1, IDB_StartMovie2);
	//m_buttonexit.LoadBitmaps(IDB_StartMovie1, IDB_StartMovie2);
	HBITMAP   hBitmap;
	hBitmap = LoadBitmap(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_sysmodel5)); //
	HBITMAP   hBitmap1;
	hBitmap = LoadBitmap(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_sysmodel6));
	HBITMAP   hBitmap2;
	hBitmap = LoadBitmap(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_sysmodel7));
}


void leftdownv::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	Ctestsystem1Doc*tdoc = (Ctestsystem1Doc*)GetDocument();
	CEdit &medit = GetEditCtrl();
	if (tdoc->issymod == 1)
	{
		OnPaint();
	}
}
void leftdownv::OnPaint()
{
	// TODO:  在此添加专用代码和/或调用基类
	//UpdateData(true);
	Invalidate();//清空客户区
	UpdateWindow();//客户区为空才可重绘
	Ctestsystem1Doc*tdoc = (Ctestsystem1Doc*)GetDocument();
	CEdit &medit = GetEditCtrl();
	if (tdoc->issymod == 1)
	{
		if (tdoc->systemnumber == 0)
		{
			CPaintDC   dc(this);
			CRect   rect;
			GetClientRect(&rect);                                 //获取对话框长宽         
			CDC   dcBmp;                                           //定义并创建一个内存设备环境  
			dcBmp.CreateCompatibleDC(&dc);                         //创建兼容性DC  
			CBitmap   bmpBackground;

			//dc.StrtchBlt....
			bmpBackground.LoadBitmap(IDB_sysmodel5);                 //载入资源中的IDB_BITMAP1图片  
			BITMAP   m_bitmap;                                     //图片变量                  
			bmpBackground.GetBitmap(&m_bitmap);                    //将图片载入位图中  
			CBitmap   *pbmpOld = dcBmp.SelectObject(&bmpBackground); //将位图选入临时内存设备环境    
			//调用函数显示图片 StretchBlt显示形状可变  
			dc.SetStretchBltMode(COLORONCOLOR);// 若不设置这个模式，图片会严重失真
			dc.StretchBlt(0, 0, rect.Width(), rect.Height()*8/9, &dcBmp, 0, 0,
				m_bitmap.bmWidth, m_bitmap.bmHeight, SRCCOPY);
			//dcBmp.DeleteDC();
			//pbmpOld->DeleteObject();
		}
		else
			if (tdoc->systemnumber == 1)
			{
				CPaintDC   dc(this);
				CRect   rect;
				GetClientRect(&rect);                                 //获取对话框长宽         
				CDC   dcBmp;                                           //定义并创建一个内存设备环境  
				dcBmp.CreateCompatibleDC(&dc);                         //创建兼容性DC  
				CBitmap   bmpBackground;
				bmpBackground.LoadBitmap(IDB_sysmodel6);                 //载入资源中的IDB_BITMAP1图片  
				BITMAP   m_bitmap;                                     //图片变量                  
				bmpBackground.GetBitmap(&m_bitmap);                    //将图片载入位图中  
				CBitmap   *pbmpOld = dcBmp.SelectObject(&bmpBackground); //将位图选入临时内存设备环境    
				//调用函数显示图片 StretchBlt显示形状可变  
				dc.SetStretchBltMode(COLORONCOLOR);// 若不设置这个模式，图片会严重失真
				dc.StretchBlt(0, 0, rect.Width(), rect.Height()*8/9, &dcBmp, 0, 0,
					m_bitmap.bmWidth, m_bitmap.bmHeight, SRCCOPY);
				//CMainFrame* pmain = (CMainFrame*)AfxGetApp()->GetMainWnd();
				//CLEFTTOP* rdv = (CLEFTTOP*)(pmain->m_wndSplitter1.GetPane(0, 1));
				//rdv->m_sysmodel.LoadBitmaps(IDB_sysmodel5, IDB_sysmodel6);
				
			}
			else
			{
				CPaintDC   dc(this);
				CRect   rect;
				GetClientRect(&rect);                                 //获取对话框长宽         
				CDC   dcBmp;                                           //定义并创建一个内存设备环境  
				dcBmp.CreateCompatibleDC(&dc);                         //创建兼容性DC  
				CBitmap   bmpBackground;
				bmpBackground.LoadBitmap(IDB_sysmodel7);                 //载入资源中的IDB_BITMAP1图片  
				BITMAP   m_bitmap;                                     //图片变量                  
				bmpBackground.GetBitmap(&m_bitmap);                    //将图片载入位图中  
				CBitmap   *pbmpOld = dcBmp.SelectObject(&bmpBackground); //将位图选入临时内存设备环境    
				//调用函数显示图片 StretchBlt显示形状可变
				dc.SetStretchBltMode(COLORONCOLOR);// 若不设置这个模式，图片会严重失真
				dc.StretchBlt(0, 0, rect.Width(), rect.Height()*8/9, &dcBmp, 0, 0,
					m_bitmap.bmWidth, m_bitmap.bmHeight, SRCCOPY);
			}
		
	}
	CWnd::OnPaint();
}
