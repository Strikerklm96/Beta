#ifndef NETWORKBOSS_HPP
#define NETWORKBOSS_HPP

#include "stdafx.hpp"
#include "Globals.hpp"
#include "Connection.hpp"
#include "IOComponent.hpp"
#include "NetworkFactory.hpp"

struct NetworkBossData
{
    NetworkBossData() :
        ioComp(game.getCoreIO())
    {
        ioComp.name = "networkboss";
    }

    IOComponentData ioComp;
};

class NetworkBoss
{
public:
    NetworkBoss(const NetworkBossData& rData);
    virtual ~NetworkBoss();

    /**BOTH**/
    NetworkFactory& getNWFactory();
    bool setRecievePort(unsigned short port);//set receiving port, returns whether the bind was successful
    void host(unsigned short port);
    bool isClient() const;

    void updateConnections();
    Connection* findConnection(const sf::IpAddress& rAdd);
    bool hasConnections();//are we connected to anyone?

    void update();
    /**BOTH**/



    /**CLIENT**/
    bool connect(const std::string& address, unsigned short port, float timeout);//clear all connections and make a connection with this server
    /**CLIENT**/



    /**SERVER**/
    void addConnection(const std::string& address, unsigned short port, float timeout);//adds a connection to our client list
    /**SERVER**/

protected:
    void input(const std::string rCommand, sf::Packet rData);
private:
    IOComponent m_io;

    std::string m_joinIP;
    unsigned short m_joinPort;
    float m_joinTimeOut;

    bool m_isOpen;
    bool m_isClient;//are we a client or a server
    sf::UdpSocket m_udp;
    std::vector<std::tr1::shared_ptr<Connection> > m_connections;

    NetworkFactory m_nwFactory;
};

#endif // NETWORKBOSS_HPP
