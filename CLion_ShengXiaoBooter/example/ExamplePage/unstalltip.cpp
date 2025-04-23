/*
 * 卸载窗口布局
 * by shengxiao
 * v1.0 Created in 2024/10/04
 * v1.2 2025/04/12
*/

#include "unstalltip.h"

//QT功能
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QVBoxLayout>

//src 功能实现.h
#include "ElaText.h"
#include "ElaPushButton.h"
#include "ElaTheme.h"
#include "ElaMessageBar.h"


extern int uninstall;//卸载控制 1U校园时长 2笙箫终端

unstalltip::unstalltip(QWidget* parent)
    : ElaWidget(parent)
{
    setWindowTitle("卸载确认");
    this->setIsFixedSize(true);
    setWindowModality(Qt::ApplicationModal);
    setWindowButtonFlags(ElaAppBarType::CloseButtonHint);

    //提示文字
    ElaText* Tips_Text = new ElaText("确定要卸载吗?", this);
    Tips_Text->setWordWrap(false);
    Tips_Text->setTextPixelSize(14);


    //================================================[确认卸载按钮]====================================================================
    ElaPushButton* Start_Uninstal_Button = new ElaPushButton("确认卸载", this);
    Start_Uninstal_Button->setBorderRadius(4);
    Start_Uninstal_Button->setLightDefaultColor(ElaThemeColor(ElaThemeType::Light, PrimaryNormal));//PrimaryNormal elathem.h
    Start_Uninstal_Button->setLightHoverColor(ElaThemeColor(ElaThemeType::Light, PrimaryHover));
    Start_Uninstal_Button->setLightPressColor(ElaThemeColor(ElaThemeType::Light, PrimaryPress));
    Start_Uninstal_Button->setLightTextColor(Qt::white);
    Start_Uninstal_Button->setDarkDefaultColor(ElaThemeColor(ElaThemeType::Dark, PrimaryNormal));
    Start_Uninstal_Button->setDarkHoverColor(ElaThemeColor(ElaThemeType::Dark, PrimaryHover));
    Start_Uninstal_Button->setDarkPressColor(ElaThemeColor(ElaThemeType::Dark, PrimaryPress));
    Start_Uninstal_Button->setDarkTextColor(Qt::white);

    connect(Start_Uninstal_Button, &ElaPushButton::clicked, this, [=]()
        {
        /*
         * 卸载功能逻辑
         * 获取路径
         * 1 U校园时长 2 笙箫终端
         */
          QString appDir = QCoreApplication::applicationDirPath();
          if(uninstall==1)
          {
              exePath = QDir(appDir).filePath("USH.exe"); //获取U校园自助时长安装路径
          }
          else if(uninstall==2)
          {
              exePath = QDir(appDir).filePath("SXTeminal.exe");
          }
          /*
           * 卸载功能逻辑
           * 移除文件
           * 工作流程:在上面IF中获取要删除软件的ID号后，进入QFile::exists(exePath)判断文件幸存,如果存在执行删除QFile::remove(exePath)
           */
          if (QFile::exists(exePath))
          {
            // 删除文件
            if (QFile::remove(exePath))
            {
                ElaMessageBar::success(ElaMessageBarType::TopRight, "成功", "成功卸载,请手动刷新页面!", 3500);//信息条
            }
            else
            {
                ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", "卸载失败请重试!", 3000);//信息条 error
            }
          }
          else
          {
              ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", "文件本身不存在!", 3000);//信息条 error
          }
          close();//所有流程结束后,关闭卸载窗口
        });

    //================================================[取消卸载按钮]====================================================================
    ElaPushButton* Cancel_Uninstal_Button = new ElaPushButton("取消卸载", this);
    Cancel_Uninstal_Button->setBorderRadius(6);

    connect(Cancel_Uninstal_Button, &ElaPushButton::clicked, this, [=]() {
        close();
    });

    //====================================================[页面总布局]==================================================================
    //垂直布局容器:文本
    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->addWidget(Tips_Text);
    textLayout->addStretch();

    //平行布局: 文本  这里设置平行布局的原因是:为了后续如果添加logo用
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->addSpacing(30);
    contentLayout->addLayout(textLayout);         //垂直布局容器:文本

    //平行布局:按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(Start_Uninstal_Button);  //开始卸载按钮
    buttonLayout->addWidget(Cancel_Uninstal_Button); //取消卸载按钮

    //垂直:总布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(contentLayout);      //平行布局:Logo 与 文本
    mainLayout->addLayout(buttonLayout);      //平行布局:按钮
}

/*
 * 析构函数
 */
unstalltip::~unstalltip()
{
}
