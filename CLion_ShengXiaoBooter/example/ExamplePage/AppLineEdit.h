#ifndef APPLINEEDIT_H
#define APPLINEEDIT_H

#include <ElaWidget.h>
#include "Downloads.h"
#include "ElaLineEdit.h"
#include "CNet.h"

class AppLineEdit : public ElaWidget
{
public:
    AppLineEdit(QWidget* parent1 = nullptr);
    ~AppLineEdit(); // 声明析构函数

private:
    Downloads* _downloadsPage{nullptr};
};

#endif
