#ifndef NETWORKINTERFACECHOOSER_H
#define NETWORKINTERFACECHOOSER_H

#include <QDialog>
#include <QListWidget>

class networkInterfaceChooser : public QDialog
{
    Q_OBJECT
  public:
    networkInterfaceChooser(QList<QString>,QWidget* parent = NULL);
    int getInterface();
  private:
    QList<QString> networkInterfaces;
    QListWidget* networkInterfacesView;
};

#endif // NETWORKINTERFACECHOOSER_H
