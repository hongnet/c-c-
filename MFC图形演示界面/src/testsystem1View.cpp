
// testsystem1View.cpp : Ctestsystem1View ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// Ctestsystem1View ����/����

Ctestsystem1View::Ctestsystem1View()
{
	// TODO:  �ڴ˴���ӹ������

}

Ctestsystem1View::~Ctestsystem1View()
{
}

BOOL Ctestsystem1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// Ctestsystem1View ����

void Ctestsystem1View::OnDraw(CDC* /*pDC*/)
{
	Ctestsystem1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
}


// Ctestsystem1View ��ӡ

BOOL Ctestsystem1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void Ctestsystem1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void Ctestsystem1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}


// Ctestsystem1View ���

#ifdef _DEBUG
void Ctestsystem1View::AssertValid() const
{
	CView::AssertValid();
}

void Ctestsystem1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Ctestsystem1Doc* Ctestsystem1View::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ctestsystem1Doc)));
	return (Ctestsystem1Doc*)m_pDocument;
}
#endif //_DEBUG


// Ctestsystem1View ��Ϣ�������



/*int Ctestsystem1View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	CRect rect;
	GetClientRect(&rect); // ��ô��ڵĴ�����Ϣָ��
	CCreateContext *pContext = (CCreateContext *)lpCreateStruct->lpCreateParams;
	m_wndSplitter2.CreateStatic(this, 1, 2); //�����ڶ��εľ�̬�ָ�
	//Ϊ��һ�����������ͼ
	m_wndSplitter2.CreateView(0, 0,//���ڵ���������
		RUNTIME_CLASS(CLEFTTOP),//��ͼ��
		CSize(rect.Width() / 4, rect.Height()),//
		pContext);
	//Ϊ�ڶ������������ͼ
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
	m_wndSplitter2.SetColumnInfo(0, x / 4, 0); //��ߴ���λ��
	m_wndSplitter2.SetColumnInfo(1, x - x / 4, 0); //�ұߴ���λ��
	m_wndSplitter2.RecalcLayout();

}*/
