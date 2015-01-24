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


    /**UTILITY**/
    NetworkFactory& getNWFactory();

    void messageLobbyLocal(const std::string& rMessage);
    bool setRecievePort(unsigned short port);//set receiving port, returns whether the bind was successful
    bool hasConnections();//are we connected to anyone?
    Connection* findConnection(const sf::IpAddress& rAdd);
    bool isClient() const;
    void addConnection(const std::string& address, unsigned short port, float timeout);//add this connection to our clients list and handshake it a few times (SERVER ONLY)


    /**CHANGE STATE**/
    void setClient(const std::string& address, unsigned short port, float timeout);//set us to client mode and connect to a host
    void setLocalOnly();//set us to local mode and ignore nw activity
    void setHost(unsigned short port, float timeout);//set us to host mode and listen for connections


    /**UPDATE**/
    void update();///get a better name!
    void updateConnectionStatus();//drop connections if they timeout, or try and connect to invalid connections

    /**Reduction**/
    void loadLevel(sf::Packet& data);
    void launch();
    void handshake(sf::Packet& data, Connection* pCon);
    void drop(sf::Packet& data, sf::IpAddress fromIP);

protected:
    void input(const std::string rCommand, sf::Packet rData);

private:
    IOComponent m_io;

    std::string m_joinIP;
    unsigned short m_joinPort;
    float m_joinTimeOut;//timeout we use for connections

    bool m_isOpen;
    bool m_isClient;//are we a client or a server
    sf::UdpSocket m_udp;
    std::vector<std::tr1::shared_ptr<Connection> > m_connections;

    NetworkFactory m_nwFactory;
};

#endif // NETWORKBOSS_HPP
