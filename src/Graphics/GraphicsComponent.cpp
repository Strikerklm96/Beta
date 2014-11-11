#include "GraphicsComponent.hpp"

#include "convert.hpp"
#include "Universe.hpp"
#include "GraphicsComponentUpdater.hpp"

using namespace std;
using namespace sf;

GraphicsComponent::GraphicsComponent(const GraphicsComponentData& rData)
{
    game.getUniverse().getGfxUpdater().store(this);
    m_rotation = 0;
    m_offset = sf::Vector2f(0,0);


    m_permanentRot = rData.permanentRot;
    m_center = rData.center;
}
GraphicsComponent::~GraphicsComponent()
{
    game.getUniverse().getGfxUpdater().free(this);

    for(int i=0; i<m_numVerts; ++i)
        (*m_pVerts)[i+m_startVert].color = sf::Color(0,0,0,0);//make them transparent so they can no longer be seen
}


void GraphicsComponent::setPosition(const b2Vec2& rWorldCoords)
{
    m_latestPosition = rWorldCoords;
    sf::Transform t = getTransform();
    sf::Vector2f offsetFixed(m_offset.x, -m_offset.y);
    sf::Vector2f centerFixed(m_center.x, -m_center.y);

    for(int i=0; i<m_numVerts; ++i)
        (*m_pVerts)[i+m_startVert].position = t.transformPoint(m_originPos[i]+offsetFixed-centerFixed);
}
void GraphicsComponent::setRotation(float radiansCCW)
{
    m_rotation = radiansCCW;
    sf::Transform t = getTransform();
    sf::Vector2f offsetFixed(m_offset.x, -m_offset.y);
    sf::Vector2f centerFixed(m_center.x, -m_center.y);

    for(int i=0; i<m_numVerts; ++i)
        (*m_pVerts)[i+m_startVert].position = t.transformPoint(m_originPos[i]+offsetFixed-centerFixed);
}
void GraphicsComponent::setOffset(const sf::Vector2f pixels)//sets the origin of us
{
    m_offset = pixels;
}



const b2Vec2& GraphicsComponent::getPosition() const
{
    return m_latestPosition;
}
float GraphicsComponent::getRotation() const
{
    return m_rotation;
}
const sf::Vector2f& GraphicsComponent::getOffset() const
{
    return m_offset;
}



sf::Transform GraphicsComponent::getTransform() const
{
    sf::Transform transform;
    transform.translate(leon::b2Tosf<float>(m_latestPosition)).rotate(leon::radToDeg(-m_rotation-m_permanentRot));
    return transform;
}






