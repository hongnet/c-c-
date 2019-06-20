// datalist.cpp : 实现文件
//

#include "stdafx.h"
#include "testsystem1.h"
#include "datalist.h"
#include "afxdialogex.h"
#include"LEFTTOP.h"
int pn=0, gn=0;
// datalist 对话框
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


// datalist 消息处理程序


BOOL datalist::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
		/*********************************************************************/
		//list的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void datalist::OnBnClickedRadio11()
{
	// TODO:  在此添加控件通知处理程序代码
	pn = 1;
	gn = 0;
	m_list.DeleteAllItems();
	int nCols = m_list.GetHeaderCtrl()->GetItemCount();
	for (int j = 0; j < nCols; j++)
	{
		m_list.DeleteColumn(0);
	}
	m_list.DeleteAllItems();
	m_list.InsertColumn(0, _T("迭代"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(1, _T("IR-NSGA-II"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(2, _T("GDE3"), LVCFMT_LEFT, 70);
	
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);//选择节点的时候选中整行

}


void datalist::OnBnClickedRadio12()
{
	m_list.DeleteAllItems();
	// TODO:  在此添加控件通知处理程序代码
	gn = 1;
	pn = 0;
	m_list.DeleteAllItems();
	int nCols = m_list.GetHeaderCtrl()->GetItemCount();
	for (int j = 0; j < nCols; j++)
	{
		m_list.DeleteColumn(0);
	}
	m_list.DeleteAllItems();
	m_list.InsertColumn(0, _T("种群"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(1, _T("IR-NSGA-II"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(2, _T("GDE3"), LVCFMT_LEFT, 70);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);//选择节点的时候选中整行
}

int zqs, das;
void datalist::OnBnClickedRadio7()
{
	// TODO:  在此添加控件通知处理程序代码
	das = 100;
}


void datalist::OnBnClickedRadio8()
{
	// TODO:  在此添加控件通知处理程序代码
	das = 250;
}


void datalist::showdata()
{
	m_list.DeleteAllItems();
	//m_list.SetItemText(0, 1,LPCTSTR( 50));
	//m_list.SetItemText(1, 1, LPCTSTR(100));
	//m_list.SetItemText(2, 1, LPCTSTR(250));
	//m_list.DeleteAllItems();//清空原有数据
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
	sprintf_s(sql, "select * from gedir50 where pop>0.99");//设计sql语言
	if (mysql_real_query(&m_mysql, sql, (unsigned long)strlen(sql)))//执行sql语言
	{
		MessageBoxW(_T("读取表格失败"));
		return;
	}
	MYSQL_RES *result;
	MYSQL_ROW row;
	if (!(result = mysql_use_result(&m_mysql)))//获取数据
	{
		MessageBoxW(_T("读取数据集失败"));
		return;
	}
	int i = 0;
	
	while (row = mysql_fetch_row(result)){//第一步是取数据，第二步是判断是否为空
		i++;
	}
	//m_list.InsertItem(0, LPCTSTR(50));

	mysql_free_result(result);//释放资源

	//char sql[100];
	sprintf_s(sql, "select * from ged50 where pop>0.99");//设计sql语言
	if (mysql_real_query(&m_mysql, sql, (unsigned long)strlen(sql)))//执行sql语言
	{
		MessageBoxW(_T("读取表格失败"));
		return;
	}
	//MYSQL_RES *result;
//	MYSQL_ROW row;
	if (!(result = mysql_use_result(&m_mysql)))//获取数据
	{
		MessageBoxW(_T("读取数据集失败"));
		return;
	}
	int j = 0;
	while (row = mysql_fetch_row(result)){//第一步是取数据，第二步是判断是否为空
		//m_list.InsertItem(0, LPCTSTR(50));
		j++;
	}
	//m_list.InsertItem(0, LPCTSTR(50));
	CString d, e;
	d.Format(_T("%d"), i);
	e.Format(_T("%d"), j);
	m_list.SetItemText(0, 1, LPCTSTR(d));
		m_list.SetItemText(0, 2, LPCTSTR(e));
		mysql_free_result(result);//释放资源
		sprintf_s(sql, "select * from gedir100 where pop>0.99");//设计sql语言
		if (mysql_real_query(&m_mysql, sql, (unsigned long)strlen(sql)))//执行sql语言
		{
			MessageBoxW(_T("读取表格失败"));
			return;
		}
		if (!(result = mysql_use_result(&m_mysql)))//获取数据
		{
			MessageBoxW(_T("读取数据集失败"));
			return;
		}
		
		 i = 0;
		while (row = mysql_fetch_row(result)){//第一步是取数据，第二步是判断是否为空
			i++;
		}
		//
		mysql_free_result(result);//释放资源

		//char sql[100];
		sprintf_s(sql, "select * from ged100 where pop>0.99");//设计sql语言
		if (mysql_real_query(&m_mysql, sql, (unsigned long)strlen(sql)))//执行sql语言
		{
			MessageBoxW(_T("读取表格失败"));
			return;
		}
		//MYSQL_RES *result;
		//	MYSQL_ROW row;
		if (!(result = mysql_use_result(&m_mysql)))//获取数据
		{
			MessageBoxW(_T("读取数据集失败"));
			return;
		}
		 j = 0;
		while (row = mysql_fetch_row(result)){//第一步是取数据，第二步是判断是否为空
			//m_list.InsertItem(0, LPCTSTR(50));
			j++;
		}
		//m_list.InsertItem(1, LPCTSTR(100));
		d.Format(_T("%d"), i);
		e.Format(_T("%d"), j);
		m_list.SetItemText(1, 1, LPCTSTR(d));

		m_list.SetItemText(1, 2, LPCTSTR(e));
	mysql_free_result(result);//释放资源
	sprintf_s(sql, "select * from gedir250 where pop>0.99");//设计sql语言
	if (mysql_real_query(&m_mysql, sql, (unsigned long)strlen(sql)))//执行sql语言
	{
		MessageBoxW(_T("读取表格失败"));
		return;
	}
	if (!(result = mysql_use_result(&m_mysql)))//获取数据
	{
		MessageBoxW(_T("读取数据集失败"));
		return;
	}
	 i = 0;
	
	while (row = mysql_fetch_row(result)){//第一步是取数据，第二步是判断是否为空
		i++;
	}
	//
	mysql_free_result(result);//释放资源

	//char sql[100];
	sprintf_s(sql, "select * from ged250 where pop>0.99");//设计sql语言
	if (mysql_real_query(&m_mysql, sql, (unsigned long)strlen(sql)))//执行sql语言
	{
		MessageBoxW(_T("读取表格失败"));
		return;
	}
	//MYSQL_RES *result;
	//	MYSQL_ROW row;
	if (!(result = mysql_use_result(&m_mysql)))//获取数据
	{
		MessageBoxW(_T("读取数据集失败"));
		return;
	}
	 j = 0;
	while (row = mysql_fetch_row(result)){//第一步是取数据，第二步是判断是否为空
		//m_list.InsertItem(0, LPCTSTR(50));
		j++;
	}
	//m_list.InsertItem(2, LPCTSTR(250));
	d.Format(_T("%d"), i);
	e.Format(_T("%d"), j);
	m_list.SetItemText(2, 1, LPCTSTR(d));
	m_list.SetItemText(2, 2, LPCTSTR(e));
	mysql_free_result(result);//释放资源
	

}

 int zs;
void datalist::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnOK();
	//MessageBoxW(LPCTSTR(sc));
	
	//	lft->datarun();
		showdata();


}


void datalist::OnBnClickedRadio1()
{
	// TODO:  在此添加控件通知处理程序代码
	zqs = 50;
}


void datalist::OnBnClickedRadio2()
{
	// TODO:  在此添加控件通知处理程序代码
	zqs = 100;
}


void datalist::OnBnClickedRadio3()
{
	// TODO:  在此添加控件通知处理程序代码
	zqs = 250;
}


void datalist::OnBnClickedRadio6()
{
	// TODO:  在此添加控件通知处理程序代码
	das = 50;
}


void datalist::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	lft->datarun();
	showdata();
}
