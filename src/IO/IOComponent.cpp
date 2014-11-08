#include "IOComponent.hpp"

#include "IOManager.hpp"

using namespace std;

IOComponent::IOComponent(const IOComponentData& rData) : m_rManager(*rData.pMyManager), m_name(rData.name), m_eventer(m_rManager)
{
    m_ioManPosition = m_rManager.give(this);
    m_eventer.add(rData.courierList);
}
IOComponent::~IOComponent()
{
    m_rManager.free(m_ioManPosition);
}
void IOComponent::event(EventType type, int value, const sf::Packet& rData)
{
    m_eventer.event(type, value, rData);
}
void IOComponent::recieve(const std::string& rCommand, const sf::Packet& rData)
{
    cout << "\nMessage Received [" << m_name << "]";
    sf::Packet copy(rData);
    m_cbFunction(rCommand, copy);
}
const std::string& IOComponent::getName() const
{
    return m_name;
}
