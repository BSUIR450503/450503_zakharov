#include "session_model.h"

SessionModel::SessionModel(QObject* parent) : QAbstractTableModel(parent){

}

SessionModel :: SessionModel(QList<TcpSession> listOfSessions, QObject *parent) : QAbstractTableModel(parent){
  this->listOfSessions = listOfSessions;
}

int SessionModel :: rowCount(const QModelIndex &parent) const{
  Q_UNUSED(parent);
  return listOfSessions.count();
}

int SessionModel :: columnCount(const QModelIndex &parent) const{
  Q_UNUSED(parent);
  return 3;
}

QVariant SessionModel :: data(const QModelIndex &index, int role) const{
  if(!index.isValid()){
    return QVariant();
  }
  if(index.row() >= listOfSessions.size()){
    return QVariant();
  }
  if(role == Qt::DisplayRole){
    TcpSession session = listOfSessions.at(index.row());
    if(index.column() == 0){
      return session.getSourceIP();
    }
    if(index.column() == 1){
      return session.getDestinationIP();
    }
    if(index.column() == 2){
      qDebug() << session.getPackets()->size();
      return session.getPackets()->size();
    }
  }else{
    return QVariant();
  }
  return QVariant();
}

QVariant SessionModel :: headerData(int section, Qt::Orientation orientation, int role) const{
  if(role != Qt::DisplayRole){
    return QVariant();
  }
  if(orientation == Qt::Horizontal){
    switch (section){
      case 0:{
        return "Source IP";
      }break;
      case 1:{
        return "Destination IP";
      }break;
      case 2:{
        return "Number of packets";
      }break;
      default:{
        return QVariant();
      }
    }
  }
  return QVariant();
}

bool SessionModel :: insertRows(int position, int rows, const QModelIndex &index){
  Q_UNUSED(index);
  beginInsertRows(QModelIndex(),position,position + rows - 1);
  for(int row = 0; row < rows; row++){
    TcpSession session;
    listOfSessions.insert(position,session);
  }
  endInsertRows();
  return true;
}

bool SessionModel ::setData(const int row, const TcpSession &value, int role){
  if(row < listOfSessions.count() && role == Qt::EditRole){
    TcpSession session = listOfSessions.value(row);
    session = value;
    listOfSessions.replace(row,session);
    emit(dataChanged(QModelIndex(),QModelIndex()));
    return true;
  }
  return false;
}

void SessionModel :: addPacket(const int index, const TCP_packet &packet){
  listOfSessions[index].getPackets()->append(packet);
  emit (dataChanged(QModelIndex(),QModelIndex()));
}

void SessionModel :: clear(){
    listOfSessions.clear();
    emit(dataChanged(QModelIndex(),QModelIndex()));
}

Qt::ItemFlags SessionModel :: flags(const QModelIndex &index) const{
  if(!index.isValid()){
    return Qt::ItemIsEnabled;
  }
  return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

QList<TcpSession>* SessionModel :: getList(){
  return &listOfSessions;
}




