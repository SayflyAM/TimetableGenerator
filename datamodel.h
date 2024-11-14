#ifndef DATAMODEL_H
#define DATAMODEL_H
#include <QString>
#include <QList>
class Subject{
private:
    QString name;

public:
    Subject(){}
    Subject(const QString &name) : name(name){}
    QString getName() const{return name;}
    void setName(const QString &name){
        this->name = name;
    }
};
class Teacher:public Subject{

};
class StudentGroup:public Subject{



};

#endif
