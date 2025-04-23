#include <QString>
#include "string"


QString uname17="用户登录";
QString useremail="Email";

/*
 * 软件基础参数
 * 数据库地址位于 MySQLDatabase.h localhost
 */
std::string lver = "1.0.3"; //本地版本
std::string build="       Build: 20250418(Release)";
std::string IP = "192.168.30.12:9"; //基础参数获取IP与软件下载IP
int NETWORK = 1; //检测是否连接至服务器 1连接 0离线
int GX1=1; //更新检查控制 1最新 0不是最新

//检测本地是否已安装
int USH1=0;//Uxiaoyuan app下载检测 1已安装 0未安装
int SXZD1=0;//笙箫终端 app下载检测 1已安装 0未安装

//卸载控制
int uninstall=0;// 卸载控制 1U校园时长 2笙箫终端

//应用软件下载开放检测参数
int USHO=1;//检测u校园20时长下载开放 1开放 0不开放
int KMSO=1;//检测kms下载开放 1开放 0不开放
int SXTO=1;//检测sx teminal下载开放 1开放 0不开放 2测试应用

//服务端参数获取变量定义
const char* spw;
const char* sver; //服务器版本
const char* upsize;//更新包大小
const char* ushsver; //u校园时长服务器版本
const char* ushssize;//u校园文件大小
const char* sxteminalsver; //笙箫终端服务器版本
const char* sxteminalssize;//笙箫终端文件大小
const char* KMSsver; //kms服务器版本
const char* KMSssize;//kms文件大小
const char* httpurlQ;//从服务器获取的主页
const char* httpupdateQ;//从服务器获取的更新链接地址







