#include "tcp_packet.h"

TCP_packet :: TCP_packet(){
    tcpHeader = NULL;
    ipHeader = NULL;
}

TCP_packet :: TCP_packet(char* buffer,int size){
#ifdef _WIN32
    unsigned short ipHeaderLength;

    ipHeader = (IPV4Header*)malloc(size);
    memcpy(ipHeader,buffer,size);

    ipHeaderLength = ipHeader->headerLength * 4;

    tcpHeader = (TCPHeader*)malloc(size - ipHeaderLength);
    memcpy(tcpHeader,buffer + ipHeaderLength,size - ipHeaderLength);

    ipHeaderData = getData(buffer,ipHeaderLength);
    tcpHeaderData = getData(buffer + ipHeaderLength,tcpHeader->dataOffset * 4);
    data = getData(buffer + ipHeaderLength + tcpHeader->dataOffset * 4,(size - tcpHeader -> dataOffset * 4 - ipHeader->headerLength * 4));
#endif

#ifdef __linux__
    unsigned short ipHeaderLength;

    ethernetHeader = (struct ethhdr*)malloc(sizeof(struct ethhdr));
    memcpy(ethernetHeader,buffer,sizeof(ethhdr));

    ipHeader =(struct iphdr*)malloc(sizeof(struct iphdr));
    memcpy(ipHeader,buffer + sizeof(struct ethhdr),sizeof(struct iphdr));
    ipHeaderLength = ipHeader->ihl * 4;

    tcpHeader = (struct tcphdr*)malloc(sizeof(struct tcphdr));
    memcpy(tcpHeader,buffer + ipHeaderLength + sizeof(struct ethhdr),sizeof(struct tcphdr));

    ipHeaderData = getData(buffer,ipHeaderLength);
    tcpHeaderData = getData(buffer + ipHeaderLength,tcpHeader->doff * 4);

    data = getData(buffer + ipHeaderLength + tcpHeader->doff * 4,(size - tcpHeader->doff * 4 - ipHeaderLength));
#endif
}

TCP_packet :: TCP_packet(const TCP_packet &packet){
#ifdef _WIN32
    this->tcpHeader = (TCPHeader*)malloc(sizeof(packet.tcpHeader));
    memcpy(this->tcpHeader,packet.tcpHeader,sizeof(packet.tcpHeader));
    this->ipHeader = (IPV4Header*)malloc(sizeof(packet.ipHeader));
    memcpy(this->ipHeader,packet.ipHeader,sizeof(packet.ipHeader));
#endif

#ifdef __linux__
    this->tcpHeader = (struct tcphdr*)malloc(sizeof(struct tcphdr));
    memcpy(this->tcpHeader,packet.tcpHeader,sizeof(struct tcphdr));
    this->ipHeader = (struct iphdr*)malloc(sizeof(struct iphdr));
    memcpy(this->ipHeader,packet.ipHeader,sizeof(struct iphdr));
#endif
    this->tcpHeaderData = packet.tcpHeaderData;
    this->ipHeaderData = packet.ipHeaderData;
    this->data = packet.data;
}

QString TCP_packet :: getSourceIP(){
    struct sockaddr_in source;

#ifdef _WIN32
    ZeroMemory(&source,sizeof(source));
    source.sin_addr.s_addr = ipHeader->sourceAddress;
#endif

#ifdef __linux__
    memset(&source,0,sizeof(source));
    source.sin_addr.s_addr = ipHeader->saddr;
#endif

    return inet_ntoa(source.sin_addr);
}

QString TCP_packet :: getDestinationIP(){
    struct sockaddr_in destination;

#ifdef _WIN32
    ZeroMemory(&destination,sizeof(destination));
    destination.sin_addr.s_addr = ipHeader->destinationAddress;
#endif

#ifdef __linux__
    memset(&destination,0,sizeof(destination));
    destination.sin_addr.s_addr = ipHeader->daddr;
#endif

    return inet_ntoa(destination.sin_addr);
}

u_short TCP_packet :: getSourcePort(){
#ifdef _WIN32
    return ntohs(tcpHeader->sourcePort);
#endif

#ifdef __linux__
    return ntohs(tcpHeader->source);
#endif
}

u_short TCP_packet :: getDestinationPort(){
#ifdef _WIN32
    return ntohs(tcpHeader->destinationPort);
#endif

#ifdef __linux__
    return ntohs(tcpHeader->dest);
#endif
}

QString TCP_packet :: getProtocol(){
    return "TCP";
}

unsigned int TCP_packet :: getSize(){
#ifdef _WIN32
    return ntohs(ipHeader->totalLength) + (unsigned int)tcpHeader->dataOffset * 4;
#endif

#ifdef __linux__
    return ntohs(ipHeader->tot_len) + (unsigned int)tcpHeader->doff * 4;
#endif
}

QString TCP_packet :: getAllData(){
    return ipHeaderData + tcpHeaderData + data;
}

QString TCP_packet :: getData(char *data, int size){
    QString packetData;
    for(int i = 0; i < size; i++){
        packetData += data[i];
    }
    return packetData;
}

u_short TCP_packet::getChecksum(){
#ifdef _WIN32
    return ntohs(tcpHeader->checksum);
#endif

#ifdef __linux__
    return ntohs(tcpHeader->check);
#endif
}

u_short TCP_packet :: getUrgentPointer(){
#ifdef _WIN32
    return tcpHeader->urgentPointer;
#endif

#ifdef __linux__
    return tcpHeader->urg_ptr;
#endif
}

TCP_packet& TCP_packet :: operator = (const TCP_packet& packet){
    delete this->ipHeader;
    delete this->tcpHeader;

#ifdef _WIN32
    this->tcpHeader = (TCPHeader*)malloc(sizeof(packet.tcpHeader));
    memcpy(this->tcpHeader,packet.tcpHeader,sizeof(packet.tcpHeader));
    this->ipHeader = (IPV4Header*)malloc(sizeof(packet.ipHeader));
    memcpy(this->ipHeader,packet.ipHeader,sizeof(packet.ipHeader));
#endif

#ifdef __linux__
    this->ethernetHeader = (struct ethhdr*)malloc(sizeof(struct ethhdr));
    memcpy(this->ethernetHeader,packet.ethernetHeader,sizeof(struct ethhdr));
    this->tcpHeader = (struct tcphdr*)malloc(sizeof(struct tcphdr));
    memcpy(this->tcpHeader,packet.tcpHeader,sizeof(struct tcphdr));
    this->ipHeader = (struct iphdr*)malloc(sizeof(struct iphdr));
    memcpy(this->ipHeader,packet.ipHeader,sizeof(struct iphdr));
#endif
    this->tcpHeaderData = packet.tcpHeaderData;
    this->ipHeaderData = packet.ipHeaderData;
    this->data = packet.data;
    return *this;
}

void TCP_packet :: saveInFile(FILE *file){
    fprintf(file,"\n\n***********************TCP Packet*************************\n");

#ifdef __linux__
    fprintf(file,"\n");
    fprintf(file,"Ethernet header\n");
    saveEthernetHeader(file);
#endif

    fprintf(file,"\n");
    fprintf(file,"IP Header\n");
    saveIpHeader(file);

    fprintf(file,"\n");
    fprintf(file,"TCP Header\n");
    saveTcpHeader(file);

    fprintf(file,"\n");
    fprintf(file," DATA Dump ");
    fprintf(file,"\n");

    fprintf(file,"IP Header\n");
    saveData(file,ipHeaderData);

    fprintf(file,"TCP Header\n");
    saveData(file,tcpHeaderData);

    fprintf(file,"Data Payload\n");
    saveData(file,data);

    fprintf(file,"\n###########################################################");

}

#ifdef __linux__
void TCP_packet ::saveEthernetHeader(FILE *file){
    fprintf(file , "\n");
    fprintf(file , "Ethernet Header\n");
    fprintf(file , "   |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n",
            ethernetHeader->h_dest[0] , ethernetHeader->h_dest[1] , ethernetHeader->h_dest[2] ,
            ethernetHeader->h_dest[3] , ethernetHeader->h_dest[4] , ethernetHeader->h_dest[5] );
    fprintf(file , "   |-Source Address      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n",
            ethernetHeader->h_source[0] , ethernetHeader->h_source[1] , ethernetHeader->h_source[2] ,
            ethernetHeader->h_source[3] , ethernetHeader->h_source[4] , ethernetHeader->h_source[5] );
    fprintf(file , "   |-Protocol            : %u \n",(unsigned short)ethernetHeader->h_proto);
}

#endif

void TCP_packet :: saveIpHeader(FILE *file){
    struct sockaddr_in source,destination;

#ifdef _WIN32
    unsigned short ipHeaderLenght;

    ZeroMemory(&source,sizeof(source));
    ZeroMemory(&destination,sizeof(destination));
    source.sin_addr.s_addr = ipHeader->sourceAddress;
    destination.sin_addr.s_addr = ipHeader->destinationAddress;
    ipHeaderLenght = ipHeader->headerLength * 4;

    fprintf(file," |-IP Version : %d\n",(unsigned int) ipHeader->version);
    fprintf(file," |-IP Header Length : %d DWORDS or %d Bytes\n",(unsigned int)ipHeader->headerLength,((unsigned int)(ipHeader->headerLength)) * 4);
    fprintf(file," |-Type Of Service : %d\n",(unsigned int)ipHeader->typeOfService);
    fprintf(file," |-IP Total Length : %d Bytes(Size of Packet)\n",ntohs(ipHeader->totalLength));
    fprintf(file," |-Identification : %d\n",ntohs(ipHeader->identificator));
    fprintf(file," |-Reserved ZERO Field : %d\n",(unsigned int)ipHeader->reserved);
    fprintf(file," |-Dont Fragment Field : %d\n",(unsigned int)ipHeader->dontFragment);
    fprintf(file," |-More Fragment Field : %d\n",(unsigned int)ipHeader->moreFragment);
    fprintf(file," |-TTL : %d\n",(unsigned int)ipHeader->timeToLive);
    fprintf(file," |-Protocol : %d\n",(unsigned int)ipHeader->protocol);
    fprintf(file," |-Checksum : %d\n",ntohs(ipHeader->checksum));
    fprintf(file," |-Source IP : %s\n",inet_ntoa(source.sin_addr));
    fprintf(file," |-Destination IP : %s\n",inet_ntoa(destination.sin_addr));
#endif

#ifdef __linux__
    memset(&source,0,sizeof(source));
    memset(&destination,0,sizeof(destination));
    source.sin_addr.s_addr = ipHeader->saddr;
    destination.sin_addr.s_addr = ipHeader->daddr;

    fprintf(file," |-IP Version : %d\n",(unsigned int) ipHeader->version);
    fprintf(file," |-IP Header Length : %d DWORDS or %d Bytes\n",(unsigned int)ipHeader->ihl,((unsigned int)(ipHeader->ihl)) * 4);
    fprintf(file," |-Type Of Service : %d\n",(unsigned int)ipHeader->tos);
    fprintf(file," |-IP Total Length : %d Bytes(Size of Packet)\n",ntohs(ipHeader->tot_len));
    fprintf(file," |-Identification : %d\n",ntohs(ipHeader->id));
    fprintf(file," |-TTL : %d\n",(unsigned int)ipHeader->ttl);
    fprintf(file," |-Protocol : %d\n",(unsigned int)ipHeader->protocol);
    fprintf(file," |-Checksum : %d\n",ntohs(ipHeader->check));
    fprintf(file," |-Source IP : %s\n",inet_ntoa(source.sin_addr));
    fprintf(file," |-Destination IP : %s\n",inet_ntoa(destination.sin_addr));
#endif
}

void TCP_packet :: saveTcpHeader(FILE *file){
#ifdef _WIN32
    fprintf(file," |-Source Port : %u\n",ntohs(tcpHeader->sourcePort));
    fprintf(file," |-Destination Port : %u\n",ntohs(tcpHeader->destinationPort));
    fprintf(file," |-Sequence Number : %u\n",ntohl(tcpHeader->sequenceNumber));
    fprintf(file," |-Acknowledge Number : %u\n",ntohl(tcpHeader->acknowledgementNumber));
    fprintf(file," |-Header Length : %d DWORDS or %d BYTES\n"
            ,(unsigned int)tcpHeader->dataOffset,(unsigned int)tcpHeader->dataOffset * 4);
    fprintf(file," |-CWR Flag : %d\n",(unsigned int)tcpHeader->congestionWindowReducedFlag);
    fprintf(file," |-ECN Flag : %d\n",(unsigned int)tcpHeader->ECN_echoFlag);
    fprintf(file," |-Urgent Flag : %d\n",(unsigned int)tcpHeader->urgentFlag);
    fprintf(file," |-Acknowledgement Flag : %d\n",(unsigned int)tcpHeader->acknowledgementFlag);
    fprintf(file," |-Push Flag : %d\n",(unsigned int)tcpHeader->pushFlag);
    fprintf(file," |-Reset Flag : %d\n",(unsigned int)tcpHeader->resetFlag);
    fprintf(file," |-Synchronise Flag : %d\n",(unsigned int)tcpHeader->synchroniseFlag);
    fprintf(file," |-Finish Flag : %d\n",(unsigned int)tcpHeader->finishFlag);
    fprintf(file," |-Window : %d\n",ntohs(tcpHeader->window));
    fprintf(file," |-Checksum : %d\n",ntohs(tcpHeader->checksum));
    fprintf(file," |-Urgent Pointer : %d\n",tcpHeader->urgentPointer);
#endif

#ifdef __linux__
    fprintf(file," |-Source Port : %u\n",ntohs(tcpHeader->source));
    fprintf(file," |-Destination Port : %u\n",ntohs(tcpHeader->dest));
    fprintf(file," |-Sequence Number : %u\n",ntohl(tcpHeader->seq));
    fprintf(file," |-Acknowledge Number : %u\n",ntohl(tcpHeader->ack_seq));
    fprintf(file," |-Header Length : %d DWORDS or %d BYTES\n"
            ,(unsigned int)tcpHeader->doff,(unsigned int)tcpHeader->doff * 4);
    fprintf(file," |-Urgent Flag : %d\n",(unsigned int)tcpHeader->urg);
    fprintf(file," |-Acknowledgement Flag : %d\n",(unsigned int)tcpHeader->ack);
    fprintf(file," |-Push Flag : %d\n",(unsigned int)tcpHeader->psh);
    fprintf(file," |-Reset Flag : %d\n",(unsigned int)tcpHeader->rst);
    fprintf(file," |-Synchronise Flag : %d\n",(unsigned int)tcpHeader->syn);
    fprintf(file," |-Finish Flag : %d\n",(unsigned int)tcpHeader->fin);
    fprintf(file," |-Window : %d\n",ntohs(tcpHeader->window));
    fprintf(file," |-Checksum : %d\n",ntohs(tcpHeader->check));
    fprintf(file," |-Urgent Pointer : %d\n",tcpHeader->urg_ptr);
#endif
}

void TCP_packet :: saveData(FILE *file, QString data){
    char a,c,line[17];
    int j;

    for(int i = 0; i < data.length(); i++){
        c = data.at(i).toLatin1();
        fprintf(file," %.2x", (unsigned char) c);

        a = (c >= 32 && c <= 128) ? (unsigned char) c : '.';

        line[i % 16] = a;
        if((i != 0 && (i + 1) % 16 == 0) || i == data.length() - 1){
            line[i % 16 + 1] = '\0';
            fprintf(file ,"          ");

            for(j = (int)strlen(line); j < 16; j++){
                fprintf(file , "   ");
            }
            fprintf(file , "%s \n" , line);
        }
    }
    fprintf(file , "\n");
}
