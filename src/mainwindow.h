#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QVector>
#include <QListWidget>

#include "employee.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   private slots:
    void addEmployee();
    void removeEmployee();
    void writeFile();
    void checkFile();

   private:
    Ui::MainWindow *ui;
    QVector<Employee> employees;
    QStandardItemModel *employeeModel;

    QByteArray serializeEmployees();
    void deserializeEmployees(const QByteArray &data);
    QByteArray applyXOR(const QByteArray &data, int key);
    quint32 calculateCRC32(const QByteArray &data);
    void updateEmployeeForm(int index);
    void onRowDoubleClicked(const QModelIndex &index);
    void onGenderCurrentIndexChanged(int index);
    void initializeTable();
    void updateTable();
};

#endif  // MAINWINDOW_H
