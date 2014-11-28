#include "NetworkBoss.hpp"

using namespace std;
using namespace sf;

NetworkBoss::NetworkBoss(const NetworkBossData& rData) : m_io(rData.ioComp, NetworkBoss::input, this)
{
    m_isClient = false;
    m_udp.unbind();
    m_udp.setBlocking(false);
    m_joinIP = "";
    m_joinPort = 0;
    m_joinTimeOut = 5.f;
}
NetworkBoss::~NetworkBoss()
{
    m_udp.unbind();
}
/**BOTH**/
bool NetworkBoss::setRecievePort(unsigned short port)//set receiving port, returns whether the bind was successful
{
    cout << "\n" << port;
    m_udp.unbind();
    if(m_udp.bind(port) != sf::Socket::Done)
    {
        cout << "\nThere was an error binding to [" << port << "].";
        ///ERROR LOG
        return false;
    }
    return true;
}
void NetworkBoss::host(unsigned short port)
{
    m_isClient = false;
    setRecievePort(port);
    m_connections.clear();
    cout << "\nHosting.";
}
bool NetworkBoss::isClient() const
{
    return m_isClient;
}

void NetworkBoss::updateConnections()//update connections
{
    for(int i=0; i<m_connections.size(); ++i)
    {
        if(game.getTime()-m_connections[i]->lastRecTime > m_connections[i]->timeout)//check if we should timeout
        {
            //Message mess;
            //mess.reset("connection_fail", "toggleHidden", voidPacket, 0.f, false);
            //game.getCoreIO().recieve(mess);

            cout << "\nConnection Dropped: [" << m_connections[i]->ip << "]::[" << m_connections[i]->port << "]";
            m_connections.erase(m_connections.begin()+i);
            --i;
        }
        else
        {
            if(m_connections[i]->valid)
            {
                sf::Packet hand;
                hand << "h";
                m_connections[i]->send(hand);
            }
            else
            {
                sf::Packet hand;
                hand << "c";
                m_connections[i]->send(hand);
            }
        }
    }
}
Connection* NetworkBoss::findConnection(const sf::IpAddress& rAdd)
{
    cout << "\n\nLooking For:" << rAdd.toString();
    cout << "\nWith Active Connections:";

    for(auto it = m_connections.begin(); it!= m_connections.end(); ++it)
    {
        cout << "\n" << (*it)->ip.toString();
        if((*it)->ip == rAdd)
        {
            cout << "\n";
            return it->get();
        }
    }
    cout << "\n";
    return NULL;
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
            int sendID;
            string type;
            data >> sendID;
            data >> type;
            Connection* pCon = findConnection(fromIP);


            if(pCon == NULL && (!m_isClient))/**WANT TO CONNECT?**/
            {
                if(type == "c")//if it wants to connect, add you to connections
                {
                    cout << "\nNew Connection:[" << fromIP.toString() << "]";
                    addConnection(fromIP.toString(), fromPort, m_joinTimeOut);//PASSWORD HERE
                }
            }


            else if(pCon != NULL)/**RECOGNIZED CONNECTION**/
            {



                if(type == "h")
                {
                    pCon->lastRecieve = sendID;
                    pCon->lastRecTime = game.getTime();
                    cout << "\nHandshake:[" << fromIP.toString() << "]";
                    pCon->valid = true;
                }
                else if(type == "d")
                {
                    pCon->lastRecieve = sendID;
                    pCon->lastRecTime = game.getTime();
                    cout << "\nDisconnect:[" << fromIP.toString() << "]";
                    for(auto it = m_connections.begin(); it!=m_connections.end(); ++it)
                        if((*it)->ip == fromIP)
                        {
                            (*it)->valid = false;
                            m_connections.erase(it);
                            break;
                        }
                }
            }
            else
            {
                ///someone tried to send us data
                cout << "\nGarbage From:[" << fromIP << "]::[" << fromPort << "]";
            }
        }
        else
            done = true;
    }


    updateConnections();
}
/**BOTH**/



/**CLIENT**/
bool NetworkBoss::connect(const std::string& address, unsigned short port, float timeout)//clear all connections and make a connection with this server
{
    m_isClient = true;
    if(not setRecievePort(port))
    {
        ///ERROR LOG
        cout << FILELINE;
        int i;
        cin >> i;
    }
    m_connections.clear();
    m_connections.push_back(std::tr1::shared_ptr<Connection>(new Connection(address, port, timeout, &m_udp, false)));
}
/**CLIENT**/



/**SERVER**/
void NetworkBoss::addConnection(const std::string& address, unsigned short port, float timeout)
{
    m_isClient = false;
    m_connections.push_back(std::tr1::shared_ptr<Connection>(new Connection(address, port, timeout, &m_udp, true)));
}
/**SERVER**/




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
            connect(m_joinIP, m_joinPort, m_joinTimeOut);
        }
    }
    else if(rCommand == "host")
    {
        if(!(m_joinPort==0))
        {
            host(m_joinPort);
        }
    }
    else
    {
        cout << "\n" << FILELINE;
    }
}
