#include "Employeeform.h"
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QSqlRelationalTableModel>
#include <QDataWidgetMapper>
#include <QSqlRelationalDelegate>
#include <QSqlRecord>
#include <QHBoxLayout>

Employeeform::Employeeform(int id, QWidget *parent)
{
//各个窗口部件申请内存
    nameLabel = new QLabel(tr("Na&me:"));
    nameEdit = new QLineEdit;
    nameLabel->setBuddy(nameEdit);

    departmentLabel = new QLabel(tr("Department:"));
    departmentComboBox = new QComboBox;
    departmentLabel->setBuddy(departmentComboBox);

    extensionLabel = new QLabel(tr("Extension:"));
    extensionLineEdit = new QLineEdit;
    extensionLabel->setBuddy(extensionLineEdit);
    extensionLineEdit->setValidator(new QIntValidator(0,9999,this));

    emailLabel = new QLabel(tr("Email:"));
    emailEdit = new QLineEdit;
    emailLabel->setBuddy(emailEdit);

    startDateLabel = new QLabel(tr("Start Date:"));
    startDateEdit = new QDateEdit;
    startDateEdit->setCalendarPopup(true);
    QDate today = QDate::currentDate();
    startDateEdit->setDateRange(today.addDays(-90), today.addDays(90));
    startDateLabel->setBuddy(startDateEdit);
    //所有按键申请内存
    firstButton = new QPushButton(tr("<< &First"));
    previousButton = new QPushButton(tr("< Previous"));
    nextButton = new QPushButton(tr("&Next >"));
    lastButton = new QPushButton(tr("Last >>"));
    addButton = new QPushButton(tr("Add"));
    deleteButton = new QPushButton(tr("Delete"));
    closeButton = new QPushButton(tr("Close"));
    //底下的按键放在按键容器中
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(addButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::AcceptRole);


    tableModel = new QSqlRelationalTableModel(this);
    tableModel->setTable("employee");
    tableModel->setRelation(Employee_DepartmentId,
                            QSqlRelation("department","id", "name"));
    tableModel->setSort(Employee_Name, Qt::AscendingOrder);
    tableModel->select();


    QSqlTableModel *relationModel = tableModel->relationModel(Employee_DepartmentId);
    departmentComboBox->setModel(relationModel);
    departmentComboBox->setModelColumn( relationModel->fieldIndex ("name"));

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(tableModel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(nameEdit, Employee_Name);
    mapper->addMapping(departmentComboBox, Employee_DepartmentId);
    mapper->addMapping(extensionLineEdit, Employee_Extension);
    mapper->addMapping (emailEdit, Employee_Email);
    mapper->addMapping(startDateEdit, Employee_StartDate);

    if (id != -1) {
        for (int row =0; row < tableModel->rowCount(); ++row) {
            QSqlRecord record = tableModel->record(row);
            if(record.value(Employee_Id).toInt() == id) {
                mapper->setCurrentIndex(row);
                break;
            }
        }
    } else {
        mapper->toFirst();
    }
    connect(firstButton, SIGNAL(clicked()), mapper, SLOT(toFirst()));
    connect(previousButton, SIGNAL(clicked()), mapper, SLOT(toPrevious()));
    connect(nextButton, SIGNAL(clicked()), mapper, SLOT(toNext()));
    connect(lastButton, SIGNAL(clicked()), mapper, SLOT(toLast()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addEmployee()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteEmployee()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));

    //窗口布局

    QHBoxLayout *topButtonLayout = new QHBoxLayout;
    topButtonLayout->setContentsMargins(20, 0, 20, 5);
    topButtonLayout->addStretch();
    topButtonLayout->addWidget(firstButton);
    topButtonLayout->addWidget(previousButton);
    topButtonLayout->addWidget(nextButton);
    topButtonLayout->addWidget(lastButton);
    topButtonLayout->addStretch();

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(topButtonLayout, 0, 0, 1, 3);
    mainLayout->addWidget(nameLabel, 1, 0);
    mainLayout->addWidget(nameEdit, 1, 1, 1, 2);
    mainLayout->addWidget(departmentLabel, 2, 0);
    mainLayout->addWidget(departmentComboBox, 2, 1, 1, 2);
    mainLayout->addWidget(extensionLabel, 3, 0);
    mainLayout->addWidget(extensionLineEdit, 3, 1);
    mainLayout->addWidget(emailLabel, 4, 0);
    mainLayout->addWidget(emailEdit, 4, 1, 1, 2);
    mainLayout->addWidget(startDateLabel, 5, 0);
    mainLayout->addWidget(startDateEdit, 5, 1);
    mainLayout->addWidget(buttonBox, 7, 0, 1, 3);
    mainLayout->setRowMinimumHeight(6, 10);

    mainLayout->setRowStretch(6, 1);
    mainLayout->setColumnStretch(2, 1);
    setLayout(mainLayout);
    if (id == -1) {
        nextButton->setFocus();
    } else {
        nameEdit->setFocus();
    }
    setWindowTitle(tr("Edit Employees"));
}

void Employeeform::done(int result)
{
    mapper->submit();
    QDialog::done(result);
}

void Employeeform::addEmployee()
{
    int row = mapper->currentIndex();
    mapper->submit();
    tableModel->insertRow(row);
    mapper->setCurrentIndex(row);
    nameEdit->clear();
    startDateEdit->setDate(QDate(QDate::currentDate()));
    nameEdit->setFocus();
}

void Employeeform::deleteEmployee()
{
    int row = mapper->currentIndex();
    tableModel->removeRow(row);
    mapper->submit();
    mapper->setCurrentIndex(qMin(row, tableModel->rowCount() - 1));
}












