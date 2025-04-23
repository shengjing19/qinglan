#ifndef ABOUT_H
#define ABOUT_H

#include <ElaWidget.h>

#include "T_BasePage.h"
#include "ElaScrollPageArea.h"
#include "ElaProgressBar.h"

class About : public ElaWidget
{
    Q_OBJECT
public:
    explicit About(QWidget* parent = nullptr);
    ~About();

private:

};

#endif // ABOUT_H
