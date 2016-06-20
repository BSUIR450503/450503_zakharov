#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <toolbar.h>
#include "sniffer.h"
#include "session_model.h"
#include "network_interface_chooser.h"
#include <QFileDialog>
#include "packet_model.h"
#include "packet_window.h"
#include <QMessageBox>

class MainWindow : public QMainWindow
{
        Q_OBJECT
    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();
    private:
        QWidget* centralWidget;
        ToolBar* toolBar;
        QVBoxLayout* mainLayout;
        QTableView* sessionView;
        SessionModel* sessionModel;
        Sniffer* sniffer;
    private slots:
        void startPressed();
        void stopPressed();
        void savePressed();
        void snifferStopped();
        void newPacket(char* buffer,int size);
        void sessionDoubleClicked(QModelIndex);
};

#endif // MAINWINDOW_H
