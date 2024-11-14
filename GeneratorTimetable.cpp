#include <QMessageBox>
#include "timetable.h"
#include <vector>
#include <QDebug> // For debugging purposes
#include <QStandardItemModel>


// Function declarations
bool isValid(const std::vector<std::vector<int>>& timetable, int teacher, int timeslot);
bool backtrack(std::vector<std::vector<int>>& timetable, int teacher, int numTeachers, int numTimeslots);




// Helper function to check if the current assignment is valid
bool isValid(const std::vector<std::vector<int>>& timetable, int teacher, int timeslot) {
    for (size_t i = 0; i < timetable.size(); ++i) {
        if (timetable[i][timeslot] == teacher) {
            return false;
        }
    }
    return true;
}

// Backtracking function to generate timetable
bool backtrack(std::vector<std::vector<int>>& timetable, int teacher, int numTeachers, int numTimeslots) {
    if (teacher == numTeachers) {
        return true;  // All teachers are assigned
    }

    for (int slot = 0; slot < numTimeslots; ++slot) {
        if (isValid(timetable, teacher, slot)) {
            timetable[teacher][slot] = teacher;

            if (backtrack(timetable, teacher + 1, numTeachers, numTimeslots)) {
                return true;
            }

            timetable[teacher][slot] = -1;  // Backtrack
        }
    }

    return false;
}

void timetable::generaterTimetable()
{
    int numTeachers = 5;  // Example number of teachers
    int numTimeslots = 5;  // Example number of timeslots

    std::vector<std::vector<int>> timetable(numTeachers, std::vector<int>(numTimeslots, -1));

    if (backtrack(timetable, 0, numTeachers, numTimeslots)) {
        QMessageBox::information(this, "Timetable", "Timetable generated successfully!");

        // Optionally display the timetable
        for (int teacher = 0; teacher < numTeachers; ++teacher) {
            for (int slot = 0; slot < numTimeslots; ++slot) {
                if (timetable[teacher][slot] != -1) {
                    // Display teacher and slot information
                    qDebug() << "Teacher" << teacher << "assigned to slot" << slot;
                }
            }
        }
    } else {
        QMessageBox::warning(this, "Timetable", "No feasible solution found.");
    }
    qDebug() << "Generating timetable...";
    // Example logic to update the table view
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->tableView->model());
    if (!model) {
        return;
    }

    // Clear the model
    model->clear();

    // Populate the model with new data (example)
    for (int row = 0; row < 5; ++row) {
        QList<QStandardItem *> items;
        for (int col = 0; col < 5; ++col) {
            items.append(new QStandardItem(QString("Item %1,%2").arg(row).arg(col)));
        }
        model->appendRow(items);
    }
}
void timetable::updateModel() {
    // Clear the model
    model->clear();

    // Update the model with current data
    for (int i = 0; i < teachers.size(); ++i) {
        QStandardItem *item = new QStandardItem(teachers[i].getName());
        model->setItem(i, 0, item);
    }

    for (int i = 0; i < subjects.size(); ++i) {
        QStandardItem *item = new QStandardItem(subjects[i].getName());
        model->setItem(i, 1, item);
    }

    for (int i = 0; i < studentgroups.size(); ++i) {
        QStandardItem *item = new QStandardItem(studentgroups[i].getName());
        model->setItem(i, 2, item);
    }
}
