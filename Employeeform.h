#ifndef EMPLOYEEFORM_H
#define EMPLOYEEFORM_H
#include <QDialog>

enum {
    Employee_Id = 0,
    Employee_Name = 1,
    Employee_DepartmentId = 2,
    Employee_Extension = 3,
    Employee_Email = 4,
    Employee_StartDate = 5
};

class QSqlRelationalTableModel;
class QDataWidgetMapper;
class QLabel;
class QLineEdit;
class QComboBox;
class QDateEdit;
class QPushButton;
class QDialogButtonBox;

class Employeeform : public QDialog
{
    Q_OBJECT
public:
    Employeeform(int id, QWidget *parent = nullptr);
    ~Employeeform() = default;

    void done(int result);
private slots:
    void addEmployee();
    void deleteEmployee();
private:
    QSqlRelationalTableModel *tableModel;
    QDataWidgetMapper *mapper;
    //标签
    QLabel *nameLabel;
    QLabel *departmentLabel;
    QLabel *extensionLabel;
    QLabel *emailLabel;
    QLabel *startDateLabel;
    //文本框 列表框
    QLineEdit *nameEdit;
    QComboBox *departmentComboBox;
    QLineEdit *extensionLineEdit;
    QLineEdit *emailEdit;
    QDateEdit *startDateEdit;
    //按键
    QPushButton *firstButton;
    QPushButton *previousButton;
    QPushButton *nextButton;
    QPushButton *lastButton;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *closeButton;
    QDialogButtonBox *buttonBox;

};

#endif // EMPLOYEEFORM_H
