#ifndef UPDATEDOWN_H
#define UPDATEDOWN_H

#include <ElaWidget.h>
#include "ElaPushButton.h"

class updatedown : public ElaWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit updatedown(QWidget* parent = nullptr);
    ~updatedown();

private:
    /*
     * ��ťȫ��ԭ��:���������°汾ʱ����ʼ��ť��ȡ�����°�ťӦ�������أ����°汾��ťӦ����ʾ ������ҪIF�ж�
     */
    ElaPushButton* Start_Download_Button;
    ElaPushButton* Cancel_Button;
    ElaPushButton* Up_To_Date_Button;

};

#endif // UPDATEDOWN_H
