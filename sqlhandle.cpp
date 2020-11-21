#include "sqlhandle.h"

QSettings* SqlHandle::dbconfig;
QSqlDatabase SqlHandle::db;
QSqlQuery* SqlHandle::query;
#include<QSqlDriver>

SqlHandle::SqlHandle(QWidget* parent): QWidget(parent){
    bool ok = false;
    QFile* ini_file = new QFile("./config/db_config.ini");
    if(ini_file -> exists())
        ok = true;
    dbconfig = new QSettings("./config/db_config.ini", QSettings::IniFormat);
    dbconfig -> setIniCodec("UTF-8");
    if(!ok){
        dbconfig -> setValue("sql/dbtype", "QMYSQL");
        dbconfig -> setValue("sql/dbhost", "localhost");
        dbconfig -> setValue("sql/dbuname", "root");
        dbconfig -> setValue("sql/dbpwd", "123456");
        dbconfig -> setValue("sql/dbname", "bdy_test");
        dbconfig -> setValue("sql/dbport", "3306");
    }
    if(!db_init())
        qDebug() << "数据库连接失败，请联系管理员抢救！";
}

SqlHandle::~SqlHandle(){
    db.close();
}

bool SqlHandle::db_init(){
    db = QSqlDatabase::addDatabase(dbconfig -> value("sql/dbtype").toString());
    db.setHostName(dbconfig -> value("sql/dbhost").toString());
    db.setUserName(dbconfig -> value("sql/dbuname").toString());
    db.setPassword(dbconfig -> value("sql/dbpwd").toString());
    db.setDatabaseName(dbconfig -> value("sql/dbname").toString());
    db.setPort(dbconfig -> value("sql/dbport").toInt());
    return db.open();
}

bool SqlHandle::ExecuteNonQuery(QString sql, QVariantList params){
    if(db.transaction()){
        query = new QSqlQuery(sql);
        for(int i = 0; i < params.length(); i ++)
            query -> addBindValue(params[i]);
        if(query -> exec()){
            db.commit();
            return true;
        }
        db.rollback();
    }
    return false;
}

//为提高系统的安全性，这里将 QVariantList 禁用掉
//QVariantList SqlHandle::ExcuteScalar(QString sql, QVariantList params){
//    QVariantList result;
QList<QString> SqlHandle::ExcuteScalar(QString sql, QVariantList params){
    QList<QString> result;
    query = new QSqlQuery(sql);
    for(int i = 0; i < params.length(); i ++)
        query -> addBindValue(params[i]);
    query -> exec();
    while(query -> next()){
        for(int i = 0 ; i < query -> record().count(); i ++)
            result << query -> value(i).toString();
    }
    return result;
}

//QList<QVariantList> SqlHandle::ExcuteDataSet(QString sql, QVariantList params){
//    QList<QVariantList> results;
//    QVariantList result;
QList<QList<QString>> SqlHandle::ExcuteDataSet(QString sql, QVariantList params){
    QList<QList<QString>> results;
    QList<QString> result;
    query = new QSqlQuery(sql);
    for(int i = 0; i < params.length(); i ++)
        query -> addBindValue(params[i]);
    query -> exec();
    while(query -> next()){
        for(int i = 0 ; i < query -> record().count(); i ++){
            result << query -> value(i).toString();
        }
        results << result;
        result.clear();
    }
    return results;
}
