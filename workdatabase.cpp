#include "workdatabase.h"

#include <QSqlQuery>
#include <QSqlRecord>

workDataBase::workDataBase()
{
    statusConnect = false;
}

bool workDataBase::connect(const QString& nameDataBase)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QApplication::applicationDirPath() + "/" + nameDataBase + ".db");

    statusConnect = db.open();

    //qDebug() << countRowTableDB("eventsCIs", "groupCI", "1");

    tryOpen(statusConnect);
    return statusConnect;
}

void workDataBase::initUser(const QString &login, const QString &password)
{
    QSqlQuery query;
    QString strF = "SELECT * FROM users WHERE login='" +
            login + "' AND password='" + password + "';";

    query.exec(strF);
    QSqlRecord rec = query.record();

    if (query.next())
    {
        QString namesUser = "";

        namesUser += query.value(rec.indexOf("last_name")).toString()  + " ";
        namesUser += query.value(rec.indexOf("name")).toString() + " ";
        namesUser += query.value(rec.indexOf("middle_name")).toString();

        int idUser = query.value(rec.indexOf("id")).toInt();

        autUserSuccefull(namesUser, idUser);
    }
    else
    {
        autUserError();
    }
}

void workDataBase::getDataByID(const QString &nameTable,
                               int id, const QString& nameColumn,
                               QString &data)
{
    QSqlQuery query;
    QString strF = "SELECT " + nameColumn +
            " FROM " + nameTable +
            " WHERE id=" + QString::number(id) + ";";

    query.exec(strF);
    QSqlRecord rec = query.record();
    query.next();

    data = query.value(rec.indexOf(nameColumn)).toString();
}

int workDataBase::countRowTableDB(const QString &nameTable)
{
    QSqlQuery query;
    QString strF = "SELECT COUNT(*) FROM " + nameTable + ";";

    query.exec(strF);
    query.next();

    return query.value("COUNT(*)").toInt();
}

int workDataBase::countRowTableDB(const QString &nameTable,
                                  const QString &nameColumn,
                                  const QString &value)
{
    QSqlQuery query;
    QString strF = "SELECT COUNT(*) FROM " + nameTable +
            " WHERE " + nameColumn + "=" + value + ";";

    query.exec(strF);
    query.next();

    return query.value("COUNT(*)").toInt();
}


