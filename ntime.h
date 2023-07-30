#ifndef NTIME_H
#define NTIME_H

#include <QTimer>
#include <QTime>

class NTime
{
public:
    NTime();

    static QTime setupTime();
    static QTime destrTime(const QTime& setupTime);
};

#endif // NTIME_H
