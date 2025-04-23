#ifndef MYSQLDATABASE_H
#define MYSQLDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>
#include <QStringList>
#include <QMutex>

class MySQLDatabase : public QObject
{
    Q_OBJECT

private:
    // 私有构造函数
    explicit MySQLDatabase(
        const QString& host = "localhost",    // 数据库主机名
        int port = 3306,                      // 数据库端口号（默认3306）
        const QString& database = "qinglan",     // 数据库名
        const QString& username = "root",     // 用户名
        const QString& password = "root",   // 密码
        QObject* parent = nullptr
        );
    ~MySQLDatabase();

    // 禁止拷贝和赋值
    MySQLDatabase(const MySQLDatabase&) = delete;
    MySQLDatabase& operator=(const MySQLDatabase&) = delete;

    // 单例实例和互斥锁
    static MySQLDatabase* instance;
    static QMutex mutex;

    // 数据库连接对象
    QSqlDatabase db;

public:
    // 获取单例实例
    static MySQLDatabase* getInstance(
        const QString& host = "localhost",    // 数据库主机名
        int port = 3306,                      // 数据库端口号（默认3306）
        const QString& database = "qinglan",     // 数据库名
        const QString& username = "root",     // 用户名
        const QString& password = "root"    // 密码
        );

    // 执行查询，返回结果集
    QList<QStringList> query(const QString& sql);

    // 获取表的字段名（表头信息）
    QStringList getTableHeaders(const QString& tableName);

    // 执行非查询语句（如 INSERT、UPDATE、DELETE）
    bool executeNonQuery(const QString& sql);
};

#endif // MYSQLDATABASE_H
