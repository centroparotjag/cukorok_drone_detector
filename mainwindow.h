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

    void on_comboBox_2_activated(const QString &arg1);

    void on_pushButton_20_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;


};
#endif // MAINWINDOW_H
