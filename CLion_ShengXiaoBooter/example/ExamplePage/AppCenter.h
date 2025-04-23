#ifndef APPCENTER_H
#define APPCENTER_H

#include "T_BasePage.h"
#include <QThread>

#include "ElaScrollPageArea.h"
#include "ElaPushButton.h"
#include "unstalltip.h"

class AppCenter : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit AppCenter(QWidget* parent = nullptr);
    void updateUI(/* 需要的参数 */);
    void initializeUIA();
    ~AppCenter();

private:
    unstalltip* _unstalldPage{nullptr};  //卸载界面
    ElaPushButton* refreshButton{nullptr}; //界面刷新按钮
    QHBoxLayout* Application_List_Area_Layout;//矩形区域内部平行总布局

    //APP1
    ElaText* App1_Name_Text;//app1名
    ElaScrollPageArea* Application_List_Area; //app列表矩形框1_U校园
    ElaPushButton* Open_Button;      //app打开按钮
    ElaPushButton* Uninstall_Button; //app卸载按钮

    //App2
    ElaText* App2_Name_Text;//app2名
    ElaScrollPageArea* Application_List_Area2;//app列表矩形框2_笙箫终端
    ElaPushButton* app2_Open_Button;//app打开按钮
    ElaPushButton* app2_Uninstall_Button;//app卸载按钮

};

//异步执行并保持窗口响应 将 system() 调用放入一个单独的线程中执行，以避免阻塞主线程
class SystemThread : public QThread
{
public:
    QString exePath;
    SystemThread(const QString &path) : exePath(path) {}
    void run() override
    {
        system(exePath.toStdString().c_str());
    }
};

#endif // APPCENTER_H
