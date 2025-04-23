/*
 * 登录页面布局
 * by shengxiao
 * v1.0 Created in 2025/03/20
 * v1.1 2025/04/12
*/
#include "Login.h"

//QT功能
#include <QHBoxLayout>

//Src功能引用
#include "ElaMessageBar.h"
#include "ElaLineEdit.h"
#include "ElaPushButton.h"
#include "ElaText.h"

//再封装SQL引用
#include "MySQLDatabase.h"

//全局引用
extern QString uname17;
extern QString useremail;

//主代码
Login::Login(QWidget* parent)
    : ElaWidget(parent)
{
    setWindowTitle("登录");
    this->setIsFixedSize(true);
    setWindowModality(Qt::ApplicationModal);
    setWindowButtonFlags(ElaAppBarType::CloseButtonHint);

    // 用户名及用户名输入框
    ElaText* User = new ElaText("用户名:", this);
    QFont UserFont = User->font();
    UserFont.setWeight(QFont::Bold); //加粗字体
    User->setFont(UserFont);
    User->setWordWrap(false);
    User->setTextPixelSize(16);

    UserEdit = new ElaLineEdit(this); // 用户名输入框
    // 设置输入框的固定高度
    UserEdit->setFixedHeight(35); // 高度
    UserEdit->setFixedWidth(220); // 宽度
    UserEdit->setPlaceholderText("请输入内容");  // 设置占位文本（通过参数传入）
    UserEdit->setClearButtonEnabled(true);    // 设置是否启用清除按钮（通过参数传入）

    // 创建水平布局--用户及用户输入框
    QHBoxLayout* User_Input_Layout = new QHBoxLayout();
    User_Input_Layout->addWidget(User);
    User_Input_Layout->addWidget(UserEdit);

    // 密码及密码输入框
    ElaText* Pw = new ElaText("密码:", this);
    QFont PwFont = Pw->font();
    PwFont.setWeight(QFont::Bold); //加粗字体
    Pw->setFont(PwFont);
    Pw->setWordWrap(false);
    Pw->setTextPixelSize(16);

    PwEdit = new ElaLineEdit(this); // 密码输入框
    // 设置输入框的固定高度
    PwEdit->setFixedHeight(35); // 高度
    PwEdit->setFixedWidth(220); // 宽度
    PwEdit->setPlaceholderText("请输入内容"); // 设置占位文本（通过参数传入）
    PwEdit->setClearButtonEnabled(true); // 设置是否启用清除按钮（通过参数传入）

    // 创建水平布局--密码及密码输入框
    QHBoxLayout* Pass_Input_Layout = new QHBoxLayout();
    // 添加输入框到布局中（靠左侧）
    Pass_Input_Layout->addWidget(Pw);
    Pass_Input_Layout->addWidget(PwEdit);

    // 按钮
    ElaPushButton* confirmButton = new ElaPushButton("确认", this);
    ElaPushButton* cancelButton = new ElaPushButton("取消", this);

    // 点击取消，页面就关闭
    connect(cancelButton, &ElaPushButton::clicked, this, &Login::close);

    // 连接登录按钮点击事件到 attemptLogin 槽
    connect(confirmButton, &QPushButton::clicked, this, &Login::attemptLogin);

    //水平布局:按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(50, 30, 50, 30);
    buttonLayout->addWidget(confirmButton); //确定按钮
    buttonLayout->addWidget(cancelButton);  //取消按钮

    //垂直布局:用户名_密码输入框_按钮
    QVBoxLayout* contentLayout = new QVBoxLayout();
    contentLayout->addSpacing(10);
    contentLayout->setSpacing(15);
    contentLayout->addLayout(User_Input_Layout);
    contentLayout->addLayout(Pass_Input_Layout);
    contentLayout->addLayout(buttonLayout);

    //垂直布局:主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(50, 25, 50, 25);
    mainLayout->addLayout(contentLayout);

}

//触发登录验证的槽函数
void Login::attemptLogin()
{
    QString username = UserEdit->text();
    QString password = PwEdit->text();

    if (username.isEmpty() || password.isEmpty())
    {
        ElaMessageBar::warning(ElaMessageBarType::Top, "警告", "输入框不能为空，请填写完整信息。", 3000, this);
        return;
    }

    // 数据库实例
    MySQLDatabase* db = MySQLDatabase::getInstance();

    // 构造 SQL 查询语句
    QString sql = QString("SELECT * FROM user where username ='%1' AND password='%2'").arg(username).arg(password);

    // 执行查询
    QList<QStringList> data = db->query(sql);

    //QString 转 QList
    QList<QStringList> userpw={{username,password}};

    //判断逻辑
    if (userpw.value(0).value(0)==data.value(0).value(1)&&userpw.value(0).value(1)==data.value(0).value(2))
    {
        uname17=username; // 更新全局变量--用户名
        useremail=data.value(0).value(3);// 更新全局变量--用户邮箱
        emit userstatusUpdata(); // 发射信号
        this->close(); // 关闭登录窗口
    }
    else
    {
        // 密码错误
        ElaMessageBar::error(ElaMessageBarType::Top, "错误", "密码或用户名错误", 3000, this);
    }
}


/*
 * 析构函数
 */
Login::~Login()
{
}
