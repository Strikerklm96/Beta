#ifndef INTELLIGENCE_HPP
#define INTELLIGENCE_HPP

#include "stdafx.hpp"
#include "IOComponent.hpp"
#include "Globals.hpp"
#include "Universe.hpp"

class Slave;

enum class Directive
{
    /**== MOVEMENT ==**/
    Up,
    Down,
    Left,
    Right,
    RollCCW,//roll left
    RollCW,//roll right


    /**== COMBAT ==**/
    FirePrimary,
    FireSecondary,

    NextWeapon,
    PrevWeapon,
    Reload,

    /**== MISC. ==**/
    Use,
    ShieldToggle,
    Special1,
    Special2,
};
enum class Request
{
    Zoom,
    MaxZoom,

    Energy,
    MaxEnergy,

    Ballistics,
    MaxBallistics,

    Missiles,
    MaxMissiles,
};


struct IntelligenceData
{
    IntelligenceData() :
        ioComp(game.getCoreIO()),
        slaveName("NOSLAVE")
    {
    }

    IOComponentData ioComp;
    std::string slaveName;
};


class Intelligence
{
public:
    Intelligence(const IntelligenceData& rData);
    virtual ~Intelligence();

    IOComponent& getIOComp();
    void setPlayerName(const std::string& rPlayerName);
    const std::string& getPlayerName() const;


    /**== CHUNK ==**/
    void setSlave(const std::string& rSlaveName);
    const std::string& getSlaveName() const;

    void setAim(const b2Vec2& world);//send our aim coordinates
    const b2Vec2& getAim() const;
    void directive(Directive issue);//send command to target

    float get(Request value);//return the requested value
    b2Body* getBodyPtr();//return our chunk body if we have one


protected:
    virtual void input(std::string rCommand, sf::Packet rData);

    int m_slavePosition;
    std::string m_slaveName;
    b2Vec2 m_aim;//where we are aiming in the world ATM

private:
    IOComponent m_io;
    std::string m_playerName;//name of us in game, like BobbyLolcatz99
};

#endif // INTELLIGENCE_HPP
