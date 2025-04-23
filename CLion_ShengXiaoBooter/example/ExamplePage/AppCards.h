#ifndef AppCards_H
#define AppCards_H

#include <ElaWidget.h>
#include "ElaText.h"
#include <QThread>
#include "Downloads.h"
#include "AppLineEdit.h"

class AppCards : public ElaWidget
{
    Q_OBJECT
public:
    explicit AppCards(QWidget* parent = nullptr);
    ~AppCards();// 声明析构函数

private:
    Downloads* _downloadsPage{nullptr};

};

//------------KMS----------------
class T1 : public ElaWidget
{
public:
    explicit T1(QWidget* parent = nullptr);
    ~T1(); // 声明析构函数

private:
    // 成员变量按声明顺序
    Downloads* _downloadsPage{nullptr};
};

//---------SX teminal----------
class T2 : public ElaWidget
{
public:
    explicit T2(QWidget* parent = nullptr);
    ~T2(); // 声明析构函数

private:
    // 成员变量按声明顺序
    Downloads* _downloadsPage{nullptr};
    AppLineEdit* AppLine{nullptr};
};


#endif // AppCards_H
