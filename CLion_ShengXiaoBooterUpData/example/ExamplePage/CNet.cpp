/*
 * Curl Network
 * 主要用于从远端服务器读取ini配置信息
 * by shengxiao
 * v1.0.2 2024/11/24
*/


#include "ElaMessageBar.h"

//自建头文件
#include "SimpleIni.h" //ini文件读取
#include "../CURL/include/curl/curl.h"//curl功能


//全局定义
const char* Sver;
const char* Smd5; //服务器版本
const char* Slog; //服务器版本

//软件参数
#define IP "192.168.30.12:5"

//=========================================================变量动态内存分配===============================================================

void setspw(const char* SSver) //动态分配内存 Password
{
    Sver = strdup(SSver);
    if (Sver == NULL) {
        perror("strdup failed");
        exit(1);
    }
}

void setsver(const char* SSmd5) //动态分配内存 启动器服务器版本
{
    Smd5 = strdup(SSmd5);
    if (Smd5 == NULL) {
        perror("strdup failed");
        exit(1);
    }
}

void setslog(const char* SSlog) //动态分配内存 启动器服务器版本
{
    Slog = strdup(SSlog);
    if (Slog == NULL) {
        perror("strdup failed");
        exit(1);
    }
}

//==============函数功能:从INI文件中读取的字符串中的转义序列（如\n）转换为C++字符串中的实际控制字符，并生成合法的字符串字面量==================================================
std::string ProcessEscapeSequences(const std::string& input)
{
    std::string output;
    output.reserve(input.length());
    for (size_t i = 0; i < input.size(); ++i)
    {
        if (input[i] == '\\' && i + 1 < input.size())
        {
            switch (input[i + 1])
            {
                case 'n':  // 换行符
                    output += '\n';
                    ++i;
                    break;
                case 't':  // 制表符
                    output += '\t';
                    ++i;
                    break;
                case '\\': // 反斜杠
                    output += '\\';
                    ++i;
                    break;
                case '"':  // 双引号
                    output += '\"';
                    ++i;
                    break;
                default:   // 未知转义，保留原样
                    output += input[i];
                    break;
            }
        }
        else
        {
            output += input[i];
        }
    }
    return output;
}

//------------------------------------------Serini() 从IIS服务器读取ini配置文件参数----------------------------------------------------------------------
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

void Serini()//Server INI
{
    //Curl初始化
    CURL* curl; //定义curl指针
    CURLcode res; //定义res变量用于存储 libcurl 操作的返回代码
    std::string url = "http://" IP "/sver.ini";
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
            std::string SSver= ini.GetValue("config", "qingver", "");  //获取远端版本更新版本号
            std::string SSmd5 = ini.GetValue("config", "qingmd5", "");//获取远端版本更新MD5校验
            //获取远端版本更新日志
            std::string rawLog = ini.GetValue("config", "updatelog", "");
            std::string processedLog = ProcessEscapeSequences(rawLog);// 处理转义字符
            std::string SSlog = "\"" + processedLog + "\"";// 生成最终字符串字面量（带双引号）

            setspw(SSver.c_str());
            setsver(SSmd5.c_str());
            setslog(SSlog.c_str());

        }
        else//未连接至服务器的变量初始化
        {
            std::string SSver = "00000";//离线时初始值
            std::string SSmd5 = "offline";//离线时初始值
            std::string SSlog = "更新日志\n123\n123\n123\n123";//离线时初始值
            setspw(SSver.c_str());
            setsver(SSmd5.c_str());
            setslog(SSlog.c_str());
        }
    }

}






