/*
 * 更新窗口布局
 * by shengxiao
 * v1.0 Created in 2024/10/06
 * v1.2 2025/04/12
*/

#include "updatedown.h"

//QT功能
#include <curl/curl.h>
#include <QDir>
#include <QProcess>
#include <QCoreApplication>
#include <QVBoxLayout>

//src 功能实现.h
#include "ElaImageCard.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaMessageBar.h"

//全局引用
extern const char* sver;    //服务器版本
extern const char* upsize;  //更新大小
extern std::string lver;    //本地版本
extern int GX1;//更新检查控制 1最新 0不是最新

/*
 * 构造函数：检测更新页面布局
 */
updatedown::updatedown(QWidget* parent)
    : ElaWidget(parent)
{
    setWindowTitle("轻蓝更新下载");
    setWindowIcon(QIcon(":/Resource/Image/update.png"));
    this->setIsFixedSize(true);
    setWindowModality(Qt::ApplicationModal);
    setWindowButtonFlags(ElaAppBarType::CloseButtonHint);

    //更新logo图片
    ElaImageCard* Update_Logo = new ElaImageCard(this);
    Update_Logo->setFixedSize(60, 60);
    Update_Logo->setIsPreserveAspectCrop(false);
    Update_Logo->setCardImage(QImage(":/Resource/Image/update.png"));
    //更新logo图片 垂直布局容器
    QVBoxLayout* Update_Logo_Layout = new QVBoxLayout();
    Update_Logo_Layout->addWidget(Update_Logo);
    Update_Logo_Layout->addStretch();

    //服务器版本字符串整合
    std::string ssver="服务器版本:";
    std::string sv=(std::string)ssver + (const char*)sver;

    //更新包大小字符串整合
    std::string ssize1="更新包大小:";
    std::string ssz1=(std::string)ssize1 + (const char*)upsize;


    //程序名
    ElaText* AppText = new ElaText("轻蓝", this);
    QFont AppTextFont = AppText->font();//设置 轻蓝 字体加粗显示
    AppTextFont.setWeight(QFont::Bold);//设置 轻蓝 字体加粗显示
    AppText->setFont(AppTextFont);//应用 轻蓝 字体加粗显示
    AppText->setWordWrap(false);
    AppText->setTextPixelSize(18);

    //本地版本
    std::string llver="当前版本:";
    std::string lv=(std::string)llver + (std::string)lver;
    ElaText* Local_Ver_Text = new ElaText(lv.c_str(), this);
    Local_Ver_Text->setWordWrap(false);
    Local_Ver_Text->setTextPixelSize(14);

    //服务器版本
    ElaText* Server_Ver_Text = new ElaText(sv.c_str(), this);
    Server_Ver_Text->setWordWrap(false);
    Server_Ver_Text->setTextPixelSize(14);
    Server_Ver_Text->setStyleSheet("color: red;");//如果不是最新版 则用红色显示文字
    if(GX1==1)
    {
        //如果是最新版 则用绿色显示文字
        Server_Ver_Text->setStyleSheet("color: green;");
    }

    //更新包大小
    ElaText* Update_Size_Text = new ElaText(ssz1.c_str(), this);//版本大小
    Update_Size_Text->setWordWrap(false);
    Update_Size_Text->setTextPixelSize(14);

    //注意文本
    ElaText* Attention_Text = new ElaText("注意:点击下载后程序将关闭并启动更新程序", this);
    Attention_Text->setWordWrap(false);
    Attention_Text->setTextPixelSize(14);

    //版权声明
    ElaText* Copyright_Text = new ElaText("版权所有 © 2024 笙箫旧景", this);
    Copyright_Text->setWordWrap(false);
    Copyright_Text->setTextPixelSize(14);


    //================================================[开始下载按钮]====================================================================
    Start_Download_Button = new ElaPushButton("开始下载更新", this);
    Start_Download_Button->setBorderRadius(4);
    Start_Download_Button->setLightDefaultColor(ElaThemeColor(ElaThemeType::Light, PrimaryNormal));//PrimaryNormal elathem.h
    Start_Download_Button->setLightHoverColor(ElaThemeColor(ElaThemeType::Light, PrimaryHover));
    Start_Download_Button->setLightPressColor(ElaThemeColor(ElaThemeType::Light, PrimaryPress));
    Start_Download_Button->setLightTextColor(Qt::white);
    Start_Download_Button->setDarkDefaultColor(ElaThemeColor(ElaThemeType::Dark, PrimaryNormal));
    Start_Download_Button->setDarkHoverColor(ElaThemeColor(ElaThemeType::Dark, PrimaryHover));
    Start_Download_Button->setDarkPressColor(ElaThemeColor(ElaThemeType::Dark, PrimaryPress));
    Start_Download_Button->setDarkTextColor(Qt::white);

    connect(Start_Download_Button, &ElaPushButton::clicked, this, [=](){
        ElaMessageBar::success(ElaMessageBarType::TopRight, "成功", "正在启动更新程序!", 2000);
        QString appDir = QCoreApplication::applicationDirPath();
        QString exePath = QDir(appDir).filePath("QingLanUpDate.exe");
        QProcess::startDetached(exePath);
        QCoreApplication::quit();
    });

    //================================================[取消下载按钮]====================================================================
    Cancel_Button = new ElaPushButton("取消", this);
    Cancel_Button->setBorderRadius(6);
    connect(Cancel_Button, &ElaPushButton::clicked, this, [=]() {
        close();
    });

    //================================================[最新版本按钮]====================================================================
    Up_To_Date_Button = new ElaPushButton("已是最新版本", this);
    Up_To_Date_Button->setBorderRadius(4);
    Up_To_Date_Button->setLightDefaultColor(ElaThemeColor(ElaThemeType::Light, PrimaryNormal));//PrimaryNormal elathem.h
    Up_To_Date_Button->setLightHoverColor(ElaThemeColor(ElaThemeType::Light, PrimaryHover));
    Up_To_Date_Button->setLightPressColor(ElaThemeColor(ElaThemeType::Light, PrimaryPress));
    Up_To_Date_Button->setLightTextColor(Qt::white);
    Up_To_Date_Button->setDarkDefaultColor(ElaThemeColor(ElaThemeType::Dark, PrimaryNormal));
    Up_To_Date_Button->setDarkHoverColor(ElaThemeColor(ElaThemeType::Dark, PrimaryHover));
    Up_To_Date_Button->setDarkPressColor(ElaThemeColor(ElaThemeType::Dark, PrimaryPress));
    Up_To_Date_Button->setDarkTextColor(Qt::white);
    Up_To_Date_Button->hide();

    connect(Up_To_Date_Button, &ElaPushButton::clicked, this, [=](){
        close();
    });

    /*
     * 程序最新时按钮的显示
     * 开始更新按钮--->隐藏
     * 取消更新按钮--->隐藏
     * 开始更新按钮--->显示
     */
    if(GX1==1)
    {
        Start_Download_Button->hide();
        Cancel_Button->hide();
        Up_To_Date_Button->show();
    }


    //====================================================[页面总布局]==================================================================
    //垂直布局容器:文本
    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->setSpacing(15);
    textLayout->setContentsMargins(0, 10, 0, 0);
    textLayout->addWidget(AppText);          //程序名
    textLayout->addWidget(Local_Ver_Text);   //本地版本
    textLayout->addWidget(Server_Ver_Text);  //服务器版本
    textLayout->addWidget(Update_Size_Text); //更新包大小
    textLayout->addWidget(Attention_Text);   //注意
    textLayout->addWidget(Copyright_Text);   //版权声明
    textLayout->addStretch();

    //平行布局:Logo 与 文本
    QHBoxLayout* Logo_Text_Layout = new QHBoxLayout();
    Logo_Text_Layout->addSpacing(30);
    Logo_Text_Layout->setContentsMargins(0, 10, 0, 0);
    Logo_Text_Layout->addLayout(Update_Logo_Layout);  //更新logo图片 垂直布局容器
    Logo_Text_Layout->addSpacing(30);
    Logo_Text_Layout->addLayout(textLayout);         //垂直布局容器:文本

    //平行布局:按钮
    QHBoxLayout* ButtonLayout = new QHBoxLayout();
    ButtonLayout->addWidget(Start_Download_Button);  //执行更新按钮
    ButtonLayout->addWidget(Cancel_Button);          //取消更新按钮
    ButtonLayout->addWidget(Up_To_Date_Button);      //已是最新按钮

    //垂直:总布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 5);
    mainLayout->addLayout(Logo_Text_Layout);  //平行布局:Logo 与 文本
    mainLayout->addLayout(ButtonLayout);      //平行布局:按钮
}

/*
 * 析构函数
 */
updatedown::~updatedown()
{

}
