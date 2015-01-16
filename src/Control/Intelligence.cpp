#include "Intelligence.hpp"

#include "SlaveLocator.hpp"
#include "Chunk.hpp"

using namespace std;

Intelligence::Intelligence(const IntelligenceData& rData) : m_aim(0,0), m_io(rData.ioComp, &Intelligence::input, this), m_nw(rData.nwComp, &Intelligence::pack, &Intelligence::unpack, this)
{
    m_slavePosition = -1;
    m_slaveName = rData.slaveName;
    //setSlave(m_slaveName);
}
Intelligence::~Intelligence()
{

}






void Intelligence::setSlave(const std::string& rSlaveName)
{
    m_slaveName = rSlaveName;
    m_slavePosition = game.getUniverse().getSlaveLocator().findPos(m_slaveName);
}
const std::string& Intelligence::getSlaveName() const
{
    return m_slaveName;
}



IOComponent& Intelligence::getIOComp()
{
    return m_io;
}
void Intelligence::setAim(const b2Vec2& world)//send our aim coordinates
{
    m_aim = world;
    Chunk* temp = game.getUniverse().getSlaveLocator().find(m_slavePosition);
    if(temp != NULL)
        temp->setAim(m_aim);
}
const b2Vec2& Intelligence::getAim() const
{
    return m_aim;
}
b2Body* Intelligence::getBodyPtr()//return position
{
    Chunk* temp = game.getUniverse().getSlaveLocator().find(m_slavePosition);
    if(temp != NULL)
        return temp->getBodyPtr();
    else
        return NULL;
}
void Intelligence::directive(Directive issue)//send command to target
{
    Chunk* temp = game.getUniverse().getSlaveLocator().find(m_slavePosition);
    if(temp != NULL)
        temp->directive(issue);
}
float Intelligence::get(Request value)//return the requested value
{
    Chunk* temp = game.getUniverse().getSlaveLocator().find(m_slavePosition);
    if(temp != NULL)
        return temp->get(value);
    else
        return 0.f;
}
void Intelligence::processDirectives()//use our stored directives to send commands
{
    if(m_directives[Directive::Up])
        directive(Directive::Up);
    if(m_directives[Directive::Down])
        directive(Directive::Down);
    if(m_directives[Directive::RollCCW])
        directive(Directive::RollCCW);
    if(m_directives[Directive::RollCW])
        directive(Directive::RollCW);
    if(m_directives[Directive::FirePrimary])
        directive(Directive::FirePrimary);
    if(m_directives[Directive::FireSecondary])
        directive(Directive::FireSecondary);
}
void Intelligence::setPlayerName(const std::string& rPlayerName)
{
    m_playerName = rPlayerName;
}
const std::string& Intelligence::getPlayerName() const
{
    return m_playerName;
}
const std::string& Intelligence::getName()
{
    return m_SIname;
}
void Intelligence::setName(const std::string& newName)
{
    m_SIname = newName;
}
void Intelligence::pack(sf::Packet& rPacket)
{

}
void Intelligence::unpack(sf::Packet& rPacket)
{

}
void Intelligence::input(std::string rCommand, sf::Packet rData)
{
    sf::Packet data(rData);
    if(rCommand == "setPlayerName")
    {
        std::string name;
        data >> name;
        setPlayerName(name);
    }
}
