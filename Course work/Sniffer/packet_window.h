#ifndef PACKETWINDOW_H
#define PACKETWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include "packet_model.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QWidget>
#include <QHeaderView>
#include <QDebug>

class PacketWindow : public QMainWindow
{
    Q_OBJECT
  public:
    PacketWindow(QString title,QList<TCP_packet> listOfPackets,QWidget* parent = NULL);
  private:
    QTableView* packetsOfSession;
    PacketModel* model;
    QVBoxLayout* verticalLayout;
    QTextEdit* data;
    QWidget* centralWidget;
    QList<TCP_packet> listOfPackets;
  private slots:
    void doubleClickOnPacket(QModelIndex);
};

#endif // PACKETWINDOW_H
