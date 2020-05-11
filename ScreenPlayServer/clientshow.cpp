#include "clientshow.h"
#include "ui_clientshow.h"

ClientShow::ClientShow(QAbstractSocket* clientConnec,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientShow)
{
    ui->setupUi(this);
    clientConnection = clientConnec;
    this->setWindowTitle(clientConnection->peerAddress().toString());
    showPic= new QTimer(this);
    connect(showPic,&QTimer::timeout,this,[this](){
          getScreen("");
    });

}

ClientShow::~ClientShow()
{
    delete ui;
}

void ClientShow::showEvent(QShowEvent *event)
{
    qDebug()<<"show Event"<<endl;
    showPic->start(5);
    return QWidget::showEvent(event);
}
void ClientShow::hideEvent(QHideEvent *event)
{
    qDebug()<<"hide Event"<<endl;
    showPic->stop();
    return QWidget::hideEvent(event);
}

void ClientShow::getScreen(std::string ip)
{
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
        QImage pix;
        pix.loadFromData(array);//这个就是存进去的QImage了
        if(!pix.isNull())
       {
            pix=pix.scaled(ui->screenShow->size());
            ui->screenShow->setPixmap(QPixmap::fromImage(pix));
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

    int func_num;

    int nrecv;
    //std::string buf;
    char buf[1025];
    memset(buf,0,sizeof(buf));
    QPixmap pix;
    std::string perPix;
   for(auto a:client_sock)
   {
       clientConnection = a;
   }
}
