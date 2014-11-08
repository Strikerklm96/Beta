#include "Message.hpp"

Message::Message()
{

}
Message::Message(const std::string& rTargetName, const std::string& rCommand, const sf::Packet& rData, float delay, bool replaceData)
{
    reset(rTargetName, rCommand, rData, delay, replaceData);
}
Message::~Message()
{

}
void Message::reset(const std::string& rTargetName, const std::string& rCommand, const sf::Packet& rData, float delay, bool replaceData)
{
    m_targetName = rTargetName;
    m_command = rCommand;
    m_data = rData;
    m_delay = delay;
    m_replaceData = replaceData;
}
void Message::tryReplaceData(const sf::Packet& rData)
{
    if(m_replaceData)
        m_data = rData;
}
float Message::changeDelay(float change)
{
    m_delay += change;
}
const std::string& Message::getTargetName() const
{
    return m_targetName;
}
const std::string& Message::getCommand() const
{
    return m_command;
}
const sf::Packet& Message::getData() const
{
    return m_data;
}
float Message::getDelay() const
{
    return m_delay;
}
