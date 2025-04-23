/*
 * Curl Network
 * by shengxiao
 * v1.0 Created in 2024/10/10
 * v1.3 2025/04/16
 * gxjc() DownRoot() DownDeskTop()
 * 该文件现存在大量的代码重复,后续再整改，现只实现功能
*/


//QT功能
#include <QStandardPaths>
#include <QCoreApplication>
#include <QDir>

#include "ElaMessageBar.h"
#include "CNet.h"

//自建头文件
#include "SimpleIni.h" //ini文件读取
#include "../CURL/include/curl/curl.h"//curl功能

//全局引用
extern int GX1;
extern int NETWORK;
extern int D11;//区分下载的项目 0默认 1U校园 2KMS激活 3笙箫终端
extern int D11A;//避免同一个项目下载重复 0默认值 4当前项目正在下载
extern int USHO;//U校园时长下载开放检测
extern int KMSO;//kms下载开放检测
extern int SXTO;//笙箫终端下载开放检测
//服务端获取参数 全局引用
extern const char* spw;  //server password
extern const char* sver; //服务器版本
extern const char* upsize;//更新包大小
extern const char* ushsver; //u校园时长服务器版本
extern const char* ushssize;//u校园文件大小
extern const char* sxteminalsver; //笙箫终端服务器版本
extern const char* sxteminalssize;//笙箫终端文件大小
extern const char* KMSsver; //kms服务器版本
extern const char* KMSssize;//kms文件大小
extern const char* httpurlQ;//从服务器获取的主页
extern const char* httpupdateQ;//从服务器获取的更新链接地址

//软件参数
extern std::string IP;
extern std::string lver; //本地版本

//=========================================================变量动态内存分配===============================================================

void setspw(const char* pw) //动态分配内存 Password
{
    spw = strdup(pw);
    if (spw == NULL) {
        perror("strdup failed");
        exit(1);
    }
}

void setsver(const char* ver) //动态分配内存 启动器服务器版本
{
    sver = strdup(ver);
    if (sver == NULL) {
        perror("strdup failed");
        exit(1);
    }
}

void setupsize(const char* size) //动态分配内存 启动器更新大小
{
    upsize = strdup(size);
    if (upsize == NULL) {
        perror("strdup failed");
        exit(1);
    }
}

void setushsver(const char* ushver) //U校园时长 远端版本 变量动态内存分配
{
    ushsver = strdup(ushver);
    if(ushsver == NULL)
    {
        perror("strdup failed");
        exit(1);
    }
}

void setushssize(const char* ushsize) //U校园时长 远端版本 变量动态内存分配
{
    ushssize = strdup(ushsize);
    if(ushssize == NULL)
    {
        perror("strdup failed");
        exit(1);
    }
}

void setsxteminalsver(const char* sxteminalver) // 笙箫终端 远端版本 变量动态内存分配
{
    sxteminalsver=strdup(sxteminalver);
    if(sxteminalsver==NULL)
    {
        perror("strdup failed");
        exit(1);
    }
}

void setsxteminalssize(const char* sxteminalsize) // 笙箫终端 远端大小 变量动态内存分配
{
    sxteminalssize=strdup(sxteminalsize);
    if(sxteminalssize==NULL)
    {
        perror("strdup failed");
        exit(1);
    }
}

void setKMSsver(const char* kmsver) // KMS 远端版本 变量动态内存分配
{
    KMSsver=strdup(kmsver);
    if(KMSsver==NULL)
    {
        perror("strdup failed");
        exit(1);
    }
}

void setKMSssize(const char* kmssize) // KMS 远端大小 变量动态内存分配
{
    KMSssize=strdup(kmssize);
    if(KMSssize==NULL)
    {
        perror("strdup failed");
        exit(1);
    }
}

void sethttpurlQ(const char* httpurl) // KMS 远端大小 变量动态内存分配
{
    httpurlQ=strdup(httpurl);
    if(httpurlQ==NULL)
    {
        perror("strdup failed");
        exit(1);
    }
}

void sethttpupdateQ(const char* httpurlupdate) // KMS 远端大小 变量动态内存分配
{
    httpupdateQ=strdup(httpurlupdate);
    if(httpupdateQ==NULL)
    {
        perror("strdup failed");
        exit(1);
    }
}

//=================================================================================================================================================

//---------------------------------------- gxjc()-----------------------------------------------------------------------
size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* s)
{
    // 计算新接收数据的总字节数
    size_t newLength = size * nmemb;
    // 获取当前字符串的长度
    size_t oldLength = s->size();
    try
    {
        s->resize(oldLength + newLength); // 调整 std::string 的大小以容纳新接收到的数据
    }
    catch (std::bad_alloc& e) // 内存分配失败时，返回 0 表示错误
    {
        return 0;
    }
    std::copy((char*)contents, (char*)contents + newLength, s->begin() + oldLength); // 将接收到的数据复制到 std::string 中
    return size * nmemb; // 返回成功处理的数据字节数
}

void gxjc()//更新函数
{
    //Curl初始化
    CURL* curl; //定义curl指针
    CURLcode res; //定义res变量用于存储 libcurl 操作的返回代码
    std::string url = "http://" + IP + "/sver.ini";
    std::string response; //定义接收对象
    curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L); //连接超时时间
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); //连接地址
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback); //设置 libcurl 的回调函数 处理接收到的数据 通过回调函数
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response); //设置 libcurl 的数据指针，用于回调函数中接收到的用户数据 赋给定义的接收对象
        res = curl_easy_perform(curl);
        if (res == CURLE_OK) //连接服务器
        {
            //SimpleIni初始化
            CSimpleIniA ini;
            SI_Error rc = ini.LoadData(response.c_str(), response.size()); //从响应字符串中加载数据到 ini 对象 数据与大小
            if (rc < 0)
            {
                //B233 = 1;
            }
            std::string pw= ini.GetValue("config", "pw", "");  //测试APP的 下载密码
            std::string ver = ini.GetValue("config", "ver", "");//获取远端版本
            std::string size = ini.GetValue("size", "mb", "");//获取更新数据包大小
            std::string ushver = ini.GetValue("config", "uver", "");//获取远端U校园时长版本
            std::string ushsize = ini.GetValue("size", "usize", "");//获取远端U校园时长版本更新大小
            std::string sxteminalver = ini.GetValue("config", "sxtver", "");//获取远端笙箫终端版本
            std::string sxteminalsize = ini.GetValue("size", "sxtsize", "");//获取远端笙箫终端版本更新大小
            std::string kmsver = ini.GetValue("config", "kmsver", "");//获取远端KMS版本
            std::string kmssize = ini.GetValue("size", "kmssize", "");//获取远端KMS版本更新大小
            std::string httpurl= ini.GetValue("config", "http", "");//获取远端主页URL
            std::string httpurlupdate= ini.GetValue("config", "httpupdate", "");//获取远端更新链接
            //获取远端下载开放规则 1开放 0不开放 2测试开发
            std::string usl = ini.GetValue("usc", "uopen", "");
            std::string kms = ini.GetValue("kms", "kopen", "");
            std::string sxt = ini.GetValue("sxt", "sxtopen", "");
            //String转int
            USHO=std::stoi(usl);
            KMSO=std::stoi(kms);
            SXTO=std::stoi(sxt);
            //为字符串动态内存分配
            setspw(pw.c_str());
            setupsize(size.c_str());
            setsver(ver.c_str());
            setushsver(ushver.c_str());
            setushssize(ushsize.c_str());
            setsxteminalsver(sxteminalver.c_str());
            setsxteminalssize(sxteminalsize.c_str());
            setKMSsver(kmsver.c_str());
            setKMSssize(kmssize.c_str());
            sethttpurlQ(httpurl.c_str());
            sethttpupdateQ(httpurlupdate.c_str());
        }
        else//未连接至服务器的变量初始化
        {
            NETWORK=0;
            std::string pw = "00000";
            std::string ver = "offline";//获取远端版本
            std::string size = "offine";//获取更新数据包大小
            std::string ushver ="offline";
            std::string ushsize ="offline";
            std::string sxteminalver ="offline";
            std::string sxteminalsize ="offline";
            std::string kmsver ="offline";
            std::string kmssize ="offline";
            std::string httpurl= "offline";
            std::string httpurlupdate="offline";
            setspw(pw.c_str());
            setupsize(size.c_str());
            setsver(ver.c_str());
            setushsver(ushver.c_str());
            setushssize(ushsize.c_str());
            setsxteminalsver(sxteminalver.c_str());
            setsxteminalssize(sxteminalsize.c_str());
            setKMSsver(kmsver.c_str());
            setKMSssize(kmssize.c_str());
            sethttpurlQ(httpurl.c_str());
            sethttpupdateQ(httpurlupdate.c_str());
            ElaMessageBar::warning(ElaMessageBarType::TopRight, "离线", "未连接至服务器,无法访问在线服务",10000);//信息条
        }
    }

    if(NETWORK==0)//离线
    {
        GX1=1; //最新
    }
    else//在线
    {
        if(lver==sver)
        {
            GX1=1; //最新
        }
        else
        {
            GX1=0;  //不是最新
        }
    }

}
//-------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------DownRoot() DownDeskTop()---------------------------------------------------------------------
// 数据写入回调函数，用于将下载的数据写入文件

static size_t write_DownRoot(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);// 将数据写入指定的文件流
    return written;
}

static size_t write_DownDeskTop(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);// 将数据写入指定的文件流
    return written;
}

void DownRoot(const char* appname,const char* url)//下载至根目录
{

    CURL* curl; //定义curl指针
    FILE* fp;
    CURLcode res;
    QString appDir = QCoreApplication::applicationDirPath();
    QString outexePath = QDir(appDir).filePath(appname);
    curl_global_init(CURL_GLOBAL_DEFAULT); // 全局初始化CURL库
    curl = curl_easy_init();// 初始化CURL句柄
    if (curl)
    {
        fp = fopen(outexePath.toUtf8().constData(), "wb");// 打开本地文件以保存数据
        curl_easy_setopt(curl, CURLOPT_URL, url); // 设置要下载的文件的URL
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_DownRoot);// 设置数据写入回调函数
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp); // 设置数据写入回调函数的参数
        res = curl_easy_perform(curl);  // 执行传输操作
        if (res != CURLE_OK)
        {
            //下载失败
        }

        curl_easy_cleanup(curl);  // 清理CURL句柄
        fclose(fp); // 关闭本地文件
    }
    curl_global_cleanup();// 全局清理CURL库
}


void DownDeskTop(const char* filename,const char* url) //下载至桌面
{
    CURL* curl; //定义curl指针
    FILE* fp;
    CURLcode res;
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    //QString filename="/kms.zip";
    QString outfilename = desktopPath + filename;
    curl_global_init(CURL_GLOBAL_DEFAULT); // 全局初始化CURL库
    curl = curl_easy_init();// 初始化CURL句柄
    if (curl)
    {
        fp = fopen(outfilename.toUtf8().constData(), "wb");// 打开本地文件以保存数据
        curl_easy_setopt(curl, CURLOPT_URL, url); // 设置要下载的文件的URL
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_DownDeskTop);// 设置数据写入回调函数
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp); // 设置数据写入回调函数的参数
        res = curl_easy_perform(curl);  // 执行传输操作

        if (res != CURLE_OK)
        {
            //下载失败
        }
        curl_easy_cleanup(curl);  // 清理CURL句柄
        fclose(fp); // 关闭本地文件
    }
    curl_global_cleanup();// 全局清理CURL库
}

//-------------------------------------------------------------------------------------------------------------------------------------------


