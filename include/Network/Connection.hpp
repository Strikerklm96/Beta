#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "stdafx.hpp"
#include "Protocol.hpp"

struct Connection
{
    Connection(const std::string& address, unsigned short targetPort, float time, sf::UdpSocket* socket, bool validated)
    {
        ip = address;
        port = targetPort;
        lastRecieve = 0;
        lastSend = 0;
        timeout = time;
        lastRecTime = game.getTime();
        valid = validated;
        pSocket = socket;
        if(pSocket != NULL)
        {
            sf::Packet data;
            data << static_cast<int>(Protocol::Connect);
            send(data);
        }
    }


    void send(const sf::Packet& rData)
    {
        sf::Packet data;
        data << ++lastSend;
        data.append(rData.getData(), rData.getDataSize());
        pSocket->send(data, ip, port);
    }

    sf::IpAddress ip;//the address to send to
    unsigned short port;//the port to send to
    unsigned lastRecieve;//the number of the last packet we received from this address so that we can ignore old packets
    unsigned lastSend;

    bool valid;
    float timeout;//how long till we drop the connection?
    float lastRecTime;//when was the last time we received a packet from this person?

    sf::UdpSocket* pSocket;

    ~Connection()
    {
        if(valid)
        {
            std::cout << "\nDropping:[" << ip.toString() << "]";
            sf::Packet data;
            data << static_cast<int>(Protocol::Drop);
            send(data);
            send(data);
            send(data);
            send(data);
        }
    }
};


#endif // CONNECTION_HPP
