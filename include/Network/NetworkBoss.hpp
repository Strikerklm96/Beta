#ifndef NETWORKBOSS_HPP
#define NETWORKBOSS_HPP

#include "stdafx.hpp"

struct NetworkBossData
{
    NetworkBossData() :
        sendIP("127.0.0.1"),
        sendPort(5000),
        receivePort(5000)
        {

        }

    sf::IpAddress sendIP;
    unsigned short sendPort;
    unsigned short receivePort;
};


class NetworkBoss
{
public:
    NetworkBoss(const NetworkBossData& rData);
    virtual ~NetworkBoss();

    sf::UdpSocket& getSocket();

    sf::Packet recieve();
    sf::Packet recieveLatest();
    void send(const sf::Packet& pack);

protected:
private:
    int latestNum;
    sf::IpAddress m_ip, m_receiveIP;
    unsigned short m_ipPort, m_receivePort;

    sf::UdpSocket m_socket;
};

#endif // NETWORKBOSS_HPP
