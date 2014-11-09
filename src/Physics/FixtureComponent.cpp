#include "FixtureComponent.hpp"
#include "Globals.hpp"

using namespace std;

FixtureComponent::FixtureComponent(const FixtureComponentData& rData)
{
    /**RECTANGLE**/
    if(rData.shape == Shape::Rectangle)
    {
        m_spShape = tr1::shared_ptr<b2Shape>(new b2PolygonShape);
        tr1::static_pointer_cast<b2PolygonShape>(m_spShape)->SetAsBox(rData.size.x, rData.size.y, rData.offset, 0);
    }
    /**CIRCLE**/
    else if(rData.shape == Shape::Circle)
    {
        m_spShape = tr1::shared_ptr<b2Shape>(new b2CircleShape);
        b2CircleShape* temp = &*tr1::static_pointer_cast<b2CircleShape>(m_spShape);
        temp->m_p.Set(rData.offset.x, rData.offset.y);
        temp->m_radius = rData.size.x;
    }


    m_fixtureDef.isSensor = rData.isSensor;
    m_fixtureDef.shape = &*m_spShape;//give our shape to our fixture definition
    m_fixtureDef.density = rData.density;
    m_fixtureDef.friction = rData.friction;
    m_fixtureDef.restitution = rData.restitution;//setting our fixture data
    ///m_fixtureDef.filter.maskBits = 0xffffffff;///COLLISION
    ///m_fixtureDef.filter.categoryBits = 0xffffffff;///collision

    if(rData.pBody != NULL)
    {
        m_pFixture = rData.pBody->CreateFixture(&m_fixtureDef);
        m_pFixture->SetUserData(this);
    }
    else
        cout << FILELINE;
}
FixtureComponent::~FixtureComponent()
{

}
