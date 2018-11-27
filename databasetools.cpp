#include "databasetools.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QString>

DataBaseTools* DataBaseTools::tools=NULL;

DataBaseTools* DataBaseTools::getDataBaseTools()
{
    if (tools == NULL)
            tools = new DataBaseTools();
    return tools;
}

void DataBaseTools::connect(const QString &dbName)
{
    path = dbName;
    QSqlDatabase::database().setDatabaseName(dbName);
    if (!QSqlDatabase::database().open()) {
        qDebug() << "Database Open Error!Code:004";
    }
}

QSqlDatabase DataBaseTools::getDB()
{
    return QSqlDatabase::database();
}

DataBaseTools::DataBaseTools()
{
    QSqlDatabase tdb = QSqlDatabase::addDatabase("QSQLITE");

    //    QString path = "D:/qt_pro/hairdresser/db/system.db";
    //    QString path = "/db/system.db";
    path = "C:/Users/user/Desktop/hairdresser/db/system.db";
    QFileInfo fileInfo(path);
    if(!fileInfo.isFile())
        path = QFileDialog::getOpenFileName(NULL,"选择数据库文件",".","SQL Lite数据库(*.db *.db3)");
    tdb.setDatabaseName(path);
    if (!tdb.open()) {
        qDebug() << "Database Open Error!Code:001";
    }
}

QStringList DataBaseTools::getFieldValues(QString tableName,QString fieldName)
{
    QString sql = "select %1 from %2";
    sql = sql.arg(fieldName,tableName);
    QSqlQuery sqlQuery;
    sqlQuery.exec(sql);
    QStringList strList;
    while(sqlQuery.next())
    {
        strList<<sqlQuery.record().value(0).toString();
    }
    return strList;
}

QString DataBaseTools::getDbPath()
{
    return path;
}
