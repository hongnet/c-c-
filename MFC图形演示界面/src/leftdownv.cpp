// leftdownv.cpp : ʵ���ļ�
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


// leftdownv ���

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


// leftdownv ��Ϣ�������


void leftdownv::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	// TODO:  �ڴ����ר�ô����/����û���
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
	// TODO:  �ڴ����ר�ô����/����û���
	//UpdateData(true);
	Invalidate();//��տͻ���
	UpdateWindow();//�ͻ���Ϊ�ղſ��ػ�
	Ctestsystem1Doc*tdoc = (Ctestsystem1Doc*)GetDocument();
	CEdit &medit = GetEditCtrl();
	if (tdoc->issymod == 1)
	{
		if (tdoc->systemnumber == 0)
		{
			CPaintDC   dc(this);
			CRect   rect;
			GetClientRect(&rect);                                 //��ȡ�Ի��򳤿�         
			CDC   dcBmp;                                           //���岢����һ���ڴ��豸����  
			dcBmp.CreateCompatibleDC(&dc);                         //����������DC  
			CBitmap   bmpBackground;

			//dc.StrtchBlt....
			bmpBackground.LoadBitmap(IDB_sysmodel5);                 //������Դ�е�IDB_BITMAP1ͼƬ  
			BITMAP   m_bitmap;                                     //ͼƬ����                  
			bmpBackground.GetBitmap(&m_bitmap);                    //��ͼƬ����λͼ��  
			CBitmap   *pbmpOld = dcBmp.SelectObject(&bmpBackground); //��λͼѡ����ʱ�ڴ��豸����    
			//���ú�����ʾͼƬ StretchBlt��ʾ��״�ɱ�  
			dc.SetStretchBltMode(COLORONCOLOR);// �����������ģʽ��ͼƬ������ʧ��
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
				GetClientRect(&rect);                                 //��ȡ�Ի��򳤿�         
				CDC   dcBmp;                                           //���岢����һ���ڴ��豸����  
				dcBmp.CreateCompatibleDC(&dc);                         //����������DC  
				CBitmap   bmpBackground;
				bmpBackground.LoadBitmap(IDB_sysmodel6);                 //������Դ�е�IDB_BITMAP1ͼƬ  
				BITMAP   m_bitmap;                                     //ͼƬ����                  
				bmpBackground.GetBitmap(&m_bitmap);                    //��ͼƬ����λͼ��  
				CBitmap   *pbmpOld = dcBmp.SelectObject(&bmpBackground); //��λͼѡ����ʱ�ڴ��豸����    
				//���ú�����ʾͼƬ StretchBlt��ʾ��״�ɱ�  
				dc.SetStretchBltMode(COLORONCOLOR);// �����������ģʽ��ͼƬ������ʧ��
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
				GetClientRect(&rect);                                 //��ȡ�Ի��򳤿�         
				CDC   dcBmp;                                           //���岢����һ���ڴ��豸����  
				dcBmp.CreateCompatibleDC(&dc);                         //����������DC  
				CBitmap   bmpBackground;
				bmpBackground.LoadBitmap(IDB_sysmodel7);                 //������Դ�е�IDB_BITMAP1ͼƬ  
				BITMAP   m_bitmap;                                     //ͼƬ����                  
				bmpBackground.GetBitmap(&m_bitmap);                    //��ͼƬ����λͼ��  
				CBitmap   *pbmpOld = dcBmp.SelectObject(&bmpBackground); //��λͼѡ����ʱ�ڴ��豸����    
				//���ú�����ʾͼƬ StretchBlt��ʾ��״�ɱ�
				dc.SetStretchBltMode(COLORONCOLOR);// �����������ģʽ��ͼƬ������ʧ��
				dc.StretchBlt(0, 0, rect.Width(), rect.Height()*8/9, &dcBmp, 0, 0,
					m_bitmap.bmWidth, m_bitmap.bmHeight, SRCCOPY);
			}
		
	}
	CWnd::OnPaint();
}
