#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h>
#include <QDateTime>
#include <QScreen>
#include<QDebug>
#include <QPixmap>
#include<QDataStream>
#include<QByteArray>
#include <iostream>
#include<future>
#include<QBuffer>
#include<QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    box= new QMessageBox(this);
    box->setModal(false);
    ui->setupUi(this);
    ConnectSocket();
    connect(this,&MainWindow::getScreenPix,this,[this](QPixmap pix){

        ui->screenPix->setPixmap(pix);
    });
    QTimer *showPic = new QTimer(this);
    connect(showPic,&QTimer::timeout,this,[this](){
          getSceenPic(this);
    });
    showPic->start(100);




}
//拷贝文件夹：
bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    qDebug() << "copyDirectoryFiles:" << fromDir << toDir;
    LOG_INFO("sour:%s,Process toDir:%s", fromDir.toStdString().c_str(), toDir.toStdString().c_str());
    if (!targetDir.exists()) {
        if (!targetDir.mkdir(targetDir.absolutePath())) {
            LOG_INFO("sour:%s,mkfailed", targetDir.absolutePath().toStdString().c_str());
            return false;
        }
    }
    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    LOG_INFO( "a copyDirectoryFiles:%d", fileInfoList.count());
    //bool m_firstRead = true;
// 	if (m_firstRead) {
// 		int isfileTMP = 0;
// 		qDebug() << "a copyDirectoryFiles:" << fileInfoList.count();
// 		foreach(QFileInfo fileInfo, fileInfoList) {
// 			if (fileInfo.isFile()) {
// 				isfileTMP++;
// 			}
// 		}
// 		m_total = fileInfoList.count() - 2 - isfileTMP; // 2为.和..
// 		m_value = 0;
// 		m_firstRead = false;
// 		qDebug() << "a copyDirectoryFiles:" << fileInfoList.count() << m_total << isfileTMP;
// 		emit sigCopyDirStation(m_value / m_total);
// 		if (m_value == m_total) {
// 			m_firstRead = true;
// 			emit sigCopyDirStation(1);
// 			emit sigCopyDirOver();
// 		}
// 	}
// 	else {
// 		m_value++;
// 		qDebug() << "a copyDirectoryFiles:" << m_value << m_total;
// 		emit sigCopyDirStation(m_value / m_total);
// 		if ((m_value / m_total == 1) || (m_value > m_total) || (m_value == m_total)) {
// 			m_firstRead = true;
// 			emit sigCopyDirOver();
// 		}
// 	}
    for(QFileInfo fileInfo:fileInfoList) {
        LOG_INFO("filename:%s", fileInfo.absoluteFilePath().toStdString().c_str());
        if (fileInfo.fileName() == "." || fileInfo.fileName() == "..") {
            continue;
        }
        if (fileInfo.isDir()) {
            if (!copyDirectoryFiles(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName()), coverFileIfExist)) {
                return false;
            }
        }
        else {
            if (coverFileIfExist && targetDir.exists(fileInfo.fileName())) {
                targetDir.remove(fileInfo.fileName());
            }
            /// 进行文件copy
            if (!QFile::copy(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName()))) {
                LOG_INFO("copy sour:%s,Process toDir:%s failed", fileInfo.filePath().toStdString().c_str(), targetDir.filePath(fileInfo.fileName()).toStdString().c_str());
                return false;
            }
        }
    }
    return true;
}
MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::ConnectSocket()
{
    std::async(std::launch::async,[this]()->bool{
        if(sktClient != INVALID_SOCKET)
        {
            closesocket(sktClient);
        }
    int ret;
    hostent *host=NULL;
    WSADATA wsadata;
        if (0 != (WSAStartup(MAKEWORD(2, 2), &wsadata)))
        {
            printf("初始化网络失败。\n");
        }
        if ((LOBYTE(wsadata.wVersion) != 2) || (HIBYTE(wsadata.wVersion) != 2))
            if (0 != (WSAStartup(MAKEWORD(2, 2), &wsadata)))
            {
                printf("网络版本错误。\n");
            }

    sktClient = socket(AF_INET,SOCK_STREAM,0);
    if(sktClient == INVALID_SOCKET)
    {
        //MessageBox("创建套接字失败");
                  box->setText(
                    "创建套接字失败");
                  box->show();
        return  false;
    }
    int TimeOut = 6000;
    if(::setsockopt(sktClient,SOL_SOCKET,SO_SNDTIMEO,(char*)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
    {
        //MessageBox("设置发送超时");
        box->setText(
          "设置发送超时");
        box->show();
    }
    unsigned long ul=1;
    //ret = ioctlsocket(sktClient,FIONBIO,(unsigned long*)&ul);
    if(ul==SOCKET_ERROR)
    {
        //AfxMessageBox("连接方式错误");
        box->setText(
          "连接方式错误");
        box->show();
    }
    //获取
    sockaddrin.sin_family=AF_INET;
    sockaddrin.sin_port=htons(8097);
    sockaddrin.sin_addr.s_addr=inet_addr("10.194.198.125");
    if(sockaddrin.sin_addr.s_addr==INADDR_NONE)
    {
        //AfxMessage("是否有此IP地址");
        box->setText(
          "连接方式错误");
        box->show();
    }
     ::connect(sktClient,(const struct sockaddr*)&sockaddrin,sizeof(sockaddrin));
//     struct timeval timeout;
//     fd_set r;
//     FD_ZERO(&r);
//     FD_SET(sktClient,&r);
//     timeout.tv_sec = 15;
//     timeout.tv_usec = 0;
//     ret=select(0,0,&r, 0,&timeout);
//     if(ret<=0)
//     {
//         closesocket(sktClient);
//         return FALSE;
//     }
//     //根据实际情况设置阻塞模式
//     unsigned long ul1 =0;
//     ret=ioctlsocket(sktClient,FIONBIO,(unsigned long*)&ul1);
//     if(ret == SOCKET_ERROR)
//     {
//         closesocket(sktClient);
//         //AfxMessage("连接方式设置错误");
//         box->setText(
//           "连接方式设置错误");
//         box->show();
//         return false;
//     }
//     char cFlag[20]="Server\0";
//     send(sktClient,cFlag ,20,0);
//     recv(sktClient,cFlag,20,0);
//    if(strcmp(cFlag,"Server\0"))
//    {
//        //AfxMessageBox("请确认远程计算机运行软件");
//        box->setText(
//          "连接方式设置错误");
//        box->show();
//        return false;
//    }

    return true;
    });
return true;

}

void MainWindow::getSceenPic(MainWindow* mW)
{
    QScreen *screen = QGuiApplication::primaryScreen();

        QString filePathName = "full-";

        filePathName += QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz");

        filePathName += ".jpg";
        QPixmap pix = screen->grabWindow(0);
        QImage img = pix.toImage();
        //pix.save(filePathName);
        QByteArray ba;
        //QPixmap p("image.png");
        QBuffer buffer(&ba);
           buffer.open(QIODevice::WriteOnly);
           bool bOk = img.save(&buffer,"PNG");
        std::string screenStr = ba.toStdString();
        QDataStream in(ba);
            //需要的有效长度为函数返回值，并非nSize
        //int m_nBuffSize = in.readRawData((char*)screenStr.data(),ba.size());
        int notSendLen =screenStr.size();
         //pix.loadFromData(QByteArray::fromStdString(screenStr));
         if(!pix.loadFromData(QByteArray::fromStdString(screenStr)))
         {
             box->setText("loadFromData failed");
             box->show();
         }
        qDebug()<<"screen Length:"<<notSendLen<<"\n";
        if(SOCKET_ERROR==send(sktClient,screenStr.c_str(),notSendLen,0))
        {
            ConnectSocket();
            qDebug()<<"send failed"<<endl;
        }
//        while(notSendLen>0)
//        {
//            char buf[1025];
//            memset(buf,0,1025);
//            if(notSendLen>1024)
//            {
//                strcpy_s(buf,1024,screenStr.data());
//                send(sktClient,buf ,1024,0);
//                screenStr = screenStr.substr(0,1024);
//            }else
//            {
//                strcpy_s(buf,sizeof(screenStr),screenStr.data());
//                send(sktClient,buf ,1024,0);
//            }

//        }
         //pix.save(filePathName);
         //ui->screenPix->setPixmap(pix);
         qDebug()<<"width height:"<<width()<<"**"<<height()<<"\n";
         pix=pix.scaled(ui->screenPix->width(), ui->screenPix->height(), Qt::KeepAspectRatio,Qt::SmoothTransformation);
         //emit mW->getScreenPix(pix);


        //ui->screenPix->setPixmap(pix);
//        if(!pix.save(filePathName, "jpg"))

//        {

//            qDebug()<<"save full screen failed";

//        }
//        else


}



