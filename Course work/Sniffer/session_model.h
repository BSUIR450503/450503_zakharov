#ifndef SESSIONMODEL_H
#define SESSIONMODEL_H

#include <QAbstractTableModel>
#include "tcpsession.h"

class SessionModel : public QAbstractTableModel
{
    Q_OBJECT
  public:
    SessionModel(QObject* parent = NULL);
    SessionModel(QList<TcpSession> listOfSessions,QObject* parent = NULL);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool insertRows(int position, int rows, const QModelIndex &index);
    bool setData(const int row, const TcpSession &value, int role);

    void addPacket(const int index,const TCP_packet& packet);
    void clear();

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QList<TcpSession>* getList();
  private:
    QList<TcpSession> listOfSessions;
};

#endif // SESSIONMODEL_H
