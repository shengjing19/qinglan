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
    QString exePath = "USH.exe"; //��ʼֵ  ����˽��ȫ�ֱ���ԭ��:�ڸ���IF��ͨ��


};

#endif // UNSTALLTIP_H
