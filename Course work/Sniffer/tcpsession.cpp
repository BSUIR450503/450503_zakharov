#include "tcpsession.h"

TcpSession ::TcpSession(){
}

TcpSession::TcpSession(QString sourceIP, QString destinatioIP){
    this->sourceIP = sourceIP;
    this->destinationIP = destinatioIP;
}

void TcpSession :: addPacket(TCP_packet newPacket){
    packets.append(newPacket);
}

void TcpSession :: setSourceIP(QString sourceIP){
  this->sourceIP = sourceIP;
}

void TcpSession :: setDestinationIP(QString destinationIP){
  this->destinationIP = destinationIP;
}

int TcpSession :: getNumberOfPackets(){
  //qDebug() << "Called";
  qDebug() << packets.size();
  return packets.size();
}

QString TcpSession :: getSourceIP(){
  return sourceIP;
}

QString TcpSession :: getDestinationIP(){
  return destinationIP;
}

TcpSession& TcpSession :: operator = (const TcpSession& session){
  this->packets = session.packets;
  this->sourceIP = session.sourceIP;
  this->destinationIP = session.destinationIP;
  return *this;
}

QList<TCP_packet>* TcpSession :: getPackets(){
  return &packets;
}

QList<TCP_packet> TcpSession :: getPacketsToSave(){
  return packets;
}

bool TcpSession :: operator == (TcpSession session){
  if(this->getSourceIP() == session.getSourceIP() && this->getDestinationIP() == session.getDestinationIP()){
    return true;
  }else{
    return false;
  }
}

bool operator == (TcpSession& first,TcpSession& second){
  if(first.getSourceIP() == second.getSourceIP() && first.getDestinationIP() == second.getDestinationIP()){
    return true;
  }else{
    return false;
  }
}
