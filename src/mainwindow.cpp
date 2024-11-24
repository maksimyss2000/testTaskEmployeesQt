#include "mainwindow.h"

#include <QCryptographicHash>
#include <QDataStream>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::addEmployee);
    connect(ui->removeButton, &QPushButton::clicked, this, &MainWindow::removeEmployee);
    connect(ui->writeButton, &QPushButton::clicked, this, &MainWindow::writeFile);
    connect(ui->checkButton, &QPushButton::clicked, this, &MainWindow::checkFile);
    connect(ui->employeeTable, &QTableView::doubleClicked, this, &MainWindow::onRowDoubleClicked);

    this->initializeTable();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initializeTable() {
    employeeModel = new QStandardItemModel(this);
    employeeModel->setColumnCount(5);
    QStringList headers = {"ФИО", "Пол", "Возраст", "Стаж", "Телефон"};
    employeeModel->setHorizontalHeaderLabels(headers);

    ui->employeeTable->setModel(employeeModel);
    ui->employeeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->employeeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->employeeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->employeeTable->setSelectionMode(QAbstractItemView::SingleSelection);
}

void MainWindow::onRowDoubleClicked(const QModelIndex &index) {
    if (!index.isValid()) {
        return;
    }
    int row = index.row();
    updateEmployeeForm(row);
}

void MainWindow::updateTable() {
    employeeModel->removeRows(0, employeeModel->rowCount());

    for (const Employee &employee : employees) {
        QList<QStandardItem *> row;

        QString sexText;
        switch (employee.sex) {
            case Male:
                sexText = "Мужской";
                break;
            case Female:
                sexText = "Женский";
                break;
        }

        row.append(new QStandardItem(employee.fullName));
        row.append(new QStandardItem(sexText));
        row.append(new QStandardItem(QString::number(employee.age)));
        row.append(new QStandardItem(QString::number(employee.experience)));
        row.append(new QStandardItem(employee.phoneNumber));

        employeeModel->appendRow(row);
    }
}

void MainWindow::addEmployee() {
    Employee employee;
    employee.fullName = ui->nameEdit->text();

    int index = ui->sexEdit->currentIndex();

    switch (index) {
        case 0:
            employee.sex = Sex::Male;
            break;
        case 1:
            employee.sex = Sex::Female;
            break;
    }

    employee.age = ui->ageEdit->value();
    employee.experience = ui->experienceEdit->value();
    employee.phoneNumber = ui->phoneEdit->text();
    employees.append(employee);

    updateTable();
}

void MainWindow::removeEmployee() {
    int currentRow = ui->employeeTable->currentIndex().row();
    if (currentRow >= 0) {
        employees.removeAt(currentRow);
        updateTable();
    }
}

void MainWindow::writeFile() {
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "Binary Files (*.bin)");
    if (fileName.isEmpty()) {
        return;
     }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл для записи");
        return;
    }

    QByteArray data = serializeEmployees();
    int key = ui->xorKeyEdit->value();
    QByteArray encryptedData = applyXOR(data, key);

    QDataStream out(&file);
    out.writeRawData(encryptedData.constData(), encryptedData.size());
    file.close();

    quint32 crc = calculateCRC32(encryptedData);

    QString checkSum = QString::number(crc, 16).toUpper();

    QFile crcFile(fileName + ".crc");
    if (crcFile.open(QIODevice::WriteOnly)) {
        QDataStream crcOut(&crcFile);
        crcOut << checkSum;
        crcFile.close();

        ui->checksumDisplay->setText(checkSum);
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить файл контрольной суммы.");
    }

    QMessageBox::information(this, "Успех", "Файл успешно записан");
}

void MainWindow::checkFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл", "", "All Files (*)");
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Вы не выбрали файл.");
        return;
    }

    QFile dataFile(fileName);
    if (!dataFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл данных.");
        return;
    }

    QByteArray fileData = dataFile.readAll();
    dataFile.close();

    quint32 calculatedCRC = calculateCRC32(fileData);
    QString calculatedChecksum = QString::number(calculatedCRC, 16).toUpper();

    QFile crcFile(fileName + ".crc");
    if (!crcFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл контрольной суммы (.crc).");
        return;
    }

    QDataStream crcIn(&crcFile);
    QString savedChecksum;
    crcIn >> savedChecksum;
    crcFile.close();

    if (calculatedChecksum == savedChecksum) {
        QMessageBox::information(this, "Успех", "Контрольная сумма совпадает!");

        ui->checksumDisplay->setText(calculatedChecksum);
        ui->checksumDisplay->setStyleSheet("color: green;");

    } else {
        QMessageBox::critical(this, "Ошибка", "Контрольная сумма не совпадает.");
        ui->checksumDisplay->setText("Ошибка CRC");
    }

    bool inputStatus;
    int xorKey = QInputDialog::getInt(this, "Введите ключ XOR", "Ключ (от 0 до 255):", 0, 0, 255, 1, &inputStatus);

    if (!inputStatus) {
        QMessageBox::warning(this, "Отмена", "Вы не ввели ключ XOR.");
        return;
    }

    QByteArray data = applyXOR(fileData, xorKey);
    ui->xorKeyEdit->setValue(xorKey);
    deserializeEmployees(data);

    QMessageBox::information(this, "Успех", "Контрольная сумма совпадает, данные успешно загружены.");
}

void MainWindow::updateEmployeeForm(int index) {
    if (index < 0 || index >= employees.size()) {
        return;
    }

    Employee employee = employees[index];

    ui->nameEdit->setText(employee.fullName);
    ui->sexEdit->setCurrentIndex(employee.sex);
    ui->ageEdit->setValue(employee.age);
    ui->experienceEdit->setValue(employee.experience);
    ui->phoneEdit->setText(employee.phoneNumber);
}

QByteArray MainWindow::serializeEmployees() {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    for (const auto &employee : employees) {
        employee.serialize(stream);
    }
    return data;
}

void MainWindow::deserializeEmployees(const QByteArray &data) {
    QDataStream stream(data);
    while (!stream.atEnd()) {
        Employee employee;
        employee.deserialize(stream);
        employees.append(employee);
    }
    updateTable();
}

QByteArray MainWindow::applyXOR(const QByteArray &data, const int key) {
    QByteArray result = data;
    for (char &byte : result) {
        byte ^= key;
    }
    return result;
}

quint32 MainWindow::calculateCRC32(const QByteArray &data) {
    quint32 crc = 0xFFFFFFFF;
    for ( const char byte : data) {
        crc ^= static_cast<quint8>(byte);
        for (int i = 0; i < 8; ++i) {
            crc = (crc >> 1) ^ (0xEDB88320 * (crc & 1));
        }
    }
    return ~crc;
}
