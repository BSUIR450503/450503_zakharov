#ifndef PACKETMODEL_H
#define PACKETMODEL_H

#include <QAbstractTableModel>
#include "tcp_packet.h"

class PacketModel : public QAbstractTableModel
{
  public:
    PacketModel(QObject* parent = NULL);
    PacketModel(QList<TCP_packet> listOfPackets,QObject* parent = NULL);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  private:
    QList<TCP_packet> listOfPackets;
};

#endif // PACKETMODEL_H
