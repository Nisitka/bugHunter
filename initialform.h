#ifndef INITIALFORM_H
#define INITIALFORM_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class initialForm;
}

class initialForm : public QWidget
{
    Q_OBJECT
signals:
    void initUser(const QString& login, const QString& password);

public slots:
    void autError();

public:
    explicit initialForm(QWidget *parent = nullptr);

    ~initialForm();

private slots:
    void runInitUser();

private:
    Ui::initialForm *ui;
};

#endif // INITIALFORM_H
