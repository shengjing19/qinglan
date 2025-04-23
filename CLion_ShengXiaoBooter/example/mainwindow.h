#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ElaWindow.h"

class Home;        //主页
class Downloads;   //下载中心
class AppCenter;   //APP中心
class Login;       //登录页面
class updatedown;  //更新页面
class Setting;     //设置
class ElaContentDialog;
class MainWindow : public ElaWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void initWindow();
    void initEdgeLayout();
    void initContent();

private slots:
    void onUserStatusUpdata(); // 响应用户名变化的槽

private:
    //创建页面对象
    Home* _homePage{nullptr};
    Downloads* _downloadsPage{nullptr};
    AppCenter* _appPage{nullptr};
    Login* _loginPage{nullptr};
    updatedown* _updPage{nullptr};
    Setting* _settingPage{nullptr};
    ElaContentDialog* _closeDialog{nullptr};

#ifdef Q_OS_WIN

#endif
    QString _elaDxgiKey{""};
    QString _viewKey{""};
    QString _aboutKey{""};
    QString _settingKey{""};
    QString _downloadsKey{""};
};
#endif // MAINWINDOW_H
