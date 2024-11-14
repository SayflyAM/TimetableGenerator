#include "timetable.h"
#include <QStandardItemModel>
#include <QDebug>
#include <random>
#include <QPdfWriter>
#include <QPainter>
#include <QFont>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextTableFormat>
#include <QTextTableCellFormat>
#include <QTextBlock>
#include <QTextTable>





timetable::timetable(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::timetable)
{
    ui->setupUi(this);

    model = new QStandardItemModel(this);
    ui->tableView->setModel(model);

    connect(ui->pushButton, &QPushButton::clicked, this, &timetable::onAdd);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &timetable::onRemove);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &timetable::onSolve);
    connect(ui->loadButton,&QPushButton::clicked,this,&timetable::loadDataFromFile);
    connect(ui->pushButton_save, &QPushButton::clicked, this, &timetable::saveTimetableToCSV);

    setupTimetableHeaders();
}

timetable::~timetable() {
    delete ui;
}

void timetable::onAdd() {

    Teacher newTeacher;
    newTeacher.setName(ui->teacherNameInput->text());
    if (!newTeacher.getName().isEmpty()) {
        teachers.append(newTeacher);
        qDebug() << "Added teacher:" << newTeacher.getName();
    } else {
        qDebug() << "Teacher name input is empty.";
    }


    Subject newSubject;
    newSubject.setName(ui->subjectNameInput->text());
    if (!newSubject.getName().isEmpty()) {
        subjects.append(newSubject);
        qDebug() << "Added subject:" << newSubject.getName();
    } else {
        qDebug() << "Subject name input is empty.";
    }

    StudentGroup newGroup;
    newGroup.setName(ui->groupNameInput->text());
    if (!newGroup.getName().isEmpty()) {
        studentgroups.append(newGroup);
        qDebug() << "Added group:" << newGroup.getName();
    } else {
        qDebug() << "Group name input is empty.";
    }


    updateModel();
}

void timetable::onRemove() {

    if (!teachers.isEmpty()) {
        qDebug() << "Removed teacher:" << teachers.last().getName();
        teachers.removeLast();
    }
    if (!subjects.isEmpty()) {
        qDebug() << "Removed subject:" << subjects.last().getName();
        subjects.removeLast();
    }
    if (!studentgroups.isEmpty()) {
        qDebug() << "Removed group:" << studentgroups.last().getName();
        studentgroups.removeLast();
    }


    updateModel();
}

void timetable::onSolve() {
    qDebug() << "Solve button clicked";
    generaterTimetable();
}
void timetable::generaterTimetable(){
    qDebug() <<"Generating  timetable..";

    model->clear();
    setupTimetableHeaders();

    vector<ClassNode> nodes;
    unordered_map<int,unordered_set<int>> graph;


    for(const auto& teacher: teachers){
        for(const auto& subject:subjects){
            for(const auto& group: studentgroups){
                nodes.emplace_back(teacher.getName(),subject.getName(),group.getName());
            }
        }
    }
    addConflictEdges(nodes,graph);
    colorGraph(nodes,graph);




    for(const auto& node: nodes){

        int row = node.timeSlot/6;
        int col = node.timeSlot%6;

        QString itemText = QString("%1,%2,%3").arg(node.teacher).arg(node.subject).arg(node.studentGroup);
        model->setItem(row,col,new QStandardItem(itemText));
        qDebug() << "Added item to model:" << itemText << "at row" << row << "col" << col;


    }








}
/*
void timetable::generateTeacherTimetablePDF(const QString& teacherName, const vector<ClassNode>& nodes) {
    QString fileName = QString("timetable_%1.pdf").arg(teacherName);
    QPdfWriter writer(fileName);

    writer.setPageSize(QPageSize::A4);
    writer.setPageMargins(QMarginsF(20, 20, 20, 20));

    QPainter painter(&writer);

    QTextDocument doc;
    QFont font("Arial", 18);
    doc.setDefaultFont(font);

    QTextTableFormat tableFormat;
    tableFormat.setCellPadding(10);
    tableFormat.setCellSpacing(5);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    tableFormat.setAlignment(Qt::AlignHCenter);

    QTextCursor cursor(&doc);
    QTextTable *table = cursor.insertTable(7, 6, tableFormat);

    QTextCharFormat headerFormat;
    headerFormat.setFontWeight(QFont::Bold);
    headerFormat.setBackground(QColor(Qt::lightGray));
    headerFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);

    QTextTableCell headerCell = table->cellAt(0, 0);
    headerCell.setFormat(headerFormat);
    headerCell.firstCursorPosition().insertText("Day");

    QStringList daysOfWeek = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};

    for (int row = 1; row < 7; ++row) {
        QTextTableCell dayCell = table->cellAt(row, 0);
        dayCell.setFormat(headerFormat);
        dayCell.firstCursorPosition().insertText(daysOfWeek[row - 1]);
    }

    for (const auto& node : nodes) {
        if (node.teacher == teacherName) {
            int row = (node.timeSlot / 6) + 1;
            int col = (node.timeSlot % 6) + 1;

            if (row >= 0 && col >= 0 && row < table->rows() && col < table->columns()) {
                QTextTableCell cell = table->cellAt(row, col);
                QTextCharFormat cellFormat;
                cellFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
                cell.setFormat(cellFormat);

                QString cellText = QString("%1\n(%2)").arg(node.subject, node.studentGroup);
                cell.firstCursorPosition().insertText(cellText);
            } else {
                qDebug() << "Invalid cell position for:" << node.subject << node.studentGroup;
            }
        }
    }

    painter.translate(20, 60);
    doc.drawContents(&painter);
    qDebug() << "PDF file " << fileName << " generated.";

    painter.end();
}
*/









void timetable::addConflictEdges(vector<ClassNode>&nodes,unordered_map<int,unordered_set<int>>&graph){
    for(int i=0;i<nodes.size();++i){
        for(int j=i+1;j<nodes.size();++j){
            if(nodes[i].teacher==nodes[j].teacher||nodes[i].studentGroup==nodes[j].studentGroup){
                graph[i].insert(j);
                graph[j].insert(i);
            }
        }
    }

}
void timetable::colorGraph(vector<ClassNode> &nodes,unordered_map<int,unordered_set<int>> &graph){
    int numColors =0;

    for(int i=0;i<nodes.size();++i){
        unordered_set<int> usedColors;
        for(int neighbor:graph[i]){
            if(nodes[neighbor].timeSlot != -1){
                usedColors.insert(nodes[neighbor].timeSlot);
            }

        }
        int color=0;

        while(usedColors.find(color) != usedColors.end()){
            ++color;
        }

        nodes[i].timeSlot = color;

        numColors = max(numColors,color+1);
        if(numColors==40)break;
    }
    qDebug()<<"Number of time slots used:"<<numColors;


}


void timetable::updateModel() {

    model->clear();
    setupTimetableHeaders();
    for (int i = 0; i < teachers.size(); ++i) {
        model->setItem(i, 0, new QStandardItem(teachers[i].getName()));
    }
    for (int i = 0; i < subjects.size(); ++i) {
        model->setItem(i, 1, new QStandardItem(subjects[i].getName()));
    }
    for (int i = 0; i < studentgroups.size(); ++i) {
        model->setItem(i, 2, new QStandardItem(studentgroups[i].getName()));
    }


}

void timetable::setupTimetableHeaders() {

    QStringList timeSlots = {"8:00-10:00", "10:00-12:00", "12:00-14:00", "14:00-16:00", "16:00-18:00","18:00-20:00"};
    QStringList daysOfWeek = {"Saturday","Sunday","Monday", "Tuesday", "Wednesday", "Thursday"};


    for (int col = 0; col < timeSlots.size(); ++col) {
        model->setHorizontalHeaderItem(col , new QStandardItem(timeSlots[col]));
    }


    for (int row = 0; row < daysOfWeek.size(); ++row) {
        model->setVerticalHeaderItem(row , new QStandardItem(daysOfWeek[row]));
    }
}

void timetable::loadDataFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Data File", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open file" << fileName;
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList parts = line.split(',');

        if (parts.size() != 3) {
            qDebug() << "Invalid line format:" << line;
            continue;
        }

        QString type = parts[0].trimmed();
        QString name = parts[1].trimmed();
        QString additionalInfo = parts[2].trimmed();

        if (type == "Teacher") {
            Teacher teacher;
            teacher.setName(name);

            teachers.append(teacher);
        } else if (type == "Subject") {
            Subject subject;
            subject.setName(name);

            subjects.append(subject);
        } else if (type == "Group") {
            StudentGroup group;
            group.setName(name);
            studentgroups.append(group);
        } else {
            qDebug() << "Unknown type:" << type;
        }
    }

    file.close();
    updateModel();
}

void timetable::saveTimetableToCSV() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Timetable", "", "CSV Files (*.csv);;All Files (*)");
    if (fileName.isEmpty()) {
        qDebug() << "Save operation canceled.";
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Could not open file for writing:" << fileName;
        return;
    }

    QTextStream out(&file);

    out << "Teacher,Subject,Student Group,Day,Time Slot\n";

    if (!model) {
        qDebug() << "Model is not initialized.";
        file.close();
        return;
    }

    if (model->rowCount() == 0 || model->columnCount() == 0) {
        qDebug() << "Model is empty or headers are missing.";
        file.close();
        return;
    }

    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QStandardItem *item = model->item(row, col);
            if (item) {
                QStringList parts = item->text().split(",");
                if (parts.size() == 3) {
                    QString teacherName = parts[0].trimmed();
                    QString subjectName = parts[1].trimmed();
                    QString groupName = parts[2].trimmed();

                    QString day;
                    QString timeSlot;

                    if (model->verticalHeaderItem(row)) {
                        day = model->verticalHeaderItem(row)->text();
                    } else {
                        continue;
                    }

                    if (model->horizontalHeaderItem(col)) {
                        timeSlot = model->horizontalHeaderItem(col)->text();
                    } else {
                        continue;
                    }

                    out << teacherName << "," << subjectName << "," << groupName << "," << day << "," << timeSlot << "\n";
                } else {
                    qDebug() << "Invalid item text format:" << item->text();
                }
            }
        }
    }

    file.close();
    qDebug() << "Timetable saved to" << fileName;
}
















