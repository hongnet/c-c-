// RIGHTTOPVIEW.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testsystem1.h"
#include "RIGHTTOPVIEW.h"
#include"LEFTTOP.h"
#include"..\TeeFiles\series.h"
#include"..\TeeFiles\point3dseries.h"
#include"..\TeeFiles\legend.h"
#include"..\TeeFiles\axes.h"
#include"..\TeeFiles\aspect.h"
#include"..\TeeFiles\axis.h"
#include"..\TeeFiles\titles.h"
#include"..\TeeFiles\panel.h"
#include"..\TeeFiles\lineseries.h"
#include"..\TeeFiles\axistitle.h"
#include"..\TeeFiles\strings.h"
#include"..\TeeFiles\pen.h"
#include"..\TeeFiles\gradient.h"
#include"..\TeeFiles\axislabels.h"
#include"..\TeeFiles\import.h"
#include"..\TeeFiles\marks.h"
//#include"..\TeeFiles\labels.h"
#include"testsystem1Doc.h"
#include<sstream>
using namespace std;
extern MYSQL m_mysql;
extern CString char2CSt(char * szBuf);
extern char * CSt2char(CString str);
 int sc, popn, gennumber, alogrithm, bdu=0;

//#include"tchar.h"
//#include"CSeries.h"
// CRIGHTTOPVIEW
//extern int  sc, alogrithm,bdu;
//extern CButton m_duibi;
IMPLEMENT_DYNCREATE(CRIGHTTOPVIEW, CFormView)

CRIGHTTOPVIEW::CRIGHTTOPVIEW()
	: CFormView(CRIGHTTOPVIEW::IDD)
	, sanwei(0)
	, erwei(0)

//	, m_teechart(0)
{

}

CRIGHTTOPVIEW::~CRIGHTTOPVIEW()
{
}

void CRIGHTTOPVIEW::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_TCHART1, m_teechart);
	//  DDX_Control(pDX, IDC_TCHART1, m_teechart);
	//  DDX_Control(pDX, IDC_TCHART1, m_teechart);
	//  DDX_Control(pDX, IDC_TCHART1, m_teechart);
	//DDX_Control(pDX, IDC_TCHART1, m_teechart);
	DDX_Control(pDX, IDC_TCHART1, m_teechart);
}

BEGIN_MESSAGE_MAP(CRIGHTTOPVIEW, CFormView)
	ON_BN_CLICKED(IDC_RADIO1, &CRIGHTTOPVIEW::OnBnClickedRadio1)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_RADIO2, &CRIGHTTOPVIEW::OnBnClickedRadio2)
//	ON_BN_CLICKED(IDC_BUTTON1, &CRIGHTTOPVIEW::OnBnClickedButton1)
END_MESSAGE_MAP()


// CRIGHTTOPVIEW ���

#ifdef _DEBUG
void CRIGHTTOPVIEW::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CRIGHTTOPVIEW::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRIGHTTOPVIEW ��Ϣ�������


void CRIGHTTOPVIEW::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO:  �ڴ����ר�ô����/����û���
	//ѡ����Ҫ��ȡ��txt�ļ�  
	/*CString filename;
	CFileDialog hFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)_TEXT("TXT Files(*.txt)|*.txt|All Files (*.*)|*.*||"), NULL);
	if (IDOK == hFileDlg.DoModal())
	{
		filename = hFileDlg.GetFileName();
		//�ж϶�ȡ���ļ��Ƿ�Ϊԭ�ļ�  
		if ("hefei_data.txt" != filename)
		{
			<span style = "white-space:pre;">    < / span>
				MessageBox("ѡ���ļ�����!");
		}
		else
		{
			fp_read = fopen(filename, "r");
			if (NULL == fp_read)
			{
				MessageBox("�ļ���ȡ����!");//�쳣����  
			}
		}
	}*/
	CRect rect;
   GetClientRect(&rect);
  // CTChart* m_teechart1 = new CTChart();
   m_teechart.MoveWindow(&rect, TRUE);
   m_teechart.Series(0).SetColor(RGB(255, 0, 0)); // ���õ�һ�����ߵ���ɫΪ��ɫ  
   m_teechart.Series(1).SetColor(RGB(0, 255, 0)); //���õڶ������ߵ���ɫΪ��ɫ  
   m_teechart.Series(0).SetTitle(_T("GED3"));
   m_teechart.Series(1).SetTitle(_T("IR-NSGA-II"));
   //m_teechart.Create(NULL,NULL, WS_VISIBLE, CRect(0, 0, 0, 0), this, 1234);
 //  CImport imp = static_cast<CImport>(m_teechart.GetImport());// create
   //imp.LoadFromFile(_T("1.tee"));
   //m_teechart.Create(NULL, WS_CHILD | WS_VISIBLE, rc, this, 1000);
   //	m_teechart.AddSeries(0);
   //m_teechart.Series(0).FillSampleValues(50);
//m_teechart.GetAspect().SetView3D(TRUE);

}


void CRIGHTTOPVIEW::OnBnClickedRadio1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ��������
	//double dx1 = 1, dy1 = 5, dz1 = 10;
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_teechart.GetAspect().SetView3D(TRUE);
	m_teechart.Series(0).Clear();
	m_teechart.Series(1).Clear();
	//m_teechart.RemoveAllSeries();
	m_teechart.GetAxis().GetDepth().SetVisible(TRUE);  //��ʾZ�� 
	//m_teechart.GetAxis().GetDepthTop().SetVisible(TRUE);
	m_teechart.GetAxis().GetDepth().GetLabels().SetVisible(TRUE);  //��ʾZ���ϵ�����  
	m_teechart.GetAxis().GetDepth().GetLabels().SetStyle(0);  //������ʾ����ķ��  
	m_teechart.GetAxis().GetDepth().GetTitle().SetCaption(_T("R"));
	//m_teechart.Series(0).GetMarks().SetVisible(TRUE);
	//CButton *p = ((CButton*)GetDlgItem(IDC_BUTTON3));
	if (bdu==1)
	{
		if (sc == 0)
		{

			{
				///m_teechart.GetAxis().GetBottom().SetMinMax(0, 0.2);
				//CSeries data1 = (CSeries)m_teechart.Series(0);
				//	data1.Clear();
				//for (int i = 0; i < 100; i++)

				_bstr_t bstrQuery("SELECT * FROM ged5");
				if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
				{
					MessageBoxW(_T("��ȡ����ʧ��"));

				}
				//else
				//MessageBoxW(_T("��ȡ���ݳɹ�"));
				MYSQL_RES *result;
				MYSQL_ROW row;
				if (!(result = mysql_use_result(&m_mysql)))
					MessageBoxW(_T("��ȡ���ʧ��"));
				//_variant_t vRecsAffected(0L);
				//	data1.AddXY(i, rand() / 100, NULL, NULL);
				if (m_teechart.GetSeriesCount() < 1)
				{
					m_teechart.AddSeries(6);
				}
				int i = 0;
				while (row = mysql_fetch_row(result))
				{
					double x, y;

					stringstream s, s1;
					s << row[0];
					s1 << row[1];
					s >> x;
					s1 >> y;
					// x = sscanf(row[0]," %f", &x);
					// y = sscanf(row[1], "%f", &y);
					// double x=atof(row[0]), y=atof(row[1]);
					//m_teechart.Series(0).AddXY(x, y, NULL, RGB(255, 0, 0));
					m_teechart.Series(0).GetAsPoint3D().AddXYZ(x, y, i, NULL, RGB(255, 0, 0));
					i++;
					//pRecordset->MoveNext();

				}
				mysql_free_result(result);
			}

		{
			_bstr_t bstrQuery("SELECT * FROM gedir5");
			if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
			{
				MessageBoxW(_T("��ȡ����ʧ��"));

			}
			//else
			//MessageBoxW(_T("��ȡ���ݳɹ�"));
			MYSQL_RES *result;
			MYSQL_ROW row;
			if (!(result = mysql_use_result(&m_mysql)))
				MessageBoxW(_T("��ȡ���ʧ��"));
			//_variant_t vRecsAffected(0L);
			//	data1.AddXY(i, rand() / 100, NULL, NULL);
			if (m_teechart.GetSeriesCount() < 1)
			{
				m_teechart.AddSeries(6);
			}
			int i = 0;
			while (row = mysql_fetch_row(result))
			{
				double x, y;

				stringstream s, s1;
				s << row[0];
				s1 << row[2];
				s >> x;
				s1 >> y;
				// x = sscanf(row[0]," %f", &x);
				// y = sscanf(row[1], "%f", &y);
				// double x=atof(row[0]), y=atof(row[1]);
				//m_teechart.Series(0).AddXY(x, y, NULL, RGB(255, 0, 0));
				m_teechart.Series(1).GetAsPoint3D().AddXYZ(x, y, i, NULL, RGB(0, 255, 0));
				i++;
				//pRecordset->MoveNext();

			}
			mysql_free_result(result);
		}
		}
		else
			if (sc == 1)
			{

				{
					_bstr_t bstrQuery("SELECT * FROM ged6");
					if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
					{
						MessageBoxW(_T("��ȡ����ʧ��"));

					}
					//else
					//MessageBoxW(_T("��ȡ���ݳɹ�"));
					MYSQL_RES *result;
					MYSQL_ROW row;
					if (!(result = mysql_use_result(&m_mysql)))
						MessageBoxW(_T("��ȡ���ʧ��"));
					//_variant_t vRecsAffected(0L);
					//	data1.AddXY(i, rand() / 100, NULL, NULL);
					if (m_teechart.GetSeriesCount() < 1)
					{
						m_teechart.AddSeries(6);
					}
					int i = 0;
					while (row = mysql_fetch_row(result))
					{
						double x, y;

						stringstream s, s1;
						s << row[0];
						s1 << row[1];
						s >> x;
						s1 >> y;
						// x = sscanf(row[0]," %f", &x);
						// y = sscanf(row[1], "%f", &y);
						// double x=atof(row[0]), y=atof(row[1]);
						//m_teechart.Series(0).AddXY(x, y, NULL, RGB(255, 0, 0));
						m_teechart.Series(0).GetAsPoint3D().AddXYZ(x, y, i, NULL, RGB(255, 0, 0));
						i++;
						//pRecordset->MoveNext();

					}
					mysql_free_result(result);
				}

			{
				_bstr_t bstrQuery("SELECT * FROM gedir6");
				if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
				{
					MessageBoxW(_T("��ȡ����ʧ��"));

				}
				//else
				//MessageBoxW(_T("��ȡ���ݳɹ�"));
				MYSQL_RES *result;
				MYSQL_ROW row;
				if (!(result = mysql_use_result(&m_mysql)))
					MessageBoxW(_T("��ȡ���ʧ��"));
				//_variant_t vRecsAffected(0L);
				//	data1.AddXY(i, rand() / 100, NULL, NULL);
				if (m_teechart.GetSeriesCount() < 1)
				{
					m_teechart.AddSeries(6);
				}
				int i = 0;
				while (row = mysql_fetch_row(result))
				{
					double x, y;

					stringstream s, s1;
					s << row[0];
					s1 << row[2];
					s >> x;
					s1 >> y;
					// x = sscanf(row[0]," %f", &x);
					// y = sscanf(row[1], "%f", &y);
					// double x=atof(row[0]), y=atof(row[1]);
					//m_teechart.Series(0).AddXY(x, y, NULL, RGB(255, 0, 0));
					m_teechart.Series(1).GetAsPoint3D().AddXYZ(x, y, i, NULL, RGB(0, 255, 0));
					i++;
					//pRecordset->MoveNext();

				}
				mysql_free_result(result);
			}
			}
			else
			{

				{
					_bstr_t bstrQuery("SELECT * FROM ged7");
					if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
					{
						MessageBoxW(_T("��ȡ����ʧ��"));

					}
					//else
					//MessageBoxW(_T("��ȡ���ݳɹ�"));
					MYSQL_RES *result;
					MYSQL_ROW row;
					if (!(result = mysql_use_result(&m_mysql)))
						MessageBoxW(_T("��ȡ���ʧ��"));
					//_variant_t vRecsAffected(0L);
					//	data1.AddXY(i, rand() / 100, NULL, NULL);
					if (m_teechart.GetSeriesCount() < 1)
					{
						m_teechart.AddSeries(6);
					}
					int i = 0;
					while (row = mysql_fetch_row(result))
					{
						double x, y;

						stringstream s, s1;
						s << row[0];
						s1 << row[1];
						s >> x;
						s1 >> y;
						// x = sscanf(row[0]," %f", &x);
						// y = sscanf(row[1], "%f", &y);
						// double x=atof(row[0]), y=atof(row[1]);
						//m_teechart.Series(0).AddXY(x, y, NULL, RGB(255, 0, 0));
						m_teechart.Series(0).GetAsPoint3D().AddXYZ(x, y, i, NULL, RGB(255, 0, 0));
						i++;
						//pRecordset->MoveNext();

					}
					mysql_free_result(result);
				}

			{
				_bstr_t bstrQuery("SELECT * FROM gedir7");
				if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
				{
					MessageBoxW(_T("��ȡ����ʧ��"));

				}
				//else
				//MessageBoxW(_T("��ȡ���ݳɹ�"));
				MYSQL_RES *result;
				MYSQL_ROW row;
				if (!(result = mysql_use_result(&m_mysql)))
					MessageBoxW(_T("��ȡ���ʧ��"));
				//_variant_t vRecsAffected(0L);
				//	data1.AddXY(i, rand() / 100, NULL, NULL);
				if (m_teechart.GetSeriesCount() < 1)
				{
					m_teechart.AddSeries(6);
				}
				int i = 0;
				while (row = mysql_fetch_row(result))
				{
					double x, y;

					stringstream s, s1;
					s << row[0];
					s1 << row[1];
					s >> x;
					s1 >> y;
					// x = sscanf(row[0]," %f", &x);
					// y = sscanf(row[1], "%f", &y);
					// double x=atof(row[0]), y=atof(row[1]);
					//m_teechart.Series(0).AddXY(x, y, NULL, RGB(255, 0, 0));
					m_teechart.Series(1).GetAsPoint3D().AddXYZ(x, y, i, NULL, RGB(0, 255, 0));
					i++;
					//pRecordset->MoveNext();

				}
				mysql_free_result(result);
			}
			}
	}
	else
		if (alogrithm == 1)
		{
			if (sc == 0)
			{
				_bstr_t bstrQuery("SELECT * FROM ged5");
				if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
				{
					MessageBoxW(_T("��ȡ����ʧ��"));

				}
				//else
				//MessageBoxW(_T("��ȡ���ݳɹ�"));
				MYSQL_RES *result;
				MYSQL_ROW row;
				if (!(result = mysql_use_result(&m_mysql)))
					MessageBoxW(_T("��ȡ���ʧ��"));
				//_variant_t vRecsAffected(0L);
				//	data1.AddXY(i, rand() / 100, NULL, NULL);
				if (m_teechart.GetSeriesCount() < 1)
				{
					m_teechart.AddSeries(6);
				}
				int i = 0;
				while (row = mysql_fetch_row(result))
				{
					double x, y;

					stringstream s, s1;
					s << row[0];
					s1 << row[1];
					s >> x;
					s1 >> y;
					// x = sscanf(row[0]," %f", &x);
					// y = sscanf(row[1], "%f", &y);
					// double x=atof(row[0]), y=atof(row[1]);
					//m_teechart.Series(0).AddXY(x, y, NULL, RGB(255, 0, 0));
					m_teechart.Series(0).GetAsPoint3D().AddXYZ(x, y, i, NULL, RGB(255, 0, 0));
					i++;
					//pRecordset->MoveNext();

				}
				mysql_free_result(result);
			}
			else
				if (sc == 1)
				{
					_bstr_t bstrQuery("SELECT * FROM ged6");
					if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
					{
						MessageBoxW(_T("��ȡ����ʧ��"));

					}
					//else
					//MessageBoxW(_T("��ȡ���ݳɹ�"));
					MYSQL_RES *result;
					MYSQL_ROW row;
					if (!(result = mysql_use_result(&m_mysql)))
						MessageBoxW(_T("��ȡ���ʧ��"));
					//_variant_t vRecsAffected(0L);
					//	data1.AddXY(i, rand() / 100, NULL, NULL);
					if (m_teechart.GetSeriesCount() < 1)
					{
						m_teechart.AddSeries(6);
					}
					int i = 0;
					while (row = mysql_fetch_row(result))
					{
						double x, y;

						stringstream s, s1;
						s << row[0];
						s1 << row[1];
						s >> x;
						s1 >> y;
						// x = sscanf(row[0]," %f", &x);
						// y = sscanf(row[1], "%f", &y);
						// double x=atof(row[0]), y=atof(row[1]);
						//m_teechart.Series(0).AddXY(x, y, NULL, RGB(255, 0, 0));
						m_teechart.Series(0).GetAsPoint3D().AddXYZ(x, y, i, NULL, RGB(255, 0, 0));
						i++;
						//pRecordset->MoveNext();

					}
					mysql_free_result(result);

				}
				else
				{
					_bstr_t bstrQuery("SELECT * FROM ged7");
					if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
					{
						MessageBoxW(_T("��ȡ����ʧ��"));

					}
					//else
					//MessageBoxW(_T("��ȡ���ݳɹ�"));
					MYSQL_RES *result;
					MYSQL_ROW row;
					if (!(result = mysql_use_result(&m_mysql)))
						MessageBoxW(_T("��ȡ���ʧ��"));
					//_variant_t vRecsAffected(0L);
					//	data1.AddXY(i, rand() / 100, NULL, NULL);
					if (m_teechart.GetSeriesCount() < 1)
					{
						m_teechart.AddSeries(6);
					}
					int i = 0;
					while (row = mysql_fetch_row(result))
					{
						double x, y;

						stringstream s, s1;
						s << row[0];
						s1 << row[1];
						s >> x;
						s1 >> y;
						// x = sscanf(row[0]," %f", &x);
						// y = sscanf(row[1], "%f", &y);
						// double x=atof(row[0]), y=atof(row[1]);
						//m_teechart.Series(0).AddXY(x, y, NULL, RGB(255, 0, 0));
						m_teechart.Series(0).GetAsPoint3D().AddXYZ(x, y, i, NULL, RGB(255, 0, 0));
						i++;
						//pRecordset->MoveNext();

					}
					mysql_free_result(result);
				}
		}
		else
		{
			if (sc == 0)
			{
				_bstr_t bstrQuery("SELECT * FROM gedir5");
				if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
				{
					MessageBoxW(_T("��ȡ����ʧ��"));

				}
				//else
				//MessageBoxW(_T("��ȡ���ݳɹ�"));
				MYSQL_RES *result;
				MYSQL_ROW row;
				if (!(result = mysql_use_result(&m_mysql)))
					MessageBoxW(_T("��ȡ���ʧ��"));
				//_variant_t vRecsAffected(0L);
				//	data1.AddXY(i, rand() / 100, NULL, NULL);
				if (m_teechart.GetSeriesCount() < 1)
				{
					m_teechart.AddSeries(6);
				}
				int i = 0;
				while (row = mysql_fetch_row(result))
				{
					double x, y;

					stringstream s, s1;
					s << row[0];
					s1 << row[2];
					s >> x;
					s1 >> y;
					// x = sscanf(row[0]," %f", &x);
					// y = sscanf(row[1], "%f", &y);
					// double x=atof(row[0]), y=atof(row[1]);
					//m_teechart.Series(0).AddXY(x, y, NULL, RGB(255, 0, 0));
					m_teechart.Series(1).GetAsPoint3D().AddXYZ(x, y, i, NULL, RGB(0, 255, 0));
					i++;
					//pRecordset->MoveNext();

				}
				mysql_free_result(result);
			}
			else
				if (sc == 1)
				{
					_bstr_t bstrQuery("SELECT * FROM gedir6");
					if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
					{
						MessageBoxW(_T("��ȡ����ʧ��"));

					}
					//else
					//MessageBoxW(_T("��ȡ���ݳɹ�"));
					MYSQL_RES *result;
					MYSQL_ROW row;
					if (!(result = mysql_use_result(&m_mysql)))
						MessageBoxW(_T("��ȡ���ʧ��"));
					//_variant_t vRecsAffected(0L);
					//	data1.AddXY(i, rand() / 100, NULL, NULL);
					if (m_teechart.GetSeriesCount() < 1)
					{
						m_teechart.AddSeries(6);
					}
					int i = 0;
					while (row = mysql_fetch_row(result))
					{
						double x, y;

						stringstream s, s1;
						s << row[0];
						s1 << row[2];
						s >> x;
						s1 >> y;
						// x = sscanf(row[0]," %f", &x);
						// y = sscanf(row[1], "%f", &y);
						// double x=atof(row[0]), y=atof(row[1]);
						//m_teechart.Series(0).AddXY(x, y, NULL, RGB(255, 0, 0));
						m_teechart.Series(1).GetAsPoint3D().AddXYZ(x, y, i, NULL, RGB(0, 255, 0));
						i++;
						//pRecordset->MoveNext();

					}
					mysql_free_result(result);
				}
				else
				{
					_bstr_t bstrQuery("SELECT * FROM gedir7");
					if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
					{
						MessageBoxW(_T("��ȡ����ʧ��"));

					}
					//else
					//MessageBoxW(_T("��ȡ���ݳɹ�"));
					MYSQL_RES *result;
					MYSQL_ROW row;
					if (!(result = mysql_use_result(&m_mysql)))
						MessageBoxW(_T("��ȡ���ʧ��"));
					//_variant_t vRecsAffected(0L);
					//	data1.AddXY(i, rand() / 100, NULL, NULL);
					if (m_teechart.GetSeriesCount() < 1)
					{
						m_teechart.AddSeries(6);
					}
					int i = 0;
					while (row = mysql_fetch_row(result))
					{
						double x, y;

						stringstream s, s1;
						s << row[0];
						s1 << row[2];
						s >> x;
						s1 >> y;
						// x = sscanf(row[0]," %f", &x);
						// y = sscanf(row[1], "%f", &y);
						// double x=atof(row[0]), y=atof(row[1]);
						//m_teechart.Series(0).AddXY(x, y, NULL, RGB(255, 0, 0));
						m_teechart.Series(1).GetAsPoint3D().AddXYZ(x, y, i, NULL, RGB(0, 255, 0));
						i++;
						//pRecordset->MoveNext();

					}
					mysql_free_result(result);
				}
		}
	
}


int CRIGHTTOPVIEW::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	//CRect rc;
	//m_teechart.Create(_T(""), WS_VISIBLE, CRect(0, 0, 0, 0), this, 1234);//teechrt create
	//m_teechart.Create(NULL, WS_CHILD | WS_VISIBLE, rc, this, 1000);
		//m_teechart.AddSeries(0);
	//m_teechart.Series(0).FillSampleValues(50);
	// TODO:  �ڴ������ר�õĴ�������
	
	return 0;
}


void CRIGHTTOPVIEW::OnBnClickedRadio2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_teechart.GetAspect().SetView3D(FALSE);
	m_teechart.Series(0).Clear();
	m_teechart.Series(1).Clear();
	//m_teechart.RemoveAllSeries();
	//m_teechart.AddSeries(0);
	//m_teechart.Series(0).SetColor(RGB(255, 0, 0));//��һ��ͼ��ɫ
	//m_teechart.GetLegend().SetVisible(FALSE);
	
	//m_teechart.Series(0).GetMarks().SetVisible(TRUE);
	
	if (bdu==1)
	{
		if (sc == 0)
		{
			//m_teechart.GetAxis().GetBottom().SetMinMax(0, 0.2);
			//CSeries data1 = (CSeries)m_teechart.Series(0);
			//	data1.Clear();
			//for (int i = 0; i < 100; i++)
			//if (alogrithm == 1)
			{
				_bstr_t bstrQuery("SELECT * FROM ged5");
				if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
				{
					MessageBoxW(_T("��ȡ����ʧ��"));

				}
				//else
				//MessageBoxW(_T("��ȡ���ݳɹ�"));
				MYSQL_RES *result;
				MYSQL_ROW row;
				if (!(result = mysql_use_result(&m_mysql)))
					MessageBoxW(_T("��ȡ���ʧ��"));
				//_variant_t vRecsAffected(0L);
				//	data1.AddXY(i, rand() / 100, NULL, NULL);
				if (m_teechart.GetSeriesCount() < 1)
				{
					m_teechart.AddSeries(6);
				}
				while (row = mysql_fetch_row(result))
				{
					double x, y;
					stringstream s, s1;
					s << row[0];
					s1 << row[1];
					s >> x;
					s1 >> y;
					// x = sscanf(row[0]," %f", &x);
					// y = sscanf(row[1], "%f", &y);
					// double x=atof(row[0]), y=atof(row[1]);
					m_teechart.Series(0).AddXY(x, y, NULL, RGB(255, 0, 0));
					//pRecordset->MoveNext();
				}
				mysql_free_result(result);
			}
			//	else
		{
			_bstr_t bstrQuery("SELECT * FROM gedir5");
			if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
			{
				MessageBoxW(_T("��ȡ����ʧ��"));

			}
			//else
			//MessageBoxW(_T("��ȡ���ݳɹ�"));
			MYSQL_RES *result;
			MYSQL_ROW row;
			if (!(result = mysql_use_result(&m_mysql)))
				MessageBoxW(_T("��ȡ���ʧ��"));
			//_variant_t vRecsAffected(0L);
			//	data1.AddXY(i, rand() / 100, NULL, NULL);
			if (m_teechart.GetSeriesCount() < 1)
			{
				m_teechart.AddSeries(6);
			}
			while (row = mysql_fetch_row(result))
			{
				double x, y;
				stringstream s, s1;
				s << row[0];
				s1 << row[2];
				s >> x;
				s1 >> y;
				// x = sscanf(row[0]," %f", &x);
				// y = sscanf(row[1], "%f", &y);
				// double x=atof(row[0]), y=atof(row[1]);
				m_teechart.Series(1).AddXY(x, y, NULL, RGB(0, 255, 0));
				//pRecordset->MoveNext();
			}
			mysql_free_result(result);
		}
		}

		else
			if (sc == 1)
			{
				//	if (alogrithm == 1)
				{
					_bstr_t bstrQuery("SELECT * FROM ged6");
					if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
					{
						MessageBoxW(_T("��ȡ����ʧ��"));

					}
					//else
					//MessageBoxW(_T("��ȡ���ݳɹ�"));
					MYSQL_RES *result;
					MYSQL_ROW row;
					if (!(result = mysql_use_result(&m_mysql)))
						MessageBoxW(_T("��ȡ���ʧ��"));
					//_variant_t vRecsAffected(0L);
					//	data1.AddXY(i, rand() / 100, NULL, NULL);
					if (m_teechart.GetSeriesCount() < 1)
					{
						m_teechart.AddSeries(6);
					}
					while (row = mysql_fetch_row(result))
					{
						double x, y;
						stringstream s, s1;
						s << row[0];
						s1 << row[1];
						s >> x;
						s1 >> y;
						// x = sscanf(row[0]," %f", &x);
						// y = sscanf(row[1], "%f", &y);
						// double x=atof(row[0]), y=atof(row[1]);
						m_teechart.Series(0).AddXY(x, y, NULL, RGB(255, 0, 0));
						//pRecordset->MoveNext();
					}
					mysql_free_result(result);
				}
				//	else
			{
				_bstr_t bstrQuery("SELECT * FROM gedir6");
				if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
				{
					MessageBoxW(_T("��ȡ����ʧ��"));

				}
				//else
				//MessageBoxW(_T("��ȡ���ݳɹ�"));
				MYSQL_RES *result;
				MYSQL_ROW row;
				if (!(result = mysql_use_result(&m_mysql)))
					MessageBoxW(_T("��ȡ���ʧ��"));
				//_variant_t vRecsAffected(0L);
				//	data1.AddXY(i, rand() / 100, NULL, NULL);
				if (m_teechart.GetSeriesCount() < 1)
				{
					m_teechart.AddSeries(6);
				}
				while (row = mysql_fetch_row(result))
				{
					double x, y;
					stringstream s, s1;
					s << row[0];
					s1 << row[2];
					s >> x;
					s1 >> y;
					// x = sscanf(row[0]," %f", &x);
					// y = sscanf(row[1], "%f", &y);
					// double x=atof(row[0]), y=atof(row[1]);
					m_teechart.Series(1).AddXY(x, y, NULL, RGB(0, 255, 0));
					//pRecordset->MoveNext();
				}
				mysql_free_result(result);
			}
			}
			else
			{
				//	if (alogrithm == 1)
				{
					_bstr_t bstrQuery("SELECT * FROM ged7");
					if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
					{
						MessageBoxW(_T("��ȡ����ʧ��"));

					}
					//else
					//MessageBoxW(_T("��ȡ���ݳɹ�"));
					MYSQL_RES *result;
					MYSQL_ROW row;
					if (!(result = mysql_use_result(&m_mysql)))
						MessageBoxW(_T("��ȡ���ʧ��"));
					//_variant_t vRecsAffected(0L);
					//	data1.AddXY(i, rand() / 100, NULL, NULL);
					if (m_teechart.GetSeriesCount() < 1)
					{
						m_teechart.AddSeries(6);
					}
					while (row = mysql_fetch_row(result))
					{
						double x, y;
						stringstream s, s1;
						s << row[0];
						s1 << row[1];
						s >> x;
						s1 >> y;
						// x = sscanf(row[0]," %f", &x);
						// y = sscanf(row[1], "%f", &y);
						// double x=atof(row[0]), y=atof(row[1]);
						m_teechart.Series(0).AddXY(x, y, NULL, RGB(255, 0, 0));
						//pRecordset->MoveNext();
					}
					mysql_free_result(result);
				}
				//	else
			{
				_bstr_t bstrQuery("SELECT * FROM gedir7");
				if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
				{
					MessageBoxW(_T("��ȡ����ʧ��"));

				}
				//else
				//MessageBoxW(_T("��ȡ���ݳɹ�"));
				MYSQL_RES *result;
				MYSQL_ROW row;
				if (!(result = mysql_use_result(&m_mysql)))
					MessageBoxW(_T("��ȡ���ʧ��"));
				//_variant_t vRecsAffected(0L);
				//	data1.AddXY(i, rand() / 100, NULL, NULL);
				if (m_teechart.GetSeriesCount() < 1)
				{
					m_teechart.AddSeries(6);
				}
				while (row = mysql_fetch_row(result))
				{
					double x, y;
					stringstream s, s1;
					s << row[0];
					s1 << row[2];
					s >> x;
					s1 >> y;
					// x = sscanf(row[0]," %f", &x);
					// y = sscanf(row[1], "%f", &y);
					// double x=atof(row[0]), y=atof(row[1]);
					m_teechart.Series(1).AddXY(x, y, NULL, RGB(0, 255, 0));
					//pRecordset->MoveNext();
				}
				mysql_free_result(result);
			}
			}
	}
	else
	{
		if (alogrithm == 1)
		{
			if (sc==0)
			{
				_bstr_t bstrQuery("SELECT * FROM ged5");
				if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
				{
					MessageBoxW(_T("��ȡ����ʧ��"));

				}
				//else
				//MessageBoxW(_T("��ȡ���ݳɹ�"));
				MYSQL_RES *result;
				MYSQL_ROW row;
				if (!(result = mysql_use_result(&m_mysql)))
					MessageBoxW(_T("��ȡ���ʧ��"));
				//_variant_t vRecsAffected(0L);
				//	data1.AddXY(i, rand() / 100, NULL, NULL);
				if (m_teechart.GetSeriesCount() < 1)
				{
					m_teechart.AddSeries(6);
				}
				while (row = mysql_fetch_row(result))
				{
					double x, y;
					stringstream s, s1;
					s << row[0];
					s1 << row[1];
					s >> x;
					s1 >> y;
					// x = sscanf(row[0]," %f", &x);
					// y = sscanf(row[1], "%f", &y);
					// double x=atof(row[0]), y=atof(row[1]);
					m_teechart.Series(0).AddXY(x, y, NULL, RGB(255, 0, 0));
					//pRecordset->MoveNext();
				}
				mysql_free_result(result);
			}
			else
				if (sc==1)
				{
					_bstr_t bstrQuery("SELECT * FROM ged6");
					if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
					{
						MessageBoxW(_T("��ȡ����ʧ��"));

					}
					//else
					//MessageBoxW(_T("��ȡ���ݳɹ�"));
					MYSQL_RES *result;
					MYSQL_ROW row;
					if (!(result = mysql_use_result(&m_mysql)))
						MessageBoxW(_T("��ȡ���ʧ��"));
					//_variant_t vRecsAffected(0L);
					//	data1.AddXY(i, rand() / 100, NULL, NULL);
					if (m_teechart.GetSeriesCount() < 1)
					{
						m_teechart.AddSeries(6);
					}
					while (row = mysql_fetch_row(result))
					{
						double x, y;
						stringstream s, s1;
						s << row[0];
						s1 << row[1];
						s >> x;
						s1 >> y;
						// x = sscanf(row[0]," %f", &x);
						// y = sscanf(row[1], "%f", &y);
						// double x=atof(row[0]), y=atof(row[1]);
						m_teechart.Series(0).AddXY(x, y, NULL, RGB(255, 0, 0));
						//pRecordset->MoveNext();
					}
					mysql_free_result(result);
				}
				else
				{
					_bstr_t bstrQuery("SELECT * FROM ged7");
					if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
					{
						MessageBoxW(_T("��ȡ����ʧ��"));

					}
					//else
					//MessageBoxW(_T("��ȡ���ݳɹ�"));
					MYSQL_RES *result;
					MYSQL_ROW row;
					if (!(result = mysql_use_result(&m_mysql)))
						MessageBoxW(_T("��ȡ���ʧ��"));
					//_variant_t vRecsAffected(0L);
					//	data1.AddXY(i, rand() / 100, NULL, NULL);
					if (m_teechart.GetSeriesCount() < 1)
					{
						m_teechart.AddSeries(6);
					}
					while (row = mysql_fetch_row(result))
					{
						double x, y;
						stringstream s, s1;
						s << row[0];
						s1 << row[1];
						s >> x;
						s1 >> y;
						// x = sscanf(row[0]," %f", &x);
						// y = sscanf(row[1], "%f", &y);
						// double x=atof(row[0]), y=atof(row[1]);
						m_teechart.Series(0).AddXY(x, y, NULL, RGB(255, 0, 0));
						//pRecordset->MoveNext();
					}
					mysql_free_result(result);
				}
		}
		else
			if (sc == 0)
			{
				_bstr_t bstrQuery("SELECT * FROM gedir5");
				if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
				{
					MessageBoxW(_T("��ȡ����ʧ��"));

				}
				//else
				//MessageBoxW(_T("��ȡ���ݳɹ�"));
				MYSQL_RES *result;
				MYSQL_ROW row;
				if (!(result = mysql_use_result(&m_mysql)))
					MessageBoxW(_T("��ȡ���ʧ��"));
				//_variant_t vRecsAffected(0L);
				//	data1.AddXY(i, rand() / 100, NULL, NULL);
				if (m_teechart.GetSeriesCount() < 1)
				{
					m_teechart.AddSeries(6);
				}
				while (row = mysql_fetch_row(result))
				{
					double x, y;
					stringstream s, s1;
					s << row[0];
					s1 << row[2];
					s >> x;
					s1 >> y;
					// x = sscanf(row[0]," %f", &x);
					// y = sscanf(row[1], "%f", &y);
					// double x=atof(row[0]), y=atof(row[1]);
					m_teechart.Series(1).AddXY(x, y, NULL, RGB(0, 255, 0));
					//pRecordset->MoveNext();
				}
				mysql_free_result(result);
			}
			else
				if (sc==1)
				{
					_bstr_t bstrQuery("SELECT * FROM gedir6");
					if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
					{
						MessageBoxW(_T("��ȡ����ʧ��"));

					}
					//else
					//MessageBoxW(_T("��ȡ���ݳɹ�"));
					MYSQL_RES *result;
					MYSQL_ROW row;
					if (!(result = mysql_use_result(&m_mysql)))
						MessageBoxW(_T("��ȡ���ʧ��"));
					//_variant_t vRecsAffected(0L);
					//	data1.AddXY(i, rand() / 100, NULL, NULL);
					if (m_teechart.GetSeriesCount() < 1)
					{
						m_teechart.AddSeries(6);
					}
					while (row = mysql_fetch_row(result))
					{
						double x, y;
						stringstream s, s1;
						s << row[0];
						s1 << row[2];
						s >> x;
						s1 >> y;
						// x = sscanf(row[0]," %f", &x);
						// y = sscanf(row[1], "%f", &y);
						// double x=atof(row[0]), y=atof(row[1]);
						m_teechart.Series(1).AddXY(x, y, NULL, RGB(0, 255, 0));
						//pRecordset->MoveNext();
					}
					mysql_free_result(result);
				}
				else
				{
					_bstr_t bstrQuery("SELECT * FROM gedir7");
					if (mysql_real_query(&m_mysql, bstrQuery, strlen(bstrQuery)))
					{
						MessageBoxW(_T("��ȡ����ʧ��"));

					}
					//else
					//MessageBoxW(_T("��ȡ���ݳɹ�"));
					MYSQL_RES *result;
					MYSQL_ROW row;
					if (!(result = mysql_use_result(&m_mysql)))
					{
						MessageBoxW(_T("��ȡ���ʧ��"));
						return;
					}
					//_variant_t vRecsAffected(0L);
					//	data1.AddXY(i, rand() / 100, NULL, NULL);
					if (m_teechart.GetSeriesCount() < 1)
					{
						m_teechart.AddSeries(6);
					}
					while (row = mysql_fetch_row(result))
					{
						double x, y;
						stringstream s, s1;
						s << row[0];
						s1 << row[2];
						s >> x;
						s1 >> y;
						// x = sscanf(row[0]," %f", &x);
						// y = sscanf(row[1], "%f", &y);
						// double x=atof(row[0]), y=atof(row[1]);
						m_teechart.Series(1).AddXY(x, y, NULL, RGB(0, 255, 0));
						//pRecordset->MoveNext();
					}
					mysql_free_result(result);
				}
	}
	//m_teechart.GetAspect().SetView3D(FALSE); //��2D��Ч����ʾ  
	//m_teechart.AddSeries(0); //���һ��Line����  
	//m_teechart.AddSeries(1); //���һ��Line����  
	//m_teechart.GetLegend().SetVisible(FALSE);
	//m_teechart.Series(0).FillSampleValues(50); //���һ��Line�������������  
	//m_teechart.Series(1).FillSampleValues(50); //��ڶ���Line�������������  

	
	//m_teechart.Series(0).SetTitle(_T("ʱ��")); //���õ�һ�����ߵ�ͼ������Ϊ ʱ��  
	//m_teechart.Series(1).SetTitle(_T("�ٶ�"));
	
}
	

	

