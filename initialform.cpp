#include "initialform.h"
#include "ui_initialform.h"

#include <QTime>

initialForm::initialForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::initialForm)
{
    ui->setupUi(this);

    this->setWindowTitle("Инициализация пользователя");

    connect(ui->autUserButton, SIGNAL(clicked()),
            this,              SLOT(runInitUser()));

    ui->infoLabel->setStyleSheet("color: red;");
}

void initialForm::autError()
{
    ui->infoLabel->setText(QTime::currentTime().toString() + ": Неверный логин или пароль!");
}

void initialForm::runInitUser()
{
    initUser(ui->loginLineEdit->text(), ui->passwordLineEdit->text());
}

initialForm::~initialForm()
{
    delete ui;
}
