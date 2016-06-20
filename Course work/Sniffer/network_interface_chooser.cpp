#include "network_interface_chooser.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

networkInterfaceChooser::networkInterfaceChooser(QList<QString> networkInterfaces,QWidget* parent) : QDialog(parent){
  this->networkInterfaces = networkInterfaces;
  networkInterfacesView = new QListWidget();
  networkInterfacesView->setSelectionMode(QAbstractItemView::SingleSelection);
  networkInterfacesView->addItems(this->networkInterfaces);

  QPushButton* okButton = new QPushButton("&Ok");
  QPushButton* cancelButton = new QPushButton("&Cancel");

  connect(okButton,SIGNAL(clicked()),SLOT(accept()));
  connect(cancelButton,SIGNAL(clicked()),SLOT(reject()));

  QHBoxLayout* buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);
  buttonLayout->setSpacing(20);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->addWidget(networkInterfacesView);
  layout->addLayout(buttonLayout);
  setLayout(layout);
  setWindowTitle("Choose interface to sniff:");
}

int networkInterfaceChooser :: getInterface(){
  return networkInterfacesView->currentRow();
}
