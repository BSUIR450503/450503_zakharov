#ifndef STRUCTURES_H
#define STRUCTURES_H

#ifdef _WIN32
typedef struct IPHeader{
        unsigned char headerLength :4;
        unsigned char version :4;
        unsigned char typeOfService;
        unsigned short totalLength;
        unsigned short identificator;

        unsigned char fragmentOffsetFirstPart :5;

        unsigned char moreFragment :1;
        unsigned char dontFragment :1;
        unsigned char reserved :1;

        unsigned char fragmentOffsetSecondPart;

        unsigned char timeToLive;
        unsigned char protocol;
        unsigned short checksum;
        unsigned int sourceAddress;
        unsigned int destinationAddress;
}IPV4Header;

typedef struct TCPHeader{
        unsigned short sourcePort;
        unsigned short destinationPort;
        unsigned int sequenceNumber;
        unsigned int acknowledgementNumber;

         unsigned char nonseSumFlag :1;
        unsigned char reserved :3;
        unsigned char dataOffset :4;

        unsigned char finishFlag :1;
        unsigned char synchroniseFlag :1;
        unsigned char resetFlag :1;
        unsigned char pushFlag :1;
        unsigned char acknowledgementFlag :1;
        unsigned char urgentFlag :1;

        unsigned char ECN_echoFlag :1;
        unsigned char congestionWindowReducedFlag :1;

        unsigned short window;
        unsigned short checksum;
        unsigned short urgentPointer;
}TCPHeader;
#endif

#ifdef __linux__
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

#endif // STRUCTURES_H
