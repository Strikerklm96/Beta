#ifndef RAYCASTCALLBACK_HPP
#define RAYCASTCALLBACK_HPP

#include "stdafx.hpp"

struct RayData
{
    b2Fixture* fixture;
    b2Vec2 point;
    float fraction;
};

class RayCastCallback : public b2RayCastCallback
{
public:
    RayCastCallback();
    virtual ~RayCastCallback();

	virtual float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
	void setIgnoreBody(b2Body* pBody);

	const RayData& getLatest() const;
	void reset();

protected:
private:
    b2Body* m_pIgnoreBody;
    std::function<void(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)> m_cbFunction;//the function we call when we get a receive call
    RayData m_latest;
};

#endif // RAYCASTCALLBACK_HPP
