/*
 * 主页面布局
 * by shengxiao
 * v1.0 Created in 2024/09/30
 * v1.5 2025/04/13
*/

#include "Home.h"

//QT功能
#include <QDesktopServices>
#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>

//src功能实现.h
#include "ElaFlowLayout.h"
#include "ElaMessageBar.h"
#include "ElaPopularCard.h"
#include "ElaText.h"
#include "ElaPromotionCard.h"
#include "ElaPromotionView.h"

/*
 * 构造函数：主页面布局
 */
Home::Home(QWidget* parent)
    : T_BasePage(parent)
{
    // 初始化提示
    ElaMessageBar::success(ElaMessageBarType::TopRight, "欢迎", "欢迎使用轻蓝", 1200);

    // 预览窗口标题
    setWindowTitle("主页");
    setTitleVisible(false);
    setContentsMargins(2, 2, 0, 0);


    //================================================[布局一|图片轮转]====================================================================
    //创建名为SoftwareView的图片轮转对象
    ElaPromotionView* SoftwareView = new ElaPromotionView(this);

    /*
     * 创建图片轮转的图片对象
     * Picture_One--->轻蓝  宣传图片
     * Picture_Two--->Kms激活 宣传图片
     * Picture_Three--->U校园自助20小时时长 宣传图片
     */
    ElaPromotionCard* Picture_One = new ElaPromotionCard(this);
    Picture_One->setCardPixmap(QPixmap(":/Resource/Image/Card/USH.png"));
    Picture_One->setCardTitle("①");
    Picture_One->setPromotionTitle("U校园时长挂载");
    Picture_One->setTitle("20小时?，很简单！");
    Picture_One->setSubTitle("by：笙箫旧景");

    ElaPromotionCard* Picture_Two = new ElaPromotionCard(this);
    Picture_Two->setCardPixmap(QPixmap(":/Resource/Image/Card/KMS.png"));
    Picture_Two->setCardTitle("②");
    Picture_Two->setPromotionTitle("Kms激活");
    Picture_Two->setTitle("将KMS指令融合在一起的选择性App");
    Picture_Two->setSubTitle("by：笙箫旧景");

    ElaPromotionCard* Picture_Three = new ElaPromotionCard(this);
    Picture_Three->setCardPixmap(QPixmap(":/Resource/Image/Card/SXZD.png"));
    Picture_Three->setCardTitle("③");
    Picture_Three->setPromotionTitle("笙箫终端");
    Picture_Three->setTitle("内测应用");
    Picture_Three->setSubTitle("by：笙箫旧景");

    //将三张图片加入Software_View图片轮转中
    SoftwareView->appendPromotionCard(Picture_One);
    SoftwareView->appendPromotionCard(Picture_Two);
    SoftwareView->appendPromotionCard(Picture_Three);
    SoftwareView->setIsAutoScroll(true);

    //================================================[布局二|旗下软件]====================================================================
    //布局二标题
    ElaText* Layout_2_Title_Text = new ElaText("旗下应用", this);
    Layout_2_Title_Text->setTextPixelSize(20);

    //创建水平布局 将布局二标题单独显示一行 ，后续可能会添加按钮 与标题文字同一行显示
    QHBoxLayout* Layout_2_Title_Text_Layout = new QHBoxLayout();
    Layout_2_Title_Text_Layout->setContentsMargins(33, 0, 0, 0);
    Layout_2_Title_Text_Layout->addWidget(Layout_2_Title_Text);  //布局二标题文字

    /*
     * 软件卡片
     * Software_Card_One--->U校园自助20小时时长
     * Software_Card_Two--->Kms激活
     * Software_Card_Three--->笙箫终端
     */

    //==============[软件卡片一|U校园自助20小时时长]===============
    ElaPopularCard* Software_Card_One = new ElaPopularCard(this);
    //------添加U校园下载页面----------

    UshoolPage = new AppCards();
    connect(Software_Card_One, &ElaPopularCard::popularCardButtonClicked, this, [=]()
    {
        UshoolPage->setFixedSize(900, 400);
        UshoolPage->moveToCenter();
        UshoolPage->show();
    });

    //--------卡片内文本信息---------
    Software_Card_One->setCardPixmap(QPixmap(":/Resource/Image/Uxiaoyuanlo.png"));
    Software_Card_One->setTitle("U校园自助20小时时长");
    Software_Card_One->setSubTitle("5.0⭐ 实用程序与工具");
    Software_Card_One->setInteractiveTips("免费下载");
    Software_Card_One->setDetailedText("笙箫旧景旗下应用，操作简单，点击开始，即可享受全自动时常挂载,已连续测试3小时确认有效，理论可以在不到1天的时间内挂完时长");
    Software_Card_One->setCardFloatPixmap(QPixmap(":/Resource/Image/IARC/IARC_7+.svg.png"));


    //==============[软件卡片二|Kms激活]===============
    ElaPopularCard* Software_Card_Two = new ElaPopularCard(this);

    KmsPage = new T1();
    connect(Software_Card_Two, &ElaPopularCard::popularCardButtonClicked, this, [=]()
    {
        KmsPage->setFixedSize(930, 440);
        KmsPage->moveToCenter();
        KmsPage->show();
    });

    //--------卡片内文本信息---------
    Software_Card_Two->setTitle("KMS激活");
    Software_Card_Two->setSubTitle("5.0⭐ 实用程序与工具");
    Software_Card_Two->setCardPixmap(QPixmap(":/Resource/Image/kmslo.png"));
    Software_Card_Two->setInteractiveTips("免费下载");
    Software_Card_Two->setDetailedText("简化Windows slmgr 命令操作，实现选择激活。");
    Software_Card_Two->setCardFloatPixmap(QPixmap(":/Resource/Image/IARC/IARC_7+.svg.png"));


    //==============[软件卡片三|笙箫终端]===============
    ElaPopularCard* Software_Card_Three = new ElaPopularCard(this);

    SXTPage = new T2();
    connect(Software_Card_Three, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
        SXTPage->setFixedSize(930, 440);
        SXTPage->moveToCenter();
        SXTPage->show();
    });

    //--------卡片内文本信息---------
    Software_Card_Three->setTitle("笙箫终端");
    Software_Card_Three->setSubTitle("α⭐ 指令程序与工具");
    Software_Card_Three->setCardPixmap(QPixmap(":/Resource/Image/sxteminal.png"));
    Software_Card_Three->setInteractiveTips("测试应用");
    Software_Card_Three->setDetailedText("将一些常用终端程序集成到一个终端去");
    Software_Card_Three->setCardFloatPixmap(QPixmap(":/Resource/Image/IARC/IARC_7+.svg.png"));



    //====================================================[页面总布局]==================================================================

    //ela流布局 ---- 软件卡片
    ElaFlowLayout* Software_Card_FlowLayout = new ElaFlowLayout(0, 5, 5);
    Software_Card_FlowLayout->setContentsMargins(30, 0, 0, 0);
    Software_Card_FlowLayout->setIsAnimation(true);
    Software_Card_FlowLayout->addWidget(Software_Card_One);
    Software_Card_FlowLayout->addWidget(Software_Card_Two);
    Software_Card_FlowLayout->addWidget(Software_Card_Three);

    //垂直总布局 --- 布局一与布局二
    QVBoxLayout* MainVLayout = new QVBoxLayout();
    MainVLayout->setSpacing(0);
    MainVLayout->setContentsMargins(0, 0, 0, 0);
    MainVLayout->addWidget(SoftwareView);               //SoftwareView图片轮转
    MainVLayout->addSpacing(20);
    MainVLayout->addLayout(Layout_2_Title_Text_Layout); //布局二标题文字
    MainVLayout->addSpacing(10);
    MainVLayout->addLayout(Software_Card_FlowLayout);  //软件卡片
    MainVLayout->addStretch();

    // 创建父容器
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("Home");
    centralWidget->setLayout(MainVLayout);
    addCentralWidget(centralWidget);

    //qDebug() << "初始化成功";
}

/*
 * 析构函数
 */
Home::~Home()
{
}

