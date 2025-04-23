#ifndef UPUI_H
#define UPUI_H

#include "T_BasePage.h"
#include "DownloadManager.h"

class ElaMenu;
class ElaPromotionView;
class UpUI : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit UpUI(QWidget* parent = nullptr);
    ~UpUI();

private:
    DownloadManager *downloadManager;
    QThread *downloadThread;
    QString downloadPath;
};

#endif // UpUI_H
