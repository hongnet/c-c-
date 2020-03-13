#include "mainwindow.h"
#include <QApplication>
#include <windows.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*1.0*/
//    HKEY hkey = {0};
//    wchar_t szPath[MAX_PATH] = L"E:\\ruanjian\\【2018-05-10】PotPlayer_1.7.11890_Dev\\PotPlayer\\PotPlayerMini.exe";
//    GetModuleFileName(nullptr,szPath,MAX_PATH);
//    RegOpenKeyEx(HKEY_LOCAL_MACHINE,L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_WRITE,&hkey);
//    if(ERROR_SUCCESS==RegSetValueEx(hkey,L"AutoDemo",0,REG_SZ,(BYTE*)szPath,MAX_PATH))
//    {
//            RegCloseKey(hkey);
//            return 0 ;
//}
//    RegCloseKey(hkey);
    /*1.1*/
    HANDLE hToken = nullptr;
    TOKEN_PRIVILEGES tkp={0};
    OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken);
    LookupPrivilegeValue(nullptr,SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken,FALSE,&tkp,sizeof(tkp),nullptr,nullptr);
    ExitWindowsEx(EWX_POWEROFF,0);
    MainWindow w;
    w.show();

    return a.exec();
}

