// net_speed_up.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include"sstream"
#include"fstream"
#include"WinSock2.h"
#include"windows.h"
#include"atlstr.h"
#pragma comment(lib,"ws2_32.lib")
using namespace std;
std::ostringstream gethttp;
std::string m_host = "gs.xunyou.com";
SOCKET m_socket;
bool initnet()
{
	WSADATA wsadata;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsadata))
	{
		//MessageBoxW(nullptr,L"初始化网络失败",L"温馨提示",MB_ICONEXCLAMATION);
		//QMessageBox::information(NULL, "title", "no init", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return false;
	}
	if ((LOBYTE((wsadata.wVersion)) != 2) || (HIBYTE(wsadata.wVersion) != 2))
		if (0 != WSAStartup(MAKEWORD(2, 2), &wsadata))
			return false;
	return true;
}
bool sendgethttp(int accout_inum)
{
	/*POST http://gs.xunyou.com/index.php/memberpanel/speedUp HTTP/1.1*/
//Host: gs.xunyou.com
//User-Agent: okhttp/2.7.0
//Accept-Encoding: gzip, deflate
//Accept: */*
//Connection: Keep-Alive
//Content-Type: application/x-www-form-urlencoded
//Content-Length: 64

//username=10302257352079&password=18856321800&client_type=android
	ostringstream lenth_str;
	lenth_str<<"username=1030225735" << accout_inum << "&password=18856321800&client_type=android";
	gethttp << "POST " << "http://gs.xunyou.com/index.php/memberpanel/speedUp" << " HTTP/1.1\r\n";
	gethttp << "Host: " << m_host << "\r\n";
	gethttp << "User-Agent: okhttp/2.7.0\r\n";
	//gethttp << "Accept-Encoding: gzip, deflate\r\n";
	gethttp << "Accept: */*\r\n";
	gethttp << "Connection: Keep-Alive\r\n";
	gethttp << "Content-Type: application/x-www-form-urlencoded\r\n";
	gethttp << "Content-Length: "<<lenth_str.str().size()<<"\r\n";
	gethttp << "\r\n";
	gethttp << "username=1030225735" << accout_inum << "&password=18856321800&client_type=android\r\n";
	lenth_str.str("");
	return true;
}

string CSt2string(string str_tmp)
{
	CString str = str_tmp.c_str();
	//注意：以下n和len的值大小不同,n是按字符计算的，len是按字节计算的
	int n = str.GetLength(); // n = 14, len = 18

	//获取宽字节字符的大小，大小是按字节计算的
	int len = WideCharToMultiByte(CP_ACP, 0, str, str.GetLength(), NULL, 0, NULL, NULL);

	//为多字节字符数组申请空间，数组大小为按字节计算的宽字节字节大小
	char * buff = new char[len + 1]; //以字节为单位

	//宽字节编码转换成多字节编码
	WideCharToMultiByte(CP_ACP, 0, str, str.GetLength() + 1, buff, len + 1, NULL, NULL);

	buff[len + 1] = '\0'; //多字节字符以'/0'结束

	return buff;
}
int main()
{
	if (!initnet())
	{
		//QMessageBox::information(NULL, "title", "no init", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

		return 0;
	}
	struct protoent *ppe;
	ppe = getprotobyname("tcp");
	m_socket = socket(PF_INET, SOCK_STREAM, ppe->p_proto);
	sockaddr_in sock_addr;
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(80);

	cout << "start" << endl;
	static streambuf* str_tmp = cout.rdbuf();
	fstream pfile("D:\\Users\\xm\\Desktop\\1.txt", ios::in | ios::out | ios::binary);
	static streambuf* file_tmp = pfile.rdbuf();
	/*if (ui->url_edit->text() != "")
	{
		m_host = ui->url_edit->text().toStdString();
	}*/
	hostent *p = gethostbyname(m_host.c_str());
	if (p == nullptr)
	{
		//MessageBox(NULL,"host_name failed","title",MB_ICONEXCLAMATION);
		//QMessageBox::information(NULL, "title", "no host_name", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return 0;
	}

	memcpy(&sock_addr.sin_addr, p->h_addr, 4);
	//qDebug() << p->h_addr;
	if (SOCKET_ERROR == ::connect(m_socket, (sockaddr *)&sock_addr, sizeof(struct sockaddr_in)))
	{
	////	//MessageBoxW(nullptr,L"连接失败",L"温馨提示",MB_ICONEXCLAMATION);
	////	//QMessageBox::information(NULL, "title", "connect failed", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		std::cout << "connect failed" << endl;
		return 0;
	}

	for (int i = 0; i <= 10000; i++)
	{
		fstream pfile("D:\\Users\\xm\\Desktop\\1.txt", ios::in | ios::out | ios::binary);
		if (i == 10000)
			i = 0;
		gethttp.str("");
		sendgethttp(i);
		char tmp_str[10240]  = {0};
		//memset(tmp_st,0X0,10240);
		sprintf(tmp_str, gethttp.str().c_str(),gethttp.str().length());
		//qDebug() << (tmp_str + "\n\n").c_str();
		std::cout << tmp_str <<"\n\n" << strlen(tmp_str) << endl;

		if (SOCKET_ERROR == ::send(m_socket, tmp_str, strlen(tmp_str), 0))
		{
			cout << "send failed" << endl;
			closesocket(m_socket);
			m_socket = socket(AF_INET, SOCK_STREAM, 0);
			Sleep(2000);
			
			if (SOCKET_ERROR == ::connect(m_socket, (sockaddr *)&sock_addr, sizeof(struct sockaddr_in)))
			{
				//MessageBoxW(nullptr,L"连接失败",L"温馨提示",MB_ICONEXCLAMATION);
				//QMessageBox::information(NULL, "title", "connect failed", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
				std::cout << "connect failed" << endl;
				closesocket(m_socket);
				m_socket = socket(AF_INET, SOCK_STREAM, 0);
				Sleep(2000);
				continue;
			}
			continue;
		}
		//MessageBoxW(nullptr,L"发送数据失败",L"温馨提示",MB_ICONEXCLAMATION);
		//QMessageBox::information(NULL, "title", "send data failed", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	//一个字节一个字节的接收信息
	//gethttp.str("");
		std::string rec_string;
		char ch[2] = { 0 };
		int nrecv = 0;
		//FILE *pfile;
		std::ostringstream display_num;
		display_num << i;

		//pfile = fopen("D:\\Users\\xm\\Desktop\\1.txt", "w");
		while ((recv(m_socket, ch, sizeof(char), 0)))
		{
			rec_string += ch;
			if (strcmp("\r", ch) == 0)
				if (recv(m_socket, ch, sizeof(char), 0))
					if (0 == strcmp("\n", ch))
						if (recv(m_socket, ch, sizeof(char), 0))
							if (0 == strcmp("\r", ch))
								if (recv(m_socket, ch, sizeof(char), 0))
									if (0 == strcmp("\n", ch))
									{
										//qDebug() << "tuichu";
										break;
									}
			//rec_string+=buf;
			//qDebug()<<"write file";

			cout.rdbuf(file_tmp);
			//cout << ch;
			pfile.flush();
			//pfile.close();
			//fwrite(&ch, 1, 1024, pfile);


		}
		rec_string.clear();

		char buf[2] = { 0 };

		if (!pfile.is_open())
		{
			//qDebug() << "open failed";
			std::cout << "open failed" << endl;
		}
		memset(buf, 0x0, sizeof(buf));
		nrecv = recv(m_socket, buf, 1, 0);
		while (recv(m_socket, buf, 1, 0))
		{
			//Encoding.UTF8.GetString(recvBytes, 0, bytes);
			
			rec_string += buf;
		}
		cout.rdbuf(str_tmp);
		cout << "写入文件之前："<<display_num.str() << endl << CSt2string(rec_string)<<endl;
		cout.rdbuf(pfile.rdbuf());
		cout << display_num.str() << endl << CSt2string(rec_string);
		cout.rdbuf(str_tmp);
		cout << "写入文件之后：" << display_num.str() << endl << CSt2string(rec_string)<<endl;
		//fwrite((display_num.str()+"\n" + str_tmp.str()).c_str(), 1, 1024, pfile);
		//qDebug() << rec_string.c_str();
		//fclose(pfile);
		pfile.flush();


		//gethttp<<i;
		//ui->account_edit->setText(QString::fromStdString(display_num.str()));
		display_num.str("");
		pfile.close();
		//rec_string.clear();
		pfile.open("D:\\Users\\xm\\Desktop\\1.txt", ios::in);
		if (!pfile.is_open())
		{
			cout << "验证是否过期文件打开失败" << endl;
		}
		//while (getline(pfile, rec_string));
		//rec_string << pfile;
		//ui->result_text_edit->setText(QString::fromStdString(rec_string));
		cout.rdbuf(str_tmp);
		cout << CSt2string(rec_string) << rec_string.length() << endl;
		static int invalid_flag=0;
		if (rec_string.find(":-1") != string::npos)
			invalid_flag = 1;
		if (rec_string.find(":1") != string::npos)
			invalid_flag = 0;
		if (rec_string.find(":-1")!=string::npos||invalid_flag)
		{
			cout.rdbuf(str_tmp);
			cout << CSt2string(rec_string) << rec_string.length()<<endl;
			pfile.flush();
			pfile.close();
			rec_string.clear();
			
			std::cout << "此账号试用期到" << endl;

			Sleep(1000);
			continue;
		}
		rec_string.clear();
		pfile.close();
		cout.rdbuf(str_tmp);
		cout << "等待1分钟" << endl;
		Sleep(1000 * 60);
		//qDebug() << "sleep finish";


	}
	std::cout << "Hello World!\n";
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
