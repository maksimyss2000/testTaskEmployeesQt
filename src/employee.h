#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QDataStream>
#include <QString>

enum Sex {
    Male,
    Female,
};

struct Employee {
    int age;
    int experience;
    QString phoneNumber;
    QString fullName;
    Sex sex;

    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);
};

#endif  // EMPLOYEE_H
