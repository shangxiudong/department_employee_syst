#include "widget.h"
#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QFile>
#include <QStringList>
#include <QtSql/QSqlQuery>
#include <QDate>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QDebug>
#include "widget.h"
#include "employeeform.h"

/***************************
*连接数据库
***************************/
bool createConnection()

{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("company .dat");
    if (!db.open()) {
        QMessageBox::warning(nullptr, QObject::tr ("DataBase cannot open"),
                            db.lastError().text ()); //调用SQLite的lasterror函数回显错误信息
        return false;
    }

    return true;
}

/***************************
*数据库中填充数据
***************************/
void createFakeData()
{
        QStringList names;
        names << "Eabha Biddell" << "Prentice Hutchison"
        << "Rameesha Davidge" << "Digby Roson" << "Nettah Newarch"
        << "Lewisha Middleton" << "Ahmed Hanmer"
        << "Jordyn-Leigh Lamant" << "Lindsay Bigham"
        << "Kaylay Weir" << "Sofia Weir" << "Coel Spurlock"
        << "Youcef Culpan" << "Lucy-Jasmine Blanchard"
        << "Ally Hodgkin" << "Ara Collinge" << "Luka_Dempster"
        << "Samanta Winster" << "Keri Palin" << "Ruiridh Bisset"
        << "Norman Epworth" << "Kezia Raw"
        << "Kaylan-Thomas Swynford" << "Kashaf Benning"
        << "Norma Yair" << "Edan Bassett" << "Akshat Mcglasson"
        << "Philippa Upton" << "Tylor Rockliff" << "Aqdas Buckznan"
        << "Briana Dowell" << "Querida North" << "Chelsay Botts"
        << "Kishanth Calloway" << "Jan Covington"
        << "Teighan Monson" << "Claudia Mendel" << "Kerra Doe"
        << "Kara Depp" << "Harlie Soole" << "Viggo Streetei"
        << "Ava Cofel" << "Catherine Balderston"
        << "Brendan Gosnay" << "Zhaoyun Haygarth" << "Deri Pepler"
        << "Vicki Hopwood" << "Amitra Bindless" << "Cerhys Hayton"
        << "Gwendoline Westall";
    QSqlQuery query;
    //丢弃旧表
    query.exec("DROP TABLE department");
    query.exec ("DROP TABLE employee");
    query.exec("DROP TABLE location");
    //创建新表
    query.exec ("CREATE TABLE location ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "name VARCHAR(40) NOT NULL)");
    query.exec("CREATE TABLE department ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "name VARCHAR(40) NOT NULL,"
                "locationid INTEGER NOT NULL,"
                "FOREIGN KEY (locationid) REFERENCES location)");
    query.exec("CREATE TABLE employee ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "name VARCHAR(40) NOT NULL,"
                "departmentid INTEGER NOT NULL,"
                "extension INTEGER NOT NULL,"
                "email VARCHAR(40) NOT NULL,"
                "startdate DATE NOT NULL,"
                "FOREIGN KEY (departmentid) REFERENCES department)");
    //填充地区和部门的信息
    query.exec("INSERT INTO location (name) VALUES ("
                "'Floor 18, 1129 Evanston Heights, New York, NY')");
    query.exec("INSERT INTO location (name) VALUES ("
                "'The Shed, Elmtree Drive, Boston, MA')");
    query.exec("INSERT INTO location (name) VALUES("
    "14 Valentine Buildings, Amor Street, Cambridge, MA')");
    query.exec("INSERT INTO location (name) VALUES ("
                "'Bunker Building, Silo Avenue, Los Angeles, CA')");
    query.exec("INSERT INTO department (name, locationid) VALUES ("
                "Sales', 1)");
    query.exec("INSERT INTO department (name, locationid) VALUES ("
                "'Marketing', 2)");
    query.exec("INSERT INTO department (name, locationid) VALUES ("
                "'Processing', 1)");
    query.exec("INSERT INTO department (name, locationid) VALUES ("
                "1 Support', 4)");
    query.exec("INSERT INTO department (name, locationid) VALUES ("
                "'Research', 3)");

//填充雇员的信息

    query.prepare("INSERT INTO employee (name, departmentid,"
                "extension, email, startdate)"
                "VALUES (:namef :departmentid, :extension,"
                ":email, :startdate)");
    for(auto name: names) {
        query.bindValue(":name",name);
        query.bindValue(":departmentid", 1+(std::rand() % 5));
        query.bindValue(":extension", 400 + (std::rand() % 100));
        query.bindValue(":email", name.toLower().replace(" ",".") + "@company.com");
        query.bindValue(":startdate",
                        QDate::currentDate().addDays(-(std::rand() % 3600)));
        query.exec();
    }
}

void showdatabase()
{
    QSqlTableModel* model = new QSqlTableModel(nullptr);
    model->setTable ("departmentr");
    model->setFilter("");
    model->select();
    int idIndex = model->record().indexOf("id");
    int typd_idIndex = model->record().indexOf("name");
    int nameIndex = model->record().indexOf ("locationid");
    if (model->rowCount() == 0) {
        qDebug() << "no data";
    }
    for (int i = 0; i < model->rowCount () ; ++i) {
        QSqlRecord record = model->record(i);
        int id = record.value(idIndex).toInt();
        QString season = record.value(typd_idIndex).toString();
        QString name = record.value(nameIndex).toString();
        qDebug() << id << "   " << season << "   " << name;
    }
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bool fileExit = QFile::exists("company.dat");
    if(!createConnection())
        return 1;
    if (!fileExit)
        createFakeData();
    Widget w;
    w.show();
    return a.exec();
}
