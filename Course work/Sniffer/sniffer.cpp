#include "sniffer.h"

#ifdef _WIN32
Sniffer::Sniffer(int socketType,int protocol,int addressFamily){
    sniffer = INVALID_SOCKET;
    sniffer = socket(addressFamily,socketType,protocol);

    gethostname(hostName,sizeof(hostName));
    localHost = gethostbyname(hostName);

    networkInterfaces = new QList<struct in_addr>();

    for(int i = 0; localHost->h_addr_list[i] != 0; i++){
        struct in_addr networkInterface;
        ZeroMemory(&networkInterface,sizeof(struct in_addr));
        CopyMemory(&networkInterface,localHost->h_addr_list[i],sizeof(struct in_addr));
        networkInterfaces->push_back(networkInterface);
    }
    stopFlag = false;
}
#endif

#ifdef __linux__
Sniffer::Sniffer(int socketType,int protocol,int addressFamily){
    Q_UNUSED(protocol);
    sniffer = socket(addressFamily,socketType,htons(ETH_P_ALL));
    stopFlag = false;
}
#endif

#ifdef _WIN32
bool Sniffer :: bindSocket(struct in_addr networkInterface){
    struct sockaddr_in destination;
    ZeroMemory(&destination,sizeof(destination));
    CopyMemory(&destination.sin_addr.s_addr,&networkInterface,sizeof(destination.sin_addr.s_addr));
    destination.sin_family = AF_INET;
    destination.sin_port = 0;
    if(bind(sniffer,(struct sockaddr*) &destination,sizeof(destination)) == SOCKET_ERROR){
        return false;
    }
    return true;
}

bool Sniffer::enablePacketCapture(){
    int j = 1;
    int in;
    if(WSAIoctl(sniffer,SIO_RCVALL,&j,sizeof(j),0,0,(LPDWORD) &in,0,0) == SOCKET_ERROR){
        return false;
    }
    return true;
}

QList<QString> Sniffer :: getNetworkInterfaces(){
    QList<QString> networkAddress;
    for(int i = 0; i < networkInterfaces->size(); i++)
        networkAddress.append(inet_ntoa(networkInterfaces->at(i)));
    return networkAddress;
}

struct in_addr Sniffer :: getNetworkInterface(int i){
    return networkInterfaces->at(i);
}
#endif

void Sniffer :: run(){
    char* buffer = (char*)malloc(65536);
    int bytesArrived;

#ifdef __linux__
    int saddr_size;
    struct sockaddr saddr;
#endif

    if(buffer == NULL){
        qDebug() << "malloc() failed.\n";
    }
    do{
#ifdef _WIN32
        bytesArrived = recvfrom(sniffer,buffer,65536,0,0,0);
#endif

#ifdef __linux__
        saddr_size = sizeof(saddr);
        bytesArrived = recvfrom(sniffer,buffer,65536,0,&saddr,(socklen_t*)&saddr_size);
#endif
        if(bytesArrived > 0){
#ifdef _WIN32
            IPHeader* ipHeader = (IPV4Header*)buffer;
#endif

#ifdef __linux__
            struct iphdr* ipHeader = (struct iphdr*)(buffer + sizeof(struct ethhdr));
#endif
            if(ipHeader->protocol == 6)
                emit packetArrived(buffer,bytesArrived);
        }else{
            qDebug() << "recvfrom() failed.\n";
        }
    }while(!stopFlag);
    free(buffer);
}

void Sniffer :: stop(){
    stopFlag = true;
}

Sniffer :: ~Sniffer(){
#ifdef _WIN32
    if(sniffer != INVALID_SOCKET){
        closesocket(sniffer);
    }
#endif
}
