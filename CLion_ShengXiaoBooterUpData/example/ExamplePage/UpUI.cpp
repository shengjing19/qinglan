/*
 * 更新程序UI
 * by shengxiao
 * v1.0 2025/04/11
*/

#include "UpUI.h"

//QT功能
#include <QTimer>
#include <QDesktopServices>
#include <QApplication>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>
#include <QProcess>
#include <QDir>

//src功能实现.h
#include "ElaFlowLayout.h"
#include "ElaText.h"
#include "ElaImageCard.h"
#include "ElaPushButton.h"
#include "ElaTheme.h"
#include "ElaScrollPageArea.h"

void Serini();
// 引用初始化全局配置
extern const char* Sver; // 服务器版本号
extern const char* Smd5; // 预期MD5值
extern const char* Slog; // 更新日志

/*
 * 构造函数：初始化UI布局和下载逻辑
 */
UpUI::UpUI(QWidget* parent)
    : T_BasePage(parent)
{

    Serini();

    // 创建图片容器（左侧）
    ElaImageCard* pixCard = new ElaImageCard(this);
    pixCard->setFixedSize(80, 80);
    pixCard->setIsPreserveAspectCrop(false);
    pixCard->setCardImage(QImage(":/Resource/Image/update.png"));

    // 图片布局（带顶部对齐）
    QVBoxLayout* pixCardLayout = new QVBoxLayout();
    pixCardLayout->addWidget(pixCard);
    pixCardLayout->setAlignment(Qt::AlignTop); // 关键点：防止图片垂直居中

    // 创建文字容器（右侧）
    QWidget* textContainer = new QWidget(this); // 新建独立容器
    QVBoxLayout* textLayout = new QVBoxLayout(textContainer);
    textLayout->setContentsMargins(0, 0, 0, 0); // 清除文字容器边距

    // 标题文本
    ElaText* biaoti = new ElaText(this);
    biaoti->setText("轻蓝更新程序，点击更新即可开始下载与安装");
    biaoti->setTextPixelSize(18);
    biaoti->setWordWrap(false);                   // 关闭自动换行
    biaoti->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred); // 水平扩展

    // 服务器版本显示
    std::string SverN="当前服务器版本:";
    std::string SverT=(std::string)SverN + (const char*)Sver;
    ElaText* Ser = new ElaText(SverT.c_str(), this);
    Ser->setWordWrap(false);
    Ser->setTextPixelSize(15);
    Ser->setStyleSheet("color: green;");

    // 更新日志区域
    ElaScrollPageArea* updatelogArea = new ElaScrollPageArea(this);//新建矩形区域
    QVBoxLayout* updatelogLayout = new QVBoxLayout(updatelogArea);
    updatelogArea->setFixedHeight(140);//设置矩形高度
    updatelogArea->setFixedWidth(500);

    ElaText* log_titel=new ElaText("更新日志",this);
    log_titel->setTextPixelSize(16);
    updatelogLayout->addWidget(log_titel);//将该文字放给aboutSwitchText中

    ElaText* log_titel1=new ElaText(Slog,this);
    log_titel1->setTextPixelSize(14);
    updatelogLayout->addWidget(log_titel1);//将该文字放给aboutSwitchText中

    // 图片右侧文字布局
    textLayout->addWidget(biaoti);
    textLayout->addSpacing(3);
    textLayout->addWidget(Ser);
    textLayout->addSpacing(3);
    textLayout->addWidget(updatelogArea);
    textLayout->addStretch();
    
    //开始下载更新按钮
    ElaPushButton* overButton = new ElaPushButton("开始下载更新", this);
    overButton->setBorderRadius(4);
    overButton->setLightDefaultColor(ElaThemeColor(ElaThemeType::Light, PrimaryNormal));//PrimaryNormal ElaThem.h
    overButton->setLightHoverColor(ElaThemeColor(ElaThemeType::Light, PrimaryHover));
    overButton->setLightPressColor(ElaThemeColor(ElaThemeType::Light, PrimaryPress));
    overButton->setLightTextColor(Qt::white);
    overButton->setDarkDefaultColor(ElaThemeColor(ElaThemeType::Dark, PrimaryNormal));
    overButton->setDarkHoverColor(ElaThemeColor(ElaThemeType::Dark, PrimaryHover));
    overButton->setDarkPressColor(ElaThemeColor(ElaThemeType::Dark, PrimaryPress));
    overButton->setDarkTextColor(Qt::white);

//==============================================================下载逻辑与校验============================================================================
    // 初始化下载管理器
    downloadThread = new QThread(this);
    downloadManager = new DownloadManager();
    downloadManager->moveToThread(downloadThread);
    downloadThread->start(); //启动线程

    // 下载按钮点击信号
    connect(overButton, &ElaPushButton::clicked, this, [=](){
        overButton->setEnabled(false);
        overButton->setProgress(0);
        downloadPath = "C://up.exe";
        QByteArray expectedMd5 = Smd5;// 从服务器获取MD5
        QMetaObject::invokeMethod(downloadManager, "startDownload",Q_ARG(QString, "http://192.168.30.12:5/exe/up.exe"),Q_ARG(QString, downloadPath),Q_ARG(QByteArray, expectedMd5));
    });

    /*
    //下载错误信息显示--调试用
    connect(downloadManager, &DownloadManager::errorOccurred,this, [=](const QString &msg){
        qDebug() << "下载错误:" << msg;
    });
    */

    //下载进度回显
    connect(downloadManager, &DownloadManager::progressChanged,this, [=](int progress){
        overButton->setProgress(progress);
        overButton->setText(QString("下载中 %1%").arg(progress));
    });

    //下载结束后的逻辑运行与MD5判断
    connect(downloadManager, &DownloadManager::downloadFinished,this, [=](bool success, const QString &msg){
        overButton->setEnabled(true);
        if(success)
        {
            overButton->setText("下载完成");
            QProcess::startDetached(downloadPath); // 启动安装程序
            QApplication::quit(); // 退出更新下载程序
        }
        else
        {
            overButton->setText(msg);//输出错误信息
            overButton->setEnabled(false); //关闭下载按钮的启用状态
            // 2秒后切换中文错误显示
            QTimer::singleShot(2000, [=](){
                overButton->setText("下载更新失败_MD5校验失败_请关闭后重试");
            });
        }
        downloadThread->quit(); //下载线程退出
    });

//===============================================页面总布局================================================================================
    // 主水平布局 当前是图片与文字区处于同一水平区
    QHBoxLayout* Pic_Text = new QHBoxLayout();
    Pic_Text->setContentsMargins(0, 0, 0, 0); // 整个内容区域边距
    Pic_Text->addLayout(pixCardLayout);//齿轮图片
    Pic_Text->addSpacing(10); // 图片与文字间隔10像素
    Pic_Text->addWidget(textContainer); // 将文字容器作为独立控件添加
    Pic_Text->addStretch();
    //mainLayout->setStretch(2, 1);          // 设置文字区域可拉伸权重

    //主垂直主布局
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(20, 10, 20, 20); // 整体边距
    mainLayout->addLayout(Pic_Text);          // 添加图片+文字区域
    mainLayout->addWidget(overButton);        //更新按钮
    //mainLayout->addWidget(overButton, 0, Qt::AlignCenter); // 按钮左对齐

    // 应用布局到父容器
    QWidget* container = new QWidget(this);
    container->setLayout(mainLayout);
    container->setWindowTitle("  ");
    addCentralWidget(container, true, true, 0); // 替换原来的downloadtitle
}


/*
 * 析构函数：清理资源
 */
UpUI::~UpUI()
{
    downloadThread->quit();
    downloadThread->wait();
    delete downloadManager;
}
