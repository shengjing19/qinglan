#ifndef DOWNLOADS_H
#define DOWNLOADS_H

#include "T_BasePage.h"
#include <ElaWidget.h>
#include "ElaScrollPageArea.h"
#include "ElaProgressBar.h"



class Downloads : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit Downloads(QWidget* parent = nullptr);
    void updateContent(/* 需要的参数 */);
    void initializeUI();
    ~Downloads();

private:
    ElaText* Area_Internal_Text;//区域内部文字
    ElaProgressBar* Area_Internal_ProgressBar;//区域内部进度条
    ElaScrollPageArea* Downloads_Area;//下载矩形区域
    QHBoxLayout* Downloads_Area_Layout;//矩形区域内部平行布局

};

#endif // DOWNLOADS_H
