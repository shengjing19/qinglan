#ifndef UNSTALLTIP_H
#define UNSTALLTIP_H

#include <ElaWidget.h>


class unstalltip : public ElaWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit unstalltip(QWidget* parent = nullptr);
    ~unstalltip();

private:
    QString exePath = "USH.exe"; //初始值  设置私有全局变量原因:在各个IF中通用


};

#endif // UNSTALLTIP_H
