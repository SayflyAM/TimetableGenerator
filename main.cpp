#include "timetable.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    timetable w;
    w.show();
    return a.exec();
}
