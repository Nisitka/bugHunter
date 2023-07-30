#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include <QHeaderView>

mainWindow::mainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Модуль анализа и обработки данных о КИ в АС ВН");

    connect(ui->matchImageButton, SIGNAL(clicked()),
            this,                 SIGNAL(runAnalys()));

    setOptNewCITable(ui->newCITableWidget);
    connect(ui->newCITableWidget, SIGNAL(cellClicked(int, int)),
            this,                 SLOT(showCI(int)));

    setOptDBCITable(ui->dataBaseCITableWidget);
    connect(ui->dataBaseCITableWidget, SIGNAL(cellClicked(int, int)),
            this,                 SLOT(showInfoCIDB(int)));

    cKPlot = ui->graphicKCIWidget;
    cKPlot->addGraph();
    cKPlot->graph(0)->setPen(QPen(QColor(32, 159, 223)));
    cKPlot->graph(0)->setBrush(QBrush(QColor(32, 159, 223, 50)));

    cCountPlot = ui->graphicCountCIWidget;
    cCountPlot->addGraph();
    cCountPlot->graph(0)->setPen(QPen(QColor(13, 255, 176)));
    cCountPlot->graph(0)->setBrush(QBrush(QColor(13, 255, 176, 50)));

    ui->cellsYComboBox->addItems(keyNameY);
    connect(ui->cellsYComboBox, SIGNAL(currentIndexChanged(int)),
            this,               SLOT(setY(int)));
    ui->cellsYComboBox->currentIndexChanged(0);

    connect(ui->appendDBNewCIButton, SIGNAL(clicked()),
            this,                    SLOT(CItoDB()));

    connect(ui->delNewCIButton, SIGNAL(clicked()),
            this,               SLOT(removeCI()));

    ui->nameLVLDBCIComboBox->addItems(namesLVL);
    connect(ui->nameLVLDBCIComboBox, SIGNAL(currentIndexChanged(int)),
            this,                    SLOT(showDBCIs(int)));

    connect(ui->updateTableCIDBButton, SIGNAL(clicked()),
            this,                      SLOT(updateTableCIDB()));

    connect(ui->delDBCIButton, SIGNAL(clicked()),
            this,              SLOT(removeCIDB()));

    setDesine();
}

void mainWindow::updateTableCIDB()
{
    showDBCIs(ui->nameLVLDBCIComboBox->currentIndex());
}

void mainWindow::showDBCIs(int idLVL)
{
    ui->dataBaseCITableWidget->clearContents();
    ui->dataBaseCITableWidget->setRowCount(0);

    ui->infoCIDBTextBrowser->clear();

    int countRow = workDataBase::countRowTableDB("CI", "interLVL", QString::number(idLVL));
    ui->dataBaseCITableWidget->setRowCount(countRow);

    QSqlQuery query;
    query.exec("SELECT id, name, date, time, userCreator FROM CI WHERE interLVL=" + QString::number(idLVL) + ";");

    QSqlRecord rec = query.record();

    int numRow = 0;
    QStringList values;
    while (query.next())
    {
        values.clear();
        values.append(query.value(rec.indexOf("id")).toString());
        values.append(query.value(rec.indexOf("name")).toString());
        values.append(query.value(rec.indexOf("date")).toString());
        values.append(query.value(rec.indexOf("time")).toString());

        int idUser = query.value(rec.indexOf("userCreator")).toInt();

        QString data;
        QString dataUser = "";
        workDataBase::getDataByID("users", idUser, "last_name", data);
        dataUser += data + " ";
        workDataBase::getDataByID("users", idUser, "name", data);
        dataUser += data + " ";
        workDataBase::getDataByID("users", idUser, "middle_name", data);
        dataUser += data;
        values.append(dataUser);

        for (int i=0; i<values.size(); i++)
        {
            QTableWidgetItem* newItem = new QTableWidgetItem(values[i]);
            newItem->setTextAlignment(Qt::AlignHCenter);

            ui->dataBaseCITableWidget->setItem(numRow, i, newItem);
        }

        numRow++;
    }
}

void mainWindow::open(const QString &namesUser_, int idUser_)
{
    namesUser = namesUser_;
    idUser= idUser_;

    ui->namesUserLabel->setText(namesUser);

    this->show();
}

void mainWindow::showInfoCIDB(int idCIfromTable)
{
    numCurCIDB = idCIfromTable;

    QSqlQuery query;
    query.exec("SELECT info FROM CI WHERE interLVL="
               + QString::number(ui->nameLVLDBCIComboBox->currentIndex()) + ";");

    QSqlRecord rec = query.record();

    for (int i=0; i<=idCIfromTable; i++)
    {
        query.next();
    }

    QString infoCI = query.value(rec.indexOf("info")).toString();

    ui->infoCIDBTextBrowser->setText(infoCI);
}

void mainWindow::removeCIDB()
{
    QSqlQuery query;
    QString idCI = ui->dataBaseCITableWidget->item(numCurCIDB, id)->text();
    query.exec("DELETE FROM CI WHERE id=" + idCI + ";");

    ui->dataBaseCITableWidget->removeRow(numCurCIDB);

    showInfoCIDB(numCurCIDB);
}

void mainWindow::setY(int idY)
{
    ui->cellsYStackedWidget->setCurrentIndex(idY);

    ui->nameYLabel->setText(namesY[idY]);
}

void mainWindow::addCI(CI *ci)
{
    newCIs.append(ci);

    int countRow = newCIs.size();
    ui->newCITableWidget->setRowCount(countRow);
    int idRow = countRow - 1;

    for (int i=0; i<horNewCILabels.size(); i++)
    {
        QTableWidgetItem* newItem = new QTableWidgetItem;
        newItem->setTextAlignment(Qt::AlignHCenter);

        ui->newCITableWidget->setItem(idRow, i, newItem);
    }
    ui->newCITableWidget->item(idRow, time)->setText(ci->getTime());
    ui->newCITableWidget->item(idRow, date)->setText(ci->getDate());
    ui->newCITableWidget->item(idRow, lvl)->setText(QString::number(ci->getLVL()));
    ui->newCITableWidget->item(idRow, img10)->setText(QString::number(ci->getImg10()));
    ui->newCITableWidget->item(idRow, img2)->setText(ci->getImg2());
}

void mainWindow::CItoDB()
{
    QString commandF =
            "INSERT INTO CI (id, name, date, time, interLVL, info, userCreator) "
            "VALUES(%1, '%2', '%3', '%4', '%5', '%6', %7);";
    QString command;
    command = commandF.arg(rand()%1000) //workDataBase::countRowTableDB("CI")
                      .arg(ui->nameNewCILineEdit->text())
                      .arg(currentCI->getDate())
                      .arg(currentCI->getTime())
                      .arg(currentCI->getLVL())
                      .arg(ui->infoCITextEdit->toPlainText())
                      .arg(QString::number(idUser));
    QSqlQuery query;
    query.exec(command);
}

void mainWindow::removeCI()
{
    ui->newCITableWidget->removeRow(currentRow);
    newCIs.removeAt(currentRow);

    // показывает следующий после удаленного
    this->showCI(currentRow);
}

void mainWindow::showCI(int idCI)
{
    currentRow = idCI;
    currentCI = newCIs[idCI];

    QVector <QString> data;

    // Y1
    currentCI->getY(CI::y1)->getData(data);
    ui->startSessionLine->setText(data[0]);
    ui->finishSessionLine->setText(data[1]);
    ui->idSessionLine->setText(data[2]);
    ui->protocolSessionLine->setText(data[3]);
    ui->portSessionLine->setText(data[4]);
    data.clear();

    // Y2
    currentCI->getY(CI::y2)->getData(data);
    ui->numBitesLine->setText(data[0]);
    ui->numPacketsLine->setText(data[1]);
    ui->timeSendingLine->setText(data[2]);
    ui->formatDataLine->setText(data[3]);
    data.clear();

    // Y3
    currentCI->getY(CI::y3)->getData(data);
    ui->typeResLine->setText(data[0]);
    ui->sizeFileLine->setText(data[1]);
    data.clear();

    // Y4
    currentCI->getY(CI::y4)->getData(data);
    ui->nameSpecialToolLine->setText(data[0]);
    data.clear();

    // Y5
    currentCI->getY(CI::y5)->getData(data);
    ui->IPLine->setText(data[0]);
    ui->MACLine->setText(data[1]);
    ui->nameSenderLine->setText(data[2]);
    data.clear();

    // Y6
    currentCI->getY(CI::y6)->getData(data);
    ui->nameTypeResLine2->setText(data[0]);
    data.clear();

    // Y7
    currentCI->getY(CI::y7)->getData(data);
    ui->nameObjectDefenceLine->setText(data[0]);
}

void mainWindow::setOptNewCITable(QTableWidget *table)
{
    // Столбцы
    table->setColumnCount(horNewCILabels.size());
    table->setHorizontalHeaderLabels(horNewCILabels);

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void mainWindow::setOptDBCITable(QTableWidget *table)
{
    // Столбцы
    table->setColumnCount(horDBCILabels.size());
    table->setHorizontalHeaderLabels(horDBCILabels);

    table->horizontalHeader()->setStretchLastSection(true);

    table->setColumnWidth(id, 30);
}

void mainWindow::setGraphicsData(double *t, double *cCI, double *kCI, int countPoint)
{
    updateGraphic(t, cCI, countPoint, countCI);
    updateGraphic(t, kCI, countPoint, kAddCI);

    delete [] t;
    delete [] cCI;
    delete [] kCI;
}

void mainWindow::updateGraphic(double* x, double* y, int countPoint, int g)
{
    QVector <double> X(countPoint), Y(countPoint);
    double Xmax = 0;
    double Ymax = 0;
    for (int i=0; i<countPoint; i++)
    {
        X[i] = x[i];
        if (Xmax < x[i]) Xmax = x[i];

        Y[i] = y[i];
        if (Ymax < y[i]) Ymax = y[i];
    }

    QCustomPlot* graphic;
    switch (g) {
    case countCI:
        graphic = cCountPlot;
        break;
    case kAddCI:
        graphic = cKPlot;
        break;
    }

    graphic->graph(0)->setData(X, Y, true);

    graphic->xAxis->setRange(0, Xmax);
    graphic->xAxis2->setRange(0, Xmax);

    graphic->yAxis->setRange(0, Ymax + 1);
    graphic->yAxis2->setRange(0, Ymax + 1);

    graphic->replot();
}

void mainWindow::setResultAnalys(int **res)
{
    setChartWidget(res);

    for (int i=0; i<4; i++)
    {
        delete [] res[i];
    }
    delete [] res;
}

void mainWindow::setChartWidget(int **res)
{
    ui->chartLayout->removeWidget(chartView);
    delete chartView;

    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    DrilldownChart *chart = new DrilldownChart();
    chart->setTheme(QChart::ChartThemeLight);
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

    QPieSeries *yearSeries = new QPieSeries(this);
    yearSeries->setName("Уровни взаимодействия КИ");

    QList<QString> groupsCI;
    groupsCI << "Отказ в обслуживании" << "Сбой информации" << "Несанкционированный доступ";
    QList<QString> names;
    names << "Ложная тревога" << "Уровень сети" << "Уровень приложения" << "Уровень сети и приложения";

    for (int i=0; i<names.size(); i++)
    {
        QPieSeries *series = new QPieSeries(this);
        series->setName("Группы КИ (" + names[i] + ")");

        for (int j=0; j<groupsCI.size(); j++)
            *series << new DrilldownSlice(res[i][j], groupsCI[j], yearSeries);

        QObject::connect(series, SIGNAL(clicked(QPieSlice*)), chart, SLOT(handleSliceClicked(QPieSlice*)));

        *yearSeries << new DrilldownSlice(series->sum(), names[i], series);
    }

    QObject::connect(yearSeries, SIGNAL(clicked(QPieSlice*)), chart, SLOT(handleSliceClicked(QPieSlice*)));

    chart->changeSeries(yearSeries);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->chartLayout->addWidget(chartView);
}

void mainWindow::setDesine()
{
    ui->icon12KafLabel->setPixmap(QPixmap(":/resurses/12Kaf").scaled(160, 160));

    ui->updateTableCIDBButton->setIcon(QIcon(QPixmap(":/resurses/updateIcon")));
}

mainWindow::~mainWindow()
{
    delete ui;
}
