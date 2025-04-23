#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ElaWindow.h"
class UpUI;      //主页
class About;     //关于页面
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

private:
    ElaContentDialog* _closeDialog{nullptr};
    UpUI* _UpUI{nullptr};
    About* _About{nullptr};
#ifdef Q_OS_WIN

#endif
    QString _elaDxgiKey{""};
    QString _viewKey{""};

};
#endif // MAINWINDOW_H
