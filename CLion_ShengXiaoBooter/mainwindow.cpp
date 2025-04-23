/*
 * 主体页面布局
 * by elawidgettools
 * modify by shengxiao
 * v1.0 Created in 2024/10/07
 * v1.4 2025/04/12
*/

#include "mainwindow.h"

//QT功能
#include <QDebug>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QStatusBar>


//src功能引用
#include "ElaContentDialog.h"
#include "ElaStatusBar.h"
#include "ElaText.h"
#include "ElaMessageBar.h"

//自建头文件
#include "Downloads.h"          //下载中心
#include "AppCenter.h"          //app中心
#include "Setting.h"           //设置
#include "ExamplePage/Home.h"  //主页
#include "updatedown.h"  //更新
#include "Login.h"  //登录

//全局引用
extern int NETWORK; //引用全局网络检测变量；检测是否连接至服务器 1连接 0离线
extern int GX1;//引用全局检查更新变量
extern QString uname17;//用户名
extern QString useremail;//用户邮箱
void gxjc();//CNet.cpp中的检查更新函数
void installjc();//检查软件安装情况,在这里只在启动时检测一遍 Downloads.cpp


//正文
#ifdef Q_OS_WIN
#endif

/*
 * 构造函数：主窗口
 */
MainWindow::MainWindow(QWidget* parent)
    : ElaWindow(parent)
{
    gxjc();//检查更新与在线检测 注意:该函数包含本程序大部分启动参数，必须放在第一位 CNet.cpp
    installjc();//检查软件安装情况

    //初始化窗口
    initWindow();

    //初始化边缘布局
    initEdgeLayout();

    //初始化页面与组件
    initContent();

    // 连接登录信号---用户状态更新
    connect(_loginPage, &Login::userstatusUpdata,this, &MainWindow::onUserStatusUpdata);

    // 拦截默认关闭事件
    _closeDialog = new ElaContentDialog(this);
    connect(_closeDialog, &ElaContentDialog::rightButtonClicked, this, &MainWindow::closeWindow);
    connect(_closeDialog, &ElaContentDialog::middleButtonClicked, this, &MainWindow::showMinimized);
    this->setIsDefaultClosed(false);
    connect(this, &MainWindow::closeButtonClicked, this, [=]() {
        _closeDialog->exec();
    });

    //窗口移动到中心
    moveToCenter();
}

/*
 * 析构函数
 */
MainWindow::~MainWindow()
{
    
}

/*
 * 构造函数：用户状态更新
 */
void MainWindow::onUserStatusUpdata()
{
    ElaMessageBar::success(ElaMessageBarType::Top, "成功", "登陆成功", 3000, this);
    // 调用界面更新方法
    setUserInfoCardTitle(uname17);
    setUserInfoCardSubTitle(useremail);
}

/*
 * 构造函数：初始化窗口
 */
void MainWindow::initWindow()
{
    resize(1200, 800);
    setUserInfoCardPixmap(QPixmap(":/Resource/Image/Cirno.jpg"));//设置用户信息卡的头像
    setUserInfoCardTitle(uname17);//设置用户信息卡的用户名字--初始
    setUserInfoCardSubTitle(useremail);//设置用户信息卡的用户邮箱--初始
    setWindowTitle("轻蓝");//程序名
    setUserInfoCardVisible(true);//用户信息卡开关
}

/*
 * 构造函数：边缘布局
 */
void MainWindow::initEdgeLayout()
{
    if(NETWORK==0)
    {
        //状态栏
        ElaStatusBar* statusBar = new ElaStatusBar(this);

        //状态栏内文字
        ElaText* statusText = new ElaText("  离线  ", this);
        statusText->setTextPixelSize(14);
        statusText->setStyleSheet("color: red;");
        statusBar->addWidget(statusText);
        this->setStatusBar(statusBar);
    }
    if(GX1==0)
    {
        ElaMessageBar::warning(ElaMessageBarType::TopRight, "重要", "有新版本可用", 5000);//信息条 error

        //状态栏
        ElaStatusBar* updateBar =new ElaStatusBar(this);

        //状态栏内文字
        ElaText* updatetext =new ElaText("当前不是最新!  ",this);
        updatetext ->setTextPixelSize(14);
        updatetext ->setStyleSheet("color:red;");
        updateBar->addWidget(updatetext);
        this->setStatusBar(updateBar);
        _updPage = new updatedown();
        _updPage->setFixedSize(425, 390);
        _updPage->moveToCenter();
        _updPage->show();//打开更新窗口
    }
}

/*
 * 构造函数：初始化页面与组件
 */
void MainWindow::initContent()
{
    //页面对象
    _homePage = new Home(this);
    _downloadsPage =new Downloads(this);
    _settingPage = new Setting(this);
    _appPage = new AppCenter(this);
    _loginPage = new Login();

    //=================================================[添加页节点]=======================================
    addPageNode("主页", _homePage, ElaIconType::House);

    addPageNode("应用中心", _appPage, 0, ElaIconType::HardDrive);
    connect(this,&ElaWindow::navigationNodeClicked,this,[=](){
        _appPage->updateUI();
    });

    //=================================================[添加页脚节点]=======================================
    addFooterNode("下载中心", _downloadsPage, _downloadsKey,0,ElaIconType::DownToLine);
    connect(this, &ElaWindow::navigationNodeClicked, this, [=](ElaNavigationType::NavigationNodeType nodeType, QString nodeKey)
    {
        if (_downloadsKey == nodeKey)
        {
            _downloadsPage->updateContent();
        }
    });

    addFooterNode("设置", _settingPage, _settingKey, 0, ElaIconType::GearComplex);

    //单击用户卡进行跳转
    connect(this, &MainWindow::userInfoCardClicked, this, [=]() {
        _loginPage->setFixedSize(400, 350);
        _loginPage->moveToCenter();
        _loginPage->show();
    });
}

