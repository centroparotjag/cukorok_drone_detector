#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtSerialPort/QSerialPortInfo>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    // читаєм список доступних в системі ComPorts
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts() ){
        QSerialPort port;
        port.setPort(info);
        qDebug() << "Назва - " + info.portName() + " " + info.description() + info.manufacturer();
        if(port.open(QIODevice::ReadWrite))
        {
            qDebug() << "Назва - " + info.portName() + " " + info.description() + info.manufacturer();
        }

    }



    //----------------------------------------------
    serial = new QSerialPort (this);
    serial -> setPortName("com4");                   // вказуєм com port для підключення
    serial -> setBaudRate(QSerialPort::Baud115200);
    serial -> setDataBits(QSerialPort::Data8);
    serial -> setParity(QSerialPort::NoParity);
    serial -> setStopBits(QSerialPort::OneStop);
    serial -> setFlowControl(QSerialPort::NoFlowControl);
    serial -> open(QIODevice::ReadWrite);            // відкриваєм порт


    connect(serial, SIGNAL(readyRead()), this, SLOT(serialRecieve()));

    serial -> write("settings ;");                   // відправляєм в компорт restart
}

MainWindow::~MainWindow()
{
    delete ui;

    serial->close();
    delete serial;
}

void MainWindow::serialRecieve(){
    QByteArray ba;
    ba = serial -> readAll();
    ui -> textBrowser -> append(ba);
}


