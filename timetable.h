#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <QMainWindow>
#include "ui_timetable.h"
#include "datamodel.h"
#include <QStandardItemModel>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>


using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class timetable;
}
QT_END_NAMESPACE

class timetable : public QMainWindow
{
    Q_OBJECT

public:
    explicit timetable(QWidget *parent = nullptr);
    ~timetable();

private slots:
    void onAdd();
    void onRemove();
    void onSolve();
    void generaterTimetable();
    void loadDataFromFile();


private:
    Ui::timetable *ui;
    QStandardItemModel *model;
    QList<Subject> subjects;
    QList<Teacher> teachers;
    QList<StudentGroup> studentgroups;




    void setupTimetableHeaders();
    void updateModel();

public:
    void saveTimetableToCSV();



    struct ClassNode{
        QString teacher;
        QString subject;
        QString studentGroup;

        int timeSlot;

        ClassNode(QString t,QString s,QString sg):teacher(t),subject(s),studentGroup(sg),timeSlot(-1){}
    };
private:
    void addConflictEdges(vector<ClassNode>&nodes,unordered_map<int,unordered_set<int>>&graph);
    void colorGraph(vector<ClassNode>&nodes,unordered_map<int,unordered_set<int>>&graph);
    //void generateTeacherTimetablePDF(const QString& teacherName, const vector<ClassNode>& nodes);






};

#endif
