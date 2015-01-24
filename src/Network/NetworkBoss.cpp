#include "NetworkBoss.hpp"
#include "Protocol.hpp"
#include "IOManager.hpp"
#include "Universe.hpp"
#include "Player.hpp"

using namespace std;
using namespace sf;

NetworkBoss::NetworkBoss(const NetworkBossData& rData) : m_io(rData.ioComp, NetworkBoss::input, this), m_nwFactory("standard"), m_nwFactoryTcp("tcp")
{
    m_state = NWState::Local;
    m_udp.unbind();
    m_udp.setBlocking(false);

    m_joinIP = "";
    m_joinPort = 5050;//port used for TCP, m_joinPort+1 used for udp

    m_timeOut = 10.f;//how long the timeout is
    m_isOpen = false;//are we accepting connections

    m_listener.listen(m_joinPort);
    m_listener.setBlocking(false);
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
NetworkFactory& NetworkBoss::getNWFactoryTcp()
{
    return m_nwFactoryTcp;
}

bool NetworkBoss::setRecievePort(unsigned short port)//set receiving port, returns whether the bind was successful
{
    m_udp.unbind();
    m_listener.close();

    m_listener.listen(port);


    if(m_udp.bind(port+1) != sf::Socket::Done)
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
    return (m_state == NWState::Client);
}
NWState NetworkBoss::getNWState() const
{
    return m_state;
}
Connection* NetworkBoss::findConnection(const sf::IpAddress& rAdd)/**FIND A CONNECTION IF WE HAVE ONE**/
{
    for(auto it = m_connections.begin(); it!= m_connections.end(); ++it)
        if((*it)->getTcpSocket().getRemoteAddress() == rAdd)
            return it->get();

    return NULL;
}
bool NetworkBoss::hasConnections()/**DO WE HAVE CONNECTIONS?**/
{
    return (m_connections.size()>0);
}
void NetworkBoss::addConnection(std::tr1::shared_ptr<sf::TcpSocket> spTcpSocket)/**SERVER ADDING CONNECTION**/
{
    m_connections.push_back(std::tr1::shared_ptr<Connection>(new Connection(&m_udp, spTcpSocket)));
}
/**UTILITY**/






/**====STATE====**/
void NetworkBoss::setClient(const std::string& address, unsigned short port, float timeout)/**JOIN HOST**/
{

    setState(NWState::Client, false, false, false, true);

    m_timeOut = timeout;
    setRecievePort(port);

    messageLobbyLocal("Connecting to [" + address + "]");  //message connecting

    sptr<sf::TcpSocket> spSocket(new sf::TcpSocket());
    spSocket->setBlocking(false);
    spSocket->connect(sf::IpAddress(address), port, sf::seconds(timeout));


    addConnection(spSocket);

}
void NetworkBoss::setLocal()//we decide to be antisocial
{
    setState(NWState::Local, false, true, true, false);
}
void NetworkBoss::setServer(unsigned short port, float timeout)//we decide to try and host
{
    setState(NWState::Server, true, true, false, true);

    m_timeOut = timeout;
    setRecievePort(port);

    std::ostringstream oss;
    oss << port;
    messageLobbyLocal("Hosting on port[" + oss.str() + "]");  //message "Hosting on port"
}
void NetworkBoss::setState(NWState state, bool open, bool acceptsLocal, bool hideLobby, bool hideConnectScreen)
{
    m_state = state;
    m_isOpen = open;
    m_connections.clear();
    game.getUniverse().getUniverseIO().toggleAcceptsLocal(acceptsLocal);

    Message clearChat("lobby_chatbox", "clear", voidPacket, 0, true);    //clear chat
    game.getCoreIO().recieve(clearChat);

    sf::Packet pack1;
    pack1 << hideLobby;
    Message hideLobbyMes("lobby", "setHidden", pack1, 0, true);          //show lobby
    game.getCoreIO().recieve(hideLobbyMes);

    sf::Packet pack2;
    pack2 << hideConnectScreen;
    Message hideMult("multiplayer_connect", "setHidden", pack2, 0, true); //hide multiplayer panel
    game.getCoreIO().recieve(hideMult);
}
/**====STATE====**/






void NetworkBoss::update()
{

    tcpListen();
    updateConnections();

    udpRecieve();
    tcpRecieve();

    sendUdp();
    sendTcp();

}

void NetworkBoss::udpRecieve()
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
            Connection* pCon = findConnection(fromIP);
            if(pCon != NULL)/**RECOGNIZED CONNECTION**/
            {
                Protocol proto = pCon->recievePacket(data);

                if(proto != Protocol::End)
                {
                    if(proto == Protocol::Control)
                        game.getUniverse().getControllerFactory().getNWFactory().process(data);
                    else if(proto == Protocol::Data)
                        m_nwFactory.process(data);
                    else
                        cout << "\n" << FILELINE << " [" << static_cast<int32_t>(proto) << "]";
                }
            }
        }
        else
            done = true;
    }
}
void NetworkBoss::tcpRecieve()//receive data from each TcpPort (tcp)
{
    sf::Packet data;
    for(int32_t i=0; i<m_connections.size(); ++i)
    {
        bool done = false;
        while(not done)
        {
            data.clear();
            if(m_connections[i]->getTcpSocket().receive(data) == sf::Socket::Done)/**FOR EACH PACKET**/
            {
                Protocol proto = m_connections[i]->recievePacket(data);

                if(proto != Protocol::End)
                {
                    if(proto == Protocol::Tcp)
                        m_nwFactoryTcp.process(data);
                    else if(proto == Protocol::LoadLevel)
                        loadLevel(data);
                    else
                        cout << "\n" << FILELINE << " [" << static_cast<int32_t>(proto) << "]";
                }
            }
            else
                done = true;
        }
    }
}
void NetworkBoss::sendUdp()
{
    sf::Packet udpPacket;
    m_nwFactoryTcp.getData(udpPacket);//WE NEED TO SEND OUR NW game DATA
    for(int32_t i=0; i<m_connections.size(); ++i)
        m_connections[i]->sendUdp(Protocol::Data, udpPacket);

    udpPacket.clear();
    game.getUniverse().getControllerFactory().getNWFactory().getData(udpPacket);//WE NEED TO SEND OUR NW game DATA
    for(int32_t i=0; i<m_connections.size(); ++i)
        m_connections[i]->sendUdp(Protocol::Control, udpPacket);
}
void NetworkBoss::sendTcp()
{
    sf::Packet tcpPacket;
    m_nwFactoryTcp.getData(tcpPacket);//WE NEED TO SEND OUR NW game DATA
    for(int32_t i=0; i<m_connections.size(); ++i)
        m_connections[i]->sendTcp(Protocol::Tcp, tcpPacket);
}
void NetworkBoss::tcpListen()//check for new connections
{
    sptr<sf::TcpSocket> spSocket(new sf::TcpSocket());
    spSocket->setBlocking(false);
    if(m_listener.accept(*spSocket) == sf::Socket::Done)
    {
        std::cout << "\nNew connection received from [" << spSocket->getRemoteAddress() << "].";
        addConnection(spSocket);
    }
}
void NetworkBoss::updateConnections()
{
    for(int32_t i=0; i<m_connections.size(); ++i)
    {
        if(m_connections[i]->getStatus() == sf::Socket::Status::Disconnected)//check if this connection is still working
        {
            if(m_state == NWState::Client)
                setLocal();//if we were a client, set us to local control
            else
            {
                m_connections.erase(m_connections.begin()+i);
                --i;
                cout << "\nConnection Dropped From Timeout";
            }
        }
    }
}






/**REDUCTION**/
void NetworkBoss::loadLevel(sf::Packet& data)//we are anyone being told to load the game
{
    std::string level;
    std::string blueprints;
    int32_t numControllers;
    std::string slave;
    std::vector<std::string> controllerList;
    int32_t localController;


    data >> level;
    data >> blueprints;
    data >> numControllers;
    for(int32_t i=0; i<numControllers; ++i)
    {
        data >> slave;
        cout << "\n[" << slave << "]";
        controllerList.push_back(slave);
    }
    data >> localController;
    cout << "\nCont" << localController;


    game.loadUniverse("meanginglessString");
    game.getUniverse().loadLevel(level, localController, blueprints, controllerList);

    Message closeMenu("overlay", "toggleMenu", voidPacket, 0, false);
    game.getCoreIO().recieve(closeMenu);
}
void NetworkBoss::launchMultiplayerGame()
{
    sf::Packet data;

    std::string level = "levels/level_1/";
    std::string blueprints = "blueprints/";

    data << level;
    data << blueprints;
    data << static_cast<int32_t>(m_connections.size()+1);//number of controllers
    for(int32_t i = 0; i<=m_connections.size(); ++i)
    {
        std::string name = ("ship_1"+std::to_string(i+1));
        cout << endl << name;
        data << name;
    }

    int32_t controller = 0;
    sf::Packet hostData(data);
    hostData << controller++;

    for(auto it = m_connections.begin(); it!=m_connections.end(); ++it)
    {
        sf::Packet launchData(data);
        launchData << controller++;
        (*it)->sendTcp(Protocol::LoadLevel, launchData);
    }

    loadLevel(hostData);
}
/**REDUCTION**/


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
        sstream >> m_timeOut;
    }
    else if(rCommand == "join")
    {
        if(m_joinIP != "" && m_joinPort > 1024)
            setClient(m_joinIP, m_joinPort, m_timeOut);
    }
    else if(rCommand == "host")
    {
        if(m_joinPort>1024)
            setServer(m_joinPort, m_timeOut);
    }
    else if(rCommand == "localOnly")
    {
        setLocal();
    }
    else if(rCommand == "launch")
    {
        if(m_state != NWState::Client)
            launchMultiplayerGame();
    }
    else
    {
        cout << "\n" << FILELINE;
    }
}


/**DROP BAD CONNECTIONS
updateConnectionStatus();




sf::Packet outData;
outData << static_cast<int>(Protocol::Data);
m_nwFactory.getData(outData);//WE NEED TO SEND OUR NW game DATA


sf::Packet outData2;
outData2 << static_cast<int>(Protocol::Control);
game.getUniverse().getControllerFactory().getNWFactory().getData(outData2);///we need to send our nw controller data


for(int32_t i = 0; i<m_connections.size(); ++i)
{
    if(m_connections[i]->valid)
    {
        m_connections[i]->send(outData);
        m_connections[i]->send(outData2);
    }
}**/


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
