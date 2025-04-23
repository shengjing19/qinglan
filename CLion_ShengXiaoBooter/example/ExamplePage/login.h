#ifndef Login_H
#define Login_H


#include <ElaWidget.h>
#include "ElaLineEdit.h"

class Login : public ElaWidget
{
    Q_OBJECT
public:
    explicit Login(QWidget* parent = nullptr);
    ~Login();
public slots:
    void attemptLogin();// 声明一个触发登录验证的槽函数（点击登录按钮时调用）
signals:
    void userstatusUpdata();//声明用户名更新信号
private:
    ElaLineEdit* UserEdit; //用户名输入
    ElaLineEdit* PwEdit;   //密码输入
};

#endif // Login_H
