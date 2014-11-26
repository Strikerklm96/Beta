#ifndef BEAM_HPP
#define BEAM_HPP

#include "QuadComponent.hpp"

struct BeamData : public QuadComponentData
{
    BeamData() :
        QuadComponentData(),
        start(),
        end()
    {

    }

    QuadComponentData start;
    QuadComponentData end;
};

class Beam : protected QuadComponent
{
public:
    Beam(const BeamData& rData);
    virtual ~Beam();

    void setStart(const b2Vec2& rStart);
    void setEnd(const b2Vec2& rEnd);

    void activate(float secs, int beamWidth);//seconds to be active, and then deactivate
    void deactivate();//regardless of our state, deactivate
    virtual void postUpdate();

protected:
private:
    Timer m_deactivateTimer;

    b2Vec2 m_startPos;
    QuadComponent m_start;

    b2Vec2 m_endPos;
    QuadComponent m_end;
};

#endif // BEAM_HPP
