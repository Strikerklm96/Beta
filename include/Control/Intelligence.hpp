#ifndef INTELLIGENCE_HPP
#define INTELLIGENCE_HPP

#include "stdafx.hpp"
#include "IOComponent.hpp"
#include "Globals.hpp"
#include "Universe.hpp"
#include "NetworkComponent.hpp"

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

    NetworkComponentData nwComp;
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
    void processDirectives();//use our stored directives to send commands

    float get(Request value);//return the requested value
    b2Body* getBodyPtr();//return our chunk body if we have one

    const std::string& getName();
    void setName(const std::string& newName);

protected:
    virtual void input(std::string rCommand, sf::Packet rData);
    virtual void pack(sf::Packet& rPacket);
    virtual void unpack(sf::Packet& rPacket);

    int m_slavePosition;
    std::string m_slaveName;
    std::string m_SIname;
    b2Vec2 m_aim;//where we are aiming in the world ATM
    std::map<Directive, bool> m_directives;//up, down, rollCW, roll CCW, ect.

private:
    IOComponent m_io;
    NetworkComponent m_nw;
    std::string m_playerName;//name of us in game, like BobbyLolcatz99, not used for anything but player reading
};

#endif // INTELLIGENCE_HPP
