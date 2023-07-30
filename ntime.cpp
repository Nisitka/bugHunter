#include "ntime.h"

#include <QDebug>

NTime::NTime()
{

}

QTime NTime::setupTime()
{
    int hour;
    int minute;

    int currentHour = QTime::currentTime().hour();
    int currentMinute = QTime::currentTime().minute();

    hour = 1 + rand() % currentHour;
    if (hour == currentHour)
    {
        do minute = currentMinute - (1 + rand()%59);
            while (minute < 0);
    }
    else minute = rand()%60;

    //qDebug() << QTime(hour, minute);
    return QTime(hour, minute);
}

QTime NTime::destrTime(const QTime& setupTime)
{
    int hour;
    int minute;

    int currentHour = QTime::currentTime().hour();
    int currentMinute = QTime::currentTime().minute();

    hour = setupTime.hour() + rand() % (2 + currentHour - setupTime.hour());
    if (hour == currentHour)
    {
        do minute = currentMinute - (1 + rand()%59);
            while (minute < 0);
    }
    else minute = rand()%60;

    //qDebug() << hour << minute << QTime(hour, minute);
    return QTime(hour, minute);
}
