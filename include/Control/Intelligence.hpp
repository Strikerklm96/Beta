#ifndef INTELLIGENCE_HPP
#define INTELLIGENCE_HPP

#include "stdafx.hpp"
#include "IOComponent.hpp"
#include "Globals.hpp"
#include "Universe.hpp"

class Slave;

enum class Directive
{
    FirePrimary,
    FireSecondary,
    Reload,

    NextWeapon,
    PreviousWeapon,

    Up,
    Down,
    Left,
    Right,
    RollCCW,//roll left
    RollCW,//roll right

    Use,
    ShieldToggle,

    Special1,
    Special2,
};
enum class Request
{
    Angle,
    Zoom,
    Energy,
    Projectiles,
    Missiles,
};


struct IntelligenceData
{
    IntelligenceData() :
        slaveName("NOSLAVE")
    {
    }

    std::string slaveName;
};


class Intelligence
{
public:
    Intelligence(const IntelligenceData& rData);
    virtual ~Intelligence();

    void setSlave(const std::string& rSlaveName);
    const std::string& getSlaveName() const;
    const b2Vec2& getAim() const;

    void setAim(const b2Vec2& world);//send our aim coordinates
    b2Body* getBodyPtr();//return position
    void directive(Directive issue);//send command to target
    float get(Request value);//return the requested value



protected:
    int m_slavePosition;
    std::string m_slaveName;
private:
    b2Vec2 m_aim;//where we are aiming in the world
    ///IOComponent m_io;???
};

#endif // INTELLIGENCE_HPP
