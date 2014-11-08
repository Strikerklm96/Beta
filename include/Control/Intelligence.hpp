#ifndef INTELLIGENCE_HPP
#define INTELLIGENCE_HPP

#include "stdafx.hpp"
#include "IOComponent.hpp"
#include "Globals.hpp"
#include "Universe.hpp"

struct IntelligenceData
{
    IntelligenceData()
    {
    }

};


class Intelligence
{
public:
    Intelligence(const IntelligenceData& rData);
    virtual ~Intelligence();

    const b2Vec2& getAim() const;
    void setAim(const b2Vec2& world);

protected:
private:
    b2Vec2 m_aim;//where we are aiming in the world
    //IOComponent m_io;
};

#endif // INTELLIGENCE_HPP
