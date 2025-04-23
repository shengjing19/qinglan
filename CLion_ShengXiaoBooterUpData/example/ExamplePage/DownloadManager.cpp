/*
 * 基于Curl下载管理
 * by shengxiao
 * v1.0 2025/04/11
*/

#include <QCryptographicHash>
#include <QFile>
#include "DownloadManager.h"


/*
 * 构造函数：初始化下载管理器
 */
DownloadManager::DownloadManager(QObject *parent) : QObject(parent)
{
    // 初始化成员变量或资源（当前无特殊操作）
}

/*
 * 启动下载任务
 * @param url 文件下载地址
 * @param savePath 本地保存路径
 * @param expectedMd5 预期的MD5校验值
 */
void DownloadManager::startDownload(const QString &url, const QString &savePath,const QByteArray &expectedMd5)
{
    // 初始化Curl
    CURL *curl = curl_easy_init();
    if(!curl)
    {
        emit errorOccurred("Failed to initialize CURL");
        return;
    }

    // 打开本地文件以写入下载内容
    FILE *fp = fopen(savePath.toUtf8().constData(), "wb");
    if(!fp)
    {
        emit errorOccurred("Failed to create output file");
        return;
    }

    // 设置CURL选项
    curl_easy_setopt(curl, CURLOPT_URL, url.toUtf8().constData());          // 设置URL
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);              // 数据写入回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);                        // 写入目标文件流
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);                      // 启用进度回调
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progressCallback); // 进度回调函数
    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, this);                // 传递当前对象指针

    // 执行下载
    CURLcode res = curl_easy_perform(curl);

    // 清理资源
    fclose(fp);
    curl_easy_cleanup(curl);


    if(res == CURLE_OK)
    {
        // 下载完成后进行MD5校验
        QFile file(savePath);
        if(!file.open(QIODevice::ReadOnly))
        {
            emit downloadFinished(false, "Unable to open file for MD5 verification");//无法打开文件进行MD5校验
            return;
        }

        QCryptographicHash hash(QCryptographicHash::Md5);
        if(hash.addData(&file))
        {
            QByteArray actualMd5 = hash.result().toHex();
            if(actualMd5 != expectedMd5.toLower())
            {
                file.remove();//移除下载错误的文件
                emit downloadFinished(false,QString("MD5_Error_DownLoad_Fail"));//.arg(QString(expectedMd5)).arg(QString(actualMd5)));//检验错误输出 .%1预期值.%2实际值
                return;
            }
        }
        emit downloadFinished(true, "");
    }
    else
    {
        //curl连接失败
        QFile::remove(savePath); // 删除无效文件
        emit downloadFinished(false, curl_easy_strerror(res));
    }


}

size_t DownloadManager::writeData(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}


/*
 * 静态方法：处理下载进度更新
 * @param clientp 用户自定义指针（指向DownloadManager实例）
 * @param dltotal 总下载字节数
 * @param dlnow 已下载字节数
 * @param ultotal 总上传字节数（未使用）
 * @param ulnow 已上传字节数（未使用）
 * @return 固定返回0
 */
int DownloadManager::progressCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    Q_UNUSED(ultotal);
    Q_UNUSED(ulnow);

    DownloadManager *manager = static_cast<DownloadManager*>(clientp);
    if(dltotal > 0)
    {
        int progress = static_cast<int>((dlnow * 100) / dltotal);
        emit manager->progressChanged(progress); // 发送进度信号
    }
    return 0;
}