#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtSerialPort/QSerialPortInfo>


void MainWindow::freq_write(void){
    QByteArray buffer = 0;

    buffer.append("freq ranges");

    freq_ranges[0] = ui-> lineEdit_f0->text().toInt();
    freq_ranges[1] = ui-> lineEdit_f1->text().toInt();
    freq_ranges[2] = ui-> lineEdit_f2->text().toInt();
    freq_ranges[3] = ui-> lineEdit_f3->text().toInt();
    freq_ranges[4] = ui-> lineEdit_f4->text().toInt();
    freq_ranges[5] = ui-> lineEdit_f5->text().toInt();

    //--------------- 1 line frequency ----------
    if (freq_ranges[0]==0){
        freq_ranges[0] = 860000000;
        ui-> lineEdit_f0 -> setText(QString::number(freq_ranges[0]));
    }
    if (freq_ranges[1]==0){
        freq_ranges[1] = 885000000;
        ui-> lineEdit_f1 -> setText(QString::number(freq_ranges[1]));
    }


    buffer.append(" " + QString::number(freq_ranges[0]) + " " + QString::number(freq_ranges[1]));


    //--------------- 2 line frequency ----------
    if( (freq_ranges[2]==0 || freq_ranges[3]==0) || ui->checkBox_fe2->checkState()==0){
        ui-> lineEdit_f2->setEnabled(false);
        ui-> lineEdit_f3->setEnabled(false);
    }
    else {
        ui-> lineEdit_f2->setEnabled(true);
        ui-> lineEdit_f3->setEnabled(true);
        buffer.append(" " + QString::number(freq_ranges[2]) + " " + QString::number(freq_ranges[3]));
    }

    //--------------- 3 line frequency ----------
    if( (freq_ranges[4]==0 || freq_ranges[5]==0) || ui->checkBox_fe3->checkState()==0){
        ui-> lineEdit_f4->setEnabled(false);
        ui-> lineEdit_f5->setEnabled(false);
    }
    else {
        ui-> lineEdit_f4->setEnabled(true);
        ui-> lineEdit_f5->setEnabled(true);
        buffer.append(" " + QString::number(freq_ranges[4]) + " " + QString::number(freq_ranges[5]));
    }

    buffer.append(";");

    //qDebug() << buffer;

    ui->textEdit->setTextColor(QColor(255,125,125));
    ui -> textEdit -> insertPlainText("-->>" + buffer + "\n");

    serial -> write(buffer);
}



void MainWindow::on_checkBox_be2_clicked()
{
    ui-> lineEdit_br1->setEnabled(ui->checkBox_be2->checkState());
}

void MainWindow::on_checkBox_be3_clicked()
{
    ui-> lineEdit_br2->setEnabled(ui->checkBox_be3->checkState());
}

void MainWindow::on_checkBox_be4_clicked()
{
    ui-> lineEdit_br3->setEnabled(ui->checkBox_be4->checkState());
}


void MainWindow::bit_rates_write(void){
    QByteArray buffer = 0;

    buffer.append("fsk");

    bitrates[0] = ui-> lineEdit_br0->text().toInt();
    bitrates[1] = ui-> lineEdit_br1->text().toInt();
    bitrates[2] = ui-> lineEdit_br2->text().toInt();
    bitrates[3] = ui-> lineEdit_br3->text().toInt();

    //--------------- 1 line frequency ----------
    if (bitrates[0]==0){
        bitrates[0] = 52600;
        ui-> lineEdit_br0 -> setText(QString::number(bitrates[0]));
    }

    buffer.append(" " + QString::number(bitrates[0]));


    //--------------- 2 line frequency ----------
    if( bitrates[1]==0 || ui->checkBox_be2->checkState()==0 ){
        ui-> lineEdit_br1->setEnabled(false);
    }
    else {
        ui-> lineEdit_br1->setEnabled(true);
        buffer.append(" " + QString::number(bitrates[1]));
    }

    //--------------- 3 line frequency ----------
    if( freq_ranges[2]==0 || ui->checkBox_be3->checkState()==0 ){
        ui-> lineEdit_br2->setEnabled(false);
    }
    else {
        ui-> lineEdit_br2->setEnabled(true);
        buffer.append(" " + QString::number(bitrates[2]));
    }

    //--------------- 4 line frequency ----------
    if( freq_ranges[3]==0 || ui->checkBox_be3->checkState()==0 ){
        ui-> lineEdit_br3->setEnabled(false);
    }
    else {
        ui-> lineEdit_br3->setEnabled(true);
        buffer.append(" " + QString::number(bitrates[3]));
    }

    buffer.append(";");

    ui->textEdit->setTextColor(QColor(255,125,125));
    ui -> textEdit -> insertPlainText("-->> " + buffer + "\n");
    //qDebug() << buffer;
    serial -> write(buffer);



}


void MainWindow::on_checkBox_fe2_clicked()
{
    ui-> lineEdit_f2->setEnabled(ui->checkBox_fe2->checkState());
    ui-> lineEdit_f3->setEnabled(ui->checkBox_fe2->checkState());
}

void MainWindow::on_checkBox_fe3_clicked()
{
    ui-> lineEdit_f4->setEnabled(ui->checkBox_fe3->checkState());
    ui-> lineEdit_f5->setEnabled(ui->checkBox_fe3->checkState());
}


void MainWindow::mode_write(void){
    if(ui->radioButton_normal->isChecked()){
        ui->textEdit->setTextColor(QColor(255,125,125));
        ui -> textEdit -> insertPlainText("-->> mode normal;\n");
        serial -> write("mode normal;");
    }
    if(ui->radioButton_rssi->isChecked()){
        ui->textEdit->setTextColor(QColor(255,125,125));
        ui -> textEdit -> insertPlainText("-->> mode rssi;\n");
        serial -> write("mode rssi;");
    }
    if(ui->radioButton_sleep->isChecked()){
        ui->textEdit->setTextColor(QColor(255,125,125));
        ui -> textEdit -> insertPlainText("-->> mode sleep;\n");
        serial -> write("mode sleep;");
    }
}

void MainWindow::function_write(void){
    if(ui->checkBox_DJI->isChecked()){
        ui->textEdit->setTextColor(QColor(255,125,125));
        ui -> textEdit -> insertPlainText("-->> dji 1;\n");
        serial -> write("dji 1;");
    }
    else {
        ui->textEdit->setTextColor(QColor(255,125,125));
        ui -> textEdit -> insertPlainText("-->> dji 0;\n");
        serial -> write("dji 0;");
    }

    if(ui->checkBox_Tracking->isChecked()){
        ui->textEdit->setTextColor(QColor(255,125,125));
        ui -> textEdit -> insertPlainText("-->> tracking 1;\n");
        serial -> write("tracking 1;");
    }
    else {
        ui->textEdit->setTextColor(QColor(255,125,125));
        ui -> textEdit -> insertPlainText("-->> tracking 0;\n");
        serial -> write("tracking 0;");
    }
}


void MainWindow::rssi_treshold_write(void){
    QByteArray buffer = 0;
    rssi_threshold = ui->lineEdit_rssi_l->text().toInt();
    buffer.append("rssi threshold " + QString::number(rssi_threshold) + ";");
    ui->textEdit->setTextColor(QColor(255,125,125));
    ui -> textEdit -> insertPlainText("-->> " + buffer + "\n");
    serial -> write(buffer);
}



