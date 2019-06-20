// rightdownv.cpp : 实现文件
//

#include "stdafx.h"
#include "testsystem1.h"
#include "rightdownv.h"
#include"LEFTTOP.h"
#include"RIGHTTOPVIEW.h"
#include"MainFrm.h"
#include"testsystem1Doc.h"
// rightdownv

IMPLEMENT_DYNCREATE(rightdownv, CEditView)

rightdownv::rightdownv()
{

}

rightdownv::~rightdownv()
{
}

BEGIN_MESSAGE_MAP(rightdownv, CEditView)
END_MESSAGE_MAP()


// rightdownv 诊断

#ifdef _DEBUG
void rightdownv::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void rightdownv::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// rightdownv 消息处理程序


void rightdownv::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	// TODO:  在此添加专用代码和/或调用基类
	// TODO:  在此添加专用代码和/或调用基类
	CEdit &mEdit = GetEditCtrl(); //获取编辑视图的控件
	mEdit.SetWindowText(_T("等待用户选择子系统数，种群数，迭代数，和要运行的算法，以及要实现的功能！"));//设置显示信息
	mEdit.EnableWindow(FALSE); //编辑控件不可编辑
}
int zqs1, xits, genes;

void rightdownv::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO:  在此添加专用代码和/或调用基类
	CMainFrame *pmain = (CMainFrame*)AfxGetApp()->GetMainWnd();
	CLEFTTOP* lft = (CLEFTTOP*)(pmain->m_wndSplitter1.GetPane(0, 0));
	CRIGHTTOPVIEW* rtv = (CRIGHTTOPVIEW*)(pmain->m_wndSplitter1.GetPane(0, 1));
	Ctestsystem1Doc* tdoc = (Ctestsystem1Doc*)GetDocument();
	CHARFORMAT cf;
	CEdit &p=GetEditCtrl();
	CPaintDC dc(&p);
	dc.SetTextColor (RGB(0, 0, 0));
	
	
	if (tdoc->isOnBnClickedButalogrun)
	{
		if (tdoc->systemnumber == 0)
			zqs1 = 5;
		else
			if (tdoc->systemnumber == 1)
				zqs1 = 6;
			else
				zqs1 = 7;
		if (tdoc->popunum == 0)
			xits = 50;
		else
			if (tdoc->popunum == 1)
				xits = 100;
			else
				xits = 250;
		if (tdoc->generationnumber == 0)
			genes = 50;
		else
			if (tdoc->generationnumber == 1)
				genes = 100;
			else
				genes = 250;
		//int n;
		if (tdoc->butalog){
			char txtt[500];

			//if (tdoc->isduibirun)
			{
				sprintf_s(txtt, "GDE3算法运行中。\r\n选择了GDE3算法,子系统数为%d,种群数为%d,迭代数为%d\r\n\r\n横轴T表示可靠性，纵轴C表示消耗的测试资源", zqs1, xits, genes);
				CEdit &medit = GetEditCtrl();
				CString a(txtt);
				medit.SetWindowText(a);
				medit.EnableWindow(FALSE);
			}

		}

		else
		{

			char txtt[500];
			sprintf_s(txtt, "IR-NSGA-II算法运行中。\r\n选择了IR-NSGA-II算法,子系统数为%d,种群数为%d,迭代数为%d\r\n横轴T表示可靠性，纵轴C表示消耗的测试资源", zqs1, xits, genes);
			_bstr_t a(txtt);
			CEdit &medit = GetEditCtrl();
			medit.SetWindowText(a);
			medit.EnableWindow(FALSE);


		}
	}
	else
	{
		//char txtt[500];
		//sprintf(txtt,)
		if (tdoc->isduibirun)
		{
			char txtt[500];
			sprintf_s(txtt, "算法未运行或运行结束\r\n子系统数为%d,种群数为%d,迭代数为%d。  \r\n运行了对比按钮\r\nTeechart将绘制GDE3，IR-NSGA-II两种算法的数据\r\n横轴T表示可靠性，纵轴C表示消耗的测试资源\r\nZ轴R表示数据量",zqs1,xits,genes);
			CEdit &medit = GetEditCtrl();
			CString a(txtt);
			medit.SetWindowText(a);
			medit.EnableWindow(FALSE);
		}
		else
			if (tdoc->butalog)
			{
				char txtt[500];
				sprintf_s(txtt, "算法未运行或运行结束\r\n选择了GDE3算法,子系统数为%d,种群数为%d,迭代数为%d。  \r\nTeechart将绘制GDE3的数据\r\n横轴T表示可靠性，纵轴C表示消耗的测试资源", zqs1, xits, genes);
				CEdit &medit = GetEditCtrl();
				CString a(txtt);
				medit.SetWindowText(a);
				medit.EnableWindow(FALSE);
			}
			else
			{
				char txtt[500];
				sprintf_s(txtt, "算法未运行或运行结束\r\n选择了IR-NSGA-II算法,子系统数为%d,种群数为%d,迭代数为%d。\r\nTeechart将绘制IR-NSGA-II的数据\r\n横轴T表示可靠性，纵轴C表示消耗的测试资源", zqs1, xits, genes);
				_bstr_t a(txtt);
				CEdit &medit = GetEditCtrl();
				medit.SetWindowText(a);
				medit.EnableWindow(FALSE);
			}
	}
	
}
