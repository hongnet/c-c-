// LOGINDLG.cpp : ʵ���ļ�
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
	//����char *�����С�����ֽ�Ϊ��λ��һ������ռ�����ֽ�
	int charLen = strlen(szBuf);
	//������ֽ��ַ��Ĵ�С�����ַ����㡣
	int len = MultiByteToWideChar(CP_ACP, 0, szBuf, charLen, NULL, 0);
	//Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ķ��ֽ��ַ���С
	TCHAR *buf = new TCHAR[len + 1];
	//���ֽڱ���ת���ɿ��ֽڱ���
	MultiByteToWideChar(CP_ACP, 0, szBuf, charLen, buf, len);
	buf[len] = '\0'; //����ַ�����β��ע�ⲻ��len+1
	//��TCHAR����ת��ΪCString
	CString pWideChar;
	pWideChar.Append(buf);//���ַ����ӵ���β
	//ɾ��������
	delete[]buf;
	return pWideChar;
}
char * CSt2char(CString str)
{
	//ע�⣺����n��len��ֵ��С��ͬ,n�ǰ��ַ�����ģ�len�ǰ��ֽڼ����
	int n = str.GetLength();
	//��ȡ���ֽ��ַ��Ĵ�С����С�ǰ��ֽڼ����
	int len = WideCharToMultiByte(CP_ACP, 0, str, str.GetLength(), NULL, 0, NULL, NULL);
	//Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ŀ��ֽ��ֽڴ�С
	char * buff = new char[len + 1]; //���ֽ�Ϊ��λ
	//���ֽڱ���ת���ɶ��ֽڱ���
	WideCharToMultiByte(CP_ACP, 0, str, str.GetLength() + 1, buff, len + 1, NULL, NULL);
	buff[len + 1] = '\0'; //���ֽ��ַ���'\0'����
	return buff;
}

// CLOGINDLG �Ի���

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


// CLOGINDLG ��Ϣ�������


BOOL CLOGINDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	mysql_init(&m_mysql);//��ʼ��MYSQL����
	mysql_options(&m_mysql, MYSQL_SET_CHARSET_NAME, "gb2312");//���ÿͻ���֧�������ַ���
	if (!mysql_real_connect(&m_mysql, "localhost", "root", "12345678", "school", 3306, NULL, 0))//����MYSQL��̨���ݿ�
	{
		MessageBoxW(_T("���ݿ�����ʧ��!"));
		return FALSE;
	}
	else
		MessageBoxW(_T("���ݿ����ӳɹ�!"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CLOGINDLG::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);//�ѽ����ϵ����ݴ��ݸ��ײ����
	CString str_user, str_password;
	m_edituser.GetWindowText(str_user);
	m_editpassword.GetWindowText(str_password);
	if (str_user == _T("") || str_password == _T(""))
	{
		MessageBoxW(_T("�û��������벻��Ϊ�գ�"));
		return;
	}
	CString mstrsql;
	mstrsql.Format(_T("select * from userinformation where uname='%s' and upassword='%s'"),
		str_user, str_password);
	//MessageBoxW(mstrsql);//�������ɵ�sql����
	char *sql = CSt2char(mstrsql);
	if (mysql_real_query(&m_mysql, sql, strlen(sql)))
	{
		MessageBoxW(_T("��ѯʧ�ܣ�����sql�����Ƿ���ȷ��"));
	}
	else
	{
		MYSQL_RES *result;
		MYSQL_ROW row;
		if (!(result = mysql_use_result(&m_mysql)))//��ȡ����
		{
			MessageBoxW(_T("��ȡ���ݼ�ʧ�ܣ�"));
			return;
		}
		row = mysql_fetch_row(result);
		if (row == NULL)
		{
			MessageBoxW(_T("�û������������"));
			return;
		}
		mysql_free_result(result);//�ͷ���Դ����һ������Ҫ
	}
	CDialogEx::OnOK();
}


void CLOGINDLG::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	mysql_close(&m_mysql);//�ر����ӣ��ͷ���Դ
	CDialogEx::OnCancel();
}


void CLOGINDLG::OnBnClickedButtonRegister()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);//�ѽ����ϵ����ݴ��ݸ��ײ����
	CString str_user, str_password;
	m_edituser.GetWindowText(str_user);
	m_editpassword.GetWindowText(str_password);
	if (str_user == _T("") || str_password == _T(""))
	{
		MessageBoxW(_T("�û��������벻��Ϊ�գ�"));
		return;
	}
	CString mstrsql;
	mstrsql.Format(_T("insert into userinformation values('%s','%s')"), str_user, str_password);
	//MessageBoxW(mstrsql);//�������ɵ�sql����
	char *sql = CSt2char(mstrsql);
	if (mysql_real_query(&m_mysql, sql, strlen(sql)))
	{
		MessageBoxW(_T("ע��ʧ�ܣ��û����Ѿ����ڣ�"));
	}
	else
	{
		MessageBoxW(_T("ע��ɹ���"));
	}
}
