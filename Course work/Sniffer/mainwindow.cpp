#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
  this->setWindowTitle("Sniffer");
  this->setFixedSize(600,400);

  centralWidget = new QWidget();

  toolBar = new ToolBar("Controls",centralWidget);
  this->addToolBar(toolBar);

  sessionView = new QTableView();
  sessionModel = new SessionModel();
  sessionView->setModel(sessionModel);
  sessionView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  sessionView->setSelectionBehavior(QAbstractItemView::SelectRows);
  sessionView->setSelectionMode(QAbstractItemView::SingleSelection);
  sessionView->setEditTriggers(QAbstractItemView::NoEditTriggers);

  mainLayout = new QVBoxLayout();
  mainLayout->setSpacing(10);
  mainLayout->setMargin(15);
  mainLayout->addWidget(sessionView);
  centralWidget->setLayout(mainLayout);
  this->setCentralWidget(centralWidget);

  sniffer = NULL;

  connect(toolBar->getStartAction(),SIGNAL(triggered(bool)),this,SLOT(startPressed()));
  connect(toolBar->getStopAction(),SIGNAL(triggered(bool)),this,SLOT(stopPressed()));
  connect(sessionView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(sessionDoubleClicked(QModelIndex)));
  connect(toolBar->getSaveAction(),SIGNAL(triggered(bool)),this,SLOT(savePressed()));
}

MainWindow::~MainWindow(){

}

void MainWindow :: startPressed(){
  sessionModel->clear();
  sniffer = new Sniffer();
  connect(sniffer,SIGNAL(packetArrived(char*,int)),this,SLOT(newPacket(char*,int)));
  connect(sniffer,SIGNAL(finished()),this,SLOT(snifferStopped()));
  toolBar->getStartAction()->setEnabled(false);
#ifdef _WIN32
  networkInterfaceChooser chooser(sniffer->getNetworkInterfaces());
  if(chooser.exec() == QDialog::Accepted){
    qDebug() << sniffer->bindSocket(sniffer->getNetworkInterface(chooser.getInterface()));
    qDebug() << sniffer->enablePacketCapture();
    sniffer->start();
  }
#endif
 #ifdef __linux__
  sniffer->start();
#endif
}

void MainWindow :: newPacket(char *buffer,int size){
  TCP_packet* packet = new TCP_packet(buffer,size);
  TcpSession session(packet->getSourceIP(),packet->getDestinationIP());

  if(sessionModel->getList()->size() != 0 && sessionModel->getList()->contains(session)){
      int index = sessionModel->getList()->indexOf(session);
      sessionModel->addPacket(index,*packet);
  }else{
    sessionModel->insertRows(sessionModel->rowCount(),1,QModelIndex());
    session.getPackets()->append(*packet);
    sessionModel->setData(sessionModel->rowCount() - 1,session,Qt::EditRole);
  }
}

void MainWindow :: sessionDoubleClicked(QModelIndex index){
  Q_UNUSED(index);

  QModelIndexList item = sessionView->selectionModel()->selectedRows();
  TcpSession selectedSession = sessionModel->getList()->value(item.first().row());
  PacketWindow* window = new PacketWindow(selectedSession.getSourceIP() + " -> " + selectedSession.getDestinationIP(),selectedSession.getPacketsToSave(),this);
  window->show();
}

void MainWindow :: stopPressed(){
  if(sniffer){
    if(sniffer->isRunning()){
      sniffer->stop();
    }
  }
}

void MainWindow :: savePressed(){
  QModelIndexList item = sessionView->selectionModel()->selectedRows();
  if(item.size() != 0){
    QString fileName = QFileDialog::getSaveFileName(this,"Save TCP sessin",QDir::currentPath(),"Text files(*.txt)");
    FILE* saveFile;
    saveFile = fopen(fileName.toUtf8().data(),"w");
    QList<TCP_packet> packetsToSave = sessionModel->getList()->value(item.first().row()).getPacketsToSave();
    foreach(TCP_packet packet,packetsToSave){
      packet.saveInFile(saveFile);
    }
    fclose(saveFile);
    QMessageBox::information(this,"Success","Session saved in file: " + fileName);
  }
}

void MainWindow :: snifferStopped(){
    QMessageBox :: information(this,"Success","Sniffer stopped.");
    toolBar->getStartAction()->setEnabled(true);
}
