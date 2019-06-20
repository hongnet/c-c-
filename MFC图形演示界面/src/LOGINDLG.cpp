// LOGINDLG.cpp : 实现文件
//

#include "stdafx.h"
#include "testsystem1.h"
#include "LOGINDLG.h"
#include "afxdialogex.h"

MYSQL m_mysql;
extern MYSQL m_mysql;
extern CString char2CSt(char * szBuf);
extern char * CSt2char(CString str);
CString char2CSt(char * szBuf)
{
	//计算char *数组大小，以字节为单位，一个汉字占两个字节
	int charLen = strlen(szBuf);
	//计算多字节字符的大小，按字符计算。
	int len = MultiByteToWideChar(CP_ACP, 0, szBuf, charLen, NULL, 0);
	//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
	TCHAR *buf = new TCHAR[len + 1];
	//多字节编码转换成宽字节编码
	MultiByteToWideChar(CP_ACP, 0, szBuf, charLen, buf, len);
	buf[len] = '\0'; //添加字符串结尾，注意不是len+1
	//将TCHAR数组转换为CString
	CString pWideChar;
	pWideChar.Append(buf);//把字符串加到后尾
	//删除缓冲区
	delete[]buf;
	return pWideChar;
}
char * CSt2char(CString str)
{
	//注意：以下n和len的值大小不同,n是按字符计算的，len是按字节计算的
	int n = str.GetLength();
	//获取宽字节字符的大小，大小是按字节计算的
	int len = WideCharToMultiByte(CP_ACP, 0, str, str.GetLength(), NULL, 0, NULL, NULL);
	//为多字节字符数组申请空间，数组大小为按字节计算的宽字节字节大小
	char * buff = new char[len + 1]; //以字节为单位
	//宽字节编码转换成多字节编码
	WideCharToMultiByte(CP_ACP, 0, str, str.GetLength() + 1, buff, len + 1, NULL, NULL);
	buff[len + 1] = '\0'; //多字节字符以'\0'结束
	return buff;
}

// CLOGINDLG 对话框

IMPLEMENT_DYNAMIC(CLOGINDLG, CDialogEx)

CLOGINDLG::CLOGINDLG(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLOGINDLG::IDD, pParent)
{

}

CLOGINDLG::~CLOGINDLG()
{
}

void CLOGINDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USER, m_edituser);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editpassword);
}


BEGIN_MESSAGE_MAP(CLOGINDLG, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLOGINDLG::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLOGINDLG::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, &CLOGINDLG::OnBnClickedButtonRegister)
END_MESSAGE_MAP()


// CLOGINDLG 消息处理程序


BOOL CLOGINDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	mysql_init(&m_mysql);//初始化MYSQL对象
	mysql_options(&m_mysql, MYSQL_SET_CHARSET_NAME, "gb2312");//设置客户端支持中文字符集
	if (!mysql_real_connect(&m_mysql, "localhost", "root", "12345678", "school", 3306, NULL, 0))//连接MYSQL后台数据库
	{
		MessageBoxW(_T("数据库连接失败!"));
		return FALSE;
	}
	else
		MessageBoxW(_T("数据库连接成功!"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CLOGINDLG::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);//把界面上的数据传递给底层代码
	CString str_user, str_password;
	m_edituser.GetWindowText(str_user);
	m_editpassword.GetWindowText(str_password);
	if (str_user == _T("") || str_password == _T(""))
	{
		MessageBoxW(_T("用户名或密码不能为空！"));
		return;
	}
	CString mstrsql;
	mstrsql.Format(_T("select * from userinformation where uname='%s' and upassword='%s'"),
		str_user, str_password);
	//MessageBoxW(mstrsql);//测试生成的sql语言
	char *sql = CSt2char(mstrsql);
	if (mysql_real_query(&m_mysql, sql, strlen(sql)))
	{
		MessageBoxW(_T("查询失败！请检查sql语言是否正确？"));
	}
	else
	{
		MYSQL_RES *result;
		MYSQL_ROW row;
		if (!(result = mysql_use_result(&m_mysql)))//获取数据
		{
			MessageBoxW(_T("读取数据集失败！"));
			return;
		}
		row = mysql_fetch_row(result);
		if (row == NULL)
		{
			MessageBoxW(_T("用户名或密码错误！"));
			return;
		}
		mysql_free_result(result);//释放资源，这一步很重要
	}
	CDialogEx::OnOK();
}


void CLOGINDLG::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	mysql_close(&m_mysql);//关闭链接，释放资源
	CDialogEx::OnCancel();
}


void CLOGINDLG::OnBnClickedButtonRegister()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);//把界面上的数据传递给底层代码
	CString str_user, str_password;
	m_edituser.GetWindowText(str_user);
	m_editpassword.GetWindowText(str_password);
	if (str_user == _T("") || str_password == _T(""))
	{
		MessageBoxW(_T("用户名或密码不能为空！"));
		return;
	}
	CString mstrsql;
	mstrsql.Format(_T("insert into userinformation values('%s','%s')"), str_user, str_password);
	//MessageBoxW(mstrsql);//测试生成的sql语言
	char *sql = CSt2char(mstrsql);
	if (mysql_real_query(&m_mysql, sql, strlen(sql)))
	{
		MessageBoxW(_T("注册失败，用户名已经存在！"));
	}
	else
	{
		MessageBoxW(_T("注册成功！"));
	}
}
