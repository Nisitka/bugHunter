#ifndef WORKDATABASE_H
#define WORKDATABASE_H

#include <QApplication>

#include <QtSql>
#include <QSqlDatabase>

// класс для работы с базой данных
class workDataBase : public QObject
{
    Q_OBJECT
signals:
    void tryOpen(bool status);
    void error(const QString& strError);

    void autUserSuccefull(const QString& namesUser, int idUser);

    void autUserError();

public slots:
    void initUser(const QString& login, const QString& password);

public:
    workDataBase();

    bool connect(const QString& nameDataBase);

    static int countRowTableDB(const QString& nameTable);

    static int countRowTableDB(const QString &nameTable,
                               const QString &nameColumn,
                               const QString &value);

    static void getDataByID(const QString& nameTable, int id, const QString& nameColumn, QString& data);

private:
    bool statusConnect;
};

#endif // WORKDATABASE_H
