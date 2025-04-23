/*
 * ����Curl���ع���
 * by shengxiao
 * v1.0 2025/04/11
*/

#include <QCryptographicHash>
#include <QFile>
#include "DownloadManager.h"


/*
 * ���캯������ʼ�����ع�����
 */
DownloadManager::DownloadManager(QObject *parent) : QObject(parent)
{
    // ��ʼ����Ա��������Դ����ǰ�����������
}

/*
 * ������������
 * @param url �ļ����ص�ַ
 * @param savePath ���ر���·��
 * @param expectedMd5 Ԥ�ڵ�MD5У��ֵ
 */
void DownloadManager::startDownload(const QString &url, const QString &savePath,const QByteArray &expectedMd5)
{
    // ��ʼ��Curl
    CURL *curl = curl_easy_init();
    if(!curl)
    {
        emit errorOccurred("Failed to initialize CURL");
        return;
    }

    // �򿪱����ļ���д����������
    FILE *fp = fopen(savePath.toUtf8().constData(), "wb");
    if(!fp)
    {
        emit errorOccurred("Failed to create output file");
        return;
    }

    // ����CURLѡ��
    curl_easy_setopt(curl, CURLOPT_URL, url.toUtf8().constData());          // ����URL
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);              // ����д��ص�����
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);                        // д��Ŀ���ļ���
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);                      // ���ý��Ȼص�
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progressCallback); // ���Ȼص�����
    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, this);                // ���ݵ�ǰ����ָ��

    // ִ������
    CURLcode res = curl_easy_perform(curl);

    // ������Դ
    fclose(fp);
    curl_easy_cleanup(curl);


    if(res == CURLE_OK)
    {
        // ������ɺ����MD5У��
        QFile file(savePath);
        if(!file.open(QIODevice::ReadOnly))
        {
            emit downloadFinished(false, "Unable to open file for MD5 verification");//�޷����ļ�����MD5У��
            return;
        }

        QCryptographicHash hash(QCryptographicHash::Md5);
        if(hash.addData(&file))
        {
            QByteArray actualMd5 = hash.result().toHex();
            if(actualMd5 != expectedMd5.toLower())
            {
                file.remove();//�Ƴ����ش�����ļ�
                emit downloadFinished(false,QString("MD5_Error_DownLoad_Fail"));//.arg(QString(expectedMd5)).arg(QString(actualMd5)));//���������� .%1Ԥ��ֵ.%2ʵ��ֵ
                return;
            }
        }
        emit downloadFinished(true, "");
    }
    else
    {
        //curl����ʧ��
        QFile::remove(savePath); // ɾ����Ч�ļ�
        emit downloadFinished(false, curl_easy_strerror(res));
    }


}

size_t DownloadManager::writeData(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}


/*
 * ��̬�������������ؽ��ȸ���
 * @param clientp �û��Զ���ָ�루ָ��DownloadManagerʵ����
 * @param dltotal �������ֽ���
 * @param dlnow �������ֽ���
 * @param ultotal ���ϴ��ֽ�����δʹ�ã�
 * @param ulnow ���ϴ��ֽ�����δʹ�ã�
 * @return �̶�����0
 */
int DownloadManager::progressCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    Q_UNUSED(ultotal);
    Q_UNUSED(ulnow);

    DownloadManager *manager = static_cast<DownloadManager*>(clientp);
    if(dltotal > 0)
    {
        int progress = static_cast<int>((dlnow * 100) / dltotal);
        emit manager->progressChanged(progress); // ���ͽ����ź�
    }
    return 0;
}