/*
 * 关于更新程序页面布局
 * by shengxiao
 * v1.0.0 2025/04/12
*/


#include "About.h"

//QT功能引用
#include <QHBoxLayout>
#include <QIcon>

//src功能引用
#include "ElaImageCard.h"
#include "ElaText.h"


/*
 * 构造函数：初始化UI布局
 */
About::About(QWidget* parent)
    : ElaWidget(parent)
{
    setWindowTitle("关于更新程序");
    this->setIsFixedSize(true);
    setWindowModality(Qt::ApplicationModal);
    setWindowButtonFlags(ElaAppBarType::CloseButtonHint);

    ElaImageCard* pixCard = new ElaImageCard(this);
    pixCard->setFixedSize(60, 60);
    pixCard->setIsPreserveAspectCrop(false);
    pixCard->setCardImage(QImage(":/Resource/Image/update.png"));

    QVBoxLayout* pixCardLayout = new QVBoxLayout();
    pixCardLayout->addWidget(pixCard);
    pixCardLayout->addStretch();

    ElaText* versionText = new ElaText("轻蓝_更新程序", this);
    QFont versionTextFont = versionText->font();
    versionTextFont.setWeight(QFont::Bold);
    versionText->setFont(versionTextFont);
    versionText->setWordWrap(false);
    versionText->setTextPixelSize(18);

    ElaText* supportText = new ElaText("版本: 1.0.0\nBuild: 20250412(Release)", this);
    supportText->setWordWrap(false);
    supportText->setTextPixelSize(14);
    ElaText* contactText = new ElaText("根据 GPLv3 许可协议开放许可", this);
    contactText->setWordWrap(false);
    contactText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    contactText->setTextPixelSize(14);
    ElaText* helperText = new ElaText("本程序UI控件依于开源程序ElaWidgeTools", this);
    helperText->setWordWrap(false);
    helperText->setTextPixelSize(14);
    ElaText* copyrightText = new ElaText("(C)2020-2025 笙箫旧景 All rights reserved", this);
    copyrightText->setWordWrap(false);
    copyrightText->setTextPixelSize(14);

    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->setSpacing(15);
    textLayout->addWidget(versionText);
    textLayout->addWidget(supportText);
    textLayout->addWidget(contactText);
    textLayout->addWidget(helperText);
    textLayout->addWidget(copyrightText);
    textLayout->addStretch();

    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->addSpacing(30);
    contentLayout->addLayout(pixCardLayout);
    contentLayout->addSpacing(30);
    contentLayout->addLayout(textLayout);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 25, 0, 0);
    mainLayout->addLayout(contentLayout);
}

/*
 * 析构函数
 */
About::~About()
{
}
