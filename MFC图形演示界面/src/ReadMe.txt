================================================================================
    MICROSOFT 基础类库 : testsystem1 项目概述
===============================================================================
运行效果：
1. 本软件中，登录注册模块是在软件初始化之前弹出的对话框，用户第一次使用
本系统需要先注册账号，注册信息将保存在 MySql 数据库中，然后用户需进行登录
操作，若账号不在数据库中，会提示账号不存在，用户需注册账号，若用户输入的
密码错误，会提示用户密码错误，只有登录成功用户才可进入系统。
![](https://raw.githubusercontent.com/hongnet/c-c-/master/MFC图形演示界面/img/毕设主界面.PNG)

2. 主界面
采用 MFC 单文档视图模式，软件主界面需要展示 4 个视图窗口，
因此需要进行 MFC 单文档的视图分割操作。 实 现 方 法 是 通过重载框架类
CMainFrame 的成员函数 OnCreateClient，使用 MFC 中的 CSplitterWnd 类，利用
CSplitterWnd 对象的 CreateStatic()和 CreateView()函数实现单文档窗口的分割，和窗
口的填充。 根据不同需要将窗口分为垂直或水平的多个分割窗口。如图 5.2 所示，
软件的主界面模块分割为四个小模块：参数设置模块，测试模型演示模块， 结果演
示模块，提示模块，这四个模块分别对应类:CFormView 和 CEditView。

![](https://raw.githubusercontent.com/hongnet/c-c-/master/MFC图形演示界面/img/毕设主界面.PNG)

3. 参数设置后通过TeeChart将mysql中大量数据显示为二维图或三维图
![](https://raw.githubusercontent.com/hongnet/c-c-/master/MFC图形演示界面/img/二维图.PNG)
![](https://raw.githubusercontent.com/hongnet/c-c-/master/MFC图形演示界面/img/三维图.PNG)

4. 点击主界面“数据比较”模块后可在主界面上弹出另一对话框，运行算法后比较两种算法最优解个数
![](https://raw.githubusercontent.com/hongnet/c-c-/master/MFC图形演示界面/img/数据比较界面.PNG)


CSeries.h CTChart.h
    主界面右上视图使用TeeChart控件绘制三维图与二维图所需头文件。

LOGINDLG.cpp LOGINDLG.h
    主界面登录对话框类
    
LEFTTOP.cpp LEFTTOP.h
    主界面左上视图类
    
leftdownv.cpp leftdownv.h
    主界面左下视图类
  
rightdownv.cpp rightdownv.h
    主界面右下信息提示框类
 
RIGHTTOPVIEW.cpp RIGHTTOPVIEW.h
    主界面右上显示三维图二维图类
    
datalist.cpp datalist.h
    点击“数据比较后弹出对话框类”
 
testsystem1.h
    这是应用程序的主头文件。
    其中包括其他项目特定的标头（包括 Resource.h），并声明 Ctestsystem1App 应用程序类。

testsystem1.cpp
    这是包含应用程序类 Ctestsystem1App 的主应用程序源文件。


对于主框架窗口：
    该项目包含一个标准的 MFC 接口。

MainFrm.h, MainFrm.cpp
    这些文件中包含框架类 CMainFrame，该类派生自
    CFrameWnd 并控制所有 SDI 框架功能。



/////////////////////////////////////////////////////////////////////////////
