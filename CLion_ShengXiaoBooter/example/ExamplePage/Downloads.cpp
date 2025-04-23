/*
 * 下载中心页面布局
 * by shengxiao
 * v1.0 Created in 2024/10/04
 * v1.3 2024/11/16
*/

#include "Downloads.h"

//QT功能
#include <QVBoxLayout>
#include <QFile>
#include <QDir>
#include <QCoreApplication>

//src功能引用
#include "ElaScrollPageArea.h"
#include "ElaText.h"
#include "ElaProgressBar.h"

//全局引用
extern int D11;
extern int D11A;
extern int USH1;//Uxiaoyuan app下载检测 1已安装 0未安装
extern int SXZD1;//sxt app下载检测 1已安装 0未安装

/*
 * 构造函数
 */
Downloads::Downloads(QWidget* parent)
    : T_BasePage(parent)
{
    // 调用初始化UI的函数
    initializeUI();
}

//初始化UI
void Downloads::initializeUI()
{
    // 预览窗口标题
    setWindowTitle("Download Center");

    //提示语
    ElaText* Tips_Text = new ElaText(this);
    Tips_Text->setText("这里面显示您正在下载的app\n注意:下载过程中如果出现“未响应”这是正常现象,下载完成后正常\n如果下载出现闪退,请确保安装路径没有中文并使用管理员权限运行启动器");
    Tips_Text->setTextInteractionFlags(Qt::TextSelectableByMouse);
    Tips_Text->setTextPixelSize(14);

    //下载矩形区域上方的提示文字
    ElaText* Area_Tips_Text = new ElaText("下载列表",this);
    Area_Tips_Text->setWordWrap(false);
    Area_Tips_Text->setTextPixelSize(18);

    Downloads_Area = new ElaScrollPageArea(this); // 创建下载矩形区域
    Downloads_Area_Layout = new QHBoxLayout(Downloads_Area);//矩形区域内部使用平行布局

    Area_Internal_Text = new ElaText("无任务", this); // 创建区域内部文字
    Area_Internal_Text->setWordWrap(false);
    Area_Internal_Text->setTextPixelSize(15);
    Downloads_Area_Layout->addWidget(Area_Internal_Text);

    Area_Internal_ProgressBar = new ElaProgressBar(this);// 创建区域内部进度条
    Area_Internal_ProgressBar->setFixedWidth(190);
    Downloads_Area_Layout->addWidget(Area_Internal_ProgressBar);

    //====================================================[页面总布局]==================================================================
    //创建父容器
    QWidget* DownloadWidget = new QWidget(this);
    DownloadWidget->setWindowTitle("下载中心");// 定义标题名
    addCentralWidget(DownloadWidget, true, true, 0);

    // 创建一个垂直布局（QVBoxLayout）并将其应用于 DownloadWidget
    QVBoxLayout* downloadLayout = new QVBoxLayout(DownloadWidget);
    downloadLayout->addWidget(Tips_Text);   //提示文本
    downloadLayout->addSpacing(30); // 间隔10
    downloadLayout->addWidget(Area_Tips_Text); //下载矩形区域上方提示文字
    downloadLayout->addWidget(Downloads_Area); //下载矩形区域
    downloadLayout->addStretch();

}

//UI更新
void Downloads::updateContent() {
    /*
     * 更新任务描述
     * D11 区分下载的项目 0空闲 1U校园 2KMS激活 3笙箫终端 是在AppCards.cpp中定义并全局声明
     * D11A 避免同一个项目下载重复 0默认值 4当前项目正在下载
     * 当在AppCards中点击了某个软件的下载,在AppCards逻辑判断中会将D11设置成相应软件的ID号
     * 而这里读取ID号进行判断是哪个软件要下载，找到后将其软件名通过UI更新函数显示在下载列表中
     * 注意:此CPP文件只是UI上的变换，具体下载实现全在AppCards.cpp中(后续将全部融合到此文件)。此文件顶多配合AppCards.cpp设置避免同一个项目重复下载逻辑判断变量D11A
     * 下载完成后的D11A置为默认值0是在AppCards.cpp中实现
     */
    if (D11 == 1)
    {
        Area_Internal_Text->setText("U校园自助20小时时长");
        D11A=4;
    }
    else if (D11 == 2)
    {
        Area_Internal_Text->setText("KMS激活");
        D11A=4;
    }
    else if (D11 == 3)
    {
        Area_Internal_Text->setText("笙箫终端");
        D11A=4;
    }
    else
    {
        Area_Internal_Text->setText("无任务");
    }

    // 更新进度条状态
    if (D11 == 1 || D11 == 2|| D11==3)
    {
        Area_Internal_ProgressBar->setMinimum(0);
        Area_Internal_ProgressBar->setMaximum(0);
    }
    else
    {
        Area_Internal_ProgressBar->setMinimum(1);
        Area_Internal_ProgressBar->setMaximum(1);
    }

    // 让布局重新计算尺寸
    Downloads_Area->updateGeometry();

}

//文件幸存检测
bool fileExists(const QString &fileName)
{
    QFile file(fileName);
    return file.exists();
}

/*
 * 检查是否安装
 * 在两个地方执行
 * 一是 启动时的检查
 * 二是 进入与切出应用中心页面
 */
void installjc()
{
    QString appDir1 = QCoreApplication::applicationDirPath();
    QString exePath1 = QDir(appDir1).filePath("USH.exe");
    QString exePath2 = QDir(appDir1).filePath("SXTeminal.exe");
    if (fileExists(exePath1))
    {
        USH1=1;// 文件存在
    }
    else
    {
        USH1=0;// 文件不存在
    }

    if (fileExists(exePath2))
    {
        SXZD1=1;// 文件存在
    }
    else
    {
        SXZD1=0;// 文件不存在
    }

}

/*
 * 析构函数
 */
Downloads::~Downloads()
{
}
