#ifndef TCP_PACKET_H
#define TCP_PACKET_H

#include "structures.h"
#include <QString>

#ifdef _WIN32
#include <WinSock2.h>
#endif

class TCP_packet
{
    public:
        TCP_packet();
        TCP_packet(char* buffer,int size);
        TCP_packet(const TCP_packet& packet);
        QString getSourceIP();
        QString getDestinationIP();
        void saveInFile(FILE* file);
        u_short getSourcePort();
        u_short getDestinationPort();
        QString getProtocol();
        u_short getChecksum();
        u_short getUrgentPointer();
        unsigned int getSize();
        QString getAllData();
        TCP_packet& operator = (const TCP_packet& packet);
    private:
#ifdef _WIN32
        TCPHeader* tcpHeader;
        IPV4Header* ipHeader;
#endif

#ifdef __linux__
        struct ethhdr* ethernetHeader;
        struct iphdr* ipHeader;
        struct tcphdr* tcpHeader;
        void saveEthernetHeader(FILE* file);
#endif
        QString ipHeaderData;
        QString tcpHeaderData;
        QString data;

        QString getData(char* data,int size);
        void saveIpHeader(FILE* file);
        void saveTcpHeader(FILE* file);
        void saveData(FILE* file,QString data);
};

#endif // TCP_PACKET_H
