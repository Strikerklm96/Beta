#include "NetworkFactory.hpp"
#include "Globals.hpp"
#include "NetworkComponent.hpp"
#include "Protocol.hpp"

using namespace std;


NetworkFactory::NetworkFactory(std::string name)
{
    nname = name;
    m_lastSendID = 0;
}
NetworkFactory::~NetworkFactory()
{

}
int NetworkFactory::give(NetworkComponent* pComponent)//we recieve a pointer to a component and we store it
{
    int position;

    if(not m_freeIndexes.empty())//check free positions
    {
        position = m_freeIndexes.back();
        m_freeIndexes.pop_back();
    }
    else//assign new position at end
    {
        position = m_componentPtrs.size();
        m_componentPtrs.resize(m_componentPtrs.size()+1);//add one
    }

    m_componentPtrs[position] = pComponent;

    return position;
}
void NetworkFactory::free(int position)//don't adjust the list, just mark the node as null and offer it as a position to future customers
{

    if(m_componentPtrs.size() > position)
    {
        m_componentPtrs[position] = NULL;
        m_freeIndexes.push_back(position);
    }
    else
    {

    cout << position << nname;
        cout << m_componentPtrs.size();
        cout << FILELINE;
        ///ERROR LOG
    }
}
void NetworkFactory::getData(sf::Packet& rPacket)
{
    std::vector<NetworkComponent*>& rPtr = m_componentPtrs;
    for(int i = 0; i < rPtr.size(); ++i)
    {
        if(rPtr[i] != NULL)
        {
            if(rPtr[i]->hasNewData())
            {
                rPacket << i;
                rPtr[i]->pack(rPacket);
                rPtr[i]->toggleNewData(false);
            }
        }
    }
}
void NetworkFactory::process(sf::Packet& rPacket)
{
    int id;
    while(rPacket >> id)
    {
        if(id < m_componentPtrs.size())
        {
            if(m_componentPtrs[id] != NULL)
            {
                m_componentPtrs[id]->unpack(rPacket);
            }
        }
        else
        {
            cout << "\n" << FILELINE;
        }
    }
}
