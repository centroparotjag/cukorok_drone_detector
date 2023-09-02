#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtSerialPort/QSerialPortInfo>

int status_connect_com = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui -> pushButton -> setText("З'єднати");
    on_pushButton_5_clicked();              // refresh com ports
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


void MainWindow::on_pushButton_5_clicked()      // оновити доступні com ports
{
    ui -> comboBox -> clear();

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts() ){
        QSerialPort port;
        port.setPort(info);
        if(port.open(QIODevice::ReadWrite))
        {
            qDebug() << "Назва - " + info.portName() + " " + info.description() + info.manufacturer();
            ui -> comboBox -> addItem(info.portName());
        }
    }
}


void MainWindow::on_pushButton_clicked()            // підключити/відключити com port
{
        if (!status_connect_com){
            qDebug() << "Порт вільний, підключаюсь";
            serial = new QSerialPort (this);
            serial -> setPortName(ui->comboBox->currentText());                   // вказуєм com port для підключення з combobox
            serial -> setBaudRate(QSerialPort::Baud115200);
            serial -> setDataBits(QSerialPort::Data8);
            serial -> setParity(QSerialPort::NoParity);
            serial -> setStopBits(QSerialPort::OneStop);
            serial -> setFlowControl(QSerialPort::NoFlowControl);

            if (serial -> open(QIODevice::ReadWrite) ){    // відкриваєм порт або перевіряєм його зайнятість
                qDebug() << "Під'єднано";
                ui -> pushButton   -> setText("Від'єднати");
                ui -> groupBox ->setStyleSheet("QGroupBox {background-color:rgb(135, 223, 71);}");
                ui -> pushButton_5 -> setEnabled(0);        // конопка оновити відключена
                ui -> comboBox     -> setEnabled(0);        // список компортів відключена
                ui -> comboBox     -> colorCount();
            }
            connect(serial, SIGNAL(readyRead()), this, SLOT(serialRecieve()));
            serial -> write("settings;");
            status_connect_com = 1;
        }
        else {
            serial -> close();
            qDebug() << "Порт зайнято, відєднуюсь";
            ui -> pushButton   -> setText("З'єднати");
            ui -> groupBox ->setStyleSheet("QGroupBox {background-color:rgb(241, 241, 241);}");
            ui -> pushButton_5 ->setEnabled(1);         // конопка оновити відключена
            ui -> comboBox -> setEnabled(1);            // список компортів відключена
            status_connect_com = 0;
        }
}




