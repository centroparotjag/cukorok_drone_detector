#include <QTextStream>
#include <QDate>
#include <QTime>

#include "mainwindow.h"
#include "ui_mainwindow.h"

QString  MainWindow::get_date_time (QString format){
    QString buff = 0;
    QTextStream out(stdout);
    //out.setCodec("CP866"); // устанавливаем CP866-кодировку для консоли

    QDate cd = QDate::currentDate(); // возвращаем текущую дату
    QTime ct = QTime::currentTime(); // возвращаем текущее время

//    out << cd.toString("dd-MM-yyyy") << endl; // в качестве разделителя используем дефис
//    out << ct.toString("hh:mm:ss.zzz")<< endl;




    if (ui->checkBox_datatime->isChecked()){
        ui->textBrowser->setTextColor(QColor(102, 178, 255));
        if (ui->radioButton_datim->isChecked()){
            buff = cd.toString("yyyy-MM-dd") + " " + ct.toString("hh:mm:ss.zzz") + "\n";
        }
        else {
            buff = ct.toString("hh:mm:ss.zzz") + "\n";
        }
    }

    return buff;



}
