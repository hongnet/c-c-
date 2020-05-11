#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<windows.h>
#include <QDebug>
#include<QPixmap>
#include<QTimer>
#include <stdlib.h>

//包头
struct PackageHeader
{
    //包头大小(sizeof(PackageHeader))
    unsigned int uTransPackageHdrSize;
    //当前包头的大小(sizeof(PackageHeader)+当前数据包长度)
    unsigned int uTransPackageSize;
    //数据的总大小
    unsigned int uDataSize;
    //数据被分成包的个数
    unsigned int uDataPackageNum;
    //数据包当前的帧号
    unsigned int uDataPackageCurrIndex;
    //数据包在整个数据中的偏移
    unsigned int uDataPackageOffset;
};
const int UDP_MAX_SIZE=1200;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    box = new QMessageBox(this);
    box->setModal(false);
    server = new QTcpServer(this);
    dataSize=0;
    //clientConnection=NULL;
    Bind("0.0.0.0");
    connect(this,&MainWindow::getScreenPix,this,[this](QPixmap pix){
         ui->screenPix->setPixmap(pix);
    });
    showPic= new QTimer(this);

    connect(showPic,&QTimer::timeout,this,[this](){
          getScreen("");
    });
    connect(server,&QTcpServer::newConnection,this,[this](){
       static int i = 0;
       TemclientConnection = ( QAbstractSocket*)server->nextPendingConnection();
       qDebug()<<"New Connect:"<<TemclientConnection->peerName();
       if(i==0)
        {
           clientConnection =TemclientConnection;
             this->setWindowTitle(clientConnection->peerAddress().toString());
           clientMenu=ui->menuBar->addMenu("switch");
        }
       if(TemclientConnection->peerAddress().toString() == clientConnection->peerAddress().toString())
       {
           clientConnection =TemclientConnection;
       }
       i=1;

        for(auto a:clientMenu->actions())
        {
            if(a->text()==TemclientConnection->peerAddress().toString())
            {
                ac_CS.find(a)->second->clientConnection = TemclientConnection;
                return;
            }
        }

            QAction* action =clientMenu->addAction(TemclientConnection->peerAddress().toString());
            ClientShow * clS = new ClientShow(TemclientConnection);
            ac_CS[action] =clS;
        connect(action,&QAction::triggered,this,[action,this](){
            ac_CS[action]->show();
//            clS->show();
        });
        getScreen("");
        showPic->start(10);
    });




}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::Bind(std::string ip)
{
//    //初始化网络
//        using namespace std;
//        WSADATA wsadata;

//        if (WSAStartup(MAKEWORD(2, 2), &wsadata))
//        {
//            printf("初始化失败,错误码是：%d\n", GetLastError());
//            return 0;
//        }
//        //检测当前电脑的socket最高版本
//        if (wsadata.wHighVersion < wsadata.wVersion)
//        {
//            printf("请求socket版本高于电脑版本，电脑版本为：%d,请求socket版本为：%d,,错误码是：%d\n",
//                    wsadata.wHighVersion, wsadata.wVersion, GetLastError());
//            return 0;
//        }


//        //TCP/IP协议用AF_INET，SOCK_STREAM表示TCPSOCK_DGRAM（UDP）、SOCK_RAW（原始socket）
//        if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
//        {
//            printf("创建socket失败,,错误码是：%d\n",
//                 GetLastError());
//            return 0;
//        }
//        SOCKADDR_IN m_addr;
//        m_addr.sin_family = AF_INET;
//        m_addr.sin_addr.S_un.S_addr =
//            //INADDR_ANY;
//            inet_addr(ip.c_str());
//        m_addr.sin_port = htons(8097);
//        SetBlock(m_socket,false);
//        //绑定本机IP
//        if (bind(m_socket, (LPSOCKADDR )&m_addr, sizeof(m_addr))==SOCKET_ERROR)
//        {
//            //char cstr[10];
//            //CString  cst;
//            //cst.Format("%d",1);

//            //sprintf(cstr, "%d", GetLastError());
//            //MessageBox(NULL,(LP)cstr,L"错误码是：\n",
//                //MB_OK);
//            //	return 0;
//        }
//        //启动监听
//        if (listen(m_socket, 5) == SOCKET_ERROR)
//        {
//            printf("启动监听失败,,错误码是：%d\n",
//                GetLastError());
//            return 0;
//        }

        qDebug()<<"Server start"<<endl;

            server->listen(QHostAddress::Any,8097);
//            connect(server,&QTcpServer::newConnection,this,[this](){
//                //showPic->start(10);
//                getScreen("");
//            });


}
void MainWindow::getScreen(std::string ip)
{

    //while(1)
    {
        SOCKADDR_IN c_addr;
        SOCKET clientSocket;
        int client_len = sizeof(c_addr);

        //if ((clientSocket=accept(m_socket, (LPSOCKADDR)&c_addr, &client_len)) != INVALID_SOCKET)
        if(clientConnection != NULL)
         {
            //qDebug()<<"IP:"<<inet_ntoa(c_addr.sin_addr)<<"上线了";
           //this->setWindowTitle("服务端");
            //printf("1.重启\n,"
             //   "2.关机\n"
             //   "3.注销\n");
            //SetBlock(clientSocket,false);
            struct timeval timeout;
             QByteArray array;
            while(clientConnection->waitForReadyRead(100)){
                    qDebug()<<"bytesAvailable"<<endl;
                    array.append((QByteArray)clientConnection->readAll());
            }
            qDebug()<<(clientConnection->peerAddress().toIPv4Address());
            qDebug()<<"recv length:"<<array.size()<<endl;
            QImage pix;
            pix.loadFromData(array);//这个就是存进去的QImage了
            if(!pix.isNull())
           {
                pix = pix.scaled(ui->screenPix->size());
                ui->screenPix->setPixmap(QPixmap::fromImage(pix));
            }

//                 fd_set r;
//                 FD_ZERO(&r);
//                 FD_SET(sktClient,&r);
//                 timeout.tv_sec = 15;
//                 timeout.tv_usec = 0;
//                 int ret=select(0,0,&r, 0,&timeout);
//                 if(ret<=0)
//                 {
//                     closesocket(sktClient);
//                     return FALSE;
//                 }
//            connect(clientConnection,&QAbstractSocket::readyRead,this,[clientConnection,this](){
//               qDebug()<<"accept a connection!"<<endl;
//                QDataStream in( clientConnection );
//                in.setVersion(QDataStream::Qt_5_2);
//                if( dataSize == 0 )//dataSize是一个自己设的qint32
//                {
//                if( clientConnection->bytesAvailable() < (sizeof(quint32)+sizeof(QString)) )
//                {
//                return;
//                }
//                in >> dataSize;//按顺序读出。先入先出顺序
//                 qDebug()<<"dataSize !"<<dataSize<<endl;
//                //in>>fileName;//fileName是设的QString
//                }
//                if( dataSize > qint32(clientConnection->bytesAvailable()))//图片比较小，让它存在clientSocket的缓存中，直到大小满足关系，即文件内容都传输完毕后才读取出来。
//                {
//                return;
//                }
//                QByteArray a;
//                in>>a;
//                QPixmap pix;
//                pix.loadFromData(a);//这个就是存进去的QImage了
//                static int i=0;
//                i++;
//                QString filename = "serverGetScreen";
//                filename = filename+QString::number(i)+".jpg";
//                pix.save(filename);
//                ui->screenPix->setPixmap(pix);
//                dataSize=0;//重置为0，等待下一次传输
//                   //connect(clientConnection,SIGNAL(readyRead()),this,SLOT(readClient()));
//                   QByteArray array;

//                   quint32 data_len = 0;
//                   quint32 len = 0xffffffff;
//                   //struct Head head;


//                   while(clientConnection->waitForReadyRead(5000)){
//                       qDebug()<<"bytesAvailable"<<endl;

//                       if(data_len == 0){
//                           QDataStream in(clientConnection);

//                           in >> len
//                           qDebug("datalen:%d",len);
//                       }
//                       data_len += clientConnection->bytesAvailable();
//                       array.append((QByteArray)clientConnection->readAll());
//                       if(data_len == len){
//                           qDebug()<<"ok,all bytes recvd"<<endl;
//                           qDebug("recvd len:%d",data_len);

//                           QPixmap picture;//method 1
//                           picture.loadFromData(array,"jpeg");
//                            ui->screenPix->setPixmap(picture);
//                            data_len=0;
//                            array.clear();
//                           //break;
//                       }
//                   }
//            });
            client_sock.insert(clientConnection);
        }

//        int func_num;

//        int nrecv;
//        //std::string buf;
//        char buf[1025];
//        memset(buf,0,sizeof(buf));
//        QPixmap pix;
//        std::string perPix;
//       for(auto a:client_sock)
//       {
//           clientConnection = a;
//       }

//          qDebug("recvd len:%d",data_len);

//          QPixmap picture;//method 1
//          picture.loadFromData(array,"jpeg");
          //ui->pic->setPixmap(picture);
          //picture.save("teacher.jpg","jpg",-1);
//       static int num = 1;
//       static uint size = 0;
//        while ((nrecv = recv(clientSocket, buf, 1024, 0)) > 0)
//            {
//            perPix+=buf;
//            memset(buf,0,sizeof(buf));
//            }
//        static int i =0;
//        i++;
//        if(perPix.size()==0)
//           return;
//        PackageHeader *packageHead = (PackageHeader *)perPix.data();
//        if (packageHead->uDataPackageCurrIndex == num)
//        {
//            num++;
//            size += packageHead->uTransPackageSize-packageHead->uTransPackageHdrSize;
//            if (size > 1024*1000)
//            {
//                qDebug()<<"image too big";
//                num = 1;
//                size = 0;
//                return;
//            }
//            if (packageHead->uDataPackageOffset > 1024*1000)
//            {
//                qDebug()<<"image too big";
//                packageHead->uDataPackageOffset = 0;
//                num = 1;
//                size = 0;
//                return;
//            }

//            memcpy(imageData.data()+packageHead->uDataPackageOffset, perPix.data()+packageHead->uTransPackageHdrSize,
//                   packageHead->uTransPackageSize-packageHead->uTransPackageHdrSize);
//            if ((packageHead->uDataPackageNum == packageHead->uDataPackageCurrIndex)
//                    && (size == packageHead->uDataSize))
//            {
//                imageData.length = packageHead->uDataSize;

//                QImage image;
//                image.loadFromData((uchar *)imageData.data,imageData.length,"JPG");
//                QPixmap pixmap=QPixmap::fromImage(image);
//                ui->labelImage_2->setPixmap(pixmap);
//                recvImageNum++;
//                ui->lineEditRevFrame->setText(QString::number(recvImageNum));
//                ui->lineEditRevSize->setText(QString::number(imageData.length));
//                memset(&imageData,0,sizeof(UdpUnpackData));
//                num = 1;
//                size = 0;
//            }
//        }
//        else
//        {
//            num = 1;
//            size = 0;
//            memset(&imageData,0,sizeof(UdpUnpackData));
//        }
//        qDebug()<<"screen Length:"<<perPix.size()<<"\n";
//        QString filename = "serverGetScreen";
//        filename = filename+QString::number(i)+".jpg";

//        if(!pix.loadFromData(QByteArray::fromStdString(perPix)))
//        {
//            box->setText("loadFromData failed");
//            box->show();
//            box->hide();
//        }
//        pix.save(filename);
        //emit getScreenPix(pix);

        //cin >> func_num;
        //ostringstream os_t;
        //os_t << func_num;
        //send(client_sock, os_t.str(), sizeof(int), 0)
    }
}

bool MainWindow::SetBlock(int sock,bool isblock)
{
    int re = 0;
//通过宏区分windows和linux，如果是windows64位程序判断 _WIN64宏
#ifdef WIN32
    unsigned long ul = 0;
    if(!isblock) ul = 1;
    re = ioctlsocket(sock, FIONBIO, (unsigned long*)&ul);
#else
    //先取到现有描述符属性，保证本次更改不变动原有属性
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags < 0) {
        return false;
    }
    if(isblock)
    {
        flags = flags & ~O_NONBLOCK;
    }
    else
    {
        flags = flags | O_NONBLOCK;
    }
    re = fcntl(sock, F_SETFL, flags);
#endif
    if (re != 0) return false;
    return true;
}
