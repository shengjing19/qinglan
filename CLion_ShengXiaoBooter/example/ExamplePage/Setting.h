#ifndef SETTING_H
#define SETTING_H

#include "T_BasePage.h"
#include "../mainwindow.h"

class ElaRadioButton;
class ElaToggleSwitch;
class ElaComboBox;
class Setting : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit Setting(QWidget* parent = nullptr);
    ~Setting();

private:
    updatedown* _updPage{nullptr};
    ElaComboBox* _themeComboBox{nullptr};
    //ElaToggleSwitch* _micaSwitchButton{nullptr}; //ÔÆÄ¸
};

#endif // SETTING_H
