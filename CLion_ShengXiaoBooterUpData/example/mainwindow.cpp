/*
 * 主体页面布局
 * by elawidgettools
 * modify by shengxiao
 * v1.0.3 2024/10/30
*/

#include "mainwindow.h"
#include <QVBoxLayout>

//src功能引用
#include "ElaContentDialog.h"
#include "ElaText.h"
#include "ElaMenuBar.h"

//自建头文件
#include "UpUI.h"
#include "About.h"

MainWindow::MainWindow(QWidget* parent)
    : ElaWindow(parent)
{
    initWindow();

    //移动到中心
    moveToCenter();
}

void MainWindow::initWindow()
{
    setIsNavigationBarEnable(false);//关闭导航信息栏
    setIsStayTop(false);//关闭置顶按钮
    setUserInfoCardVisible(false);//关闭用户信息卡
    resize(781, 422);
    setWindowTitle("轻蓝_更新程序");//程序名

    //关于更新程序菜单
    _About =new About();
    ElaMenuBar* menuBar = new ElaMenuBar(this);
    menuBar->setFixedHeight(30);
    QWidget* customWidget = new QWidget(this);
    QVBoxLayout* customLayout = new QVBoxLayout(customWidget);
    customLayout->setContentsMargins(0, 0, 0, 0);
    customLayout->addWidget(menuBar);
    customLayout->addStretch();
    this->setCustomWidget(ElaAppBarType::RightArea, customWidget);
    this->setCustomWidgetMaximumWidth(125);
    /*
     * menuBar->addElaIconAction(ElaIconType::DesktopArrowDown, "关于更新程序");
     * 监听的是菜单栏中的某个菜单项被点击
     *QAction::triggered：当用户点击菜单项时，该信号会被触发，是Qt中处理菜单项点击的标准方式
     */
    QAction* aboutAction = menuBar->addElaIconAction(ElaIconType::DesktopArrowDown, "关于更新程序");
    connect(aboutAction, &QAction::triggered, this, [=]() {// 连接该菜单栏中关于更新程序菜单项的 triggered 信号
        _About->setFixedSize(500, 320);
        _About->moveToCenter();
        _About->show();
    });

    //主UI
    _UpUI = new UpUI(this);
    addPageNode("主UI", _UpUI, ElaIconType::House);
}

MainWindow::~MainWindow()
{

}