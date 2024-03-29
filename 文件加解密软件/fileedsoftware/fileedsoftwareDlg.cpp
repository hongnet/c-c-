
// fileedsoftwareDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "fileedsoftware.h"
#include "fileedsoftwareDlg.h"
#include "afxdialogex.h"
//#include"Resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CfileedsoftwareDlg 对话框



CfileedsoftwareDlg::CfileedsoftwareDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILEEDSOFTWARE_DIALOG, pParent)
	, m_path(_T(""))
	, m_password(_T(""))
//	, m_password(_T(""))
, m_cpass(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CfileedsoftwareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EPth, m_path);
	DDX_Text(pDX, IDC_Epass, m_password);
	//(pDX, IDC_Ecpss, m_cpass);
	DDX_Text(pDX, IDC_Ecpss, m_cpass);
}

BEGIN_MESSAGE_MAP(CfileedsoftwareDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CfileedsoftwareDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDecryption, &CfileedsoftwareDlg::OnBnClickedecryption)
	ON_BN_CLICKED(IDC_BSELECT, &CfileedsoftwareDlg::OnBnClickedBselect)
	//ON_EN_CHANGE(IDC_Epass, &CfileedsoftwareDlg::OnEnChangeEpass)
	ON_BN_CLICKED(IDC_Bpass, &CfileedsoftwareDlg::OnBnClickedBpass)
END_MESSAGE_MAP()


// CfileedsoftwareDlg 消息处理程序

BOOL CfileedsoftwareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CfileedsoftwareDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CfileedsoftwareDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CfileedsoftwareDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

__int64 CfileedsoftwareDlg::getpass()
{
	__int64 epss = 99799499399599;
	UpdateData(TRUE);
	char *ppss = m_password.GetBuffer(0);
	DWORD plen=strlen(ppss);
	for(int i=0;i<(int)plen;i++)
	{
	   epss ^= ppss[i] | 128;
	}
	return epss;

}


void CfileedsoftwareDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
	return;
}


void CfileedsoftwareDlg::OnBnClickedecryption()
{
	// TODO: 在此添加控件通知处理程序代码
	CFile *cfile = new CFile;
	char *data;
	__int64 m_pss = getpass();

	if (!cfile->Open(m_path, CFile::shareDenyNone | CFile::modeReadWrite))
	{
		MessageBox("文件打开失败", "错误", MB_ICONERROR);
		return;
	}
	int flen = cfile->GetLength();
	data = new char[flen];
	cfile->SeekToBegin();
	cfile->Read(data, flen);
	//加入验证码
	char *cnum = new char[4];
	cfile->Seek(-4, CFile::end);
	cfile->Read(cnum, 4);
	for (int i = 0; i < 4; i++)
	{
		cnum[i] ^= m_pss;
	}
	int rtn = strcmp(cnum, "cjh");
	if (strcmp(cnum, "cjh"))
	{
		MessageBox("验证码不正确！", "错误", MB_ICONERROR);
	}
	delete []cnum;
	for (int i = 0; i < flen; i++)
	{
		data[i] ^= m_pss;
		data[i] ^= (flen-4);
	}
	cfile->SeekToBegin();
	cfile->Write(data, flen);
	cfile->SetLength (flen- 4);
	cfile->Close();
	delete[] data;
	delete  cfile;
	MessageBox("文件已解密", "信息", MB_OK);
}


void CfileedsoftwareDlg::OnBnClickedBselect()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog  fdlg(TRUE);
	if (fdlg.DoModal() == IDOK)
	{
		m_path = fdlg.GetPathName();
		UpdateData(FALSE);

	}
	else
	{
		MessageBox("选择文件无效", "错误", MB_ICONERROR);
		return;
	}
		
}

void CfileedsoftwareDlg::OnBnClickedBpass()
{
	// TODO: 在此添加控件通知处理程序代码
	CFile *cfile=new CFile;
	char *data;
	__int64 m_pss= getpass();
	if (!cfile->Open(m_path, CFile::shareDenyNone | CFile::modeReadWrite))
	{
		MessageBox("文件打开失败","错误", MB_ICONERROR);
		return;
	}
	int flen = cfile->GetLength();
	data = new char[flen];
	cfile->SeekToBegin();
	cfile->Read(data, flen);
	for (int i = 0; i < flen; i++)
	{
		data[i] ^= m_pss;
		data[i] ^= flen;
	}
	cfile->SeekToBegin();
	cfile->Write(data, flen);
	delete[] data;
	//加入验证码
	char cnum[4] = "cjh";
	for (int i = 0; i <4; i++)
	{
		cnum[i] ^= m_pss;
	}
	cfile->SeekToEnd();
	cfile->Write(cnum, 4);
	cfile->Close();
	delete  cfile;
	MessageBox("文件加密已完成", "信息", MB_OK);
}
