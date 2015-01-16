#include "NetworkBoss.hpp"
#include "Protocol.hpp"
#include "IOManager.hpp"
#include "Universe.hpp"

using namespace std;
using namespace sf;

NetworkBoss::NetworkBoss(const NetworkBossData& rData) : m_io(rData.ioComp, NetworkBoss::input, this)
{
    m_isClient = false;
    m_udp.unbind();
    m_udp.setBlocking(false);
    m_joinIP = "";
    m_joinPort = 5050;
    m_joinTimeOut = 5.f;
    m_isOpen = false;
}
NetworkBoss::~NetworkBoss()
{
    m_udp.unbind();
}
/**UTILITY**/
NetworkFactory& NetworkBoss::getNWFactory()
{
    return m_nwFactory;
}
bool NetworkBoss::setRecievePort(unsigned short port)//set receiving port, returns whether the bind was successful
{
    m_udp.unbind();
    if(m_udp.bind(port) != sf::Socket::Done)
    {
        cout << "\nThere was an error binding to port [" << port << "]";
        ///ERROR LOG
        return false;
    }
    return true;
}
void NetworkBoss::messageLobbyLocal(const std::string& rMessage)
{
    sf::Packet data;
    data << rMessage;
    Message mes("lobby_chatbox", "addLineLocal", data, 0, true);
    game.getCoreIO().recieve(mes);
}
bool NetworkBoss::isClient() const
{
    return m_isClient;
}
Connection* NetworkBoss::findConnection(const sf::IpAddress& rAdd)
{
    for(auto it = m_connections.begin(); it!= m_connections.end(); ++it)
        if((*it)->ip == rAdd)
            return it->get();

    return NULL;
}
bool NetworkBoss::hasConnections()
{
    return (m_connections.size()>0);
}
void NetworkBoss::addConnection(const std::string& address, unsigned short port, float timeout)//a server adding a connection
{
    m_connections.push_back(std::tr1::shared_ptr<Connection>(new Connection(address, port, timeout, &m_udp, true)));
    sf::Packet hand;
    hand << static_cast<int32_t>(Protocol::Handshake);
    m_connections.back()->send(hand);
    m_connections.back()->send(hand);
    m_connections.back()->send(hand);
    m_connections.back()->send(hand);
}
/**UTILITY**/



void NetworkBoss::setClient(const std::string& address, unsigned short port, float timeout)//we decide to try to join a host
{
    m_isClient = true;
    m_isOpen = false;
    m_connections.clear();
    game.getUniverse().getUniverseIO().toggleAcceptsLocal(false);

    m_connections.push_back(std::tr1::shared_ptr<Connection>(new Connection(address, port, timeout, &m_udp, false)));

    m_joinTimeOut = timeout;
    setRecievePort(port);

    Message clearChat("lobby_chatbox", "clear", voidPacket, 0, true);    //clear chat
    game.getCoreIO().recieve(clearChat);

    messageLobbyLocal("Connecting to [" + m_connections.front()->ip.toString() +"]");  //message connecting

    sf::Packet falsePacket;
    falsePacket << false;
    Message hideLobby("lobby", "setHidden", falsePacket, 0, true);          //show lobby
    game.getCoreIO().recieve(hideLobby);

    sf::Packet truePacket;
    truePacket << true;
    Message hideMult("multiplayer_connect", "setHidden", truePacket, 0, true); //hide multiplayer panel
    game.getCoreIO().recieve(hideMult);
}
void NetworkBoss::setLocalOnly()//we decide to be antisocial
{
    m_isClient = false;
    m_isOpen = false;
    m_connections.clear();
    game.getUniverse().getUniverseIO().toggleAcceptsLocal(true);

    sf::Packet pack1;
    pack1 << true;
    Message hideLobby("lobby", "setHidden", pack1, 0, true);          //show lobby
    game.getCoreIO().recieve(hideLobby);

    sf::Packet pack2;
    pack2 << false;
    Message hideMult("multiplayer_connect", "setHidden", pack2, 0, true); //hide multiplayer panel
    game.getCoreIO().recieve(hideMult);
}
void NetworkBoss::setHost(unsigned short port, float timeout)//we decide to try and host
{
    m_isClient = false;
    m_isOpen = true;
    m_connections.clear();
    game.getUniverse().getUniverseIO().toggleAcceptsLocal(true);

    m_joinTimeOut = timeout;
    setRecievePort(port);


    Message clearChat("lobby_chatbox", "clear", voidPacket, 0, true);    //clear chat
    game.getCoreIO().recieve(clearChat);

    std::ostringstream oss;
    oss << m_udp.getLocalPort();
    messageLobbyLocal("Hosting on port[" + oss.str() + "]");  //message "Hosting on port"

    sf::Packet falsePacket;
    falsePacket << false;
    Message hideLobby("lobby", "setHidden", falsePacket, 0, true);          //show lobby
    game.getCoreIO().recieve(hideLobby);

    sf::Packet truePacket;
    truePacket << true;
    Message hideMult("multiplayer_connect", "setHidden", truePacket, 0, true); //hide multiplayer panel
    game.getCoreIO().recieve(hideMult);
}






void NetworkBoss::updateConnectionStatus()
{
    for(int i=0; i<m_connections.size(); ++i)
    {
        if(game.getTime()-m_connections[i]->lastRecTime > m_connections[i]->timeout)//check if this connection will timeout
        {
            if(m_isClient)
                setLocalOnly();//if we were a client, set us to local control
            else
            {
                m_connections.erase(m_connections.begin()+i);
                --i;
            }
        }
        else
        {
            if(not m_connections[i]->valid)
            {
                sf::Packet con;
                con << static_cast<int32_t>(Protocol::Connect);
                m_connections[i]->send(con);
            }
        }
    }
}


void NetworkBoss::update()
{
    sf::Packet data;
    sf::IpAddress fromIP;
    unsigned short fromPort;

    bool done = false;
    while(not done)
    {
        data.clear();
        if(m_udp.receive(data, fromIP, fromPort) == sf::Socket::Done)/**FOR EACH PACKET**/
        {
            int32_t sendID;
            int32_t typeInt;
            data >> sendID;
            data >> typeInt;
            Protocol type = static_cast<Protocol>(typeInt);
            Connection* pCon = findConnection(fromIP);


            if(pCon == NULL && (!m_isClient) && m_isOpen)/**WANT TO CONNECT?**/
            {
                if(type == Protocol::Connect)//if it wants to connect, add you to connections
                {
                    addConnection(fromIP.toString(), fromPort, m_joinTimeOut);///PASSWORD CHECK HERE

                    sf::Packet ip;
                    ip << (fromIP.toString() + ": joined.");
                    Message someoneJoined("lobby_chatbox", "addLine", ip, 0, true);     //we are host witnessing a client connect
                    game.getCoreIO().recieve(someoneJoined);
                }
                else
                {
                    cout << "\nConnection already made with this client.";
                }
            }


            else if(pCon != NULL)/**RECOGNIZED CONNECTION**/
            {

                if(pCon->lastRecieve <= sendID)
                {
                    pCon->lastRecieve = sendID;
                    pCon->lastRecTime = game.getTime();


                    if(type == Protocol::Handshake)//as a client successfully connect
                    {
                        if(pCon->valid == false)
                        {
                            pCon->valid = true;
                            sf::Packet ip;
                            ip << ("Connection successful!");
                            Message conMade("lobby_chatbox", "addLineLocal", ip, 0, true);    //we are client witnessing ourselves finish connecting
                            game.getCoreIO().recieve(conMade);
                        }
                    }
                    else if(type == Protocol::Drop)
                    {
                        cout << "\nDisconnecting:[" << fromIP.toString() << "]";
                        for(auto it = m_connections.begin(); it!=m_connections.end(); ++it)     //we are host witnessing a client disconnect
                            if((*it)->ip == fromIP)
                            {
                                (*it)->valid = false;
                                m_connections.erase(it);

                                sf::Packet ip;
                                ip << (fromIP.toString() + ": disconnected.");
                                Message someoneDiscon("lobby_chatbox", "addLine", ip, 0, true);
                                game.getCoreIO().recieve(someoneDiscon);

                                break;
                            }
                    }
                    else if(type == Protocol::Lobby)
                    {
                        /**
                        //Launch Game send:("universe", "level", "localPlayerSlave", "blueprints")
                        //Update Slots send:("slotName", "playerName")
                        **/

                    }
                    else if(type == Protocol::LoadLevel)
                    {
                        game.loadUniverse("stuff");
                    }
                    else if(type == Protocol::Data)
                    {
                        m_nwFactory.process(data);//
                    }
                    else
                    {
                        cout << "\nMessage with unrecognized protocol!";
                    }
                }

            }
            else
            {
                ///someone tried to send us data
            }
        }
        else
            done = true;
    }



    /**DROP BAD CONNECTIONS**/
    updateConnectionStatus();



    /**NW COMPONENTS**/
    sf::Packet outData;
    outData << static_cast<int32_t>(Protocol::Data);
    m_nwFactory.getData(outData);///WE NEED TO SEND OUR NW FACTORY DATA

    for(int i = 0; i<m_connections.size(); ++i)
    {
        if(m_connections[i]->valid)
            m_connections[i]->send(outData);
    }
    /**NW COMPONENTS**/



}

/*
bool NetworkBoss::connect(const std::string& address, unsigned short port, float timeout) //we are client trying to connect to host
{
    m_isClient = true;
    game.getUniverse().getUniverseIO().toggleAcceptsLocal(false);
    if(not setRecievePort(port))
    {
        ///ERROR LOG
        cout << FILELINE;
        int i;
        cin >> i;
    }
    m_isOpen = false;
    m_connections.clear();
    m_connections.push_back(std::tr1::shared_ptr<Connection>(new Connection(address, port, timeout, &m_udp, false)));




    Message clearChat("lobby_chatbox", "clear", voidPacket, 0, true);
    game.getCoreIO().recieve(clearChat);

    sf::Packet ip;
    ip << ("Connecting...");
    Message hostingMes("lobby_chatbox", "addLineLocal", ip, 0, true);
    game.getCoreIO().recieve(hostingMes);

    sf::Packet falsePacket;
    falsePacket << false;
    Message hideLobby("lobby", "setHidden", falsePacket, 0, true);
    game.getCoreIO().recieve(hideLobby);

    sf::Packet truePacket;
    truePacket << true;
    Message hideMult("multiplayer_connect", "setHidden", truePacket, 0, true);
    game.getCoreIO().recieve(hideMult);

}
*/




void NetworkBoss::input(const std::string rCommand, sf::Packet rData)
{
    if(rCommand == "joinIP")
    {
        rData >> m_joinIP;
    }
    else if(rCommand == "joinPort")
    {
        std::string port;
        rData >> port;
        std::stringstream sstream(port);
        sstream >> m_joinPort;
    }
    else if(rCommand == "joinTime")
    {
        std::string time;
        rData >> time;
        std::stringstream sstream(time);
        sstream >> m_joinTimeOut;
    }
    else if(rCommand == "join")
    {
        if(!(m_joinIP=="") && !(m_joinPort==0))
        {
            setClient(m_joinIP, m_joinPort, m_joinTimeOut);
        }
    }
    else if(rCommand == "host")
    {
        if(not (m_joinPort<1024))
        {
            setHost(m_joinPort, m_joinTimeOut);
        }
    }
    else if(rCommand == "localOnly")
    {
        setLocalOnly();
    }
    else
    {
        cout << "\n" << FILELINE;
    }
}
