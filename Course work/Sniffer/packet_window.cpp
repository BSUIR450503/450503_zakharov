#include "packet_window.h"

PacketWindow :: PacketWindow(QString title,QList<TCP_packet> listOfPackets,QWidget* parent) : QMainWindow(parent){
  packetsOfSession = new QTableView();
  model = new PacketModel(listOfPackets);

  verticalLayout = new QVBoxLayout();
  data = new QTextEdit();

  centralWidget = new QWidget();

  this->listOfPackets = listOfPackets;

  packetsOfSession->setModel(model);
  packetsOfSession->setSelectionBehavior(QAbstractItemView::SelectRows);
  packetsOfSession->setSelectionMode(QAbstractItemView::SingleSelection);
  packetsOfSession->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  packetsOfSession->setColumnWidth(3,50);

  data->setReadOnly(true);

  verticalLayout->addWidget(packetsOfSession);
  verticalLayout->addWidget(data);

  centralWidget->setLayout(verticalLayout);

  this->setFixedSize(500,300);
  this->setWindowTitle(title);
  this->setCentralWidget(centralWidget);

  connect(packetsOfSession,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClickOnPacket(QModelIndex)));
}

void PacketWindow :: doubleClickOnPacket(QModelIndex index){
  Q_UNUSED(index);

  QModelIndexList item = packetsOfSession->selectionModel()->selectedRows();
  QString packetData = listOfPackets[item.first().row()].getAllData();
  QString result;
  for(int i = 0; i < packetData.length(); i++){
      result[i] = (packetData[i].toLatin1() >= 32 && packetData[i].toLatin1() <= 128) ? packetData[i].toLatin1() : '.';
  }
  data->setText(result);
}


