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
