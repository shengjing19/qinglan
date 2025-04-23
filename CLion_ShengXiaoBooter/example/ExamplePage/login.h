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
    void attemptLogin();// ����һ��������¼��֤�Ĳۺ����������¼��ťʱ���ã�
signals:
    void userstatusUpdata();//�����û��������ź�
private:
    ElaLineEdit* UserEdit; //�û�������
    ElaLineEdit* PwEdit;   //��������
};

#endif // Login_H
