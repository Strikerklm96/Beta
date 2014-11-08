#ifndef IOCOMPONENT_HPP
#define IOCOMPONENT_HPP

#include "EventTypes.hpp"
#include "stdafx.hpp"
#include "Courier.hpp"
#include "Eventer.hpp"

class IOManager;

struct IOComponentData
{
    IOComponentData(IOManager& rManager) :
        name("NONAME"),
        pMyManager(&rManager)
    {

    }
    std::string name;//the name we are located by
    std::vector<Courier> courierList;//list of couriers to event on
    IOManager* pMyManager;//pointer to our manager (to send a ptr to us to and for sending messages
};


class IOComponent
{
public:

    IOComponent(const IOComponentData& rData);
    virtual ~IOComponent();

    void event(EventType type, int value, const sf::Packet& rData);//EventType occurred, has value, and other data
    void recieve(const std::string& rCommand, const sf::Packet& rData);
    const std::string& getName() const;

    template <typename T>
    void bindCallback(void (T::*func)(const std::string&, const sf::Packet&), T* const classPtr)//because C++
    {
        m_cbFunction = std::bind(func, classPtr, std::placeholders::_1, std::placeholders::_2);
    }

protected:
private:
    IOManager& m_rManager;
    std::string m_name;
    int m_ioManPosition;//position of us in the io manager

    Eventer m_eventer;
    std::function<void(const std::string&, const sf::Packet&)> m_cbFunction;//the function we call when we get a receive call
};

#endif // IOCOMPONENT_HPP
