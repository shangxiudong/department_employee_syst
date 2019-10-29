#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QPushButton;
class QSqlRelationalTableModel;
class QTableView;
class QPushButton;
class QLabel;
class QDialogButtonBox;

enum {
    Department_Id = 0,
    Department_Name = 1,
    Department_LocationId = 2
};

class Widget:public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private slots:
    void updateEmployeeView();
    void addDepartment();
    void deleteDepartment();
    void editEmployees();
    void showdatabase();
private:
    void createDepartmentPanel();
    void createEmployeePanel();
    //两个表格窗口嵌在主窗口里
    QWidget *departmentPanel;
    QWidget *employeePanel;
    //两个关系型数据库模型
    QSqlRelationalTableModel *departmentModel;
    QSqlRelationalTableModel *employeeModel;
    //两个模型视图
    QTableView *departmentView;
    QTableView *employeeView;
    //几个按钮
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *editButton;
    QPushButton *quitButton;
    //按键箱，存放上面的几个按键
    QDialogButtonBox *buttonBox;
    //两个标签
    QLabel *departmentLabel;
    QLabel *employeeLabel;
};

#endif // WIDGET_H
