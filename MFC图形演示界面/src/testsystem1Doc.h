
// testsystem1Doc.h : Ctestsystem1Doc ��Ľӿ�
//


#pragma once


class Ctestsystem1Doc : public CDocument
{
protected: // �������л�����
	Ctestsystem1Doc();
	DECLARE_DYNCREATE(Ctestsystem1Doc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	_ConnectionPtr m_pConnection;
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~Ctestsystem1Doc();
	BOOL  isOnBnClickedButalogrun = 0;
	int butalog, systemnumber, generationnumber, popunum,isduibirun=0;
	bool issymod=0;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
