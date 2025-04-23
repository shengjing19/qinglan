/*
 * 窗口基本页面
 * by elawidgettools
 * 重要页面--各种小窗口的显示支撑
*/
#include "T_BasePage.h"

#include <QHBoxLayout>
#include <QVBoxLayout>


#include "ElaTheme.h"

T_BasePage::T_BasePage(QWidget* parent)
    : ElaScrollPage(parent)
{
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=]() {
        if (!parent)
        {
            update();
        }
    });
}

T_BasePage::~T_BasePage()
{
}
