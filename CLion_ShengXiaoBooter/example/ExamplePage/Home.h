#ifndef HOME_H
#define HOME_H

#include "T_BasePage.h"
#include "AppCards.h"

class Home : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit Home(QWidget* parent = nullptr);
    ~Home();

private:
    AppCards* UshoolPage{nullptr};
    T1* KmsPage{nullptr};
    T2* SXTPage{nullptr};
};

#endif // HOME_H
