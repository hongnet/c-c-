#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMessageBox>
#include<iostream>
#include <future>
#include<windows.h>
#include <QDebug>
#include<QPixmap>
#include<QTimer>
#include<set>
#include <QTcpServer>
#include <QAbstractSocket>
#include<clientshow.h>
#include<map>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int Bind(std::string ip);
    QMessageBox *box;
    void getScreen(std::string ip);
    //创建socket
    SOCKET m_socket;
    std::set<QAbstractSocket*> client_sock;
    bool SetBlock(int sock,bool isblock);
    std::string imageData;
    QAbstractSocket* clientConnection;
    QTcpServer * server;
    QTimer *showPic ;
    qint32 dataSize;
    std::map<QAction*,ClientShow *> ac_CS;
    QMenu* clientMenu;
    QAbstractSocket* TemclientConnection;


private:
    Ui::MainWindow *ui;

signals:
    void getScreenPix(QPixmap pix);
};

#endif // MAINWINDOW_H
