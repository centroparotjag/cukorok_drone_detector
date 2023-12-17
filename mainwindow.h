#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void serialRecieve();

    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_22_clicked();

    void on_comboBox_2_activated(void);

    void on_pushButton_20_clicked();

    void on_pushButton_2_clicked();


    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    //void on_checkBox_fe1_clicked();

    void on_checkBox_fe2_clicked();

    void on_checkBox_fe3_clicked();

    void on_checkBox_be2_clicked();

    void on_checkBox_be3_clicked();

    void on_checkBox_be4_clicked();

public:

    int current_mode = 0;
    int freq_ranges[6] = {0};
    int rssi_threshold = 0;
    int tracking = 0;
    int bitrates[4] = {0};
    int spreading_factors[4] = {0};
    int dji_enable = 0;
    int rssi_scan_thr = 0;


    void serial_data_parser(QByteArray data);
    int detect_data(QByteArray data, QByteArray find);
    QByteArray parse_settings(QByteArray data, QByteArray find);
    void freq_ranges_parser(QByteArray data);
    int num_parser(QByteArray data, int* i);
    int keyword_search(QByteArray data, QByteArray keyword);
    int dji_parser(QByteArray data);
    int rssi_scan_threshold(QByteArray data);
    void view_data_setting (void);
    void freq_write(void);
    void mode_write(void);
    void function_write(void);
    void rssi_treshold_write(void);
    void bit_rates_write(void);

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;


};
#endif // MAINWINDOW_H
