#pragma once


// rightdownv ��ͼ

class rightdownv : public CEditView
{
	DECLARE_DYNCREATE(rightdownv)

protected:
	rightdownv();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~rightdownv();

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


