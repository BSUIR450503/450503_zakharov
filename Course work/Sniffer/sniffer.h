#ifndef SOCKET_H
#define SOCKET_H

#include <QList>
#include <QDebug>
#include <QThread>
#include <QMessageBox>

#include "tcp_packet.h"
#include "structures.h"

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#define SIO_RCVALL _WSAIOW(IOC_VENDOR,1)
#endif

#ifdef __linux__
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#endif

class Sniffer : public QThread
{
    Q_OBJECT
    public:
#ifdef _WIN32
        Sniffer(int sockType = SOCK_RAW, int protocol = IPPROTO_IP, int addressFamily = AF_INET);
        bool enablePacketCapture();
        bool bindSocket(struct in_addr networkInterface);
        QList<QString> getNetworkInterfaces();
        struct in_addr getNetworkInterface(int i);
#endif

#ifdef __linux__
        Sniffer(int sockType = SOCK_RAW, int protocol = IPPROTO_TCP, int addressFamily = AF_PACKET);
#endif
        void run();
        void stop();
        ~Sniffer();
    private:
#ifdef _WIN32
        SOCKET sniffer;
        char hostName[100];
        HOSTENT* localHost;
        QList<struct in_addr>* networkInterfaces;
#endif

#ifdef __linux__
        int sniffer;
#endif
        bool stopFlag;
  signals:
        void packetArrived(char* buffer,int size);
};

#endif // SOCKET_H
