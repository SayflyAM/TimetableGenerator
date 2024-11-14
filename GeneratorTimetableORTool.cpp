#include "timetable.h"
#include <QMessageBox>
#include <vector>
using namespace std;



bool isValid(const vector<vector<int>>& timetable, int teacher, int timeslot) {
    for ( const auto& slots : timetabl) {
        if (slots[timeslot] == teacher) {
            return false;
        }
    }
    return true;
}

// Backtracking function to generate timetable
bool backtrack(vector<vector<int>>& timetable, int teacher, int timeslot, int numTeachers, int numTimeslots) {
    if (teacher == numTeachers) {
        return true;  // All teachers are assigned
    }

    for (int slot = 0; slot < numTimeslots; ++slot) {
        if (isValid(timetable, teacher, slot)) {
            timetable[teacher][slot] = teacher;

            if (backtrack(timetable, teacher + 1, 0, numTeachers, numTimeslots)) {
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

    vector<vector<int>> timetable(numTeachers, vector<int>(numTimeslots, -1));

    if (backtrack(timetable, 0, 0, numTeachers, numTimeslots)) {
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
}
