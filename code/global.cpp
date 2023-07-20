
#include "global.hpp"

//Variaveis compartilhadas
bool tabelaParticipantesUpdate = false;
mutex mtx, table_mtx;
string getLocalIpAddress()
{
    string erro = "erro";
    struct ifaddrs *ifaddr, *ifa;
    int family, s, n;
    char host[1025];
    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        return "not found";
    }

    for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++)
    {
        if (ifa->ifa_addr == NULL)
        {
            continue;
        }

        family = ifa->ifa_addr->sa_family;

        if (family == AF_INET || family == AF_INET6)
        {
            s = getnameinfo(ifa->ifa_addr, (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

            if (s != 0)
            {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                return "not found";
            }

            //printf("%s: %s\n", ifa->ifa_name, host);
            std::string name(ifa->ifa_name);
            if ("lo" != name)
            {
                std::string str(host);
                freeifaddrs(ifaddr);
                return str;
            }
        }
    }

    freeifaddrs(ifaddr);
    return "IP Address not found";
}

string gethostname()
{
    char hostname[256];
    int size = sizeof(hostname);
    gethostname(hostname, size);
    std::string str(hostname);
    if (hostname == NULL)
    {
        return getLocalIpAddress();
    }
    return hostname;
}

string getMacAddress()
{
    unsigned char *mac = new unsigned char[1];
    mac[0] = 1;
    char macAd[18];
    struct ifreq ifr;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    string erro = "erro";
    struct ifaddrs *ifaddr, *ifa;
    int family, s, n;
    char host[1025];
    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        //return 1;
    }

    for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++)
    {
        if (ifa->ifa_addr == NULL)
        {
            continue;
        }

        family = ifa->ifa_addr->sa_family;

        if (family == AF_INET || family == AF_INET6)
        {
            s = getnameinfo(ifa->ifa_addr, (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

            if (s != 0)
            {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                //return 1;
            }

            std::string name(ifa->ifa_name);
            if ("lo" != name)
            {
                std::string str(name);
                freeifaddrs(ifaddr);
                if (sockfd < 0)
                {
                    perror("Failed to create socket");
                    //return macAd;
                }

                strcpy(ifr.ifr_name, ifa->ifa_name);

                if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0)
                {
                    perror("ioctl failed");
                    //return macAd;
                }

                mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;

                snprintf(macAd, sizeof(macAd), "%02X:%02X:%02X:%02X:%02X:%02X",
                         mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
                std::string strmacAd(macAd);
                close(sockfd);
                return strmacAd;
            }
        }
    }

    freeifaddrs(ifaddr);
    return "MacAddress not found";
}

// Função para adicionar um novo nó à lista encadeada
void novoParticipante(participante *&tabelaParticipantes, std::string hostname, std::string ip_address, std::string mac_address, std::string status)
{
    participante *newparticipante = new participante;
    newparticipante->hostname = hostname;
    newparticipante->ip_address = ip_address;
    newparticipante->mac_address = mac_address;
    newparticipante->status = status;
    newparticipante->next = nullptr;

    if (tabelaParticipantes == nullptr)
    {
        tabelaParticipantes = newparticipante;
        return;
    }

    participante *currparticipante = tabelaParticipantes;
    while (currparticipante->next != nullptr)
    {
        currparticipante = currparticipante->next;
    }

    currparticipante->next = newparticipante;
}

void excluirParticipante(participante *&tabelaParticipantes, std::string mac_address)
{
    participante *participanteAtual = tabelaParticipantes;
    participante *participanteAnterior = nullptr;

    while (participanteAtual != nullptr)
    {
        if (participanteAtual->mac_address == mac_address)
        {
            if (participanteAnterior == nullptr)
            {
                tabelaParticipantes = participanteAtual->next;
            }
            else
            {
                participanteAnterior->next = participanteAtual->next;
            }
            delete participanteAtual;
            return;
        }
        participanteAnterior = participanteAtual;
        participanteAtual = participanteAtual->next;
    }
    cout << "user not found\n";

    return;
}
// Função para imprimir a lista encadeada
void printList(participante *tabelaParticipantes)
{
    participante *currparticipante = tabelaParticipantes;
    while (currparticipante != nullptr)
    {
        std::cout << ">> Hostname: " << currparticipante->hostname << ", IP: " << currparticipante->ip_address << ", MAC Address: " << currparticipante->mac_address << ", Status: " << currparticipante->status << std::endl;
        mtx.lock();
        currparticipante = currparticipante->next;
        mtx.unlock();
    }
}

bool setStatusTabela(participante *&tabelaParticipantes, std::string ip_address, std::string status)
{
    participante *participanteAtual = tabelaParticipantes;
    while (participanteAtual->ip_address != ip_address)
    {
        if (participanteAtual->next != nullptr)
        {
            participanteAtual = participanteAtual->next;
        }
        else
        {
            cout << "user not found\n";
            return false; //nao encontrei na lista
        }
    }
    participanteAtual->status = status;
    return true;
}

bool estaNaTabela(participante *&tabelaParticipantes, std::string mac_address)
{
    participante *participanteAtual = tabelaParticipantes;
    while (participanteAtual != nullptr)
    {
        if (participanteAtual->mac_address == mac_address)
        {
            return true;
        }

    participanteAtual = participanteAtual->next;

    }
    return false;
}
