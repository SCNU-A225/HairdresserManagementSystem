#ifndef DATABASETOOLS_H
#define DATABASETOOLS_H

#include <qdebug.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>


/***
类名：DataBaseTools
作用：数据库工具类，使用了单例设计模式
***/
class DataBaseTools
{
public:
    static DataBaseTools* getDataBaseTools();//获取工具类接口
    QSqlDatabase getDB();//获取db对象
    void connect(const QString &dbName);//连接数据库，更新db对象
    QStringList getFieldValues(QString tableName,QString fieldName);
    QString getDbPath();//获取数据库文件位置

private:
    DataBaseTools();
    QString path;//数据库文件路径
    static DataBaseTools* tools;
};


#endif // DATABASETOOLS_H
