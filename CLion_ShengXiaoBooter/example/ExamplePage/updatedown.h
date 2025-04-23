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
     * 按钮全局原因:当满足最新版本时，开始按钮与取消更新按钮应当被隐藏，最新版本按钮应当显示 所以需要IF判断
     */
    ElaPushButton* Start_Download_Button;
    ElaPushButton* Cancel_Button;
    ElaPushButton* Up_To_Date_Button;

};

#endif // UPDATEDOWN_H
