#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui -> setupUi(this);
//    SqlHandle* handle = new SqlHandle(this);
//    QList<QString> uname = {"admin1", "admin2", "admin3", "admin4", "admin5", "admin6", "admin7"};
//    QVariantList params;
//    for(int i = 0; i < uname.length(); i ++){
//        params << 0 << uname[i] << 123;
//        qDebug() << handle -> ExecuteNonQuery("insert into users values(?, ?, ?)", params);
//        params << uname[i];
//        qDebug() << handle -> ExecuteNonQuery("delete from users where uname = ?", params);
//        params.clear();
//    }
//    params << 123;
//    qDebug() << handle -> ExcuteDataSet("select * from users where pwd = ?", params);


}

MainWindow::~MainWindow(){
    delete ui;
}
