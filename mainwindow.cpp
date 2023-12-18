#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtSerialPort/QSerialPortInfo>

int status_connect_com = 0;


QString version ="V1.0.1";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(version);
    ui->menu->setTitle("Конфігуратор дрон детектора - \"Ванільний цукор\"");

    ui->textEdit->setReadOnly(false);
    //ui->textEdit->setStyleSheet("background-color: rgb(4,13,135);");
    ui->textEdit->setStyleSheet("QTextEdit { background-color: rgb(4,13,135) }");

    ui->textEdit->setFontPointSize(10);
    ui->textEdit->setTextColor(QColor(200, 200, 0));
    ui -> textEdit -> insertPlainText("Сайт розробника дрон детектора:\nhttps://drone-spices.com/index.html\nhttps://bit.ly/dsc042\n");
    ui->textEdit->setTextColor(QColor(200, 200, 0));
    ui -> textEdit -> insertPlainText("\nЗауваження і пропозиції по конфігуратору надсилайте за цією адресою: centroparotjag@gmail.com");

    ui -> pushButton -> setText("З'єднати");
    //ui -> comboBox_2 -> setEnabled(0);        // відключений
    ui -> groupBox_2 -> setEnabled(0);          // відключений
    ui -> groupBox_4 -> setEnabled(0);          // конфігурація відключений
    ui -> groupBox_9 -> setEnabled(0);          // команда (comboBox_2) відключений
    ui -> groupBox_10 -> setEnabled(0);         // WI-FI термінал відключений
    ui -> pushButton_4 -> setEnabled(0);

    ui->textEdit->setFontPointSize(10);
    on_pushButton_5_clicked();                  // refresh com ports

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
    ui->textEdit->setTextColor(QColor(0, 255, 0));
    //ui -> textBrowser -> append(ba);
    ui -> textEdit -> insertPlainText(ba);

    serial_data_parser(ba);

    //----------- auto scroll text -------------------
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(cursor);
    //-----------------------------------------------
}


void MainWindow::on_pushButton_5_clicked()      // оновити доступні com ports
{
    ui -> comboBox -> clear();
    ui->textEdit->setTextColor(QColor(102, 178, 255));
    ui -> textEdit -> insertPlainText("\n----------------------------------------------------------------------------------\n");
    ui -> textEdit -> insertPlainText("Доступні для підключення порти:\n");

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts() ){
        QSerialPort port;
        port.setPort(info);
        if(port.open(QIODevice::ReadWrite))
        {
            //qDebug() << "Назва - " + info.portName() + " " + info.description() + info.manufacturer();

            ui -> textEdit -> insertPlainText(info.portName() + " " + info.description() + info.manufacturer());
            ui -> comboBox -> addItem(info.portName());
        }
    }
    ui -> textEdit -> insertPlainText("\n----------------------------------------------------------------------------------\n");
    //----------- auto scroll text -------------------
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(cursor);
    //-----------------------------------------------
}


void MainWindow::on_pushButton_clicked()            // підключити/відключити com port
{
        if (!status_connect_com){
            //qDebug() << "Порт вільний, підключаюсь";
            serial = new QSerialPort (this);
            serial -> setPortName(ui->comboBox->currentText());                // вказуєм com port для підключення з combobox
            serial -> setBaudRate(QSerialPort::Baud115200);
            serial -> setDataBits(QSerialPort::Data8);
            serial -> setParity(QSerialPort::NoParity);
            serial -> setStopBits(QSerialPort::OneStop);
            serial -> setFlowControl(QSerialPort::NoFlowControl);
            serial->setReadBufferSize(1024);

            if (serial -> open(QIODevice::ReadWrite) ){    // відкриваєм порт або перевіряєм його зайнятість
                //qDebug() << "Під'єднано";
                ui -> pushButton   -> setText("Від'єднати");
                ui -> groupBox ->setStyleSheet("QGroupBox {background-color:rgb(135, 223, 71);}");
                ui -> pushButton_5 -> setEnabled(0);        // конопка оновити відключена
                ui -> comboBox     -> setEnabled(0);        // список компортів відключена
                ui -> groupBox_2 -> setEnabled(1);          // включений
                //ui -> groupBox_4 -> setEnabled(1);          // включений
                ui -> groupBox_9 -> setEnabled(1);          // список команд включений
                ui -> groupBox_4 -> setEnabled(1);
                ui -> pushButton_4 -> setEnabled(1);
                ui -> groupBox_10 -> setEnabled(1);         // включений
                ui -> comboBox     -> colorCount();

                ui->textEdit->setTextColor(QColor(255, 255, 0));
                ui -> textEdit -> insertPlainText("Підключено до: " + ui->comboBox->currentText() + "\n");

                //----------- auto scroll text -------------------
                QTextCursor cursor = ui->textEdit->textCursor();
                cursor.movePosition(QTextCursor::End);
                ui->textEdit->setTextCursor(cursor);
                //-----------------------------------------------

                view_data_setting ();

            }
            connect(serial, SIGNAL(readyRead()), this, SLOT(serialRecieve()));
            status_connect_com = 1;
        }
        else {
            serial -> close();
            //qDebug() << "Порт зайнято, відєднуюсь";
            ui -> pushButton   -> setText("З'єднати");
            ui -> groupBox ->setStyleSheet("QGroupBox {background-color:rgb(241, 241, 241);}");
            ui -> pushButton_5 ->setEnabled(1);         // конопка оновити включений
            ui -> comboBox -> setEnabled(1);            // список компортів включений
            ui -> groupBox_2 -> setEnabled(0);          // відключений
            ui -> groupBox_4 -> setEnabled(0);          // відключений
            ui -> pushButton_4 -> setEnabled(0);
            ui -> groupBox_9 -> setEnabled(0);          // список команд відключений
            ui -> groupBox_10 -> setEnabled(0);         // відключений
            status_connect_com = 0;
            ui->textEdit->setTextColor(QColor(255, 255, 0));
            ui -> textEdit -> insertPlainText("Відключено від: " + ui->comboBox->currentText() + "\n");

            //----------- auto scroll text -------------------
            QTextCursor cursor = ui->textEdit->textCursor();
            cursor.movePosition(QTextCursor::End);
            ui->textEdit->setTextCursor(cursor);
            //-----------------------------------------------
        }
}



//----------- WI-FI and update FW ----------------
void MainWindow::on_pushButton_10_clicked()
{
    QString ssid = ui->lineEdit->text();
    QString pass = ui->lineEdit_2->text();
    QByteArray ba = "wifi client connect ";
    ba.append(ssid + " " + pass + ";");
    serial -> write(ba);
    ui->textEdit->setTextColor(QColor(255, 125, 125));
    ui -> textEdit -> insertPlainText("\n-->> wifi client connect "+ ssid + " " + pass + ";\n");
}

void MainWindow::on_pushButton_19_clicked()
{
    serial -> write("wifi client disconnect;");
    ui->textEdit->setTextColor(QColor(255, 125, 125));
    ui -> textEdit -> insertPlainText("\n-->> wifi client disconnect;\n");
}

void MainWindow::on_pushButton_20_clicked()
{
    serial -> write("update;");
    ui->textEdit->setTextColor(QColor(255, 125, 125));
    ui -> textEdit -> insertPlainText("\n-->> update;\n");
}
//----------------------------------------------------


void MainWindow::on_pushButton_22_clicked()   // write data from terminal
{
    QString qs = ui->lineEdit_15->text();
    QByteArray ba=0;
    ba += qs;
    serial -> write(ba);
    ui->textEdit->setTextColor(QColor(255, 125, 125));
    ui -> textEdit -> insertPlainText("\n-->> " + ba + "\n");
}


//------------------- combo box comands --------------------------
void MainWindow::on_comboBox_2_activated(void)
{
    ui->textEdit->setTextColor(QColor(255, 125, 125));
    ui -> textEdit -> insertPlainText("\n-->> " + ui-> comboBox_2 -> currentText()+ "\n");

    switch(ui-> comboBox_2 -> currentIndex()) {
      case 0:
        serial -> write("status;");
        break;
      case 1:
        serial -> write("settings;");
        break;
      case 2:
        serial -> write("restart;");
        break;
      case 3:
        serial -> write("wifi client scan;");
        break;
      case 4:
        serial -> write("update;");
        break;
      case 5:
        serial -> write("upload captures;");
        break;
      case 6:
        serial -> write("verbose 1;");
        break;
      case 7:
        serial -> write("verbose 0;");
        break;
      case 8:
        serial -> write("tracking enable;");
        break;
      case 9:
        serial -> write("tracking disable;");
        break;
      case 10:
        serial -> write("dji 1;");
        break;
      case 11:
        serial -> write("dji 0;");
        break;
      case 12:
        serial -> write("test alarm orlan;");
        break;
      case 13:
        serial -> write("test alarm eleron;");
        break;
      case 14:
        serial -> write("test alarm rssi;");
        break;
      case 15:
       serial -> write("test alarm tracking;");
       break;
    }
}


//------------ read config -----------------
void MainWindow::on_pushButton_2_clicked()
{
    ui->textEdit->setTextColor(QColor(255, 125, 125));
    ui -> textEdit -> insertPlainText("\n-->> settings;\n");
    serial -> write("settings;");
}

//------------- restore default -------------
void MainWindow::on_pushButton_4_clicked()
{
    ui->textEdit->setTextColor(QColor(255, 125, 125));
    ui -> textEdit -> insertPlainText("\n-->> reset settings;\n");
    serial -> write("reset settings;");
}

void MainWindow::on_pushButton_3_clicked()
{
    if(!freq_ranges[0] && !freq_ranges[1] && !freq_ranges[2] && !freq_ranges[3] && !freq_ranges[4] && !freq_ranges[5] &&
       !bitrates[0] && !bitrates[0] && !bitrates[0] && !bitrates[0])
    {
        ui->textEdit->setTextColor(QColor(255, 0, 0));
        ui -> textEdit -> insertPlainText("Error writing settings!\nData fields are empty.\n"
                                    "Fill in the settings fields or read the settings...\n");
    }
    else {

        ui->textEdit->setTextColor(QColor(255,255,0));
        ui -> textEdit -> insertPlainText("\nWriting parameters to the device.\n");

        freq_write();
        mode_write();
        function_write();
        rssi_treshold_write();
        bit_rates_write();
    }
}



void MainWindow::on_pushButton_11_clicked()
{
    ui->textEdit->setTextColor(QColor(255, 125, 125));
    ui -> textEdit -> insertPlainText("\n-->> wifi client scan;\n");
    serial -> write("wifi client scan;");
}
