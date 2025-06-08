
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::processWeatherData(const QJsonDocument &doc)
{
    QJsonArray list = doc.object()["list"].toArray();
    QString result;

    for (const QJsonValue &val : list) {
        QJsonObject obj = val.toObject();
        QString dt = obj["dt_txt"].toString();

        qDebug() << dt;
        if (!isTargetHour(dt)) continue;

        double temp = obj["main"].toObject()["temp"].toDouble();
        QString weather = obj["weather"].toArray().at(0).toObject()["description"].toString();

        result = QString("%1 Темп: %2°C %3\n")
                     .arg(dt)
                     .arg(temp)
                     .arg(weather);
    }

    if (!result.isEmpty()) {
        ui->resultTextEdit->setPlainText(result);
        updateHistory(result);
    }
}

bool MainWindow::isTargetHour(const QString &dt)
{
    return dt.contains("15:00:00") || dt.contains("09:00:00") || dt.contains("21:00:00");
}

void MainWindow::updateHistory(const QString &entry)
{
    ui->historyTextEdit->append(entry);
}
