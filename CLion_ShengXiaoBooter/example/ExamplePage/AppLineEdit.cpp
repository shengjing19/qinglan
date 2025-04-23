/*
 * 测试应用程序下载密码布局窗口与下载
 * by shengxiao
 * v1.0 Created in 2024/11/17
 * v1.1 2025/04/13
*/

#include "AppLineEdit.h"

//QT功能
#include <QDir>
#include <QVBoxLayout>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QTimer>

//src功能集
#include "ElaLineEdit.h"
#include "ElaText.h"
#include "ElaMessageBar.h"
#include "ElaTheme.h"
#include "ElaPushButton.h"

//全局引用
extern int D11;
extern int D11A;
extern const char* spw;
extern int SXZD1;//检测笙箫终端是否已安装
extern std::string IP;

//判断字符串内是否是纯数字
bool is_numeric(const std::string& str)
{
    return std::all_of(str.begin(), str.end(), ::isdigit);
}

/*
 * 构造函数：测试应用程序下载密码页面布局与具体实现逻辑
 */
AppLineEdit::AppLineEdit(QWidget* parent)
    : ElaWidget(parent)
{
    setWindowTitle("测试密码");
    this->setIsFixedSize(true);
    setWindowModality(Qt::ApplicationModal);
    setWindowButtonFlags(ElaAppBarType::CloseButtonHint);

    //提示文本
    ElaText* tipsEdit = new ElaText("请输入:",this);
    tipsEdit->setWordWrap(false);
    tipsEdit->setTextPixelSize(14);

    //输入框
    ElaLineEdit* userline =new ElaLineEdit(this);
    userline->setFixedSize(350, 28);

    //================================================[开始下载按钮]====================================================================
    ElaPushButton* DownloadButton = new ElaPushButton("下载", this);
    DownloadButton->setBorderRadius(4);
    DownloadButton->setLightDefaultColor(ElaThemeColor(ElaThemeType::Light, PrimaryNormal));//PrimaryNormal elathem.h
    DownloadButton->setLightHoverColor(ElaThemeColor(ElaThemeType::Light, PrimaryHover));
    DownloadButton->setLightPressColor(ElaThemeColor(ElaThemeType::Light, PrimaryPress));
    DownloadButton->setLightTextColor(Qt::white);
    DownloadButton->setDarkDefaultColor(ElaThemeColor(ElaThemeType::Dark, PrimaryNormal));
    DownloadButton->setDarkHoverColor(ElaThemeColor(ElaThemeType::Dark, PrimaryHover));
    DownloadButton->setDarkPressColor(ElaThemeColor(ElaThemeType::Dark, PrimaryPress));
    DownloadButton->setDarkTextColor(Qt::white);
    
    _downloadsPage =new Downloads();
    /*
     * 密码校验与开始下载判断逻辑
     * UPW为用户输入的密码
     * SERPW是存放在远端服务器的下载密钥
     * 当UPW==SERPW时才能进入开始下载判断逻辑
     */
    connect(DownloadButton, &ElaPushButton::clicked, this, [=](){
        QString userpw0=userline->text();//接收输入文本
        std::string userpw = userpw0.toStdString();//将QString转为std::string
        if ((!is_numeric(userpw))||userpw.empty())//判断是否在'0'到'9'的范围内 排除字母输入与空输入
        {
            ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", "密码出错", 4000);
            return ; //中断后续逻辑判断
        }
        int UPW = std::stoi(userpw);//将std::string转int整数
        int SERPW=std::stoi(spw);
        if(UPW==SERPW)
        {
            D11=3; //区分下载的项目 0空闲 1U校园 2KMS激活 3笙箫终端 是在AppCards.cpp中定义并全局声明
            ElaMessageBar::success(ElaMessageBarType::TopRight, "成功", "已添加到下载目录!", 2000);
            close();//关闭密码输入窗口
            _downloadsPage->updateContent();
            _downloadsPage->setFixedSize(950, 480);//长  高
            _downloadsPage->setWindowIcon(QIcon(":/include/Image/control/AnimatedIcon.png"));
            _downloadsPage->show();//打开下载页面窗口

            //多线程
            std::string DownLoadURL="http://" + IP + "/softwares/SXTeminal.exe";
            DownOverThread* overThread = new DownOverThread("SXTeminal.exe",DownLoadURL.c_str());
            overThread->start();  // 启用线程

            // 连接下载完成回显信号
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
        else
        {
            ElaMessageBar::error(ElaMessageBarType::TopRight, "失败", "密码出错", 4000);
        }
        //close();
    });

    //================================================[取消下载按钮]====================================================================
    ElaPushButton* cancelButton = new ElaPushButton("取消", this);
    cancelButton->setBorderRadius(6);
    connect(cancelButton, &ElaPushButton::clicked, this, [=]() {
        close();
    });

    //====================================================[页面总布局]==================================================================
    //垂直布局;文字与输入框
    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->setSpacing(15);
    textLayout->setContentsMargins(0, 10, 0, 0);
    textLayout->addWidget(tipsEdit);//提示输入信息
    textLayout->addWidget(userline);//输入框
    textLayout->addStretch();

    //平行布局 将垂直布局:文字与输入框再单独放在平行布局中 方便日后扩展
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->addSpacing(30);
    contentLayout->addLayout(textLayout);//文字与输入框

    //平行布局:按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(DownloadButton);//下载按钮
    buttonLayout->addWidget(cancelButton);//取消按钮

    //主垂直布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(contentLayout);//备用扩展布局，现存文字与输入框
    mainLayout->addLayout(buttonLayout);// 按钮的平行布局
}

/*
 * 析构函数
 */
AppLineEdit::~AppLineEdit()
{
    // 如果有需要清理的资源，放在这里
}

