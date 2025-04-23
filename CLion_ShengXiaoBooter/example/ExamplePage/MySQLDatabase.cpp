/*
 * MySQL部分功能再封装
 * 引用于开源程序:QT-管理系统demo by 电石QwQ
*/
#include "MySQLDatabase.h"
#include <QDebug>
#include <qsqlerror.h>
#include <qsqlrecord.h>

// 初始化静态成员变量
MySQLDatabase* MySQLDatabase::instance = nullptr;
QMutex MySQLDatabase::mutex;

// 构造函数
MySQLDatabase::MySQLDatabase(const QString& host, int port, const QString& database, const QString& username, const QString& password, QObject* parent)
    : QObject(parent)
{
    if (!QSqlDatabase::isDriverAvailable("QMYSQL"))
    {
        qWarning() << "QMYSQL driver is not available.";
        return;
    }

    // 初始化数据库连接
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(host);
    db.setPort(port); // 设置端口号
    db.setDatabaseName(database);
    db.setUserName(username);
    db.setPassword(password);

    // 尝试连接数据库
    if (!db.open())
    {
        qWarning() << "Failed to connect to database:" << db.lastError().text();
    }
}

// 析构函数
MySQLDatabase::~MySQLDatabase()
{
    if (db.isOpen()) {
        db.close(); // 关闭数据库连接
    }
}

// 获取单例实例
MySQLDatabase* MySQLDatabase::getInstance(const QString& host, int port, const QString& database, const QString& username, const QString& password)
{
    if (instance == nullptr) {
        mutex.lock(); // 加锁，确保线程安全
        if (instance == nullptr) {
            instance = new MySQLDatabase(host, port, database, username, password);
        }
        mutex.unlock();
    }
    return instance;
}

// 执行查询
QList<QStringList> MySQLDatabase::query(const QString& sql)
{
    QList<QStringList> result;

    QSqlQuery query(db);
    if (!query.exec(sql))
    {
        qWarning() << "Query failed:" << query.lastError().text();
        return result;
    }

    // 获取查询结果
    while (query.next())
    {
        QStringList row;
        for (int i = 0; i < query.record().count(); ++i) {
            row.append(query.value(i).toString());
        }
        result.append(row);
    }

    return result;
}

// 获取表头信息，包含字段注释
QStringList MySQLDatabase::getTableHeaders(const QString& tableName)
{
    QStringList headers;

    QString sql = QString("SELECT COLUMN_COMMENT FROM information_schema.COLUMNS "
                          "WHERE TABLE_SCHEMA = '%1' AND TABLE_NAME = '%2' "
                          "ORDER BY ORDINAL_POSITION")
                      .arg(db.databaseName())
                      .arg(tableName);

    QSqlQuery query(db);
    if (!query.exec(sql)) {
        qWarning() << "Failed to get table headers:" << query.lastError().text();
        return headers;
    }

    // 获取表头信息
    while (query.next()) {
        headers.append(query.value(0).toString());
    }

    return headers;
}

// 执行非查询语句
bool MySQLDatabase::executeNonQuery(const QString& sql)
{
    QSqlQuery query(db);
    if (!query.exec(sql)) {
        qWarning() << "Execute non-query failed:" << query.lastError().text();
        return false;
    }
    return true;
}
