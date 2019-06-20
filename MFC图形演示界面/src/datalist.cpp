// datalist.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testsystem1.h"
#include "datalist.h"
#include "afxdialogex.h"
#include"LEFTTOP.h"
int pn=0, gn=0;
// datalist �Ի���
extern MYSQL m_mysql;
extern CLEFTTOP *lft;
IMPLEMENT_DYNAMIC(datalist, CDialogEx)

datalist::datalist(CWnd* pParent /*=NULL*/)
	: CDialogEx(datalist::IDD, pParent)
{

}

datalist::~datalist()
{
}

void datalist::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_show, m_list);
}


BEGIN_MESSAGE_MAP(datalist, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO11, &datalist::OnBnClickedRadio11)
	ON_BN_CLICKED(IDC_RADIO12, &datalist::OnBnClickedRadio12)
	ON_BN_CLICKED(IDC_RADIO7, &datalist::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, &datalist::OnBnClickedRadio8)
	ON_BN_CLICKED(IDOK, &datalist::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &datalist::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &datalist::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &datalist::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO6, &datalist::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_BUTTON1, &datalist::OnBnClickedButton1)
END_MESSAGE_MAP()


// datalist ��Ϣ�������


BOOL datalist::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
		/*********************************************************************/
		//list�ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void datalist::OnBnClickedRadio11()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	pn = 1;
	gn = 0;
	m_list.DeleteAllItems();
	int nCols = m_list.GetHeaderCtrl()->GetItemCount();
	for (int j = 0; j < nCols; j++)
	{
		m_list.DeleteColumn(0);
	}
	m_list.DeleteAllItems();
	m_list.InsertColumn(0, _T("����"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(1, _T("IR-NSGA-II"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(2, _T("GDE3"), LVCFMT_LEFT, 70);
	
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);//ѡ��ڵ��ʱ��ѡ������

}


void datalist::OnBnClickedRadio12()
{
	m_list.DeleteAllItems();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	gn = 1;
	pn = 0;
	m_list.DeleteAllItems();
	int nCols = m_list.GetHeaderCtrl()->GetItemCount();
	for (int j = 0; j < nCols; j++)
	{
		m_list.DeleteColumn(0);
	}
	m_list.DeleteAllItems();
	m_list.InsertColumn(0, _T("��Ⱥ"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(1, _T("IR-NSGA-II"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(2, _T("GDE3"), LVCFMT_LEFT, 70);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);//ѡ��ڵ��ʱ��ѡ������
}

int zqs, das;
void datalist::OnBnClickedRadio7()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	das = 100;
}


void datalist::OnBnClickedRadio8()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	das = 250;
}


void datalist::showdata()
{
	m_list.DeleteAllItems();
	//m_list.SetItemText(0, 1,LPCTSTR( 50));
	//m_list.SetItemText(1, 1, LPCTSTR(100));
	//m_list.SetItemText(2, 1, LPCTSTR(250));
	//m_list.DeleteAllItems();//���ԭ������
	CString a, b, c;
	a.Format(_T("%d"), 50);
	b.Format(_T("%d"), 100);
	c.Format(_T("%d"), 250);
	m_list.InsertItem(0, LPCTSTR(a));
	//m_list.SetItemText(0, 1, LPCTSTR(50));
	//m_list.SetItemText(0, 2, LPCTSTR(50));
	m_list.InsertItem(1, LPCTSTR(b));
	m_list.InsertItem(2, LPCTSTR(c));
	
	
	char sql[100];
	sprintf_s(sql, "select * from gedir50 where pop>0.99");//���sql����
	if (mysql_real_query(&m_mysql, sql, (unsigned long)strlen(sql)))//ִ��sql����
	{
		MessageBoxW(_T("��ȡ���ʧ��"));
		return;
	}
	MYSQL_RES *result;
	MYSQL_ROW row;
	if (!(result = mysql_use_result(&m_mysql)))//��ȡ����
	{
		MessageBoxW(_T("��ȡ���ݼ�ʧ��"));
		return;
	}
	int i = 0;
	
	while (row = mysql_fetch_row(result)){//��һ����ȡ���ݣ��ڶ������ж��Ƿ�Ϊ��
		i++;
	}
	//m_list.InsertItem(0, LPCTSTR(50));

	mysql_free_result(result);//�ͷ���Դ

	//char sql[100];
	sprintf_s(sql, "select * from ged50 where pop>0.99");//���sql����
	if (mysql_real_query(&m_mysql, sql, (unsigned long)strlen(sql)))//ִ��sql����
	{
		MessageBoxW(_T("��ȡ���ʧ��"));
		return;
	}
	//MYSQL_RES *result;
//	MYSQL_ROW row;
	if (!(result = mysql_use_result(&m_mysql)))//��ȡ����
	{
		MessageBoxW(_T("��ȡ���ݼ�ʧ��"));
		return;
	}
	int j = 0;
	while (row = mysql_fetch_row(result)){//��һ����ȡ���ݣ��ڶ������ж��Ƿ�Ϊ��
		//m_list.InsertItem(0, LPCTSTR(50));
		j++;
	}
	//m_list.InsertItem(0, LPCTSTR(50));
	CString d, e;
	d.Format(_T("%d"), i);
	e.Format(_T("%d"), j);
	m_list.SetItemText(0, 1, LPCTSTR(d));
		m_list.SetItemText(0, 2, LPCTSTR(e));
		mysql_free_result(result);//�ͷ���Դ
		sprintf_s(sql, "select * from gedir100 where pop>0.99");//���sql����
		if (mysql_real_query(&m_mysql, sql, (unsigned long)strlen(sql)))//ִ��sql����
		{
			MessageBoxW(_T("��ȡ���ʧ��"));
			return;
		}
		if (!(result = mysql_use_result(&m_mysql)))//��ȡ����
		{
			MessageBoxW(_T("��ȡ���ݼ�ʧ��"));
			return;
		}
		
		 i = 0;
		while (row = mysql_fetch_row(result)){//��һ����ȡ���ݣ��ڶ������ж��Ƿ�Ϊ��
			i++;
		}
		//
		mysql_free_result(result);//�ͷ���Դ

		//char sql[100];
		sprintf_s(sql, "select * from ged100 where pop>0.99");//���sql����
		if (mysql_real_query(&m_mysql, sql, (unsigned long)strlen(sql)))//ִ��sql����
		{
			MessageBoxW(_T("��ȡ���ʧ��"));
			return;
		}
		//MYSQL_RES *result;
		//	MYSQL_ROW row;
		if (!(result = mysql_use_result(&m_mysql)))//��ȡ����
		{
			MessageBoxW(_T("��ȡ���ݼ�ʧ��"));
			return;
		}
		 j = 0;
		while (row = mysql_fetch_row(result)){//��һ����ȡ���ݣ��ڶ������ж��Ƿ�Ϊ��
			//m_list.InsertItem(0, LPCTSTR(50));
			j++;
		}
		//m_list.InsertItem(1, LPCTSTR(100));
		d.Format(_T("%d"), i);
		e.Format(_T("%d"), j);
		m_list.SetItemText(1, 1, LPCTSTR(d));

		m_list.SetItemText(1, 2, LPCTSTR(e));
	mysql_free_result(result);//�ͷ���Դ
	sprintf_s(sql, "select * from gedir250 where pop>0.99");//���sql����
	if (mysql_real_query(&m_mysql, sql, (unsigned long)strlen(sql)))//ִ��sql����
	{
		MessageBoxW(_T("��ȡ���ʧ��"));
		return;
	}
	if (!(result = mysql_use_result(&m_mysql)))//��ȡ����
	{
		MessageBoxW(_T("��ȡ���ݼ�ʧ��"));
		return;
	}
	 i = 0;
	
	while (row = mysql_fetch_row(result)){//��һ����ȡ���ݣ��ڶ������ж��Ƿ�Ϊ��
		i++;
	}
	//
	mysql_free_result(result);//�ͷ���Դ

	//char sql[100];
	sprintf_s(sql, "select * from ged250 where pop>0.99");//���sql����
	if (mysql_real_query(&m_mysql, sql, (unsigned long)strlen(sql)))//ִ��sql����
	{
		MessageBoxW(_T("��ȡ���ʧ��"));
		return;
	}
	//MYSQL_RES *result;
	//	MYSQL_ROW row;
	if (!(result = mysql_use_result(&m_mysql)))//��ȡ����
	{
		MessageBoxW(_T("��ȡ���ݼ�ʧ��"));
		return;
	}
	 j = 0;
	while (row = mysql_fetch_row(result)){//��һ����ȡ���ݣ��ڶ������ж��Ƿ�Ϊ��
		//m_list.InsertItem(0, LPCTSTR(50));
		j++;
	}
	//m_list.InsertItem(2, LPCTSTR(250));
	d.Format(_T("%d"), i);
	e.Format(_T("%d"), j);
	m_list.SetItemText(2, 1, LPCTSTR(d));
	m_list.SetItemText(2, 2, LPCTSTR(e));
	mysql_free_result(result);//�ͷ���Դ
	

}

 int zs;
void datalist::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
	//MessageBoxW(LPCTSTR(sc));
	
	//	lft->datarun();
		showdata();


}


void datalist::OnBnClickedRadio1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	zqs = 50;
}


void datalist::OnBnClickedRadio2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	zqs = 100;
}


void datalist::OnBnClickedRadio3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	zqs = 250;
}


void datalist::OnBnClickedRadio6()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	das = 50;
}


void datalist::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	lft->datarun();
	showdata();
}
