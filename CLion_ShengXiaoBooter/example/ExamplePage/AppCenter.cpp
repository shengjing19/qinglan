/*
 * 应用中心页面布局
 * by shengxiao
 * v1.0 Created in 2024/09/30
 * v1.3 2024/10/15
*/

#include "AppCenter.h"

//QT功能
#include <QVBoxLayout>
#include <QDir>
#include <QCoreApplication>
#include <QString>

//src功能引用
#include "ElaText.h"

//全局引用
extern int USH1;//Uxiaoyuan app下载检测 1已安装 0未安装
extern int SXZD1;//sxt app下载检测 1已安装 0未安装
extern int uninstall;//卸载控制 1U校园时长 2笙箫终端
void installjc();//检查软件安装情况

/*
 * 构造函数
 */
AppCenter::AppCenter(QWidget* parent)
    : T_BasePage(parent)
{
    // 调用初始化UI的函数
    initializeUIA();
}

// 初始化UI的函数
void AppCenter::initializeUIA()
{

    // 标题
    setWindowTitle("App Center");

    //提示语
    ElaText* Tips_Text = new ElaText(this);
    Tips_Text->setText("这里面包含了您已下载的app，您可以进行打开与卸载操作,\n注意：卸载完成后请手动刷新页面\n如果卸载出错,请检查程序是否在运行或使用管理员权限运行启动器");
    Tips_Text->setTextInteractionFlags(Qt::TextSelectableByMouse);
    Tips_Text->setTextPixelSize(14);

    //界面刷新按钮
    refreshButton = new ElaPushButton("页面刷新", this);
    refreshButton->setFixedWidth(100);//修改宽度190

    connect(refreshButton, &ElaPushButton::clicked, this,[=](){ // 连接刷新按钮的点击信号到刷新数据的槽函数
        updateUI();
    });

    //App列表矩形区域上方的提示文字
    ElaText* upText = new ElaText("App列表",this);
    upText->setWordWrap(false);
    upText->setTextPixelSize(18);

    // 创建App列表矩形区域
    Application_List_Area = new ElaScrollPageArea(this);
    Application_List_Area_Layout = new QHBoxLayout(Application_List_Area);//矩形区域内部使用平行布局

    //================================================[App1|U校园自助20小时时长]=======================================
    //创建区域内部文字_App1|U校园自助20小时时长
    App1_Name_Text = new ElaText("U校园自助20小时时长", this);
    App1_Name_Text->setWordWrap(false);
    App1_Name_Text->setTextPixelSize(16);
    Application_List_Area_Layout->addWidget(App1_Name_Text);

    // 创建区域内部打开按钮_App1|U校园自助20小时时长=============打开按钮与其逻辑
    Open_Button = new ElaPushButton("打开", this);
    Open_Button->setFixedWidth(190);
    connect(Open_Button, &ElaPushButton::clicked, this, [=](){
         QString appDir = QCoreApplication::applicationDirPath();
         QString exePath = QDir(appDir).filePath("USH.exe");
         //system(exePath.toStdString().c_str());
         SystemThread *thread = new SystemThread(exePath);
         thread->start();
    });

    // 创建区域内部卸载按钮_App1|U校园自助20小时时长=============卸载按钮与其逻辑
    Uninstall_Button = new ElaPushButton("卸载", this);
    Uninstall_Button->setFixedWidth(190);

    _unstalldPage = new unstalltip();
    connect(Uninstall_Button, &ElaPushButton::clicked, this, [=]()
    {
        uninstall=1;
        _unstalldPage->setFixedSize(350, 150);
        _unstalldPage->moveToCenter();
        _unstalldPage->show();
    });

    //将按钮加入到矩形区域内部
    Application_List_Area_Layout->addWidget(Open_Button);
    Application_List_Area_Layout->addWidget(Uninstall_Button);

    //===================================================[App2|笙箫终端]=======================================

    Application_List_Area2 = new ElaScrollPageArea(this);
    Application_List_Area_Layout =new QHBoxLayout(Application_List_Area2);

    //创建区域2内部文字_App2|笙箫终端
    App2_Name_Text = new ElaText("笙箫终端",this);
    App2_Name_Text->setWordWrap(false);
    App2_Name_Text->setTextPixelSize(16);
    Application_List_Area_Layout->addWidget(App2_Name_Text);

    // 创建区域内部打开按钮_App2|笙箫终端=============打开按钮与其逻辑
    app2_Open_Button = new ElaPushButton("打开",this);
    app2_Open_Button->setFixedWidth(190);
    connect(app2_Open_Button,&ElaPushButton::clicked,this,[=](){
        QString appDir = QCoreApplication::applicationDirPath();
        QString exePath = QDir(appDir).filePath("SXTeminal.exe");
        SystemThread *thread = new SystemThread(exePath);
        thread->start();

    });

    // 创建区域内部卸载按钮_App2|笙箫终端=============卸载按钮与其逻辑
    app2_Uninstall_Button = new ElaPushButton("卸载",this);
    app2_Uninstall_Button->setFixedWidth(190);
    connect(app2_Uninstall_Button,&ElaPushButton::clicked,this,[=](){
        uninstall=2;
        _unstalldPage->setFixedSize(350, 150);
        _unstalldPage->moveToCenter();
        _unstalldPage->show();

    });

    //将按钮加入到矩形区域内部
    Application_List_Area_Layout->addWidget(app2_Open_Button);
    Application_List_Area_Layout->addWidget(app2_Uninstall_Button);

    //====================================================[页面总布局]==================================================================
    // 定义父容器
    QWidget* Apptitle = new QWidget(this);
    Apptitle->setWindowTitle("应用中心");
    addCentralWidget(Apptitle, true, true, 0);

    // 创建一个垂直布局（总布局）并将其应用于 Apptitle
    QVBoxLayout* AppLayout = new QVBoxLayout(Apptitle);
    AppLayout->addWidget(Tips_Text);     //应用中心提示文本
    AppLayout->addWidget(refreshButton); //页面刷新按钮
    AppLayout->addSpacing(30); // 间隔10
    AppLayout->addWidget(upText); // 矩形区域上方提示文字
    AppLayout->addWidget(Application_List_Area); //应用程序列表区域1
    AppLayout->addWidget(Application_List_Area2);//应用程序列表区域2
    AppLayout->addStretch();

}

/*
 * UI实时更新函数
 * USH1 U校园自助20小时时长 安装检测参数 1:已安装 0:未安装
 * SXZD1 笙箫终端 安装检测参数 1:已安装 0:未安装
 * 该控件显示逻辑仅仅适用于5个App以下使用，如果大量App的话那就只能重写
 */
void AppCenter::updateUI()
{
    installjc();
    //======================================[U校园自助20小时时长 安装检测]=================================
    if(USH1==1)
    {
        //如果已安装
        App1_Name_Text->setText("U校园自助20小时时长");

        //按钮显示--这里为什么要有按钮显示的代码，请在下方看[如果双方都不存在]时的注释
        Open_Button->show();
        Uninstall_Button->show();

        //如果U校园被卸载，现在已经存在App2,要重新安装App1,则将App1的控件全部显示
        if(SXZD1==1)
        {
            /*
             * 注意:这里为什么要执行APP1的显示
             * 是因为如果App1被卸载掉，其所有控件全部被隐藏[else if(USH1==0)]，且现在已经存在App2，又要再重新安装App1时，则将其控件重新显示回来
             * App1的控件默认是全部显示的，就算没有安装也要显示“无应用,请前往主页下载”（仅所有APP都未安装）
             *
             * 这里的代码解决 轻蓝 在运行的状态下 将App1卸载 如果已经存在App2，再将App1下载回来时，会导致App1不显示,只能重启软件才显示的问题
             */
            //直接显示区域---所有的控件都会被显示
            Application_List_Area->show();
        }
    }
    else if(USH1==0&&SXZD1==1)
    {
        //如果U校园不存在但笙箫终端存在
        //将 U校园的布局隐藏
        //以便让笙箫终端的布局置顶
        //直接隐藏区域---所有的控件都会被隐藏
        Application_List_Area->hide();//app列表矩形框1_U校园--隐藏
    }

    //====================================[笙箫终端 安装检测]=================================
    if(SXZD1==1)
    {
        //如果已安装 App2所有控件全部显示
        App2_Name_Text->setText("笙箫终端");
        Application_List_Area2->show();
    }
    else if(SXZD1==0)
    {
        //如果未安装 App2所有控件全部不显示
        Application_List_Area2->hide();
    }

    //====================================[如果双方都不存在]=================================
    if(SXZD1==0&&USH1==0)
    {
        //如果未安装
        App1_Name_Text->setText("无应用,请前往主页下载");

        /*
         * 按钮隐藏
         * 这里不用Application_List_Area->hide()的原因是，如果俩应用都没下载，则必须要留一个矩形区域来显示"无应用,请前往主页下载",且不能显示按钮
         * Q:这里为什么把按钮隐藏了之后，下面Application_List_Area->show();显示的时候，却不显示
         * A:这是因为按钮是加入在矩形区域内的，关闭矩形区域只是将其显示的总出口关闭，而不是真正的关闭按钮的显示，
         * 所以在单独给其关闭后，即使将矩形区域显示，它也不会显示，除非再将按钮重新开放，
         * 这也是上方为什么会有Open_Button->show(); 的原因。
         *
         * Tips:App1的矩形框不光要包含App1的显示，也要顾及到"无应用,请前往主页下载"的显示，所以其逻辑比较绕
         */
        Open_Button->hide();
        Uninstall_Button->hide();

        //这里显示的原因是:在U校园不存在但笙箫终端存在[USH1==0&&SXZD1==1]的判断逻辑中会关闭app列表矩形框1的显示
        //也就是说当，卸载App2的时候App1的框架在本次运行中将不会再显示
        //所以加一个显示来确保，App1的正常显示
        Application_List_Area->show();
    }

    Application_List_Area->updateGeometry();
    Application_List_Area2->updateGeometry();
}

/*
 * 析构函数
 */
AppCenter::~AppCenter()
{
}
