#include "sqlhandle.h"

//将类中的静态成员变量全局化
QSettings* SqlHandle::dbconfig;
QSqlDatabase SqlHandle::db;
QSqlQuery* SqlHandle::query;

/**
 * @brief SqlHandle::SqlHandle
 * @param parent
 * 配置并初始化 MySQL 数据库连接
 */
SqlHandle::SqlHandle(QWidget* parent): QWidget(parent){
    //默认数据库配置文件不存在
    bool ok = false;
    //通过路径获取系统中的文件
    QFile* ini_file = new QFile("./config/db_config.ini");
    //检查该文件路径下的文件是否存在，如果存在，则将 ok 置为 true
    if(ini_file -> exists())
        ok = true;
    //获取配置文件，若文件不存在，则在路径下新建配置文件
    dbconfig = new QSettings("./config/db_config.ini", QSettings::IniFormat);
    //将配置文件的字符集设置为 UTF-8
    dbconfig -> setIniCodec("UTF-8");
    //如果配置文件不存在，则向配置文件中添加配置数据
    if(!ok){
        dbconfig -> setValue("sql/dbtype", "QMYSQL");
        dbconfig -> setValue("sql/dbhost", "localhost");
        dbconfig -> setValue("sql/dbuname", "root");
        dbconfig -> setValue("sql/dbpwd", "123456");
        dbconfig -> setValue("sql/dbname", "bdy_test");
        dbconfig -> setValue("sql/dbport", "3306");
    }
    //开启数据库连接，开启失败则用 QDebug 打印连接失败
    if(!db_init())
        qDebug() << "数据库连接失败，请联系管理员抢救！";
}

/**
 * @brief SqlHandle::~SqlHandle
 * 关闭 MySQL 数据库连接
 */
SqlHandle::~SqlHandle(){
    //关闭数据库连接
    db.close();
}

/**
 * @brief SqlHandle::db_init
 * @return
 * 读取数据库配置并开启 MySQL 数据库连接
 */
bool SqlHandle::db_init(){
    //从配置文件中获取配置信息
    db = QSqlDatabase::addDatabase(dbconfig -> value("sql/dbtype").toString());
    db.setHostName(dbconfig -> value("sql/dbhost").toString());
    db.setUserName(dbconfig -> value("sql/dbuname").toString());
    db.setPassword(dbconfig -> value("sql/dbpwd").toString());
    db.setDatabaseName(dbconfig -> value("sql/dbname").toString());
    db.setPort(dbconfig -> value("sql/dbport").toInt());
    //返回一个是否已开启的数据库连接的布尔值
    return db.open();
}

/**
 * @brief SqlHandle::ExecuteNonQuery
 * @param sql
 * @param params
 * @return
 * 执行数据库的增、删、改操作，返回值为布尔值，判定操作是否执行成功
 */
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

/**
 * @brief SqlHandle::ExcuteScalar
 * @param sql
 * @param params
 * @return
 * 执行 SQL 查询操作，返回单行数据
 */
#if SQLHANDLE_USE_QSTRING   //为提高系统的安全性，这里将 QVariantList 禁用掉
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
#else
QVariantList SqlHandle::ExcuteScalar(QString sql, QVariantList params){
    QVariantList result;
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
#endif

/**
 * @brief SqlHandle::ExcuteDataSet
 * @param sql
 * @param params
 * @return
 * 执行 SQL 查询操作，返回多行数据
 */
#if SQLHANDLE_USE_QSTRING
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
#else
QList<QVariantList> SqlHandle::ExcuteDataSet(QString sql, QVariantList params){
    QList<QVariantList> results;
    QVariantList result;
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
#endif
