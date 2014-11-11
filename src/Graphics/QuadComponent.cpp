#include "QuadComponent.hpp"

#include "Convert.hpp"
#include "BatchLayers.hpp"
#include "Globals.hpp"
#include "Universe.hpp"

using namespace std;
using namespace sf;

QuadComponent::QuadComponent(const QuadComponentData& rData) : GraphicsComponent(rData)
{
    m_numVerts = 4;

    m_originPos.resize(m_numVerts);
    m_originPos[0] = sf::Vector2f(-rData.dimensions.x/2, -rData.dimensions.y/2);
    m_originPos[1] = sf::Vector2f(-rData.dimensions.x/2,  rData.dimensions.y/2);
    m_originPos[2] = sf::Vector2f( rData.dimensions.x/2,  rData.dimensions.y/2);
    m_originPos[3] = sf::Vector2f( rData.dimensions.x/2, -rData.dimensions.y/2);

    QuadData dat = game.getUniverse().getBatchLayers().request(rData.texName, rData.layer);

    m_pVerts = dat.vertexList;
    m_startVert = dat.firstElement;

}
QuadComponent::~QuadComponent()
{

}
void QuadComponent::postUpdate()
{

}
