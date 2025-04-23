#ifndef UNIQUE_HEADER_NAME_H  // 唯一标识符
#define UNIQUE_HEADER_NAME_H

#include <QObject>
#include <QThread>
#include <curl/curl.h>


/*
 * 下载管理类：封装Curl下载逻辑，支持进度跟踪和MD5校验
 */
class DownloadManager : public QObject
{
Q_OBJECT
public:
    explicit DownloadManager(QObject *parent = nullptr);

//公共反应槽--响应信号（Signal） 声明的槽函数可以在类外部被直接调用或通过信号连接。
public slots:
    //                                                                           增加MD5参数
    void startDownload(const QString &url, const QString &savePath ,const QByteArray &expectedMd5); // 启动下载任务

signals:
    void progressChanged(int percentage); //下载进度持续更新信号--进度更新信号（百分比）
    void downloadFinished(bool success, const QString &errorMsg);  //下载完成信号
    void errorOccurred(const QString &msg);// 错误发生信号

private:
    static size_t writeData(void *ptr, size_t size, size_t nmemb, FILE *stream); // 静态方法：数据写入回调
    static int progressCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);// 静态方法：进度更新回调
};
#endif
