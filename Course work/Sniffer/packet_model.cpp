#include "packet_model.h"

PacketModel::PacketModel(QObject* parent) : QAbstractTableModel(parent){

}

PacketModel :: PacketModel(QList<TCP_packet> listOfPackets, QObject *parent) : QAbstractTableModel(parent){
    this->listOfPackets = listOfPackets;
}

int PacketModel :: rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return listOfPackets.size();
}

int PacketModel :: columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return 5;
}

QVariant PacketModel :: data(const QModelIndex &index, int role) const{
    if(!index.isValid()){
        return QVariant();
    }

    if(index.row() >= listOfPackets.size()){
        return QVariant();
    }

    if(role == Qt::DisplayRole){
        TCP_packet packet = listOfPackets.at(index.row());
        if(index.column() == 0){
            return packet.getSourcePort();
        }
        if(index.column() == 1){
            return packet.getDestinationPort();
        }
        if(index.column() == 2){
            return packet.getProtocol();
        }
        if(index.column() == 3){
            return packet.getSize();
        }
        if(index.column() == 4){
            return packet.getChecksum();
        }
    }else{
        return QVariant();
    }
    return QVariant();
}

QVariant PacketModel :: headerData(int section, Qt::Orientation orientation, int role) const{
    if(role != Qt::DisplayRole){
        return QVariant();
    }
    if(orientation == Qt::Horizontal){
        switch(section){
        case 0:{
            return "Source port";
        }break;
        case 1:{
            return "Destination port";
        }break;
        case 2:{
            return "Protocol";
        }break;
        case 3:{
            return "Packet size(bytes)";
        }break;
        case 4:{
            return "Checksum";
        }break;
        default:{
            return QVariant();
        }break;
        }
    }
    return QVariant();
}
