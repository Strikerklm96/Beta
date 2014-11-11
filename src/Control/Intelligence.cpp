#include "Intelligence.hpp"

#include "SlaveLocator.hpp"
#include "Chunk.hpp"

using namespace std;

Intelligence::Intelligence(const IntelligenceData& rData) : m_aim(0,0)//, m_io(rData.ioComp)
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




void Intelligence::setAim(const b2Vec2& world)//send our aim coordinates
{
    m_aim = world;
    Chunk* temp = game.getUniverse().getSlaveLocator().find(m_slavePosition);
    if(temp != NULL)
        temp->setAim(m_aim);
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

