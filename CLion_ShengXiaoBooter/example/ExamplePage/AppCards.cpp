/*
 * 应用程序卡片布局
 * by shengxiao
 * v1.0 Created in 2024/09/30
 * v1.3 2025/04/16
*/

#include "AppCards.h"

//QT功能
#include <QHBoxLayout>
#include <QStandardPaths>
#include <QTimer>

//src功能引用
#include "ElaImageCard.h"
#include "ElaText.h"
#include "ElaPushButton.h"
#include "ElaTheme.h"
#include "ElaMessageBar.h"

//自建头文件
#include "Downloads.h" //下载页面
#include "CNet.h"

//全局引用
void downkms(const char* filename,const char* url);
bool fileExists(const QString &fileName);
int D11=0; //区分下载的项目 0空闲 1U校园 2KMS激活 3笙箫终端
int D11A=0;//避免同一个项目下载重复 0默认值 4当前项目正在下载
extern int USHO;//U校园时长下载开放检测
extern int KMSO;//kms下载开放检测
extern int SXTO;//笙箫终端下载开放检测
extern int USH1;//检测u校园时长是否已安装
extern int SXZD1;//检测笙箫终端是否已安装
extern int NETWORK;//网络连接检测
extern std::string IP;
extern const char* sxteminalsver;
extern const char* sxteminalssize;
extern const char* ushsver;
extern const char* ushssize;
extern const char* KMSsver;
extern const char* KMSssize;

/*
 * 构造函数：U校园自助20小时时长-窗口
 */
AppCards::AppCards(QWidget* parent)
    : ElaWidget(parent)
{
    setWindowTitle("U校园自助20小时时长");
    setWindowIcon(QIcon(":/Resource/Image/Uxiaoyuanlo.png"));
    this->setIsFixedSize(true);
    setWindowModality(Qt::ApplicationModal);
    setWindowButtonFlags(ElaAppBarType::CloseButtonHint);

    //Uxiaoyuan logo
    ElaImageCard* USHlogo = new ElaImageCard(this);
    USHlogo->setFixedSize(60, 60);
    USHlogo->setIsPreserveAspectCrop(false);
    USHlogo->setCardImage(QImage(":/Resource/Image/Uxiaoyuanlo.png"));

    //Uxiaoyuan app图片
    ElaImageCard* USHphoto = new ElaImageCard(this);
    USHphoto->setFixedSize(520, 270);
    USHphoto->setIsPreserveAspectCrop(false);
    USHphoto->setCardImage(QImage(":/Resource/Image/Uxiaoyuanp.png"));

    //Uxiaoyuan 文字部分
    ElaText* appname = new ElaText("U校园自助20小时时长", this);
    QFont appnameText = appname->font();
    appnameText.setWeight(QFont::Bold);
    appname->setFont(appnameText);
    appname->setWordWrap(false);
    appname->setTextPixelSize(18);

    //软件最新版本
    std::string banben="版本: ";
    std::string ushoolver=(std::string)banben+(std::string)ushsver;
    ElaText* verText = new ElaText(ushoolver.c_str(), this);
    verText->setWordWrap(false);
    verText->setTextPixelSize(14);

    //支持系统版本
    ElaText* supportText = new ElaText("Windows支持版本: 10及以上", this);
    supportText->setWordWrap(false);
    supportText->setTextPixelSize(14);

    //软件大小
    std::string daxiao="大小: ";
    std::string ushoolsize=(std::string)daxiao+(std::string)ushssize;
    ElaText* AppSize = new ElaText(ushoolsize.c_str(), this);
    AppSize->setWordWrap(false);
    AppSize->setTextInteractionFlags(Qt::TextSelectableByMouse);
    AppSize->setTextPixelSize(14);

    //软件当前下载状态
    ElaText* stateText = new ElaText("状态：下载开放", this);
    if(NETWORK==0)
    {
        //离线情况
        stateText->setText("状态：离线");
        stateText->setStyleSheet("color: red;");
    }
    else
    {//在线
        if(USHO==0)
        {
            //下载未开放状态
            stateText->setText("状态：下载未开放");
            stateText->setStyleSheet("color: red;");
        }
        else
        {
            //正常状态
            stateText->setStyleSheet("color: green;");
        }
    }
    stateText->setWordWrap(false);
    stateText->setTextPixelSize(14);

    //版权
    ElaText* copyrightText = new ElaText("版权所有 © 2024 笙箫旧景", this);
    copyrightText->setWordWrap(false);
    copyrightText->setTextPixelSize(14);

    //================================================[开始下载按钮]====================================================================
    ElaPushButton* DownloadButton = new ElaPushButton("开始下载", this);
    DownloadButton->setBorderRadius(4);
    DownloadButton->setLightDefaultColor(ElaThemeColor(ElaThemeType::Light, PrimaryNormal));//PrimaryNormal elathem.h
    DownloadButton->setLightHoverColor(ElaThemeColor(ElaThemeType::Light, PrimaryHover));
    DownloadButton->setLightPressColor(ElaThemeColor(ElaThemeType::Light, PrimaryPress));
    DownloadButton->setLightTextColor(Qt::white);
    DownloadButton->setDarkDefaultColor(ElaThemeColor(ElaThemeType::Dark, PrimaryNormal));
    DownloadButton->setDarkHoverColor(ElaThemeColor(ElaThemeType::Dark, PrimaryHover));
    DownloadButton->setDarkPressColor(ElaThemeColor(ElaThemeType::Dark, PrimaryPress));
    DownloadButton->setDarkTextColor(Qt::white);

    //--------------------下载点击后的逻辑------------------
    _downloadsPage =new Downloads();
    connect(DownloadButton, &ElaPushButton::clicked, this, [=](){
        //离线
        if(NETWORK!=1)
        {
            ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", "未连接至服务器", 2000);
            return;
        }
        //检查服务器是否开发下载
        if(USHO!=1)
        {
            ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", "服务器未开放下载", 2000);
            return;
        }
        //检查是否安装 1安装 0未安装   避免重复安装
        if(USH1==1)
        {
            ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", "请勿重复安装", 2000);
            return;
        }
        //检查是否重复下载
        if(D11!=0)
        {
            ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", "一次只能添加一个下载任务", 2000);
            return;
        }
        if(D11A!=0)//检查是否已存在一个任务 D11A的参数变更在Download.cpp
        {
            ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", "已经有一个任务了", 2000);
            return;
        }

        D11=1;//设置软件下载锁为U校园20小时时长，在其下载结束之前其他软件不能下载
        ElaMessageBar::success(ElaMessageBarType::TopRight, "成功", "已添加到下载目录!", 2000);
        _downloadsPage->updateContent();
        _downloadsPage->setFixedSize(950, 480);//长  高
        _downloadsPage->setWindowIcon(QIcon(":/include/Image/control/AnimatedIcon.png"));
        _downloadsPage->show();


        //多线程
        std::string DownLoadURL="http://" + IP + "/softwares/USH.exe";
        DownOverThread* overThread = new DownOverThread("USH.exe",DownLoadURL.c_str());
        overThread->start();  // 启用线程

        // 连接下载完成回显信号
        connect(overThread, &DownOverThread::downloadRootFinished, this, [=](bool success, const QString& msg) {

            D11 = 0; // 无论成功与否都解除下载锁
            D11A = 0; // 下载列表置为空闲

            if (success)
            {
                USH1 = 1; //声明已安装 避免重复 tips：downloads.cpp中的安装检查仅在点击应用中心的时候进行检查,这里进行声明，是封堵在主页时一直重复安装的问题
                ElaMessageBar::success(ElaMessageBarType::TopRight, "成功", msg, 4000);
            }
            else
            {
                /*
                 * 这里没有USH1=0;的原因
                 * 是因为在启动程序的时候会有一次检查软件安装情况的初始化检查，如果没有安装其值则为0，
                 * 再加上卸载时会进入和切出应用中心，而应用中心的UI无论是进入还是切出都会更新
                 * 然后会再一次进行软件安装性检查
                 * 所以没有什么必要在这里再重新设置一遍
                 */
                ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", msg, 4000);
            }
            _downloadsPage->updateContent();
        });

        // 自动删除线程对象
        connect(overThread, &QThread::finished, overThread, &QObject::deleteLater);

        close();
    });

    //================================================[取消下载按钮]====================================================================
    ElaPushButton* cancelButton = new ElaPushButton("取消", this);
    cancelButton->setBorderRadius(6);
    connect(cancelButton, &ElaPushButton::clicked, this, [=]() {
        close();
    });

    //====================================================[此窗口总布局]==================================================================
    //Uxiaoyuanlogo 的垂直容器
    QVBoxLayout* USHlogoLayout = new QVBoxLayout();
    USHlogoLayout->addWidget(USHlogo);
    USHlogoLayout->addStretch();

    //Uxiaoyuan软件图片垂直容器
    QVBoxLayout* USHphotoLayout = new QVBoxLayout();
    USHphotoLayout->addWidget(USHphoto);
    USHphotoLayout->addStretch();

    //文字 垂直容器
    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->setSpacing(15);
    textLayout->setContentsMargins(0, 10, 0, 0);
    textLayout->addWidget(appname);
    textLayout->addWidget(verText);
    textLayout->addWidget(supportText);
    textLayout->addWidget(AppSize);
    textLayout->addWidget(stateText);
    textLayout->addWidget(copyrightText);
    textLayout->addStretch();

    //logo 图片 文字 相连接的平行容器
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->addSpacing(30);
    contentLayout->setContentsMargins(0, 10, 0, 0);
    contentLayout->addLayout(USHlogoLayout);//ush logo
    contentLayout->addSpacing(30);
    contentLayout->addLayout(textLayout); //ush 文字属性
    contentLayout->addSpacing(30);
    contentLayout->setContentsMargins(0, 10, 350, 0);//将图片从右向左移350位
    contentLayout->addLayout(USHphotoLayout); //ush photo

    //按钮 平行容器
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(DownloadButton);
    buttonLayout->addWidget(cancelButton);

    //总垂直容器
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 5);
    mainLayout->addLayout(contentLayout);//logo 图片 文字 相连接的平行容器
    mainLayout->addLayout(buttonLayout);//按钮 平行容器
}


/*
 * 构造函数：Kms激活应用介绍窗口
 */
T1::T1(QWidget* parent)
    : ElaWidget(parent)
{
    setWindowTitle("KMS激活");
    setWindowIcon(QIcon(":/Resource/Image/kmslo.png"));
    this->setIsFixedSize(true);
    setWindowModality(Qt::ApplicationModal);
    setWindowButtonFlags(ElaAppBarType::CloseButtonHint);

    //kms激活 logo
    ElaImageCard* kmslogo = new ElaImageCard(this);
    kmslogo->setFixedSize(60, 60);
    kmslogo->setIsPreserveAspectCrop(false);
    kmslogo->setCardImage(QImage(":/Resource/Image/kmslo.png"));

    //kms app图片
    ElaImageCard* kmsphoto = new ElaImageCard(this);
    kmsphoto->setFixedSize(520, 270);
    kmsphoto->setIsPreserveAspectCrop(false);
    kmsphoto->setCardImage(QImage(":/Resource/Image/kmsf.png"));

    //kms激活文字部分
    ElaText* appname = new ElaText("Kms激活", this);
    QFont appnameText = appname->font();
    appnameText.setWeight(QFont::Bold);
    appname->setFont(appnameText);
    appname->setWordWrap(false);
    appname->setTextPixelSize(18);

    //软件版本
    std::string banben="版本: ";
    std::string KMSver=(std::string)banben+(std::string)KMSsver;
    ElaText* verText = new ElaText(KMSver.c_str(), this);
    verText->setWordWrap(false);
    verText->setTextPixelSize(14);

    //支持的系统
    ElaText* supportText = new ElaText("Windows支持版本: 7及以上\n包含Server2008及以上", this);
    supportText->setWordWrap(false);
    supportText->setTextPixelSize(14);

    //软件大小
    std::string daxiao="大小: ";
    std::string size=(std::string)daxiao+(std::string)KMSssize;
    ElaText* AppSize = new ElaText(size.c_str(), this);
    AppSize->setWordWrap(false);
    AppSize->setTextInteractionFlags(Qt::TextSelectableByMouse);
    AppSize->setTextPixelSize(14);

    //软件开放下载状态
    ElaText* stateText = new ElaText("状态: 开放下载", this);
    if(NETWORK==0)
    {
        //离线
        stateText->setText("状态: 离线");
        stateText->setStyleSheet("color: red;");
    }
    else if(NETWORK==1)
    {//在线
        if(KMSO==0)
        {
            //未开放下载状态
            stateText->setText("状态: 未开放下载");
            stateText->setStyleSheet("color: red;");
        }
        else if(KMSO==1)
        {
            //开放下载
            stateText->setStyleSheet("color: green;");
        }
    }
    stateText->setWordWrap(false);
    stateText->setTextPixelSize(14);

    //版权信息
    ElaText* copyrightText = new ElaText("请注意由于KMS激活软件特殊性,\n本软件下载完成后不添加到应用中心,\n请前往桌面查看是一个带有\nkms的压缩包,请解压后使用\n\n版权所有 © 2024 笙箫旧景", this);
    copyrightText->setWordWrap(false);
    copyrightText->setTextPixelSize(14);

    //================================================[开始下载按钮]====================================================================
    ElaPushButton* DownloadButton = new ElaPushButton("开始下载", this);
    DownloadButton->setBorderRadius(4);
    DownloadButton->setLightDefaultColor(ElaThemeColor(ElaThemeType::Light, PrimaryNormal));//PrimaryNormal elathem.h
    DownloadButton->setLightHoverColor(ElaThemeColor(ElaThemeType::Light, PrimaryHover));
    DownloadButton->setLightPressColor(ElaThemeColor(ElaThemeType::Light, PrimaryPress));
    DownloadButton->setLightTextColor(Qt::white);
    DownloadButton->setDarkDefaultColor(ElaThemeColor(ElaThemeType::Dark, PrimaryNormal));
    DownloadButton->setDarkHoverColor(ElaThemeColor(ElaThemeType::Dark, PrimaryHover));
    DownloadButton->setDarkPressColor(ElaThemeColor(ElaThemeType::Dark, PrimaryPress));
    DownloadButton->setDarkTextColor(Qt::white);

    //--------------------下载点击后的逻辑------------------
    _downloadsPage =new Downloads();
    connect(DownloadButton, &ElaPushButton::clicked, this, [=](){
        if(NETWORK==0)
        {
            ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", "未连接至服务器", 2000);
        }
        else if(NETWORK==1)
        {
            if(KMSO==1)
            {
                if(D11!=0)
                {
                    ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", "一次只能添加一个下载任务", 2000);
                    return;
                }
                if(D11A==4)
                {
                    ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", "已经有一个任务了", 2000);
                    return;
                }
                D11=2;//设置软件下载锁为Kms激活，在其下载结束之前其他软件不能下载
                ElaMessageBar::success(ElaMessageBarType::TopRight, "成功", "已添加到下载目录!", 2000);
                _downloadsPage->updateContent();
                _downloadsPage->setFixedSize(950, 480);//长  高
                _downloadsPage->setWindowIcon(QIcon(":/include/Image/control/AnimatedIcon.png"));
                _downloadsPage->show();

                //创建线程对象
                std::string DownLoadURL="http://" + IP + "/softwares/kms.zip";
                DownDeskTopThread* DeskTopThread = new DownDeskTopThread("/kms.zip",DownLoadURL.c_str(),"kms.zip");

                DeskTopThread->start();//启用线程

                // 连接下载完成回显信号
                connect(DeskTopThread, &DownDeskTopThread::downloadDeskTopFinished, this, [=](bool success, const QString& msg) {
                    D11=0;//解除下载锁
                    D11A=0;//下载列表置为空

                    if(success)
                    {
                        ElaMessageBar::success(ElaMessageBarType::TopRight, "成功", "下载至桌面成功!", 4000);
                    }
                    else
                    {
                        ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", "下载Kms激活出错", 4000);
                    }

                    _downloadsPage->updateContent();
                });

                close();
            }
            else
            {
                ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", "服务器未开放下载", 2000);
                close();
            }
        }
    });

    //================================================[取消下载按钮]====================================================================
    ElaPushButton* cancelButton = new ElaPushButton("取消", this);
    cancelButton->setBorderRadius(6);
    connect(cancelButton, &ElaPushButton::clicked, this, [=]()
    {
        close();
    });

    //================================================[此窗口总布局]====================================================================
    //kms激活 logo 垂直容器
    QVBoxLayout* KMSlogoLayout = new QVBoxLayout();
    KMSlogoLayout->addWidget(kmslogo);
    KMSlogoLayout->addStretch();

    //Kms软件图片垂直容器
    QVBoxLayout* KMSphotoLayout = new QVBoxLayout();
    KMSphotoLayout->addWidget(kmsphoto);
    KMSphotoLayout->addStretch();

    //定义文字部分_垂直容器
    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->setSpacing(15);
    textLayout->setContentsMargins(0, 10, 0, 0);
    textLayout->addWidget(appname);
    textLayout->addWidget(verText);
    textLayout->addWidget(supportText);
    textLayout->addWidget(AppSize);
    textLayout->addWidget(stateText);
    textLayout->addWidget(copyrightText);
    textLayout->addStretch();

    //logo 文字 图片 相连接的平行容器
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->addSpacing(30);
    contentLayout->setContentsMargins(0, 5, 0, 0);
    contentLayout->addLayout(KMSlogoLayout);
    contentLayout->addSpacing(30);
    contentLayout->addLayout(textLayout);
    contentLayout->addSpacing(30);
    contentLayout->setContentsMargins(0, 5, 300, 0);//将图片从右向左移350位
    contentLayout->addLayout(KMSphotoLayout);

    //定义按钮位置_平行容器
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(DownloadButton);
    buttonLayout->addWidget(cancelButton);

    //总垂直容器
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 5);
    mainLayout->addLayout(contentLayout);
    mainLayout->addLayout(buttonLayout);
}

/*
 * 构造函数：笙箫终端(ShengXiaoTeminal)软件介绍窗口
 */
T2::T2(QWidget* parent)
    : ElaWidget(parent)
{
    setWindowTitle("笙箫终端");
    setWindowIcon(QIcon(":/Resource/Image/sxteminal.png"));
    this->setIsFixedSize(true);
    setWindowModality(Qt::ApplicationModal);
    setWindowButtonFlags(ElaAppBarType::CloseButtonHint);

    //笙箫终端 logo
    ElaImageCard* sxtlogo = new ElaImageCard(this);
    sxtlogo->setFixedSize(60, 60);
    sxtlogo->setIsPreserveAspectCrop(false);
    sxtlogo->setCardImage(QImage(":/Resource/Image/sxteminal.png"));

    //笙箫终端 app图片
    ElaImageCard* sxtp = new ElaImageCard(this);
    sxtp->setFixedSize(520, 270);
    sxtp->setIsPreserveAspectCrop(false);
    sxtp->setCardImage(QImage(":/Resource/Image/SXTeminal1.png"));

    //笙箫终端属性文字部分
    ElaText* appname = new ElaText("笙箫终端", this);
    QFont appnameText = appname->font();
    appnameText.setWeight(QFont::Bold);
    appname->setFont(appnameText);
    appname->setWordWrap(false);
    appname->setTextPixelSize(18);

    //软件版本
    std::string banben="版本: ";
    std::string sxtver=(std::string)banben+(std::string)sxteminalsver;
    ElaText* verText = new ElaText(sxtver.c_str(), this);
    verText->setWordWrap(false);
    verText->setTextPixelSize(14);

    //软件支持的系统
    ElaText* supportText = new ElaText("Windows支持版本: 10及以上", this);
    supportText->setWordWrap(false);
    supportText->setTextPixelSize(14);

    //软件大小
    std::string daxiao="大小: ";
    std::string sxsize=(std::string)daxiao+(std::string)sxteminalssize;
    ElaText* AppSize = new ElaText(sxsize.c_str(), this);
    AppSize->setWordWrap(false);
    AppSize->setTextInteractionFlags(Qt::TextSelectableByMouse);
    AppSize->setTextPixelSize(14);

    //软件开放下载状态
    ElaText* stateText = new ElaText("状态: 开放下载", this);
    if(NETWORK==0)
    {
        //离线
        stateText->setText("状态: 离线");
        stateText->setStyleSheet("color: red;");
    }
    else if(NETWORK==1)
    {
        //在线
        if(SXTO==0)
        {
            stateText->setText("状态:未开放下载");
            stateText->setStyleSheet("color: red;");
        }
        else if(SXTO==2)
        {
            stateText->setText("状态:测试应用,需要密码下载");
            stateText->setStyleSheet("color: red;");
        }
        else
        {
            stateText->setStyleSheet("color: green;");
        }
    }

    stateText->setWordWrap(false);
    stateText->setTextPixelSize(14);

    //版权文字
    ElaText* copyrightText = new ElaText("版权所有 © 2024 笙箫旧景", this);
    copyrightText->setWordWrap(false);
    copyrightText->setTextPixelSize(14);

    //================================================[开始下载按钮]====================================================================
    ElaPushButton* DownloadButton = new ElaPushButton("开始下载", this);
    DownloadButton->setBorderRadius(4);
    DownloadButton->setLightDefaultColor(ElaThemeColor(ElaThemeType::Light, PrimaryNormal));//PrimaryNormal elathem.h
    DownloadButton->setLightHoverColor(ElaThemeColor(ElaThemeType::Light, PrimaryHover));
    DownloadButton->setLightPressColor(ElaThemeColor(ElaThemeType::Light, PrimaryPress));
    DownloadButton->setLightTextColor(Qt::white);
    DownloadButton->setDarkDefaultColor(ElaThemeColor(ElaThemeType::Dark, PrimaryNormal));
    DownloadButton->setDarkHoverColor(ElaThemeColor(ElaThemeType::Dark, PrimaryHover));
    DownloadButton->setDarkPressColor(ElaThemeColor(ElaThemeType::Dark, PrimaryPress));
    DownloadButton->setDarkTextColor(Qt::white);

    //--------------------下载点击后的逻辑------------------
    _downloadsPage =new Downloads();
    AppLine =new AppLineEdit();
    connect(DownloadButton, &ElaPushButton::clicked, this, [=](){
        if(NETWORK!=1)
        {
            //网络中断
            ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", "未连接至服务器", 2000);
            return;
        }
        //检查服务器是否开发下载
        if(SXTO==1||SXTO==2)
        {
            //检查是否安装 1安装 0未安装
            if(SXZD1==0)
            {
                //检查是否重复下载
                if(D11!=0)
                {
                    ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", "一次只能添加一个下载任务", 2000);
                    return;
                }
                if(D11A==4)//检查是否已存在一个任务
                {
                    ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", "已经有一个任务了", 2000);
                    return;
                }
                if(SXTO==1)//准许下载
                {
                    D11=3;//设置软件下载锁为笙箫终端，在其下载结束之前其他软件不能下载
                    ElaMessageBar::success(ElaMessageBarType::TopRight, "成功", "已添加到下载目录!", 2000);
                    _downloadsPage->updateContent();
                    _downloadsPage->setFixedSize(950, 480);//长  高
                    _downloadsPage->setWindowIcon(QIcon(":/include/Image/control/AnimatedIcon.png"));
                    _downloadsPage->show();

                    //多线程
                    std::string DownLoadURL="http://" + IP + "/softwares/SXTeminal.exe";
                    DownOverThread* overThread = new DownOverThread("SXTeminal.exe",DownLoadURL.c_str());
                    overThread->start();  // 启用线程

                    connect(overThread, &DownOverThread::downloadRootFinished, this, [=](bool success, const QString& msg) {

                        D11 = 0; // 无论成功与否都解除下载锁
                        D11A = 0; // 下载列表置为空闲

                        if (success)
                        {
                            SXZD1=1; //声明已安装
                            ElaMessageBar::success(ElaMessageBarType::TopRight, "成功", msg, 4000);
                        }
                        else
                        {
                            ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", msg, 4000);
                        }
                        _downloadsPage->updateContent();
                    });

                    // 自动删除线程对象
                    connect(overThread, &QThread::finished, overThread, &QObject::deleteLater);

                    close();
                }
                else if(SXTO==2)
                {
                    //测试应用，启用测试输入密码界面下载
                    AppLine->setFixedSize(450, 230);
                    AppLine->show();
                }
                close();
            }
            else if(SXZD1==1)
            {
                ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", "请勿重复安装", 2000);
                close();
            }
        }
        else
        {
            ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", "服务器未开放下载", 2000);
            close();
        }
    });

    //================================================[取消下载按钮]====================================================================
    ElaPushButton* cancelButton = new ElaPushButton("取消", this);
    cancelButton->setBorderRadius(6);
    connect(cancelButton, &ElaPushButton::clicked, this, [=](){
        close();
    });

    //================================================[此窗口总布局]====================================================================
    //笙箫终端 logo 平行容器
    QVBoxLayout* sxtlogoLayout = new QVBoxLayout();
    sxtlogoLayout->addWidget(sxtlogo);
    sxtlogoLayout->addStretch();

    //笙箫终端软件图片 平行容器
    QVBoxLayout* sxtphotoLayout = new QVBoxLayout();
    sxtphotoLayout->addWidget(sxtp);
    sxtphotoLayout->addStretch();

    //文字部分 垂直容器
    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->setSpacing(15);
    textLayout->setContentsMargins(0, 10, 0, 0);
    textLayout->addWidget(appname);
    textLayout->addWidget(verText);
    textLayout->addWidget(supportText);
    textLayout->addWidget(AppSize);
    textLayout->addWidget(stateText);
    textLayout->addWidget(copyrightText);
    textLayout->addStretch();

    //logo 文字 图片 相连接的平行布局
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->addSpacing(30);
    contentLayout->setContentsMargins(0, 5, 0, 0);
    contentLayout->addLayout(sxtlogoLayout);
    contentLayout->addSpacing(30);
    contentLayout->addLayout(textLayout);
    contentLayout->addSpacing(30);
    contentLayout->setContentsMargins(0, 5, 300, 0);//将图片从右向左移350位
    contentLayout->addLayout(sxtphotoLayout);

    //定义按钮位置_平行布局
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(DownloadButton);
    buttonLayout->addWidget(cancelButton);

    //垂直总容器
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 5);
    mainLayout->addLayout(contentLayout);
    mainLayout->addLayout(buttonLayout);
}


AppCards::~AppCards()
{
}

T1::~T1() {
    // 如果有需要清理的资源，放在这里
}

T2::~T2() {
    // 如果有需要清理的资源，放在这里
}

