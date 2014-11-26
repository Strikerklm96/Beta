#include "RayCastCallback.hpp"

using namespace std;

RayCastCallback::RayCastCallback()
{
    m_pIgnoreBody = NULL;
    m_latest.fixture = NULL;
    m_latest.point = b2Vec2(-1,-1);
}
RayCastCallback::~RayCastCallback()
{

}
float32 RayCastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
{
    if(m_pIgnoreBody!=fixture->GetBody() && fraction<=m_latest.fraction && !fixture->IsSensor())
    {
        m_latest.fixture = fixture;
        m_latest.point = point;
        m_latest.fraction = fraction;
        return fraction;
    }
    else
        return m_latest.fraction;
}
void RayCastCallback::setIgnoreBody(b2Body* pBody)
{
    m_pIgnoreBody = pBody;
}
const RayData& RayCastCallback::getLatest() const
{
    return m_latest;
}
void RayCastCallback::reset()
{
    m_latest.point = b2Vec2(0,0);
    m_latest.fixture = NULL;
    m_latest.fraction = 1.f;
}
