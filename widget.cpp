#include "widget.h"
#include "employeeform.h"

#include <QtSql>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QHeaderView>
#include <QLabel>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    showdatabase();
    createDepartmentPanel();
    createEmployeePanel();
    //为按键申请内存
    addButton = new QPushButton(tr("&Add Dept."));
    deleteButton = new QPushButton(tr("&Delete Dept."));
    editButton = new QPushButton(tr("&Edit Employees..."));
    quitButton = new QPushButton(tr("&Quit"));
    //按键存放在QDialogButtonBox 里面
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(addButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(editButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::ActionRole);
    //按键连接槽函数
    connect(addButton, SIGNAL(clicked()), this, SLOT(addDepartment()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteDepartment()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editEmployees()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    //布局
    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->addWidget(departmentPanel);
    mainlayout->addWidget(employeePanel);
    mainlayout->addWidget(buttonBox);
    setLayout(mainlayout);
}

Widget::~Widget()
{
}

void Widget::updateEmployeeView()
{
    QModelIndex index = departmentView->currentIndex();
    if(index.isValid()) {
        QSqlRecord record = departmentModel->record(index.row());
        int id = record.value("id").toInt();
        employeeModel->setFilter(QString("departmentid = %1").arg(id));
        employeeLabel->setText(tr("E&ployees in the %1 Department")
                               .arg(record.value("name").toString()));
    } else {
        employeeModel->setFilter("departmentid = -1");
        employeeLabel->setText(tr("E&mployees"));
    }
    employeeModel->select();
    employeeView->horizontalHeader()->setVisible(
                employeeModel->rowCount() > 0);
}

void Widget::addDepartment()
{
    int row = departmentModel->rowCount();
    departmentModel->insertRow(row);
    QModelIndex index = departmentModel->index(row, Department_Name);
    departmentView->setCurrentIndex(index);
    departmentView->edit(index);
}

void Widget::deleteDepartment()
{
    QModelIndex index = departmentView->currentIndex();
    if(!index.isValid())
        return;
    QSqlDatabase::database().transaction();
    QSqlRecord record = departmentModel->record(index.row());
    int id = record.value(Department_Id).toInt();
    int numEmployees = 0;
    QSqlQuery query(QString("SELECT COUNT(*) FROM employee WHERE"
                            "departmentid = 1%").arg(id));
    if(query.next())
        numEmployees = query.value(0).toInt();
    if(numEmployees > 0) {
        int r = QMessageBox::warning(this, tr("Delete Department"),
                                     tr("Delete 1% and all its employees?")
                                     .arg(record.value(Department_Name).toString()),
                                     QMessageBox::Yes | QMessageBox::No);
        if(r == QMessageBox::No) {
            QSqlDatabase::database().rollback();
            return;
        }
    }
    departmentModel->removeRow(index.row());
    departmentModel->submitAll();
    QSqlDatabase::database().commit();

    updateEmployeeView();
    departmentView->setFocus();
}

void Widget::editEmployees()
{
    int employeeId = -1;
    QModelIndex index = employeeView->currentIndex();
    if(index.isValid()) {
        QSqlRecord record = employeeModel->record(index.row());
        employeeId = record.value(Employee_Id).toInt();
    }
    Employeeform form(employeeId, this);
    form.exec();
    updateEmployeeView();
}

void Widget::showdatabase()
{
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable("employee");
    model->setFilter("");
    model->select();
    int idIndex = model->record().indexOf("id");
    int typd_idIndex = model->record().indexOf("name");
    int nameIndex = model->record().indexOf("departmendid");
    int emailIndex = model->record().indexOf("email");
    if(model->rowCount() == 0) {
        qDebug() << "no employee";
    }
    for(int i = 0; i < model->rowCount(); ++i) {
        QSqlRecord record = model->record(i);
        int id = record.value(idIndex).toInt();
        QString season = record.value(typd_idIndex).toString();
        QString name = record.value(nameIndex).toString();
        QString email = record.value(emailIndex).toString();
        qDebug() << "id: " << id << "   " << season << "   " << name << "    " << email;
    }
}

void Widget::createDepartmentPanel()
{
    departmentPanel = new QWidget;
    departmentModel = new QSqlRelationalTableModel(this);
    //将模型关联到表
    departmentModel->setTable("department");
    //设置外键关联的表，外键对应的列，要显示的列
    departmentModel->setRelation(Department_LocationId,
                                 QSqlRelation("location", "id", "name"));
    //设置显示排列顺序
    departmentModel->setSort(Department_Name, Qt::AscendingOrder);
    //设置模型的表头
    departmentModel->setHeaderData(Department_Name, Qt::Horizontal,
                                   tr("Dept."));
    departmentModel->setHeaderData(Department_LocationId, Qt::Horizontal,
                                   tr("Location"));
    departmentModel->select();
    int idIndex = departmentModel->record().indexOf("id");
    int typd_idIndex = departmentModel->record().indexOf("name");
    int nameIndex = departmentModel->record().indexOf("locationid");
    for(int i = 0; i < departmentModel->rowCount(); ++i) {
        QSqlRecord record = departmentModel->record(i);
        int id = record.value(idIndex).toInt();
        QString season = record.value(typd_idIndex).toString();
        QString name = record.value(nameIndex).toString();
        qDebug() << id << "  " << season << "  " << name;

    }
    //模型视图
    departmentView = new QTableView;
    departmentView->setModel(departmentModel);
    //设置下面这一行以使外键关联的文本能够正常显示并能够修改传回模型
    departmentView->setItemDelegate(new QSqlRelationalDelegate(this));
    //选择时高亮，隐藏Department_Id
    departmentView->setSelectionMode(QAbstractItemView::SingleSelection);
    departmentView->setSelectionBehavior(QAbstractItemView::SelectRows);
    departmentView->setColumnHidden(Department_Id, true);
    departmentView->horizontalHeader()->setStretchLastSection(true);

    departmentLabel = new QLabel(tr("Depar&tments"));
    departmentLabel->setBuddy(departmentView);
    //鼠标选择不同的行的时候，更新员工列表
    connect(departmentView->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex &,
                                     const QModelIndex)),
            this, SLOT(updateEmployeeView()));
    //布局
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(departmentLabel);
    layout->addWidget(departmentView);
    departmentPanel->setLayout(layout);
}

void Widget::createEmployeePanel()
{
    employeePanel = new QWidget;
    employeeModel = new QSqlRelationalTableModel(this);
    employeeModel->setTable("employee");
    employeeModel->setRelation(Employee_DepartmentId,
                               QSqlRelation("department", "id", "name"));
    employeeModel->setSort(Employee_Name, Qt::AscendingOrder);
    employeeModel->setHeaderData(Employee_Name, Qt::Horizontal, tr("Name"));
    employeeModel->setHeaderData(Employee_Extension, Qt::Horizontal, tr("Ext."));
    employeeModel->setHeaderData(Employee_Email, Qt::Horizontal, tr("Email"));
    employeeView = new QTableView;
    employeeView->setModel(employeeModel);
    employeeView->setSelectionMode(QAbstractItemView::SingleSelection);
    employeeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    employeeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    employeeView->horizontalHeader()->setStretchLastSection(true);
    employeeView->setColumnHidden(Employee_Id, true);
    employeeView->setColumnHidden(Employee_DepartmentId, true);
    employeeView->setColumnHidden(Employee_StartDate, true);
    employeeLabel = new QLabel(tr("E&mployees"));
    employeeLabel->setBuddy(employeeView);
    //布局
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(employeeLabel);
    layout->addWidget(employeeView);
    employeePanel->setLayout(layout);
}








