// rightdownv.cpp : ʵ���ļ�
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


// rightdownv ���

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


// rightdownv ��Ϣ�������


void rightdownv::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	// TODO:  �ڴ����ר�ô����/����û���
	// TODO:  �ڴ����ר�ô����/����û���
	CEdit &mEdit = GetEditCtrl(); //��ȡ�༭��ͼ�Ŀؼ�
	mEdit.SetWindowText(_T("�ȴ��û�ѡ����ϵͳ������Ⱥ��������������Ҫ���е��㷨���Լ�Ҫʵ�ֵĹ��ܣ�"));//������ʾ��Ϣ
	mEdit.EnableWindow(FALSE); //�༭�ؼ����ɱ༭
}
int zqs1, xits, genes;

void rightdownv::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO:  �ڴ����ר�ô����/����û���
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
				sprintf_s(txtt, "GDE3�㷨�����С�\r\nѡ����GDE3�㷨,��ϵͳ��Ϊ%d,��Ⱥ��Ϊ%d,������Ϊ%d\r\n\r\n����T��ʾ�ɿ��ԣ�����C��ʾ���ĵĲ�����Դ", zqs1, xits, genes);
				CEdit &medit = GetEditCtrl();
				CString a(txtt);
				medit.SetWindowText(a);
				medit.EnableWindow(FALSE);
			}

		}

		else
		{

			char txtt[500];
			sprintf_s(txtt, "IR-NSGA-II�㷨�����С�\r\nѡ����IR-NSGA-II�㷨,��ϵͳ��Ϊ%d,��Ⱥ��Ϊ%d,������Ϊ%d\r\n����T��ʾ�ɿ��ԣ�����C��ʾ���ĵĲ�����Դ", zqs1, xits, genes);
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
			sprintf_s(txtt, "�㷨δ���л����н���\r\n��ϵͳ��Ϊ%d,��Ⱥ��Ϊ%d,������Ϊ%d��  \r\n�����˶ԱȰ�ť\r\nTeechart������GDE3��IR-NSGA-II�����㷨������\r\n����T��ʾ�ɿ��ԣ�����C��ʾ���ĵĲ�����Դ\r\nZ��R��ʾ������",zqs1,xits,genes);
			CEdit &medit = GetEditCtrl();
			CString a(txtt);
			medit.SetWindowText(a);
			medit.EnableWindow(FALSE);
		}
		else
			if (tdoc->butalog)
			{
				char txtt[500];
				sprintf_s(txtt, "�㷨δ���л����н���\r\nѡ����GDE3�㷨,��ϵͳ��Ϊ%d,��Ⱥ��Ϊ%d,������Ϊ%d��  \r\nTeechart������GDE3������\r\n����T��ʾ�ɿ��ԣ�����C��ʾ���ĵĲ�����Դ", zqs1, xits, genes);
				CEdit &medit = GetEditCtrl();
				CString a(txtt);
				medit.SetWindowText(a);
				medit.EnableWindow(FALSE);
			}
			else
			{
				char txtt[500];
				sprintf_s(txtt, "�㷨δ���л����н���\r\nѡ����IR-NSGA-II�㷨,��ϵͳ��Ϊ%d,��Ⱥ��Ϊ%d,������Ϊ%d��\r\nTeechart������IR-NSGA-II������\r\n����T��ʾ�ɿ��ԣ�����C��ʾ���ĵĲ�����Դ", zqs1, xits, genes);
				_bstr_t a(txtt);
				CEdit &medit = GetEditCtrl();
				medit.SetWindowText(a);
				medit.EnableWindow(FALSE);
			}
	}
	
}
