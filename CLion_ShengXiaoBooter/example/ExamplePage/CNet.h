#ifndef CNet_H
#define CNet_H

#include <QThread>
#include <QString>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDir>
#include <QFile>

void DownRoot(const char* appname,const char* url);//下载至软件根路径
void DownDeskTop(const char* filename,const char* url);//下载至桌面

//===============下载线程=================
class DownOverThread : public QThread
{
    Q_OBJECT
public:

    //多线程下载
    DownOverThread(const char* _appname, const char* _url) :
        appname(QString::fromUtf8(_appname)),
        url(QString::fromUtf8(_url)) {}

    //运行重写
    void run() override
    {
        // 转换 QString 为 C 字符串（保证生命周期）
        QByteArray appnameData = appname.toUtf8();
        QByteArray urlData = url.toUtf8();
        DownRoot(appnameData.constData(), urlData.constData());


        QString appDir1 = QCoreApplication::applicationDirPath();
        QString exePath = QDir(appDir1).filePath(appname);
        bool fileExists = QFile::exists(exePath);
        emit downloadRootFinished(fileExists, fileExists ? "下载成功!" : "下载出错");
    }


signals:
    // 正确声明信号  // 添加下载结果信号（参数：是否成功、消息内容）
    void downloadRootFinished(bool success, const QString& message);


private:
    QString appname;
    QString url;

};

//===============下载线程[下载至桌面]=================
class DownDeskTopThread : public QThread
{
Q_OBJECT
public:

    //多线程下载
    DownDeskTopThread(const char* _appname, const char* _url,const char* _RetrieveName) : //,const char* RetrieveName
            appname(QString::fromUtf8(_appname)),
            RetrieveName(QString::fromUtf8(_RetrieveName)),
            url(QString::fromUtf8(_url)) {}

    //运行重写
    void run() override
    {
        // 转换 QString 为 C 字符串（保证生命周期）
        QByteArray appnameData = appname.toUtf8();
        QByteArray urlData = url.toUtf8();
        DownDeskTop(appnameData.constData(), urlData.constData());

        QString appDir1 = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QString exePath = QDir(appDir1).filePath(RetrieveName);
        bool fileExists = QFile::exists(exePath);
        emit downloadDeskTopFinished(fileExists, fileExists ? "下载成功!" : "下载出错");
    }


signals:
    // 正确声明信号  // 添加下载结果信号（参数：是否成功、消息内容）
    void downloadDeskTopFinished(bool success, const QString& message);


private:
    QString appname;
    QString RetrieveName;
    QString url;

};


#endif //CNet_H
