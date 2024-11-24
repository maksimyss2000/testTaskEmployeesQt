#include "employee.h"

void Employee::serialize(QDataStream &out) const {
    out << fullName
        << static_cast<qint32>(sex)
        << age
        << experience
        << phoneNumber;
}

void Employee::deserialize(QDataStream &in) {
    qint32 sex_int;
    in >> fullName >> sex_int >> age >> experience >> phoneNumber;
    sex = static_cast<Sex>(sex_int);
}
