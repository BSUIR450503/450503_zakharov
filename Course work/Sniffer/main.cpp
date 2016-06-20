#include "mainwindow.h"
#include <QApplication>
#include <stdio.h>

#ifdef _WIN32
#pragma comment(lib,"ws2_32.lib")
#endif

#include <QMessageBox>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

#ifdef _WIN32
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2,2),&wsa) != NO_ERROR){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Failed to initialized WSA!\nCode:" + QString::number(WSAGetLastError()));
        messageBox.setFixedSize(500,200);
        return 1;
    }
#endif

    MainWindow w;
    w.show();

    return a.exec();
}
