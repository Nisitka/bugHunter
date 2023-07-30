#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTableWidget>

#include "qcustomplot.h"
#include "ci.h"

#include "drilldownchart.h"
#include "drilldownslice.h"

#include <QtCore/QTime>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QPieSeries>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class mainWindow;
}

class mainWindow : public QWidget
{
    Q_OBJECT
signals:
    // провести диагностику КИ
    void runAnalys();

public slots:
    // открыть главное окно под пользователем
    void open(const QString& namesUser, int idUser);

    // установить результаты обсчета анализатора
    void setGraphicsData(double* t, double *cCI, double *kCI, int countPoint);

    // отобразить результаты анализа
    void setResultAnalys(int** res);

    //
    void addCI(CI*);

public:
    explicit mainWindow(QWidget *parent = 0);
    ~mainWindow();

private slots:
    void setChartWidget(int **res);

    // отобразить информацию CI
    void showCI(int idCI);

    // выбрать ячейку образа
    void setY(int idY);

    // добавить КИ в базу данных
    void CItoDB();

    // удалить из таблицы КИ
    void removeCI();

    // удалить КИ из БД
    void removeCIDB();

    // отобразить КИ из БД
    void showDBCIs(int idLVL);

    //
    void showInfoCIDB(int idCIfromTable);

    //
    void updateTableCIDB();

private:
    // оператор
    QString namesUser;
    int idUser;

    enum LVL{falseErr, net, app, netApp};
    QStringList namesLVL = {"Ложная тревога",
                            "Сетевой",
                            "Приложения",
                            "Сети и приложения"};

    // графики
    void updateGraphic(double* x, double* y, int countPoint, int g);
    enum graphics{countCI, kAddCI};
    QCustomPlot* cCountPlot;
    QCustomPlot* cKPlot;

    // КИ
    QList <CI*> newCIs;
    CI* currentCI;
    int currentRow;

    int numCurCIDB;

    // настройка таблицы новый КИ
    void setOptNewCITable(QTableWidget* table);
    QStringList horNewCILabels = {"Образ (2)", "Образ (10)", "Уровень", "Время", "Дата"};
    enum idHorNewCILabels{img2, img10, lvl, time, date};

    // настройка таблицы КИ из БД
    void setOptDBCITable(QTableWidget* table);
    QStringList horDBCILabels = {"id", "Название", "Дата", "Время", "Оператор"};
    enum idHorDBCILabels{id, name, date2, time2, info, user};

    QChartView* chartView = NULL;

    // ячейки образа
    QStringList keyNameY = {"Y1", "Y2", "Y3", "Y4", "Y5", "Y6", "Y7"};
    QStringList namesY = {"Канал взаимодействия",
                          "Данные канала взаимодействия",
                          "Ресурс",
                          "Средство безопасности",
                          "Передатчик",
                          "Источник воздействия",
                          "АС ВН"};

    void setDesine();

    Ui::mainWindow *ui;
};

#endif // MAINWINDOW_H
