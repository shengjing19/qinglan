#ifndef UNIQUE_HEADER_NAME_H  // Ψһ��ʶ��
#define UNIQUE_HEADER_NAME_H

#include <QObject>
#include <QThread>
#include <curl/curl.h>


/*
 * ���ع����ࣺ��װCurl�����߼���֧�ֽ��ȸ��ٺ�MD5У��
 */
class DownloadManager : public QObject
{
Q_OBJECT
public:
    explicit DownloadManager(QObject *parent = nullptr);

//������Ӧ��--��Ӧ�źţ�Signal�� �����Ĳۺ������������ⲿ��ֱ�ӵ��û�ͨ���ź����ӡ�
public slots:
    //                                                                           ����MD5����
    void startDownload(const QString &url, const QString &savePath ,const QByteArray &expectedMd5); // ������������

signals:
    void progressChanged(int percentage); //���ؽ��ȳ��������ź�--���ȸ����źţ��ٷֱȣ�
    void downloadFinished(bool success, const QString &errorMsg);  //��������ź�
    void errorOccurred(const QString &msg);// �������ź�

private:
    static size_t writeData(void *ptr, size_t size, size_t nmemb, FILE *stream); // ��̬����������д��ص�
    static int progressCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);// ��̬���������ȸ��»ص�
};
#endif
