/*
 * 设置页面布局
 * by shengxiao
 * v1.0 Created in 2024/09/30
 * v1.4 2025/04/18
*/

#include "Setting.h"

//QT功能
#include <QHBoxLayout>
#include <QFile>

//src功能引用.h
#include "ElaComboBox.h"
#include "ElaScrollPageArea.h"
#include "ElaText.h"
#include "ElaPushButton.h"
#include "ElaTheme.h"
#include "ElaMessageBar.h"

//自建头文件
#include "updatedown.h"//更新下载页面

//全局引用
extern int GX1;     //更新检查控制 1最新 0不是最新
extern int NETWORK; //引用全局网络检测变量；检测是否连接至服务器 1连接 0离线
extern std::string lver; //本地版本
extern std::string build;//构建号

/*
 * 构造函数：设置页面布局与部分设置逻辑
 */
Setting::Setting(QWidget* parent)
    : T_BasePage(parent)
{
    // 窗口标题
    setWindowTitle("Setting");

    //================================================[设置一|关于我们]====================================================================
    //关于我们矩形区域上方标题_About_Us_Text
    ElaText* About_Us_Text = new ElaText("关于我们", this);
    About_Us_Text->setWordWrap(false);
    About_Us_Text->setTextPixelSize(18);

    //设置关于我们矩形区域_aboutSwitchArea
    ElaScrollPageArea* aboutSwitchArea = new ElaScrollPageArea(this);
    QHBoxLayout* aboutSwitchLayout = new QHBoxLayout(aboutSwitchArea);//创建关于我们矩形区域的平行布局
    aboutSwitchArea->setFixedHeight(96);//设置矩形高度

    //关于我们矩形区域内容一字符串整合
    std::string aboutl="\n    轻蓝 ";
    std::string aboutl2="\n    (C)2020-2025 笙箫旧景 All rights reserved\n";
    std::string aboutlv=(std::string)aboutl + (std::string)lver + (std::string)build + (std::string)aboutl2;

    //关于我们矩形区域内容一
    ElaText* aboutSwitchText = new ElaText(aboutlv.c_str(), this);
    aboutSwitchText->setWordWrap(false);
    aboutSwitchText->setAlignment(Qt::AlignLeft);//设置文本对齐方式 顶部
    aboutSwitchText->setTextPixelSize(15);//设置字号
    aboutSwitchLayout->addWidget(aboutSwitchText);//将该文字放到aboutSwitchLayout平行布局中

    //关于我们矩形区域内容二
    ElaText* aboutSwitchText2 = new ElaText("             轻蓝是根据 GPLv3 许可协议开放许可的\n             本程序UI控件依于开源程序ElaWidgeTools", this);
    aboutSwitchText2->setWordWrap(false);
    aboutSwitchText2->setTextPixelSize(13);
    aboutSwitchLayout->addWidget(aboutSwitchText2);//将该文字放到aboutSwitchLayout平行布局中

    //================================================[设置二|检查更新]====================================================================
    //检查更新矩形区域上方标题_Check_for_Updates_Text
    ElaText* Check_for_Updates_Text = new ElaText("检查更新",this);
    Check_for_Updates_Text->setWordWrap(false);
    Check_for_Updates_Text->setTextPixelSize(18);

    //设置检查更新矩形区域_CFUSwitchArea
    ElaScrollPageArea* CFUSwitchArea = new ElaScrollPageArea(this);
    QHBoxLayout* CFUSwitchLayout = new QHBoxLayout(CFUSwitchArea); //创建检查更新矩形区域的平行布局

    //检查更新矩形区域内容一字符串整合
    std::string llver="版本:";
    std::string lv=(std::string)llver + (std::string)lver;

    //检查更新矩形区域文本内容
    ElaText* upSwitchText = new ElaText(lv.c_str(), this);
    upSwitchText->setWordWrap(false);
    upSwitchText->setTextPixelSize(15);
    CFUSwitchLayout->addWidget(upSwitchText);

    //检查更新矩形区域 检查更新按钮
    ElaPushButton* upButton = new ElaPushButton("检查更新", this);
    upButton->setFixedWidth(80);
    CFUSwitchLayout->addWidget(upButton);

    /*
     * 检查更新逻辑实现
     * 引用更新页面窗口
     * 从Configx.cpp文件读取通过参数初始化函数gxjc();初始的值来判断 是否连接服务器与是否是最新版本
     * 存在问题:由于参数初始化函数gxjc();只在启动时运行一次，如果离线的话不能第一时间连接至服务器只能重新打开重新获取参数
     */
    _updPage = new updatedown();
    connect(upButton, &ElaPushButton::clicked, this, [=]()
    {
        if(NETWORK==0)
        {
            ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", "未连接至服务器", 2000);//信息条
        }
        else
        {
            if(GX1==1)
            {
                ElaMessageBar::success(ElaMessageBarType::TopRight, "最新", "已是最新版本!", 2000);//信息条
                _updPage->setFixedSize(425, 390);
                _updPage->moveToCenter();
                _updPage->show();//打开更新页面窗口
            }
            else
            {
                ElaMessageBar::warning(ElaMessageBarType::TopRight, "不是最新", "请下载更新!", 2000);//信息条
                _updPage->setFixedSize(425, 390);
                _updPage->moveToCenter();
                _updPage->show();//打开更新页面窗口
            }
        }

    });

    //=====================检查更新_子设置[更新包删除]==============================
    //设置检查更新矩形区域_CFUSwitchArea
    ElaScrollPageArea* UPDelSwitchArea = new ElaScrollPageArea(this);
    QHBoxLayout* UPDelSwitchLayout = new QHBoxLayout(UPDelSwitchArea); //创建更新包删除矩形区域的平行布局

    ElaText* UPDel_Text =new ElaText("删除更新包",this);
    UPDel_Text->setTextPixelSize(15);
    UPDel_Text->setWordWrap(false);
    UPDelSwitchLayout->addWidget(UPDel_Text);

    ElaPushButton* UPDel_Button =new ElaPushButton("检索并删除",this);
    UPDel_Button->setFixedWidth(89);
    UPDelSwitchLayout->addWidget(UPDel_Button);

    //执行逻辑
    connect(UPDel_Button,&ElaPushButton::clicked,this,[=] (){
        QString exepath = "C://up.exe";
        QFile file(exepath);
        if(file.exists())
        {
            ElaMessageBar::success(ElaMessageBarType::Top,"成功","发现更新包",2000);
        }
        else
        {
            ElaMessageBar::error(ElaMessageBarType::Top,"错误","未发现更新包,可能已被删除",2500);
            return;
        }

        if(file.remove())
        {
            ElaMessageBar::success(ElaMessageBarType::Top,"成功","更新包已删除",2000);
        }
        else
        {
            ElaMessageBar::error(ElaMessageBarType::Top,"错误","删除出错,请手动前往查看(C://up.exe)",2500);
        }

    });


    //================================================[设置三|应用程序设置]====================================================================
    //应用程序设置矩形区域上方标题_themeText
    ElaText* Application_Settings_Text = new ElaText("应用程序设置", this);
    Application_Settings_Text->setWordWrap(false);
    Application_Settings_Text->setTextPixelSize(18);

    //=====================应用程序设置_子设置[主题切换theme]==============================
    //ElaComboBox 私有全局
    _themeComboBox = new ElaComboBox(this);
    _themeComboBox->addItem("日间模式");
    _themeComboBox->addItem("夜间模式");

    //设置主题切换矩形区域_themeSwitchArea
    ElaScrollPageArea* themeSwitchArea = new ElaScrollPageArea(this);
    QHBoxLayout* themeSwitchLayout = new QHBoxLayout(themeSwitchArea); //创建主题切换矩形区域的平行布局

    //主题切换矩形区域文本内容
    ElaText* themeSwitchText = new ElaText("主题切换", this);
    themeSwitchText->setWordWrap(false);
    themeSwitchText->setTextPixelSize(15);

    themeSwitchLayout->addWidget(themeSwitchText); //主题切换
    themeSwitchLayout->addStretch();
    themeSwitchLayout->addWidget(_themeComboBox); //下拉选项

    //日间模式
    connect(_themeComboBox, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, [=](int index) {
        if (index == 0)
        {
            eTheme->setThemeMode(ElaThemeType::Light);
        }
        else
        {
            eTheme->setThemeMode(ElaThemeType::Dark);
        }
    });

    //暗夜模式
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeComboBox->blockSignals(true);
        if (themeMode == ElaThemeType::Light)
        {
            _themeComboBox->setCurrentIndex(0);
        }
        else
        {
            _themeComboBox->setCurrentIndex(1);
        }
        _themeComboBox->blockSignals(false);
    });

    //=====================应用程序设置_子设置[云母设置mica]==============================
    /*
    eApp->setIsEnableMica(true);
    _micaSwitchButton = new ElaToggleSwitch(this);
    ElaScrollPageArea* micaSwitchArea = new ElaScrollPageArea(this);
    QHBoxLayout* micaSwitchLayout = new QHBoxLayout(micaSwitchArea);
    ElaText* micaSwitchText = new ElaText("云母效果", this);
    micaSwitchText->setWordWrap(false);
    micaSwitchText->setTextPixelSize(15);
    micaSwitchLayout->addWidget(micaSwitchText);
    micaSwitchLayout->addStretch();
    micaSwitchLayout->addWidget(_micaSwitchButton);
    connect(_micaSwitchButton, &ElaToggleSwitch::toggled, this, [=]( ) {
        bool checked
        eApp->setIsEnableMica(true);
    });
    */


    //====================================================[页面总布局]==================================================================
    //创建垂直布局 ----主布局
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addSpacing(30);                 //间隔30
    mainLayout->addWidget(About_Us_Text);              //关于我们
    mainLayout->addWidget(aboutSwitchArea);            //关于我们矩形区域
    mainLayout->addSpacing(10);                //间隔10
    mainLayout->addWidget(Check_for_Updates_Text);    //检查更新
    mainLayout->addWidget(CFUSwitchArea);             //检查更新矩形区域
    mainLayout->addWidget(UPDelSwitchArea);           //更新包删除矩形区域
    mainLayout->addSpacing(10);               //间隔10
    mainLayout->addWidget(Application_Settings_Text);//应用程序设置
    mainLayout->addWidget(themeSwitchArea);          //应用程序设置_子设置_主题切换矩形区域
    //mainLayout->addWidget(micaSwitchArea);         //应用程序设置_子设置_云母矩形区域
    mainLayout->addSpacing(10);               //间隔10
    mainLayout->addStretch();
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // 创建父容器
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);          //应用主布局到父容器
    centralWidget->setWindowTitle("Setting");
    addCentralWidget(centralWidget, true, true, 0);
}

/*
 * 析构函数
 */
Setting::~Setting()
{
}
