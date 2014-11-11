#include "NetworkBoss.hpp"

using namespace std;
using namespace sf;

NetworkBoss::NetworkBoss(const NetworkBossData& rData)
{
    latestNum = 0;
    m_ip = rData.sendIP;
    m_ipPort = rData.sendPort;
    m_receivePort = rData.receivePort;
    m_socket.setBlocking(false);

    m_socket.unbind();
    if(m_socket.bind(m_receivePort) != sf::Socket::Done)
    {
        cout << "\nThere was an error binding to [" << m_receivePort << "]";
    }
    else
        cout << "\nSuccessful bind.";
}
NetworkBoss::~NetworkBoss()
{

}
sf::UdpSocket& NetworkBoss::getSocket()
{
    return m_socket;
}
sf::Packet NetworkBoss::recieve()
{
    sf::Packet receivePacket;
    if (m_socket.receive(receivePacket, m_receiveIP, m_receivePort) != sf::Socket::Done)
    {
        cout << "\nThere was an error receiving the packet from [" << m_receiveIP << "]::[" << m_receivePort << "]";
    }
    return receivePacket;
}
sf::Packet NetworkBoss::recieveLatest()
{
    sf::Packet receivePacket;
    sf::Packet tempPack;
    int tempNum;
    if(m_socket.receive(receivePacket, m_receiveIP, m_receivePort) == sf::Socket::Done)
    {
        while(m_socket.receive(tempPack, m_receiveIP, m_receivePort) == sf::Socket::Done)
        {
            receivePacket = tempPack;
        }
        receivePacket >> tempNum;
        if(tempNum > latestNum)
        {
        cout << "\n(" << latestNum << ")";
            latestNum = tempNum;
            return receivePacket;
        }

    }
}
void NetworkBoss::send(const sf::Packet& pack)
{
    sf::Packet copy(pack);
    if (m_socket.send(copy, m_ip, m_ipPort) != sf::Socket::Done)
    {
        cout << "\nThere was an error sending the packet to [" << m_ip << "]::[" << m_ipPort << "]";
    }
}
