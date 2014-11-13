#include "Chunk.hpp"
#include "Module.hpp"
#include "SlaveLocator.hpp"
#include "Intelligence.hpp"

using namespace std;

Chunk::Chunk(const ChunkData& rData) : GameObject(rData), m_body(rData.bodyComp)
{
    for(auto it = rData.moduleData.begin(); it!=rData.moduleData.end(); ++it)
        m_modules.push_back(std::tr1::shared_ptr<Module>( (*it)->generate(m_body.getBodyPtr()) ));

    m_slavePosition = game.getUniverse().getSlaveLocator().give(this);
}
Chunk::~Chunk()
{
    game.getUniverse().getSlaveLocator().free(m_slavePosition);
}
void Chunk::prePhysUpdate()
{
    for(auto it = m_modules.begin(); it!=m_modules.end(); ++it)
        (*it)->prePhysUpdate();
}
void Chunk::postPhysUpdate()
{
    for(auto it = m_modules.begin(); it!=m_modules.end(); ++it)
        (*it)->postPhysUpdate();
}
const std::string& Chunk::getName() const
{
    return m_io.getName();
}


void Chunk::setAim(const b2Vec2& world)//send our aim coordinates
{

}
void Chunk::directive(Directive issue)//send command to target
{
    const float force = 16;
    switch(issue)
    {
    case(Directive::Up):
            m_body.getBodyPtr()->ApplyForceToCenter(b2Vec2(0, force), true);
            break;
    case(Directive::Down):
            m_body.getBodyPtr()->ApplyForceToCenter(b2Vec2(0,-force), true);
            break;
    case(Directive::RollCCW):
            m_body.getBodyPtr()->ApplyForceToCenter(b2Vec2(-force,0), true);
            break;
    case(Directive::RollCW):
            m_body.getBodyPtr()->ApplyForceToCenter(b2Vec2(force,0), true);
            break;
    }
}
float Chunk::get(Request value) const//return the requested value
{
    return 0;///FIX THIS
}
b2Body* Chunk::getBodyPtr()
{
    return m_body.getBodyPtr();
}
