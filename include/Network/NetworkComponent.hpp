#ifndef NETWORKCOMPONENT_HPP
#define NETWORKCOMPONENT_HPP

#include "stdafx.hpp"
#include "NetworkBoss.hpp"

struct NetworkComponentData
{
    NetworkComponentData()
        {

        }
};


class NetworkComponent // for syncing data between objects on different computers
{
public:
    template <typename T>
    NetworkComponent(const NetworkComponentData& rData, void (T::*pack)(sf::Packet&), void (T::*unpack)(const sf::Packet&), T* const classPtr) : m_rFactory(game.getNwBoss().getNWFactory())
    {
        m_packFunction = std::bind(pack, classPtr, std::placeholders::_1);
        m_unpackFunction = std::bind(unpack, classPtr, std::placeholders::_1);
        m_newData = false;

        m_factoryID = m_rFactory.give(this);///WE NEED TO GIVE NW FACTORY THIS
    }
    virtual ~NetworkComponent();

    bool toggleNewData(bool newData);//set new data
    bool isNewData();//do we have new data

protected:
private:
    std::function<void(sf::Packet&)> m_packFunction;//the function we call when we get a receive call
    std::function<void(const sf::Packet&)> m_unpackFunction;//the function we call when we get a receive call

    NetworkFactory& m_rFactory;
    int m_factoryID;
    bool m_newData;//is there new data to be sent?
};

#endif // NETWORKCOMPONENT_HPP
