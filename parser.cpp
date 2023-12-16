#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtSerialPort/QSerialPortInfo>

QByteArray big_data = 0;

int MainWindow::detect_data(QByteArray data, QByteArray find){
    int num = 0;

    for(int i = 0; i< data.size(); i++){
        if (data[i] == find[num]){
            num++;
        }
        else {
            num = 0;
        }

        if (num == find.size()){
            return 1;
        }
    }
    return 0;
}


//current_mode=99,rssi_threshold=94, freq_ranges:["860000000-88500
//0000","895000000-928000000","970000000-1020000000"], tracking=0,
// bitrates=[57600,80000,15235,85100], spreading_factors=[6,7,8,0]


int current_mode = 0;
int freq_ranges[6] = {0};
int rssi_threshold = 0;
int tracking = 0;
int bitrates[4] = {0};
int spreading_factors[4] = {0};
int dji_enable = 0;

QByteArray MainWindow::parse_settings(QByteArray data, QByteArray find){
    int num = 0;
    int i = 0;
    for( ; i< data.size(); i++){
        if (data[i] == find[num]){
            num++;
        }
        else {
            num = 0;
        }

        if (num == find.size()){
            find = 0;
            num = 0;

            i++;
            for( ; data.at(i) != ','; i++){
                find[num] = data[i];
                num++;
            }
            return find;
        }
    }
    return 0;
}


int MainWindow::num_parser(QByteArray data, int* i){
    int k = *i;
    int n = 0;
    bool ok;
    QByteArray buf = 0;
    for ( ; (data.at(k)>= 0x30 && data.at(k)<= 0x39) ||
            (data.at(k)>= 0x41 && data.at(k)<= 0x46) ||
            (data.at(k)>= 0x61 && data.at(k)<= 0x66) ; k++ ) {
        buf[n] = data[k];
        n++;
    }

    for ( ; !((data.at(k)>= 0x30 && data.at(k)<= 0x39) ||
              (data.at(k)>= 0x41 && data.at(k)<= 0x46) ||
              (data.at(k)>= 0x61 && data.at(k)<= 0x66)) ; k++) {  }   //

    *i = k;
    return buf.toInt(&ok, 10);
}


int MainWindow::keyword_search(QByteArray data, QByteArray keyword){
    int num = 0;

    for(int i = 0; i< data.size(); i++) {
        if (data[i] == keyword[num]){
            num++;
        }
        else {
            num = 0;
        }

        if (num == keyword.size()){
            return i+1;
        }
    }

    return 0;
}


int MainWindow::freq_ranges_parser(QByteArray data){
    int i = 0;

    i = keyword_search(data, "freq_ranges:[\"");
    freq_ranges[0] = num_parser(data, &i);
    freq_ranges[1] = num_parser(data, &i);
    freq_ranges[2] = num_parser(data, &i);
    freq_ranges[3] = num_parser(data, &i);
    freq_ranges[4] = num_parser(data, &i);
    freq_ranges[5] = num_parser(data, &i);

    i = keyword_search(data, "bitrates=[");
    bitrates[0] = num_parser(data, &i);
    bitrates[1] = num_parser(data, &i);
    bitrates[2] = num_parser(data, &i);
    bitrates[3] = num_parser(data, &i);

    i = keyword_search(data, "spreading_factors=[");
    spreading_factors[0] = num_parser(data, &i);
    spreading_factors[1] = num_parser(data, &i);
    spreading_factors[2] = num_parser(data, &i);
    spreading_factors[3] = num_parser(data, &i);
}

int MainWindow::dji_parser(QByteArray data){
    int i = 0;
    i = keyword_search(data, "enabled=");
    return num_parser(data, &i);
}


int find_num = 0;

int MainWindow::serial_data_parser(QByteArray data){
    //-------------- find settings ------------------------------------
    if(detect_data(data, "current_mode") || detect_data(data, "tracking") || detect_data(data, "spreading_factors")||
            detect_data(data, "DJI:") || detect_data(data, "scan")){
        big_data.append(data);
        find_num++;

    }
    else {
        big_data = 0;
        find_num = 0;
    }

    if (find_num == 3 && detect_data(data, "spreading_factors")){
        find_num = 0;
        bool ok;
        current_mode   = parse_settings(big_data, "current_mode=").toInt(&ok, 10);
        rssi_threshold = parse_settings(big_data, "rssi_threshold=").toInt(&ok, 10);
        tracking       = parse_settings(big_data, "tracking=").toInt(&ok, 10);
        freq_ranges_parser(big_data);
        serial -> write("dji;");
    }

    //----------- dji --------------------
    if (detect_data(data, "scan")){
        dji_enable = dji_parser(big_data);
    }

    //-----------------------------------------------------------------
}
