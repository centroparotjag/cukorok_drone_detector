#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtSerialPort/QSerialPortInfo>

int status_connect_com = 0;

QString version ="V1.0.7";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(version);
    ui->menu->setTitle("Конфігуратор дрон детектора - \"Ванільний цукор\"");

    QPixmap      pix(":/resource/resource/cukorok_fone.png");
    ui->label_7->setPixmap(pix);

    ui->textBrowser->viewport()->setAutoFillBackground(false);      // transparency background
    ui->textBrowser->setReadOnly(true);
    //ui->textBrowser->setStyleSheet("background-color: rgb(4,13,135);");
    //ui->textBrowser->setStyleSheet("QtextBrowser { background-color: rgb(4,13,135) }");




    ui->textBrowser->setFontPointSize(10);
    ui -> textBrowser -> insertPlainText(get_date_time ("full"));
    ui->textBrowser->setTextColor(QColor(255, 255, 0));
    ui -> textBrowser -> insertPlainText("Зроблено на основі цього ресурсу: https://monitor.vanilka.com.ua/\n");
    ui -> textBrowser -> insertPlainText("https://drone-spices.com/index.html\n");
    ui -> textBrowser -> insertPlainText("https://vanilka.com.ua/\n");
    ui->textBrowser->setTextColor(QColor(255, 255, 0));
    ui -> textBrowser -> insertPlainText("Зауваження і пропозиції по конфігуратору надсилайте за адресою: centroparotjag@gmail.com\n");
    ui->textBrowser->setTextColor(QColor(255, 0, 0));
    ui -> textBrowser -> insertPlainText("\nЯкщо цукорок не відповідає на команди, вимкніть голосовий модуль (якщо такий у вас є)!\n");

    ui -> pushButton -> setText("З'єднати");
    //ui -> comboBox_2 -> setEnabled(0);        // відключений
    ui -> groupBox_2 -> setEnabled(0);          // відключений
    ui -> groupBox_4 -> setEnabled(0);          // конфігурація відключений
    ui -> groupBox_9 -> setEnabled(0);          // команда (comboBox_2) відключений
    ui -> groupBox_10 -> setEnabled(0);         // WI-FI термінал відключений
    ui -> pushButton_4 -> setEnabled(0);

    ui->textBrowser->setFontPointSize(10);
    on_pushButton_5_clicked();                  // refresh com ports

    //ui->radioButton_datim->setChecked(true);
    ui->radioButton_time->setChecked(true);
    //-------------------------------------------------------------------------------------------------------------------------------------------------------
    ui->pushButton_4->hide();                   // приховав за вимогою розробників кнопку - "Скинути на заводські налаштування", є глюк з цим функціоналом!!!
                                                // Є баг що скидається активація і користувачам доводиться повторно активувати через відправку кода адмінам.
    //-------------------------------------------------------------------------------------------------------------------------------------------------------


}

MainWindow::~MainWindow()
{
    delete ui;
    serial->close();
    delete serial;
}

QByteArray ba_all;
void MainWindow::serialRecieve(){
    QByteArray ba;
    ba = serial -> readAll();

    //ui -> textBrowser -> append(ba);

    if( (int)ba[ba.size()-1] < 0x20 ){
        ba_all += ba;
        ui -> textBrowser -> insertPlainText(get_date_time ("full"));
        ui->textBrowser->setTextColor(QColor(0, 255, 0));
        ui -> textBrowser -> insertPlainText(ba_all);
        serial_data_parser(ba_all);
        ba_all = 0;
    }
    else{
        ba_all += ba;
    }


//    ui -> textBrowser -> insertPlainText(ba);

//    serial_data_parser(ba);

    //----------- auto scroll text -------------------
    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(cursor);
    //-----------------------------------------------
}


void MainWindow::on_pushButton_5_clicked()      // оновити доступні com ports
{
    ui -> comboBox -> clear();
    ui -> textBrowser -> insertPlainText("\n");
    ui -> textBrowser -> insertPlainText(get_date_time ("full"));
    ui -> textBrowser -> setTextColor(QColor(255, 255, 255));
    ui -> textBrowser -> insertPlainText("----------------------------------------------------------------------------------\n");
    ui -> textBrowser -> insertPlainText("Доступні для підключення порти:\n");

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts() ){
        QSerialPort port;
        port.setPort(info);
        if(port.open(QIODevice::ReadWrite))
        {
            //qDebug() << "Назва - " + info.portName() + " " + info.description() + info.manufacturer();

            ui -> textBrowser -> insertPlainText(info.portName() + " " + info.description() + info.manufacturer() + "\n");
            ui -> comboBox -> addItem(info.portName());
        }
    }
    ui -> textBrowser -> insertPlainText("\n----------------------------------------------------------------------------------\n");
    //----------- auto scroll text -------------------
    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(cursor);
    //-----------------------------------------------
}

void MainWindow::auto_scroll_down(void){
    //----------- auto scroll text -------------------
    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(cursor);
    //-----------------------------------------------
}

void MainWindow::on_pushButton_clicked()            // підключити/відключити com port
{

    ui -> textBrowser -> insertPlainText(get_date_time ("full"));
        if (!status_connect_com){
            //qDebug() << "Порт вільний, підключаюсь";
            serial = new QSerialPort (this);
            serial->setSettingsRestoredOnClose(false);

            serial->close();  // close any previously open port

            serial -> setPortName(ui->comboBox->currentText());                // вказуєм com port для підключення з combobox
            serial -> setBaudRate(QSerialPort::Baud115200);
            serial -> setDataBits(QSerialPort::Data8);
            serial -> setParity(QSerialPort::NoParity);
            serial -> setStopBits(QSerialPort::OneStop);
            serial -> setFlowControl(QSerialPort::NoFlowControl);
            serial -> setReadBufferSize(1024);

            serial -> setDataTerminalReady(false);
            serial -> setRequestToSend(false);
            serial -> setSettingsRestoredOnClose(false);


            //serial -> ;


            if (serial -> open(QIODevice::ReadWrite) ){    // відкриваєм порт або перевіряєм його зайнятість
                serial -> setSettingsRestoredOnClose(false);
                //qDebug() << "Під'єднано";
                //qDebug() << "setDataTerminalReady" << serial->setDataTerminalReady(false);
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

                ui->textBrowser->setTextColor(QColor(255, 255, 0));
                ui -> textBrowser -> insertPlainText("Підключено до: " + ui->comboBox->currentText() + "\n");

                //----------- auto scroll text -------------------
                QTextCursor cursor = ui->textBrowser->textCursor();
                cursor.movePosition(QTextCursor::End);
                ui->textBrowser->setTextCursor(cursor);
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
            ui->textBrowser->setTextColor(QColor(255, 255, 0));
            ui -> textBrowser -> insertPlainText("Відключено від: " + ui->comboBox->currentText() + "\n");

            //----------- auto scroll text -------------------
            QTextCursor cursor = ui->textBrowser->textCursor();
            cursor.movePosition(QTextCursor::End);
            ui->textBrowser->setTextCursor(cursor);
            //-----------------------------------------------
        }
}



//----------- WI-FI and update FW ----------------
void MainWindow::on_pushButton_10_clicked()
{
    ui -> textBrowser -> insertPlainText("\n");
    ui -> textBrowser -> insertPlainText(get_date_time ("full"));
    QString ssid = ui->lineEdit->text();
    QString pass = ui->lineEdit_2->text();
    QByteArray ba = "wifi client connect ";
    ba.append(ssid + " " + pass + ";");
    serial -> write(ba);
    ui->textBrowser->setTextColor(QColor(255, 125, 125));
    ui -> textBrowser -> insertPlainText("-->> wifi client connect "+ ssid + " " + pass + ";\n");
}

void MainWindow::on_pushButton_19_clicked()
{
    ui -> textBrowser -> insertPlainText("\n");
    ui -> textBrowser -> insertPlainText(get_date_time ("full"));
    serial -> write("wifi client disconnect;");
    ui->textBrowser->setTextColor(QColor(255, 125, 125));
    ui -> textBrowser -> insertPlainText("-->> wifi client disconnect;\n");
}

void MainWindow::on_pushButton_20_clicked()
{
    ui -> textBrowser -> insertPlainText("\n");
    ui -> textBrowser -> insertPlainText(get_date_time ("full"));
    serial -> write("update;");
    ui->textBrowser->setTextColor(QColor(255, 125, 125));
    ui -> textBrowser -> insertPlainText("-->> update;\n");
}
//----------------------------------------------------


void MainWindow::on_pushButton_22_clicked()   // write data from terminal
{
    ui -> textBrowser -> insertPlainText("\n");
    ui -> textBrowser -> insertPlainText(get_date_time ("full"));
    QString qs = ui->lineEdit_15->text();
    QByteArray ba=0;
    ba += qs;
    serial -> write(ba);
    ui->textBrowser->setTextColor(QColor(255, 125, 125));
    ui -> textBrowser -> insertPlainText("-->> " + ba + "\n");
    auto_scroll_down();
}


//------------------- combo box comands --------------------------
void MainWindow::on_comboBox_2_activated(void)
{
    ui -> textBrowser -> insertPlainText("\n");
    ui -> textBrowser -> insertPlainText(get_date_time ("full"));
    ui->textBrowser->setTextColor(QColor(255, 125, 125));
    ui -> textBrowser -> insertPlainText("-->> " + ui-> comboBox_2 -> currentText()+ "\n");
    auto_scroll_down();

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
      case 16:
        serial -> write("log view;");
        break;
      case 17:
        serial -> write("log reset;");
        break;
      case 18:
        serial -> write("screensaver 1;");
        break;
      case 19:
        serial -> write("screensaver 0;");
        break;
      case 20:
        serial -> write("autoreturn 1;");
        break;
      case 21:
        serial -> write("autoreturn 0;");
       break;
    }
}


//------------ read config -----------------
void MainWindow::on_pushButton_2_clicked()
{
    ui -> textBrowser -> insertPlainText("\n");
    ui -> textBrowser -> insertPlainText(get_date_time ("full"));
    ui->textBrowser->setTextColor(QColor(255, 125, 125));
    ui -> textBrowser -> insertPlainText("-->> settings;\n");
    auto_scroll_down();
    serial -> write("settings;");
}

//------------- restore default -------------
void MainWindow::on_pushButton_4_clicked()
{
    ui -> textBrowser -> insertPlainText("\n");
    ui -> textBrowser -> insertPlainText(get_date_time ("full"));
    ui->textBrowser->setTextColor(QColor(255, 125, 125));
    ui -> textBrowser -> insertPlainText("\n-->> reset settings;\n");
    auto_scroll_down();
    serial -> write("reset settings;"); // Небезпечно!!!! Є баг що скидається активація і користувачам доводиться повторно активувати через відправку кода адмінам.
}

void MainWindow::on_pushButton_3_clicked()
{
    if(!freq_ranges[0] && !freq_ranges[1] && !freq_ranges[2] && !freq_ranges[3] && !freq_ranges[4] && !freq_ranges[5] &&
       !bitrates[0] && !bitrates[0] && !bitrates[0] && !bitrates[0])
    {
        ui->textBrowser->setTextColor(QColor(255, 0, 0));
        ui -> textBrowser -> insertPlainText("Error writing settings!\nData fields are empty.\n"
                                    "Fill in the settings fields or read the settings...\n");
    }
    else {

        ui->textBrowser->setTextColor(QColor(255,255,0));
        ui -> textBrowser -> insertPlainText("\nWriting parameters to the device.\n");

        ui -> textBrowser -> insertPlainText(get_date_time ("full"));
        freq_write();
        ui -> textBrowser -> insertPlainText(get_date_time ("full"));
        mode_write();
        ui -> textBrowser -> insertPlainText(get_date_time ("full"));
        function_write();
        ui -> textBrowser -> insertPlainText(get_date_time ("full"));
        rssi_treshold_write();
        ui -> textBrowser -> insertPlainText(get_date_time ("full"));
        bit_rates_write();
    }
}



void MainWindow::on_pushButton_11_clicked()
{
    ui -> textBrowser -> insertPlainText("\n");
    ui -> textBrowser -> insertPlainText(get_date_time ("full"));
    ui->textBrowser->setTextColor(QColor(255, 125, 125));
    ui -> textBrowser -> insertPlainText("-->> wifi client scan;\n");
    auto_scroll_down();
    serial -> write("wifi client scan;");
}

void MainWindow::on_pushButton__Clear_log_clicked()
{
    ui -> textBrowser -> clear();
}
