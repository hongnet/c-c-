#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <windows.h>
#include <QMessageBox>
#include<cstring>
namespace Ui {
class MainWindow;
}

typedef struct _MyFileInfo{
    bool InvalidDir;
    TCHAR szFileName[128];
    bool IsDirectory;
    bool Next;
}MyFileInfo,pMyFileInfo;

//屏幕信息类型
typedef struct _MyScreenInfo{
    UINT ScrX;                      //宽（像素）
    UINT ScrY;                      //高（像素）
    DWORD dwSize;                   //图像大小
}MyScreenInfo,*pMyScreenInfo;

//控制鼠标事件类型
typedef struct _MyMouseEvent{
  DWORD dwFlags;
  unsigned long dx;
  unsigned long dy;
  bool DoubleClick;

}MyMouseEvent,*pMyMouseEvent;

//IP数据类型
typedef struct _SearIP{
    bool active;
    bool Searched;
}SearIP,*pSearIP;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //CString srListIP;
    void DownloadFile();
    void ShowStatusDlg();
    //void DeleteTreeChildItem(HTREEITEM hit);
    void GetDriveInfo();
    void TestConnect();
    //void GetItPath(HTREEITEM hitem,char*path);

    struct sockaddr_in sockaddrin;
    char charReceive[300];
    char charMission[300];
    bool ConnectSocket();
    SOCKET sktClient;
      QMessageBox *box;
      void getSceenPic(MainWindow* mW);
    //CString strIP="127.0.0.1";
    //DWORD dwIP =inet_addr(strIP);
    //unsigned char *pIp=(unsigned char *)&dwIP;
    //m_IPAddr.SetAddress(*pIp,*(pIp+1),*(pIp+2),*(pIp+3));
private:
    Ui::MainWindow *ui;

signals:
    void getScreenPix(QPixmap pix);
};

#endif // MAINWINDOW_H
